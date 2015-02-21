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
#include "Network/NetMessage.hpp"

// ========================================================================= //

Client::Client(void) :
m_peer(nullptr),
m_packet(nullptr),
m_serverSystemAddr(),
m_serverIP(),
m_port(0),
m_connected(false),
m_info()
{

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
            m_connected = true;
            printf("Connected to server! %d\n", m_serverSystemAddr.GetPort());

            // Register user info with server.            
            this->registerUserInfo();
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
        m_info.username = username.c_str();

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
        m_serverSystemAddr = RakNet::SystemAddress();
    }
}

// ========================================================================= //

uint32_t Client::send(const RakNet::BitStream& bit,
                      const PacketPriority priority,
                      const PacketReliability reliability)
{
    return m_peer->Send(&bit, 
                        priority, 
                        reliability, 
                        0, 
                        m_serverSystemAddr, 
                        false);
}

// ========================================================================= //

// Private methods:

// ========================================================================= //

void Client::registerUserInfo(void)
{
    RakNet::BitStream bit;
    bit.Write(static_cast<RakNet::MessageID>(NetMessage::Register));
    //bit.Write(m_info.username);
    m_info.Serialize(true, &bit);

    this->send(bit, HIGH_PRIORITY, RELIABLE);
}

// ========================================================================= //