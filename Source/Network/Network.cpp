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
m_mode(Network::Mode::Null),
m_initialized(false),
m_gameActive(false),
m_localPlayer(nullptr),
m_players(),
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

Network::Player& Network::addPlayer(const NetworkID id, const std::string& username)
{
    m_players[id].username = username;
    m_players[id].entity = nullptr;
    return m_players[id];
}

// ========================================================================= //

void Network::removePlayer(const NetworkID id)
{
    m_players.erase(id);
}

// ========================================================================= //

void Network::clearPlayerList(void)
{
    m_players.clear();
}

// ========================================================================= //