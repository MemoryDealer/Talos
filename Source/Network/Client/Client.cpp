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
// File: Client.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Client class.
// ========================================================================= //

#include "Client.hpp"
#include "Config/Config.hpp"
#include "Network/NetData.hpp"

// ========================================================================= //

Client::Client(void) :
m_peer(nullptr),
m_packet(nullptr),
m_serverSystemAddr(),
m_serverIP(),
m_port(0),
m_connected(false),
m_id(0),
m_players()
{
    this->setMode(Network::Mode::Client);
}

// ========================================================================= //

Client::~Client(void)
{

}

// ========================================================================= //

void Client::init(void)
{
    // Setup RakNet peer interface.
    RakNet::SocketDescriptor sd;
    m_peer = RakNet::RakPeerInterface::GetInstance();
    m_peer->Startup(1, &sd, 1);

    Talos::Config c("Data/Network/net.cfg");
    bool simulate = c.parseBool("Simulator", "active");
    if (simulate){
        const float packetLoss = c.parseReal("Simulator", "packetLoss");
        const int delay = c.parseInt("Simulator", "delay");

        m_peer->ApplyNetworkSimulator(packetLoss, delay / 2, 0);
    }

    this->setInitialized(true);
}

// ========================================================================= //

void Client::destroy(void)
{
    if (m_connected){
        m_peer->CloseConnection(m_serverSystemAddr, 
                                true, 
                                0, 
                                IMMEDIATE_PRIORITY);
    }

    RakNet::RakPeerInterface::DestroyInstance(m_peer);

    this->setInitialized(false);
}

// ========================================================================= //

void Client::update(void)
{
    for (m_packet = m_peer->Receive();
         m_packet;
         m_peer->DeallocatePacket(m_packet), m_packet = m_peer->Receive()){
        switch (m_packet->data[0]){
        default:
            break;

        case ID_CONNECTION_REQUEST_ACCEPTED:
            // Store server's system address.
            m_serverSystemAddr = m_packet->systemAddress;            

            // Register user info with server.            
            this->registerWithServer();
            break;

        case NetMessage::Register:
            {
                NetEvent e(NetMessage::Register);
                NetData::ClientRegistration reg;
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));
                reg.Serialize(false, &bs);

                e.s1 = reg.username.C_String();
                this->pushEvent(e);
            }
            break;

        case NetMessage::RegistrationSuccessful:
            m_connected = true;
            this->pushEvent(NetEvent(NetMessage::RegistrationSuccessful));
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
            }
            break;

        case NetMessage::PlayerList:
            {
                // Build player list received from server.
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));

                // Clear current player list.
                m_players.clear();

                // Read number of players and allocate space.
                uint32_t num = 0;
                bs.Read(num);
                m_players.resize(num);
                // Copy each string into local player list.
                for (uint32_t i = 0; i < num; ++i){
                    RakNet::RakString username;
                    int id;
                    bs.Read(username);
                    bs.Read(id);
                    m_players[id] = username.C_String();

                    NetEvent e(NetMessage::Register);
                    e.s1 = username.C_String();
                    this->pushEvent(e);
                }
            }
            break;
        }
    }
}

// ========================================================================= //

void Client::connect(const std::string& addr, 
                     const int port,
                     const std::string& username)
{
    if (m_connected == false){
        m_serverIP = addr;
        m_port = port;

        this->setUsername(username);

        m_peer->Connect(m_serverIP.c_str(), m_port, nullptr, 0);
    }
}

// ========================================================================= //

void Client::reconnect(void)
{
    if (m_connected == false){
        m_peer->Connect(m_serverIP.c_str(), m_port, nullptr, 0);
    }
}

// ========================================================================= //

void Client::disconnect(void)
{
    if (m_connected == true){
        m_peer->CloseConnection(m_serverSystemAddr, 
                                true, 
                                0, 
                                IMMEDIATE_PRIORITY);

        // Reset server system address.
        m_serverSystemAddr = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
    }
}

// ========================================================================= //

uint32_t Client::send(const RakNet::BitStream& bs,
                      const PacketPriority priority,
                      const PacketReliability reliability)
{
    return m_peer->Send(&bs, 
                        priority, 
                        reliability, 
                        0, 
                        m_serverSystemAddr, 
                        false);
}

// ========================================================================= //

uint32_t Client::chat(const std::string& msg)
{
    RakNet::BitStream bs;
    NetData::Chat chat;    
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::Chat));
    chat.msg = msg.c_str();
    chat.Serialize(true, &bs);

    return this->send(bs, MEDIUM_PRIORITY, RELIABLE);
}

// ========================================================================= //

// Private methods:

// ========================================================================= //

void Client::registerWithServer(void)
{
    RakNet::BitStream bs;
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::Register));
    NetData::ClientRegistration reg;
    reg.username = this->getUsername().c_str();
    reg.Serialize(true, &bs);

    this->send(bs, HIGH_PRIORITY, RELIABLE);

    this->lockEventQueue();
}

// ========================================================================= //