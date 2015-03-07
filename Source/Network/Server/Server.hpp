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
// File: Server.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Server class.
// ========================================================================= //

#ifndef __SERVER_HPP__
#define __SERVER_HPP__

// ========================================================================= //

#include "Network/Network.hpp"

// ========================================================================= //

namespace std{
    template<> 
    struct hash<RakNet::RakNetGUID>
    {
        size_t operator()(const RakNet::RakNetGUID& g) const
        {
            return hash<unsigned long>()(g.ToUint32(g));
        }
    };
}

// ========================================================================= //
// Operates network functionality for running a server with multiple clients.
class Server final : public Network
{
public:
    // Default initializes member data.
    explicit Server(void);

    // Empty destructor.
    virtual ~Server(void) override;

    // Loads server settings from config file and sets up server connection.
    virtual void init(const int port, const std::string& username) override;

    // Destroys server connection.
    virtual void destroy(void) override;

    // Receives and handles incoming packets on server port.
    virtual void update(void) override;

    // Sends bitstream to client with corresponding GUID.
    uint32_t send(const RakNet::AddressOrGUID identifier,
                  const RakNet::BitStream& bs,
                  const PacketPriority priority,
                  const PacketReliability reliability) override;

    // Sends bitstream to all registered clients.
    virtual uint32_t broadcast(const RakNet::BitStream& bs,
                               const PacketPriority priority,
                               const PacketReliability reliability,
                               const RakNet::SystemAddress& exclude = 
                                   RakNet::UNASSIGNED_SYSTEM_ADDRESS) override;

    // Sends chat message to all registered clients.
    virtual uint32_t chat(const std::string& msg) override;   

    // Sends a list of all connected client's usernames and server's username.
    virtual void sendPlayerList(const RakNet::AddressOrGUID& identifier,
                                bool broadcast = false) override;

    // Broadcasts start game message to all clients.
    virtual void startGame(void) override;

    // Broadcasts end game message to all clients. Broadcasts player list.
    virtual void endGame(void) override;

    // Getters:

    // === //

    struct Player{
        RakNet::RakString username;
        RakNet::SystemAddress systemAddress;
        int id;
        Entity* entity;
    };

    typedef std::unordered_map<RakNet::RakNetGUID,
        std::shared_ptr<Player>> PlayerList;

private:
    // Processes new client registration.
    void registerNewClient(void);

    RakNet::RakPeerInterface* m_peer;
    RakNet::Packet* m_packet;
    unsigned int m_tickRate;

    // Player instance of user running server.
    std::shared_ptr<Player> m_host;

    // Hash table of connected players.
    PlayerList m_players;
};

// ========================================================================= //

#endif

// ========================================================================= //