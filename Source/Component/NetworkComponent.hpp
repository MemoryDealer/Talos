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
#include "Network/Update.hpp"

// ========================================================================= //
// Handles client-side network needs such as client-side prediction, and
// server reconciliation.
class NetworkComponent : public Component
{
public:
    // Default initializes member data.
    explicit NetworkComponent(void);

    // Empty.
    virtual ~NetworkComponent(void) override;

    // Empty.
    virtual void init(void) override;

    // Empty.
    virtual void destroy(void) override;

    // Applies server reconciliation to associated actor component.
    virtual void update(void) override;

    // Handles command and transform update messages.
    virtual void message(ComponentMessage& msg) override;

    // Setters:

    // Sets internal actor component pointer.
    void setActorComponentPtr(const ActorComponentPtr actorC);

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
    // Commands not yet applied by server.
    std::list<PendingCommand> m_pendingCommands;
    // Pending server updates that need to be applied locally.
    std::queue<TransformUpdate> m_serverUpdates;

    // Have direct access to actor component, the coupling here is acceptable.
    ActorComponentPtr m_actorC;
};

// ========================================================================= //

#endif

// ========================================================================= //