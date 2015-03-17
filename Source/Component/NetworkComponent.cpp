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
// File: NetworkComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements NetworkComponent class.
// ========================================================================= //

#include "ActorComponent.hpp"
#include "Command/CommandRepository.hpp"
#include "Component/ComponentMessage.hpp"
#include "NetworkComponent.hpp"
#include "Network/Network.hpp"
#include "World/World.hpp"

// ========================================================================= //

NetworkComponent::NetworkComponent(void) :
m_pendingCommands(),
m_serverUpdates(),
m_actorC(nullptr),
m_world(nullptr)
{

}

// ========================================================================= //

NetworkComponent::~NetworkComponent(void)
{

}

// ========================================================================= //

void NetworkComponent::init(World& world)
{
    
}

// ========================================================================= //

void NetworkComponent::destroy(World& world)
{

}

// ========================================================================= //

void NetworkComponent::update(World& world)
{
    // Apply pending updates from server (server reconciliation).
    while (!m_serverUpdates.empty()){
        // Get next server update.
        TransformUpdate update = m_serverUpdates.front();

        // Set the transform of the actor to this update from the past.
        m_actorC->setPosition(update.position);

        // Process pending commands (those not applied by server yet).
        auto i = std::begin(m_pendingCommands);
        while (i != std::end(m_pendingCommands)){
            // This command has been applied, remove it from the list.
            if (i->sequenceNumber <= update.sequenceNumber){
                i = m_pendingCommands.erase(i);
            }
            // Replay this command since the server has yet to send 
            // an update for it.
            else{
                // Set orientation to that of the time of this command.
                m_actorC->setYawOrientation(i->yawOrientation);
                m_actorC->setPitchOrientation(i->pitchOrientation);

                // Execute the command on the actor.
                ComponentMessage msg = 
                    ComponentMessage(ComponentMessage::Type::Command);
                msg.data = i->type;
                m_actorC->message(msg);                                      

                ++i;
            }
        }

        m_actorC->update(world);
            
        m_serverUpdates.pop();
    }
}

// ========================================================================= //

void NetworkComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::TransformUpdate:
        // Enqueue transform update for processing later.
        m_serverUpdates.push(boost::get<TransformUpdate>(msg.data));
        break;

    case ComponentMessage::Type::Command:
        {
            // Enqueue this command into the pending commands queue.
            PendingCommand command;
            command.type = boost::get<CommandType>(msg.data);
            
            // Save current orientation of actor.
            command.yawOrientation = m_actorC->getYawOrientation();
            command.pitchOrientation = m_actorC->getPitchOrientation();

            // Assign the sequence number. This will be  used for knowing 
            // which commands to replay each frame.
            command.sequenceNumber = 
                m_world->getNetwork()->getLastInputSequenceNumber();
            
            m_pendingCommands.push_back(command);
        }
        break;
    }
}

// ========================================================================= //