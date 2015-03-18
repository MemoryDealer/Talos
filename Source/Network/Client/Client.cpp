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
#include "Command/Command.hpp"
#include "Component/ComponentMessage.hpp"
#include "Config/Config.hpp"
#include "Entity/Entity.hpp"
#include "Network/NetData.hpp"
#include "Network/Update.hpp"

// ========================================================================= //

Client::Client(void) :
m_peer(nullptr),
m_packet(nullptr),
m_serverSystemAddr(),
m_serverIP(),
m_port(0),
m_connected(false),
m_localID(1),
m_username(""),
m_lastInputSequenceNumber(0)
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
    if (m_connected == true){
        return;
    }

    // Setup RakNet peer interface.
    RakNet::SocketDescriptor sd;
    m_peer = RakNet::RakPeerInterface::GetInstance();
    m_peer->Startup(1, &sd, 1);

    Talos::Config c("Data/Network/net.cfg");
    bool simulate = c.parseBool("simulator", "active");
    if (simulate){
        const float packetLoss = c.parseReal("simulator", "packetLoss");
        const int delay = c.parseInt("simulator", "delay");

        m_peer->ApplyNetworkSimulator(packetLoss, delay, 0);
    }

    this->setInitialized(true);
}

// ========================================================================= //

void Client::destroy(void)
{
    if (m_connected){
        this->disconnect();
    }

    this->setInitialized(false);
}

// ========================================================================= //

void Client::update(void)
{
    // Too much overhead?
    if (this->initialized() == false){
        return;
    }

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

        case ID_DISCONNECTION_NOTIFICATION:
            m_connected = false;
            break;

        case ID_CONNECTION_LOST:
            {
                this->pushEvent(NetEvent(NetMessage::LostConnection));
                m_connected = false;
            }
            break;

        case NetMessage::ClientDisconnect:
            {
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));

                // Get the disconnected player's ID.
                NetworkID id;
                bs.Read(id);

                Assert(id != 0, "Disconnect with ID 0");

                // Notify engine state to remove player.
                NetEvent e(NetMessage::ClientDisconnect);
                e.data = this->getPlayer(id).username;
                this->pushEvent(e);

                // Remove player entry.
                this->removePlayer(id);
            }
            break;

        case NetMessage::Register:
            {
                // Another client has connected, process it.
                NetData::ClientRegistration reg;
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));
                reg.Serialize(false, &bs);

                // Add player to list.
                this->addPlayer(reg.id, Network::toString(reg.username));

                NetEvent e(NetMessage::Register);
                e.data = Network::toString(reg.username);
                this->pushEvent(e);
            }
            break;

        case NetMessage::RegistrationSuccessful:
            {
                // Get the ID assigned by server. Since registration was successful, now
                // we can insert a Player instance for the local player into Network.
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));
                NetworkID id;
                bs.Read(id);

                // Add local player instance.
                this->addPlayer(id, m_username);

                // Set local player pointer.
                this->setLocalPlayer(&this->getPlayer(id));

                // Store local ID.
                m_localID = id;
                
                NetEvent e(NetMessage::RegistrationSuccessful);
                e.data = m_username;
                this->pushEvent(e);
                m_connected = true;
            }
            break;

        case NetMessage::Chat:
            {
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));
                NetData::Chat chat;
                chat.Serialize(false, &bs);

                // Get player instance.
                Network::Player player = this->getPlayer(chat.id);
                
                // Prepend username in front of message.
                NetEvent e(NetMessage::Chat);
                e.data = player.username +
                    ": " +
                    Network::toString(chat.msg);
                this->pushEvent(e);
            }
            break;

        case NetMessage::PlayerList:
            {
                // Build player list received from server.
                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));

                // Get player list size.
                uint32_t num;
                bs.Read(num);

                // Insert each player into player list.
                for (uint32_t i = 0; i < num; ++i){
                    NetworkID id;
                    RakNet::RakString username;

                    bs.Read(id);
                    bs.Read(username);

                    // Make sure this isn't the local player instance.
                    if (id == m_localID){
                        continue;
                    }

                    // Insert the player into player list.
                    this->addPlayer(id, Network::toString(username));

                    // Notify engine state of new player.
                    NetEvent e(NetMessage::Register);
                    e.data = Network::toString(username);
                    this->pushEvent(e);
                }
            }
            break;

        case NetMessage::StartGame:
            this->pushEvent(NetEvent(NetMessage::StartGame));
            break;

        case NetMessage::EndGame:
            this->pushEvent(NetEvent(NetMessage::EndGame));
            break;

        case NetMessage::PlayerUpdate:            
            {
                // Don't process this update if game is not active.
                if (!this->gameActive()){
                    break;
                }

                RakNet::BitStream bs(m_packet->data, m_packet->length, false);
                bs.IgnoreBytes(sizeof(RakNet::MessageID));

                // Get player ID.
                NetworkID id;
                bs.Read(id);

                TransformUpdate transform;

                bs.Read(transform.sequenceNumber);

                // Read in position values.
                Ogre::Vector3 pos;
                bs.Read(pos.x);
                bs.Read(pos.y);
                bs.Read(pos.z);

                // Read in orientation values.
                Ogre::Quaternion orientation;
                bs.Read(orientation.w);
                bs.Read(orientation.x);
                bs.Read(orientation.y);
                bs.Read(orientation.z);

                // Notify engine state of player update.
                NetEvent e(NetMessage::PlayerUpdate);
                
                // Store EntityID for engine state to access from World.
                transform.id = this->getPlayer(id).entity->getID();
                transform.position = pos;
                transform.orientation = orientation;
                e.data = transform;
                this->pushEvent(e);
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
        m_username = username;

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
        RakNet::BitStream bs;
        bs.Write(static_cast<RakNet::MessageID>(NetMessage::ClientDisconnect));
        this->send(bs, IMMEDIATE_PRIORITY, RELIABLE);

        std::thread t(&Client::_shutdown, this);
        t.detach();

        this->clearPlayerList();
    }
}

