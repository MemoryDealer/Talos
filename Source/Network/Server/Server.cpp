// ========================================================================= //
// Talos - A 3D game engine with network multiplayer.
// Copyright(C) 2015 Jordan Sparks <unixunited@live.com>
//
// This program is free software; you can redistribute it and / or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// ========================================================================= //
// File: Server.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Server class.
// ========================================================================= //

#include "Config/Config.hpp"
#include "Network/NetData.hpp"
#include "Server.hpp"

// ========================================================================= //

Server::Server(void) :
m_peer(nullptr),
m_packet(nullptr),
m_tickRate(8),
m_host(nullptr),
m_players()
{
    this->setMode(Network::Mode::Server);
}

// ========================================================================= //

Server::~Server(void)
{

}

// ========================================================================= //

void Server::init(const int port, const std::string& username)
{
    // Prepare server data.
    int maxClients = 0;
    bool simulate = false;
    float packetLoss = 0.f;
    int delay = 0;

    // Load server settings from config file.
    Talos::Config c("Data/Network/net.cfg");
    if (c.isLoaded()){
        maxClients = c.parseInt("core", "maxClients");

        simulate = c.parseBool("simulator", "active");
        if (simulate){
            packetLoss = c.parseReal("simulator", "packetLoss");
            delay = c.parseInt("simulator", "delay");
        }
    }
    else{
        // File failed to load, fill default values.
        maxClients = 16;
    }
    
    // Server data ready, setup server connection.
    RakNet::SocketDescriptor sd(port, 0);
    m_peer = RakNet::RakPeerInterface::GetInstance();
    m_peer->Startup(maxClients, &sd, 1);
    m_peer->SetMaximumIncomingConnections(maxClients);
    if (simulate){
        // Apply network simulation for lag. Use half of delay since it will
        // be applied to clients as well.
        m_peer->ApplyNetworkSimulator(packetLoss, delay / 2, 0);
    }

    // Allocate Player instance for host.
    m_host.reset(new Player());
    m_host->username = username.c_str();
    m_host->entity = nullptr;

    this->setUsername(username);
    this->setInitialized(true);
}

// ========================================================================= //

void Server::destroy(void)
{
    RakNet::RakPeerInterface::DestroyInstance(m_peer);

    this->setInitialized(false);
}

// ========================================================================= //

void Server::update(void)
{
    // Receive incoming packets.
    for (m_packet = m_peer->Receive();
         m_packet;
         m_peer->DeallocatePacket(m_packet), m_packet = m_peer->Receive()){
        switch (m_packet->data[0]){
        default:
            break;

        case ID_REMOTE_NEW_INCOMING_CONNECTION:
            printf("Connection request received from: %s\n", m_packet->systemAddress.ToString());
            break;

        case ID_DISCONNECTION_NOTIFICATION:

            break;

        case NetMessage::Register:
            this->registerNewClient();
            break;

        case NetMessage::Chat:
            {
                NetEvent e(NetMessage::Chat);
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));
                NetData::Chat chat;
                chat.Serialize(false, &bs);

                e.s1 = chat.msg.C_String();
                this->pushEvent(e);

                // Broadcast chat message to all other clients.
                this->broadcast(bs, MEDIUM_PRIORITY, RELIABLE, m_packet->systemAddress);
            }
            break;
        }
    }
}

// ========================================================================= //

uint32_t Server::send(const RakNet::AddressOrGUID identifier,
                      const RakNet::BitStream& bs,
                      const PacketPriority priority,
                      const PacketReliability reliability)
{
    return m_peer->Send(&bs,
                        priority,
                        reliability,
                        0,
                        identifier,
                        false);
}

// ========================================================================= //

uint32_t Server::broadcast(const RakNet::BitStream& bs,
                           const PacketPriority priority,
                           const PacketReliability reliability,
                           const RakNet::SystemAddress& exclude)
{
    m_peer->Send(&bs,
                 priority,
                 reliability,
                 0,
                 exclude, // Address will be excluded from broadcast.
                 true); // true means broadcast to all connected peers.

    return 0;
}

// ========================================================================= //

uint32_t Server::chat(const std::string& msg)
{
    RakNet::BitStream bs;
    NetData::Chat chat;
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::Chat));
    chat.msg = msg.c_str();
    chat.Serialize(true, &bs);

    return this->broadcast(bs, MEDIUM_PRIORITY, RELIABLE);
}

// ========================================================================= //

// Private methods:

// ========================================================================= //

void Server::registerNewClient(void)
{
    NetData::ClientRegistration reg;
    RakNet::BitStream bs(m_packet->data, m_packet->length, false);
    bs.IgnoreBytes(sizeof(RakNet::MessageID));
    reg.Serialize(false, &bs);

    // Insert player into player list.
    std::shared_ptr<Player> player;
    player.reset(new Player());
    player->username = reg.username;
    player->systemAddress = m_packet->systemAddress;
    player->entity = nullptr;
    m_players[m_peer->GetGuidFromSystemAddress(m_packet->systemAddress)] =
        player;

    // Send registration confirmation to client.
    RakNet::BitStream confirm;
    confirm.Write(static_cast<RakNet::MessageID>(
        NetMessage::RegistrationSuccessful));
    this->send(player->systemAddress, confirm, MEDIUM_PRIORITY, RELIABLE);

    // Broadcast new player registration.
    this->broadcast(bs, MEDIUM_PRIORITY, RELIABLE, m_packet->systemAddress);

    // Add event for engine state.
    NetEvent e(NetMessage::Register);
    e.s1 = player->username;
    this->pushEvent(e);
}

// ========================================================================= //