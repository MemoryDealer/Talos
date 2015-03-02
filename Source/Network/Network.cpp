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
// File: Network.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Network class.
// ========================================================================= //

#include "Network.hpp"

// ========================================================================= //

Network::Network(void) :
m_mode(),
m_initialized(false),
m_username(""),
m_events(),
m_immediateEvents(),
m_eventQueueLocked(false)
{

}

// ========================================================================= //

Network::~Network(void)
{

}

// ========================================================================= //

void Network::init(void)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::init() called");
}

// ========================================================================= //

void Network::init(const int port, const std::string& username)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::init(port, username) called");
}

// ========================================================================= //

void Network::destroy(void)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::destroy() called");
}

// ========================================================================= //

void Network::update(void)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::update() called");
}

// ========================================================================= //

void Network::connect(const std::string& addr,
                      const int port,
                      const std::string& username)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::connect() called");
}

// ========================================================================= //

void Network::reconnect(void)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::reconnect() called");
}

// ========================================================================= //

void Network::disconnect(void)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::disconnect() called");
}

// ========================================================================= //

uint32_t Network::send(const RakNet::BitStream& bs,
                       const PacketPriority priority,
                       const PacketReliability reliability)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::send() called");

    return 0;
}

// ========================================================================= //

uint32_t Network::send(const RakNet::AddressOrGUID identifier,
                       const RakNet::BitStream& bs,
                       const PacketPriority priority,
                       const PacketReliability reliability)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::send() (server) called");

    return 0;
}

// ========================================================================= //

uint32_t Network::broadcast(const RakNet::BitStream& bs,
                            const PacketPriority priority,
                            const PacketReliability reliability,
                            const RakNet::SystemAddress& exclude)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::broadcast() called");

    return 0;
}

// ========================================================================= //

uint32_t Network::chat(const std::string& msg)
{
    Talos::Log::getSingleton().log(
        "WARNING: Default Network::chat() called");

    return 0;
}

// ========================================================================= //

void Network::sendPlayerList(const RakNet::AddressOrGUID& identifier)
{
    
}

// ========================================================================= //

void Network::pushEvent(const NetEvent& e)
{
    switch (e.type){
    default:
        m_events.push(e);
        break;

    case NetMessage::RegistrationSuccessful:
    case NetMessage::UsernameAlreadyInUse:
        m_immediateEvents.push(e);
        break;
    }
    
}

// ========================================================================= //

const bool Network::hasPendingEvent(void) const
{
    return (m_eventQueueLocked == true) ? 
        (m_immediateEvents.empty() == false) : 
        (m_events.empty() == false);
}

// ========================================================================= //

const NetEvent Network::getNextEvent(void)
{
    if (m_events.empty() == true){
        return NetEvent();
    }
    // If event queue is locked, return Null event and keep queued events.
    else if (m_eventQueueLocked == true){
        if (m_immediateEvents.empty() == true){
            return NetEvent();
        }

        NetEvent e = m_immediateEvents.front();
        m_immediateEvents.pop();
        return e;        
    }

    NetEvent e = m_events.front();
    m_events.pop();
    return e;
}

// ========================================================================= //