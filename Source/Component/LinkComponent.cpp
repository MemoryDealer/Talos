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
// File: LinkComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements LinkComponent class.
// ========================================================================= //

#include "ComponentMessage.hpp"
#include "Entity/Entity.hpp"
#include "LinkComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

LinkComponent::LinkComponent(void) :
m_linkedIDs()
{

}

// ========================================================================= //

LinkComponent::~LinkComponent(void)
{

}

// ========================================================================= //

void LinkComponent::init(void)
{

}

// ========================================================================= //

void LinkComponent::destroy(void)
{

}

// ========================================================================= //

void LinkComponent::update(void)
{

}

// ========================================================================= //

void LinkComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::Action:
        {
            // Link activate all linked entities.
            ComponentMessage linkActivate(ComponentMessage::Type::LinkActivate);
            for (auto& i : m_linkedIDs){
                this->getWorld()->getEntityPtr(i)->message(linkActivate);
            }
        }
        break;
    }
}

// ========================================================================= //

// Component functions:

// ========================================================================= //

void LinkComponent::addLinkID(const EntityID id)
{
    m_linkedIDs.push_back(id);
}

// ========================================================================= //