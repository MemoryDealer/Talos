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
// File: NetworkComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines NetworkComponent class.
// ========================================================================= //

#ifndef __NETWORKCOMPONENT_HPP__
#define __NETWORKCOMPONENT_HPP__

// ========================================================================= //

#include "ActorComponent.hpp"
#include "Command/CommandTypes.hpp"
//#include "Component.hpp"
#include "Network/Update.hpp"

// ========================================================================= //
// Handles client-side network needs such as client-side prediction, server
// reconciliation, and interpolation.
class NetworkComponent : public Component
{
public:
    explicit NetworkComponent(void);

    virtual ~NetworkComponent(void) override;

    // 
    virtual void init(World& world) override;

    // 
    virtual void destroy(World& world) override;

    //
    virtual void update(World& world) override;

    // Handles input and transform update messages.
    virtual void message(ComponentMessage& msg) override;

    // Setters:

    // Sets internal actor component pointer.
    void setActorComponentPtr(const ActorComponentPtr actorC);

    // Sets internal scene component pointer.
    void setSceneComponentPtr(const SceneComponentPtr sceneC);

    // Sets internal World pointer.
    void setWorld(World* world);

    // === //

    // An input the client has sent but not yet received an update for from the 
    // server.
    struct PendingCommand{
        CommandType type;
        Ogre::Quaternion yawOrientation;
        Ogre::Quaternion pitchOrientation;
        uint32_t sequenceNumber;
    };

private:
    std::list<PendingCommand> m_pendingCommands;
    std::queue<TransformUpdate> m_serverUpdates;

    // Have direct access to sister components, the coupling here is acceptable.
    ActorComponentPtr m_actorC;
    SceneComponentPtr m_sceneC;

    // Needed to access Client's last input sequence number.
    World* m_world;
};

// ========================================================================= //

// Setters:

inline void NetworkComponent::setActorComponentPtr(
    const ActorComponentPtr actorC){
    m_actorC = actorC;
}

inline void NetworkComponent::setSceneComponentPtr(
    const SceneComponentPtr sceneC){
    m_sceneC = sceneC;
}

inline void NetworkComponent::setWorld(World* world){
    m_world = world;
}

// ========================================================================= //

#endif

// ========================================================================= //