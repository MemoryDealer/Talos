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
// File: WeaponComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements WeaponComponent class.
// ========================================================================= //

#include "ComponentMessage.hpp"
#include "Core/Talos.hpp"
#include "Entity/Entity.hpp"
#include "Physics/PScene.hpp"
#include "WeaponComponent.hpp"
#include "Weapon/AttackFlare.hpp"
#include "World/World.hpp"

// ========================================================================= //

WeaponComponent::WeaponComponent(void) :
m_node(nullptr),
m_entity(nullptr),
m_clearDepth(false),
m_attackFlare(nullptr),
m_animationState(nullptr)
{

}

// ========================================================================= //

WeaponComponent::~WeaponComponent(void)
{

}

// ========================================================================= //

void WeaponComponent::init(void)
{

}

// ========================================================================= //

void WeaponComponent::destroy(void)
{

}

// ========================================================================= //

void WeaponComponent::update(void)
{
    // Process weapon animation if attacking.
    if (m_animationState->getEnabled()){
        m_animationState->addTime(Talos::MS_PER_UPDATE);

        if (m_animationState->hasEnded()){
            m_animationState->setEnabled(false);
            m_animationState->setTimePosition(0.f);
        }
    }

    m_attackFlare->update();
}

// ========================================================================= //

void WeaponComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::Command:
        {
            CommandType type = boost::get<CommandType>(msg.data);
            if (type == CommandType::Weapon){
                this->attack();
            }
        }
        break;
    }
}

// ========================================================================= //

// Component functions:

// ========================================================================= //

void WeaponComponent::setup(Ogre::SceneNode* actorRollNode)
{
    m_node = actorRollNode->createChildSceneNode();
    m_entity = this->getWorld()->getSceneManager()->createEntity("laserrifle.mesh");
    m_entity->setMaterialName("WeaponDefault");    
    m_node->attachObject(m_entity);

    m_node->translate(0.9f, -0.6f, -6.0f);
    m_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(-90.f));
    m_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(180.f));    

    // Setup node for render listener, so the weapon renders last.
    if (m_clearDepth){
        m_entity->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_9);
    }

    // Create animation for firing.
    m_node->setInitialState();

    const Ogre::Real length = 400.f;
    Ogre::Animation* animation = this->getWorld()->getSceneManager()->
        createAnimation("Weapon", length);
    animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);

    Ogre::NodeAnimationTrack* track = animation->createNodeTrack(0, m_node);
    
    // Setup key frames.
    Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0.f);
    //kf->setTranslate(Ogre::Vector3(0.f, 0.f, 0.f));

    kf = track->createNodeKeyFrame(length / 2.f);
    kf->setTranslate(Ogre::Vector3(0.f, 0.f, 1.5f));

    kf = track->createNodeKeyFrame(length);
    kf->setTranslate(Ogre::Vector3::ZERO);

    m_animationState = this->getWorld()->getSceneManager()->
        createAnimationState("Weapon");
    m_animationState->setLoop(false);

    // Setup attack flare.
    m_attackFlare.reset(new AttackFlare());
    m_attackFlare->init(this->getWorld(), m_node, "");
}

// ========================================================================= //

void WeaponComponent::attack(void)
{
    if (!m_animationState->getEnabled()){
        m_animationState->setEnabled(true);
        this->hitscan();
        m_attackFlare->activate();
    }
}

// ========================================================================= //

void WeaponComponent::hitscan(void)
{
    Ogre::SceneNode* pitchNode = m_node->getParentSceneNode()->
        getParentSceneNode();
    Ogre::SceneNode* yawNode = pitchNode->getParentSceneNode();

    Ogre::Vector3 dir = pitchNode->getOrientation() *
        yawNode->getOrientation() *
        Ogre::Vector3::NEGATIVE_UNIT_Z;

    PScene::Ray ray;
    ray.dir = Physics::toPx(dir);
    ray.dir.normalize();
    ray.dist = 10000.f; // Weapon range...
    ray.origin = Physics::toPx(m_node->_getDerivedPosition());

    const PxU32 size = 5;
    PxRaycastHit hitBuffer[size];
    PxRaycastBuffer buf(hitBuffer, size);

    bool hasHit = this->getWorld()->getPScene()->getScene()->raycast(ray.origin,
                                                                     ray.dir,
                                                                     ray.dist,
                                                                     buf);
    if (hasHit){
        for (PxU32 i = 0; i < buf.nbTouches; ++i){
            if (buf.touches[i].distance > 0.f){
                const EntityID id = Physics::toEntityID(buf.touches[i].actor);
                EntityPtr entity = this->getWorld()->getEntityPtr(id);

                if (entity){
                    ComponentMessage msg(ComponentMessage::Type::Hitscan);
                    entity->message(msg);
                    printf("Hit iD: %d\n", id);
                }
            }
        }
    }
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void WeaponComponent::setClearDepth(const bool clear)
{
    m_clearDepth = clear;
}

// ========================================================================= //