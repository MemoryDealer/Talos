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
// File: Spectator.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Spectator class.
// ========================================================================= //

#ifndef __SPECTATOR_HPP__
#define __SPECTATOR_HPP__

// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/ActorComponent.hpp"

// ========================================================================= //

class SpectatorCommand : public Command
{
public:
    explicit SpectatorCommand(void){
        this->setType(CommandType::Spectator);
    }
    
    virtual void execute(EntityPtr entity) override{
        ActorComponentPtr actor = entity->getComponent<ActorComponent>();

        actor->setMode(ActorComponent::Mode::Spectator);
    }

    virtual void unexecute(EntityPtr entity) override{
        ActorComponentPtr actor = entity->getComponent<ActorComponent>();

        actor->setMode(ActorComponent::Mode::Player);
    }
};

// ========================================================================= //

#endif

// ========================================================================= //