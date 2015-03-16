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

#include "Command/Command.hpp"
#include "Command/CommandRepository.hpp"
#include "Component/ComponentMessage.hpp"
#include "Config/Config.hpp"
#include "Entity/Entity.hpp"
#include "Network/NetData.hpp"
#include "Network/Update.hpp"
#include "Server.hpp"

// ========================================================================= //

Server::Server(void) :
m_peer(nullptr),
m_packet(nullptr),
m_tickRate(8),
m_tick(),
m_clients(),
m_commandRepo(new CommandRepository())
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
        // Apply network simulation for lag.
        m_peer->ApplyNetworkSimulator(packetLoss, delay, 0);
    }

    // Add local player instance.
    this->addPlayer(0, username);
    this->setLocalPlayer(&this->getPlayer(0));
    
    this->setInitialized(true);
    m_tick.reset();
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

            break;

        case ID_DISCONNECTION_NOTIFICATION:
        case ID_CONNECTION_LOST:
            {               
                // First make sure this disconnection wasn't already processed.
                if (m_clients.count(m_packet->guid) == 0){
                    break;
                }

                NetworkID id = m_clients[m_packet->guid].id;

                // Send notification to all connected clients.
                RakNet::BitStream bs;
                bs.Write(static_cast<RakNet::MessageID>(
                    NetMessage::ClientDisconnect));
                bs.Write(id);
                this->broadcast(bs, MEDIUM_PRIORITY, RELIABLE);        

                // Notify engine state to remove player.
                NetEvent e(NetMessage::ClientDisconnect);
                e.data = this->getPlayer(id).username;
                this->pushEvent(e);

                // Remove player from player and client hash tables.
                this->removePlayer(id);
                m_clients.erase(m_packet->guid);
            }
            break;

        case NetMessage::ClientDisconnect:
            {
                // Manually close connection to client.
                m_peer->CloseConnection(m_packet->guid, false);

                NetworkID id = m_clients[m_packet->guid].id;

                // Send notification to all connected clients.
                RakNet::BitStream bs;
                bs.Write(static_cast<RakNet::MessageID>(
                    NetMessage::ClientDisconnect));
                // Write NetworkID.
                bs.Write(id);
                this->broadcast(bs, MEDIUM_PRIORITY, RELIABLE);
                
                // Notify engine state to remove client.
                NetEvent e(NetMessage::ClientDisconnect);
                e.data = this->getPlayer(id).username;
                this->pushEvent(e);

                // Remove player from player and client hash tables.
                this->removePlayer(id);
                m_clients.erase(m_packet->guid);
            }
            break;

        case NetMessage::Register:
            this->registerNewClient();
            break;

        case NetMessage::Chat:
            {
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));
                NetData::Chat chat;
                chat.Serialize(false, &bs);

                NetworkID id = m_clients[m_packet->guid].id;

                // Broadcast chat message to all other clients.
                RakNet::BitStream bsOut;
                NetData::Chat chatOut;
                bsOut.Write(static_cast<RakNet::MessageID>(NetMessage::Chat));
                chatOut.msg = chat.msg;
                chatOut.id = id;
                chatOut.Serialize(true, &bsOut);
                this->broadcast(bsOut, 
                                MEDIUM_PRIORITY, 
                                RELIABLE, 
                                m_packet->systemAddress);

                // Notify engine state to show chat message.
                Network::Player player = this->getPlayer(id);
                NetEvent e(NetMessage::Chat);
                e.data = player.username +
                    ": " +
                    Network::toString(chat.msg);
                this->pushEvent(e);
            }
            break;

        case NetMessage::ClientCommand:
            {
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));
                CommandType type;
                bs.Read(type);
                bs.Read(m_clients[m_packet->guid].lastCommandSequenceNumber);
                CommandPtr command = m_commandRepo->getCommand(type);

                Network::Player player = this->getPlayer(m_clients[m_packet->guid].id);
                Assert(player.entity != nullptr, "Invalid entity for player");
                
                //printf("Received input from GUID: %d\tID: %d\n", m_packet->guid, m_players[m_packet->guid]->id);
                //printf("EntityID: %d\n", player.entity->getID());
                command->execute(player.entity);
            }
            break;

        case NetMessage::ClientMouseMove:
            {
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));

                // Get mouse move relative values.
                MouseMove mm;
                bs.Read(mm.relx);
                bs.Read(mm.rely);

                // Send the player's entity a look message.
                ComponentMessage msg(ComponentMessage::Type::Look);
                msg.data = mm;
                this->getPlayer(m_clients[m_packet->guid].id).entity->message(msg);
            }
            break;
        }
    }
    
    if (!this->gameActive()){
        return;
    }

    // Update clients.
    if (m_tick.getMilliseconds() > m_tickRate){
        // Send local player update.
        this->playerUpdate(0, 0, this->getLocalPlayer()->entity);

        // Send all other player updates.
        for (auto& i : m_clients){
            this->playerUpdate(i.second.id,
                               i.second.lastCommandSequenceNumber,
                               this->getPlayer(i.second.id).entity);
        }

        m_tick.reset();
    }
}

