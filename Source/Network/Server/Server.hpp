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

#include "stdafx.hpp"

// ========================================================================= //
// Operates network functionality for running a server with multiple clients.
class Server final
{
public:
    explicit Server(void);

    ~Server(void);

    void init(void);

    void destroy(void);

    void update(void);

private:
    RakNet::RakPeerInterface* m_peer;
    RakNet::Packet* m_packet;
    unsigned int m_tickRate;
};

// ========================================================================= //

#endif

// ========================================================================= //