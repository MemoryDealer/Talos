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
// File: TrackComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines TrackComponent class.
// ========================================================================= //

#ifndef __TRACKCOMPONENT_HPP__
#define __TRACKCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Moves an entity along a defined path (a "track") each frame. Can be looped
// or stopped when it reaches the end (and reversed with a trigger).
class TrackComponent : public Component
{
public:
    // Default initializes member data.
    explicit TrackComponent(void);

    // Empty destructor.
    virtual ~TrackComponent(void) override;

    virtual void init(void) override;

    // Frees any track data.
    virtual void destroy(void) override;

    // Advances track animation.
    virtual void update(void) override;

    // Handles activation/deactivation messages.
    virtual void message(ComponentMessage& msg) override;

    // Component functions:

    // Adds key frame data into internal list for later setup.
    void addKeyFrame(const Ogre::Real dt,
                     const Ogre::Vector3& pos,
                     const Ogre::Quaternion& orientation = 
                        Ogre::Quaternion::IDENTITY);

    // Creates node animation track on specified node.
    void setup(Ogre::SceneNode* node);

    // Reverses direction of animation.
    void reverse(void);

    // Setters:

    // Sets track animation to loop back to beginning when it ends.
    void setLoop(const bool loop);

    // Sets track animation to continuously loop back and forth if true.
    void setReversalLoop(const bool loop);

    // === //

    struct KeyFrame{
        Ogre::Real dt;
        Ogre::Vector3 pos;
        Ogre::Quaternion orientation;
    };

private:
    Ogre::Animation* m_animation;
    Ogre::AnimationState* m_animationState;
    Ogre::NodeAnimationTrack* m_track;
    std::vector<KeyFrame> m_keyFrames;
    bool m_loop, m_reversalLoop;
    bool m_forward; // Direction the animation is progressing.
};

// ========================================================================= //

#endif

// ========================================================================= //