// ========================================================================= //

void Server::playerUpdate(const NetworkID id, 
                          const uint32_t lastCommandSequence,
                          EntityPtr entity)
{
    // @TODO: Only send lastCommandSequence to client needing it.

    RakNet::BitStream bs;
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::PlayerUpdate));

    // ID.
    bs.Write(id);
    
    // Write the player's last processed input sequence number.
    bs.Write(lastCommandSequence);

    // Transform data.
    ComponentMessage msg(ComponentMessage::Type::GetPosition);
    entity->message(msg);
    Ogre::Vector3 pos = boost::get<Ogre::Vector3>(msg.data);
    bs.Write(pos.x);
    bs.Write(pos.y);
    bs.Write(pos.z);

    /*msg = ComponentMessage(ComponentMessage::Type::GetOrientation);
    entity->message(msg);
    Ogre::Quaternion orientation = boost::get<Ogre::Quaternion>(msg.data);
    bs.Write(orientation.w);
    bs.Write(orientation.x);
    bs.Write(orientation.y);
    bs.Write(orientation.z);*/

    this->broadcast(bs, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED);
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
    chat.msg = Network::toRakString(msg);
    chat.id = 0;
    chat.Serialize(true, &bs);

    return this->broadcast(bs, LOW_PRIORITY, RELIABLE);
}

// ========================================================================= //

void Server::sendPlayerList(const RakNet::AddressOrGUID& identifier,
                            bool broadcast)
{
    RakNet::BitStream bs;
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::PlayerList));

    // Get the list of all players (includes server local player).
    Network::PlayerList players = this->getPlayerList();

    // Write number of players.
    uint32_t num = players.size();
    bs.Write(num);

    // Write all player usernames and IDs.
    for (auto& i : players){
        bs.Write(i.first);
        bs.Write(Network::toRakString(i.second.username));
    }

    // Send with low priority since this has a potentially high overhead.
    if (broadcast){
        this->broadcast(bs, LOW_PRIORITY, RELIABLE);
    }
    else{
        this->send(identifier, bs, LOW_PRIORITY, RELIABLE);
    }
}

// ========================================================================= //

void Server::startGame(void)
{
    RakNet::BitStream bs;
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::StartGame));

    this->broadcast(bs, HIGH_PRIORITY, RELIABLE);
}

// ========================================================================= //

void Server::endGame(void)
{
    RakNet::BitStream bs;
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::EndGame));

    this->broadcast(bs, IMMEDIATE_PRIORITY, RELIABLE);

    // Reset all player's entity's to nullptr, to prepare for next game.
    for (auto& i : m_clients){
        this->getPlayer(i.second.id).entity = nullptr;
    }

    this->getLocalPlayer()->entity = nullptr;

    this->setGameActive(false);
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
    static NetworkID netIDCounter = 1; // Start at 1, server player is 0.
    NetworkID id = netIDCounter++;
    this->addPlayer(id, Network::toString(reg.username));
    this->addClientInstance(m_packet->guid, id);

    // Send registration confirmation to client.
    RakNet::BitStream bsConfirm;
    bsConfirm.Write(static_cast<RakNet::MessageID>(
        NetMessage::RegistrationSuccessful));
    bsConfirm.Write(id);
    this->send(m_packet->guid, bsConfirm, MEDIUM_PRIORITY, RELIABLE);

    // Send player list to new client.
    this->sendPlayerList(m_packet->guid);

    // Broadcast new player registration.
    RakNet::BitStream bsOut;
    bsOut.Write(static_cast<RakNet::MessageID>(NetMessage::Register));
    reg.id = id;
    reg.Serialize(true, &bsOut);
    this->broadcast(bsOut, LOW_PRIORITY, RELIABLE, m_packet->systemAddress);

    // Add event for engine state.
    NetEvent e(NetMessage::Register);
    e.data = Network::toString(reg.username);
    this->pushEvent(e);
}

// ========================================================================= //

void Server::addClientInstance(RakNet::RakNetGUID guid, const NetworkID id)
{
    m_clients[guid].id = id;
    m_clients[guid].lastCommandSequenceNumber = 0;
}

// ========================================================================= //