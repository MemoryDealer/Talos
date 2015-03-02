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
// File: Client.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Client class.
// ========================================================================= //

#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

// ========================================================================= //

#include "Network/Network.hpp"

// ========================================================================= //
// Operates network functionality for connecting to remote server.
class Client final : public Network
{
public:
    // Default initializes member data.
    explicit Client(void);

    // Empty destructor.
    virtual ~Client(void) override;

    // Loads client settings from config file and connects to server on 
    // specified address and port
    virtual void init(void) override;

    // Closes client connection to server.
    virtual void destroy(void) override;

    // Receives incoming packets from server and handles them.
    virtual void update(void) override;

    // Sends connection request to server. A response will be received in 
    // the update loop.
    virtual void connect(const std::string& addr,
                         const int port,
                         const std::string& username) override;

    // Sends a connection request to server using current server data.
    virtual void reconnect(void) override;

    // Disconnects from currently connected server.
    virtual void disconnect(void) override;

    virtual uint32_t send(const RakNet::BitStream& bs,
                          const PacketPriority priority, 
                          const PacketReliability reliability) override;

    // Sends chat message to server.
    virtual uint32_t chat(const std::string& msg) override;

    typedef std::unordered_map<int, RakNet::RakString> PlayerList;

private:
    // Sends registration info to server.
    void registerWithServer(void);

    RakNet::RakPeerInterface* m_peer;
    RakNet::Packet* m_packet;
    RakNet::SystemAddress m_serverSystemAddr;
    std::string m_serverIP;
    int m_port;
    bool m_connected;
    int m_id;

    PlayerList m_players;
};

// ========================================================================= //

#endif

// ========================================================================= //