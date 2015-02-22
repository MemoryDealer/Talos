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
#include "Network/Client/Client.hpp"
#include "Network/NetData.hpp"
#include "Server.hpp"

// ========================================================================= //

Server::Server(void) :
m_peer(nullptr),
m_packet(nullptr),
m_tickRate(8)
{

}

// ========================================================================= //

Server::~Server(void)
{

}

// ========================================================================= //

void Server::init(void)
{
    // Prepare server data.
    int port = 0;
    int maxClients = 0;
    bool simulate = false;
    float packetLoss = 0.f;
    int delay = 0;

    // Load server settings from config file.
    Talos::Config c("Data/Network/net.cfg");
    if (c.isLoaded()){
        port = c.parseInt("core", "port");
        maxClients = c.parseInt("core", "maxClients");

        simulate = c.parseBool("simulator", "active");
        if (simulate){
            packetLoss = c.parseReal("simulator", "packetLoss");
            delay = c.parseInt("simulator", "delay");
        }
    }
    else{
        // File failed to load, fill default values.
        port = 7042;
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
}

// ========================================================================= //

void Server::destroy(void)
{
    RakNet::RakPeerInterface::DestroyInstance(m_peer);
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
        }
    }
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

    printf("New client %s connected!\n", reg.username.C_String());
}

// ========================================================================= //