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
m_username("")
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