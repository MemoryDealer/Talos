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
m_sceneC(nullptr),
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
    // Apply pending updates from server.
    while (!m_serverUpdates.empty()){
        if (m_actorC){            
            TransformUpdate update = m_serverUpdates.front();
            //printf("Applying server update %d\n", update.sequenceNumber);

            ComponentMessage msg(ComponentMessage::Type::TransformUpdate);
            msg.data = update;
            m_actorC->message(msg);

            // Replay any updates not yet processed by server.
            auto i = std::begin(m_pendingCommands);
            while (i != std::end(m_pendingCommands)){
                if (i->sequenceNumber <= update.sequenceNumber){
                    i = m_pendingCommands.erase(i);
                }
                else{
                    // Reset orientation.
                    /*ComponentMessage msg(ComponentMessage::Type::SetOrientation);
                    msg.data = i->actorState.orientation;
                    m_actorC->message(msg);

                    msg = ComponentMessage(ComponentMessage::Type::Set2ndOrientation);
                    msg.data = i->actorState.orientation2;
                    m_actorC->message(msg);*/

                    //printf("\tReplaying input %d\n", i->sequenceNumber);
                    ComponentMessage msg = ComponentMessage(ComponentMessage::Type::Command);
                    msg.data = i->type;
                    //m_actorC->setOrientation(i->actorState.orientation);
                    m_actorC->message(msg);
                    
                    //m_actorC->setState(i->actorState);
                    m_actorC->update(world);
                    //  use an actor state

                    ++i;
                }
            }
            
            m_serverUpdates.pop();
        }
    }
    //printf("NetworkComponent updated.\n");
}

// ========================================================================= //

void NetworkComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::TransformUpdate:
        {
            // Enqueue transform update for processing later.
            TransformUpdate transform = boost::get<TransformUpdate>(msg.data);
            m_serverUpdates.push(transform);   
            
        }
        break;

    case ComponentMessage::Type::Command:
        {
            // Enqueue this command into the pending commands queue.
            PendingCommand command;
            command.type = boost::get<CommandType>(msg.data);
            
            // Get orientations.
            ComponentMessage msg(ComponentMessage::Type::GetOrientation);
            m_actorC->message(msg);
            command.actorState.orientation = boost::get<Ogre::Quaternion>(msg.data);
            
            msg = ComponentMessage(ComponentMessage::Type::Get2ndOrientation);
            m_actorC->message(msg);
            command.actorState.orientation2 = boost::get<Ogre::Quaternion>(msg.data);


            // Assign a sequence number and increment the counter. This will be
            // used for knowing which commands to replay each frame.
            command.sequenceNumber = 
                m_world->getNetwork()->getLastInputSequenceNumber();
            
            m_pendingCommands.push_back(command);
        }
        break;
    }
}

// ========================================================================= //