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
// File: Look.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Look class.
// ========================================================================= //

#ifndef __LOOK_HPP__
#define __LOOK_HPP__

// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/ActorComponent.hpp"

// ========================================================================= //

class LookCommand;

typedef std::shared_ptr<LookCommand> LookCommandPtr;

// ========================================================================= //

class LookCommand : public Command
{
public:
    void setXY(const int relx, const int rely);

    virtual void execute(EntityPtr entity) override{
        ActorComponentPtr actor = entity->getComponent<ActorComponent>();

        actor->look(m_relx, m_rely);
    }

private:
    int m_relx, m_rely;
};

// ========================================================================= //

inline void LookCommand::setXY(const int relx, const int rely){
    m_relx = relx;
    m_rely = rely;
}

// ========================================================================= //

#endif

// ========================================================================= //