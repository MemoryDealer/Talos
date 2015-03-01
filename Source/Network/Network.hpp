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

// ========================================================================= //

struct NetEvent{
    explicit NetEvent(void) : type(NetMessage::Null) { }
    explicit NetEvent(const NetMessage msg) : type(msg) { }

    int type;

    std::string s1;
};

// ========================================================================= //
// Abstract class for either Server or Client operations (Server/Client derives
// from this class).
class Network
{
public:
    enum Mode{
        Server = 0,
        Client
    };

    // Default initializes member data.
    explicit Network(void);

    // Empty destructor.
    virtual ~Network(void) = 0;

    // Empty.
    virtual void init(void);

    // Empty.
    virtual void init(const int port, const std::string& username);

    // Empty.
    virtual void destroy(void);

    // Empty.
    virtual void update(void);

    // Empty.
    virtual void connect(const std::string& addr,
                         const int port,
                         const std::string& username);

    // Empty.
    virtual void reconnect(void);

    // Empty.
    virtual void disconnect(void);

    // Empty.
    virtual uint32_t send(const RakNet::BitStream& bs,
                          const PacketPriority priority,
                          const PacketReliability reliability);

    // Empty.
    virtual uint32_t send(const RakNet::AddressOrGUID identifier,
                          const RakNet::BitStream& bs,
                          const PacketPriority priority,
                          const PacketReliability reliability);

    // Empty.
    virtual uint32_t broadcast(const RakNet::BitStream& bs,
                               const PacketPriority priority,
                               const PacketReliability reliability,
                               const RakNet::SystemAddress& exclude =
                               RakNet::UNASSIGNED_SYSTEM_ADDRESS);

    // Empty.
    virtual uint32_t chat(const std::string& msg);

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

    // Returns username.
    const std::string getUsername(void) const;

    // Setters:

    // Sets mode (merely a flag).
    void setMode(const Mode mode);

    // Sets internal initialization flag.
    void setInitialized(const bool initialized);

    // Sets username.
    void setUsername(const std::string& username);

private:
    Mode m_mode;
    bool m_initialized;
    std::string m_username;

    std::queue<NetEvent> m_events;
};

// ========================================================================= //

// Getters:

inline const Network::Mode Network::getMode(void) const{
    return m_mode;
}

inline const bool Network::initialized(void) const{
    return m_initialized;
}

inline const std::string Network::getUsername(void) const{
    return m_username;
}

// Setters:

inline void Network::setMode(const Mode mode){
    m_mode = mode;
}

inline void Network::setInitialized(const bool initialized){
    m_initialized = initialized;
}

inline void Network::setUsername(const std::string& username){
    m_username = username;
}

// ========================================================================= //

#endif

// ========================================================================= //