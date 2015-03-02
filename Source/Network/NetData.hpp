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
// File: NetData.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines data structures that will be sent over the network. Provides 
// serialization methods for each using RakNet::BitStream.
// ========================================================================= //

#ifndef __NETDATA_HPP__
#define __NETDATA_HPP__

// ========================================================================= //

#include "NetMessage.hpp"
#include "stdafx.hpp"

// ========================================================================= //

namespace NetData
{;

// ========================================================================= //

struct String{
    RakNet::RakString str;

    void Serialize(const bool write, RakNet::BitStream* bs){
        bs->Serialize(write, str);
    }
};

// ========================================================================= //

struct ClientRegistration{
    RakNet::RakString username;
    int id;

    void Serialize(const bool write, RakNet::BitStream* bs){
        bs->Serialize(write, username);
        bs->Serialize(write, id);
    }
};

// ========================================================================= //

struct RegistrationConfirmation{
    std::vector<RakNet::RakString> players;
    int size;
    int id;

    void Serialize(const bool write, RakNet::BitStream* bs){
        bs->Serialize(write, size);
        for (int i = 0; i < players.size(); ++i){
            bs->Serialize(write, *players[i].C_String());
        }
        bs->Serialize(write, id);
    }
};

// ========================================================================= //

struct Chat{
    RakNet::RakString msg;
    int id;

    void Serialize(const bool write, RakNet::BitStream* bs){
        bs->Serialize(write, msg);
        bs->Serialize(write, id);
    }
};

// ========================================================================= //

}

// ========================================================================= //

#endif

// ========================================================================= //