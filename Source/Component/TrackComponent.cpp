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
// File: TrackComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements TrackComponent class.
// ========================================================================= //

#include "ComponentMessage.hpp"
#include "Core/Talos.hpp"
#include "TrackComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

TrackComponent::TrackComponent(void) :
m_animation(nullptr),
m_animationState(nullptr),
m_track(nullptr),
m_keyFrames(),
m_enabled(false),
m_loop(false),
m_reversalLoop(false),
m_forward(true)
{

}

// ========================================================================= //

TrackComponent::~TrackComponent(void)
{

}

// ========================================================================= //

void TrackComponent::init(void)
{

}

// ========================================================================= //

void TrackComponent::destroy(void)
{

}

// ========================================================================= //

void TrackComponent::update(void)
{     
    m_animationState->addTime(
        (m_forward) ? 
        Talos::MS_PER_UPDATE : 
        -Talos::MS_PER_UPDATE);
    
    // Reverse the animation if 
    if (m_reversalLoop){
        if (m_animationState->hasEnded()){
            m_forward = false;
        }
        else if (m_animationState->getTimePosition() == 0.f){
            m_forward = true;
        }
    }
}

// ========================================================================= //

void TrackComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::Action:
        if (!m_loop && !m_reversalLoop){
            this->reverse();
        }
        break;
    }
}

// ========================================================================= //

// Component functions:

// ========================================================================= //

void TrackComponent::addKeyFrame(const Ogre::Real dt,
                                 const Ogre::Vector3& pos,
                                 const Ogre::Quaternion& orientation)
{
    KeyFrame kf;
    kf.dt = dt;
    kf.pos = pos;
    kf.orientation = orientation;

    m_keyFrames.push_back(kf);
}

// ========================================================================= //

void TrackComponent::setup(Ogre::SceneNode* node)
{
    Ogre::SceneManager* scene = this->getWorld()->getSceneManager();

    // Create a name for this animation.
    static uint32_t nameCounter = 0;
    Ogre::String name = "TrackComponentAnim" + 
        Ogre::StringConverter::toString(nameCounter++);

    // Determine length of keyframe track.
    Ogre::Real length = m_keyFrames.back().dt;

    // Create the animation and set it to spline interpolation.
    m_animation = scene->createAnimation(name, length);
    m_animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);

    // Create animation track.
    m_track = m_animation->createNodeTrack(0, node);
    for (auto& i : m_keyFrames){
        // Create the key frame with time step.
        Ogre::TransformKeyFrame* kf = m_track->createNodeKeyFrame(i.dt);

        // Set the position of where the node will be at this time.
        kf->setTranslate(i.pos);

        // Set the orientation if specified.
        if (i.orientation != Ogre::Quaternion::IDENTITY){
            kf->setRotation(i.orientation);
        }
        else{
            kf->setRotation(node->_getDerivedOrientation());
        }
    }

    // Create the animation state for controlling the animation.
    m_animationState = scene->createAnimationState(name);

    if (m_enabled){
        m_animationState->setEnabled(true);
    }
    
    // Turn off looping if not set.
    if (!m_loop){
        m_animationState->setLoop(false);
    }
}

// ========================================================================= //

void TrackComponent::setEnabled(const bool enabled)
{
    m_enabled = enabled;
    if (m_animationState){
        m_animationState->setEnabled(enabled);
    }
}

// ========================================================================= //

void TrackComponent::reverse(void)
{
    if (!m_animationState->getEnabled()){
        m_animationState->setTimePosition(0.f);
        m_animationState->setEnabled(true);
        return;
    }

    m_forward = !m_forward;
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void TrackComponent::setLoop(const bool loop)
{
    m_loop = loop;
}

// ========================================================================= //

void TrackComponent::setReversalLoop(const bool loop)
{
    m_reversalLoop = loop;
}

// ========================================================================= //