// ========================================================================= //

void Client::_shutdown(void)
{
    m_peer->Shutdown(3000);

    // Reset server system address.
    m_serverSystemAddr = RakNet::UNASSIGNED_SYSTEM_ADDRESS;

    RakNet::RakPeerInterface::DestroyInstance(m_peer);

    m_connected = false;
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
    chat.msg = Network::toRakString(msg);
    chat.Serialize(true, &bs);

    return this->send(bs, MEDIUM_PRIORITY, RELIABLE);
}

// ========================================================================= //

uint32_t Client::sendCommand(CommandPtr command)
{
    RakNet::BitStream bs;
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::ClientCommand));
    bs.Write(command->type);
    bs.Write(++m_lastInputSequenceNumber);

    return this->send(bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}

// ========================================================================= //

uint32_t Client::sendMouseMove(const int32_t relx, const int32_t rely)
{
    RakNet::BitStream bs;
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::ClientMouseMove));
    bs.Write(relx);
    bs.Write(rely);

    return this->send(bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}

// ========================================================================= //

void Client::endGame(void)
{
    this->setGameActive(false);

    // Set all player entities to null to prepare for next game.
    Network::PlayerList& players = this->getPlayerList();
    for (auto& i : players){
        i.second.entity = nullptr;
    }
}

// ========================================================================= //

// Private methods:

// ========================================================================= //

void Client::registerWithServer(void)
{
    RakNet::BitStream bs;
    bs.Write(static_cast<RakNet::MessageID>(NetMessage::Register));
    NetData::ClientRegistration reg;
    reg.username = Network::toRakString(m_username);
    reg.Serialize(true, &bs);

    this->send(bs, HIGH_PRIORITY, RELIABLE);

    this->lockEventQueue();
}

// ========================================================================= //