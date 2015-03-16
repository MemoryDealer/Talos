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
// File: Network.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Network class.
// ========================================================================= //

#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

// ========================================================================= //

#include "NetMessage.hpp"
#include "stdafx.hpp"
#include "Update.hpp"

// ========================================================================= //

struct NetEvent{
    explicit NetEvent(void) : type(NetMessage::Null) { }
    explicit NetEvent(const NetMessage msg) : type(msg) { }

    uint32_t type;

    boost::variant<
        std::string,
        TransformUpdate
        > data;
};

// ========================================================================= //
// Abstract class for either Server or Client operations (Server/Client derives
// from this class).
class Network
{
public:
    enum class Mode{
        Null = 0,
        Server,
        Client
    };

    // Default initializes member data.
    explicit Network(void);

    // Empty destructor.
    virtual ~Network(void) = 0;

    // === //

    // Interfaces:

    virtual void init(void) { }

    virtual void init(const int port, const std::string& username) { }

    virtual void destroy(void) { }

    virtual void update(void) { }

    virtual void connect(const std::string& addr,
                         const int port,
                         const std::string& username) { }


    virtual void reconnect(void) { }

    virtual void disconnect(void) { }

    virtual uint32_t send(const RakNet::BitStream& bs,
                          const PacketPriority priority,
                          const PacketReliability reliability) {
        return 0;
    }

    virtual uint32_t send(const RakNet::AddressOrGUID identifier,
                          const RakNet::BitStream& bs,
                          const PacketPriority priority,
                          const PacketReliability reliability) {
        return 0;
    }

    virtual uint32_t broadcast(const RakNet::BitStream& bs,
                               const PacketPriority priority,
                               const PacketReliability reliability,
                               const RakNet::SystemAddress& exclude =
                               RakNet::UNASSIGNED_SYSTEM_ADDRESS) {
        return 0;
    }

    virtual uint32_t chat(const std::string& msg) {
        return 0; 
    }

    virtual void sendPlayerList(const RakNet::AddressOrGUID& identifier,
                                bool broadcast = false) { }

    virtual void startGame(void) { }

    virtual void endGame(void) { }

    virtual uint32_t sendCommand(CommandPtr command) {
        return 0;
    }

    virtual const uint32_t getLastInputSequenceNumber(void) const{
        return 0;
    }

    virtual uint32_t sendMouseMove(const int32_t relx, const int32_t rely) {
        return 0;
    }

    // === //

    // A player in a networked game, including the local player.
    struct Player{
        std::string username;
        EntityPtr entity;
    };

    // Table of active players, their server-assigned ID is the key.
    typedef std::unordered_map<NetworkID, Player> PlayerList;

    // Modifies key-value id in internal player list. Sets entity pointer to null.
    Player& addPlayer(const NetworkID id, const std::string& username);

    // Removes player instance from player list.
    void removePlayer(const NetworkID id);

    // === //

    // Enqueues NetEvent.
    void pushEvent(const NetEvent& e);

    // Returns true if there is at least one NetEvent in the queue.
    const bool hasPendingEvent(void) const;

    // Returns next NetEvent in queue.
    const NetEvent getNextEvent(void);

    // Getters:

    // Returns mode of operation.
    const Mode getMode(void) const;

    // Returns true if Network service has been initialized.
    const bool initialized(void) const;

    // Returns true if game is active (e.g., GameState is running).
    const bool gameActive(void) const;

    // Returns local Player instance.
    Player* getLocalPlayer(void) const;

    // Returns true if player with id exists in player list.
    const bool playerExists(const NetworkID id) const;

    // Returns reference to Player with corresponding network ID.
    Player& getPlayer(const NetworkID id);

    // Returns active player list.
    PlayerList& getPlayerList(void);

    // Setters:

    // Sets mode (merely a flag).
    void setMode(const Mode mode);

    // Sets internal initialization flag.
    void setInitialized(const bool initialized);

    // Sets internal game active flag.
    void setGameActive(const bool active);

    // Sets pointer to Player instance.
    void setLocalPlayer(Player* player);

    // Locks event queue, preventing events from being popped.
    void lockEventQueue(void);

    // Unlocks event queue, allowing events to be popped.
    void unlockEventQueue(void);

    // Conversion functions:

    static std::string toString(const RakNet::RakString& rs){
        return std::string(rs.C_String());
    }

    static RakNet::RakString toRakString(const std::string& str){
        return RakNet::RakString(str.c_str());
    }

private:
    Mode m_mode;
    bool m_initialized, m_gameActive;
    Player* m_localPlayer; // Local player, points to instance in PlayerList.
    PlayerList m_players;

    std::queue<NetEvent> m_events;
    std::queue<NetEvent> m_immediateEvents; // Can't be locked.
    bool m_eventQueueLocked;
};

// ========================================================================= //

// Getters:

inline const Network::Mode Network::getMode(void) const{
    return m_mode;
}

inline const bool Network::initialized(void) const{
    return m_initialized;
}

inline const bool Network::gameActive(void) const{
    return m_gameActive;
}

inline Network::Player* Network::getLocalPlayer(void) const{
    return m_localPlayer;
}

inline const bool Network::playerExists(const NetworkID id) const{
    return (m_players.count(id) != 0);
}

inline Network::Player& Network::getPlayer(const NetworkID id){
    Assert(m_players.count(id) != 0, "getPlayer(), ID invalid");
    //if (m_players.count(id) != 0){
        return m_players[id];
    //}
}

inline Network::PlayerList& Network::getPlayerList(void){
    return m_players;
}

// Setters:

inline void Network::setMode(const Mode mode){
    m_mode = mode;
}

inline void Network::setInitialized(const bool initialized){
    m_initialized = initialized;
}

inline void Network::setGameActive(const bool active){
    m_gameActive = active;
}

inline void Network::setLocalPlayer(Player* player){
    m_localPlayer = player;
}

inline void Network::lockEventQueue(void){
    m_eventQueueLocked = true;
}

inline void Network::unlockEventQueue(void){
    m_eventQueueLocked = false;
}

// ========================================================================= //

#endif

// ========================================================================= //