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
// File: ActorComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements ActorComponent class.
// ========================================================================= //

#include "ActorComponent.hpp"
#include "CameraComponent.hpp"
#include "CC/DCC.hpp"
#include "CC/KCC.hpp"
#include "ComponentMessage.hpp"
#include "Entity/Entity.hpp"
#include "ModelComponent.hpp"
#include "Physics/PScene.hpp"
#include "World/World.hpp"

// ========================================================================= //

ActorComponent::ActorComponent(void) :
SceneComponent(),
m_rootNode(nullptr),
m_yawNode(nullptr),
m_pitchNode(nullptr),
m_rollNode(nullptr),
m_yawOrientation(Ogre::Quaternion::IDENTITY),
m_pitchOrientation(Ogre::Quaternion::IDENTITY),
m_translate(Ogre::Vector3::ZERO),
m_speed(0.20f),
m_cc(CC::Kinematic),
m_dcc(nullptr),
m_kcc(nullptr),
m_mode(Mode::Player),
m_controllerAxis(new ControllerAxisMotion())
{
    
}

// ========================================================================= //

ActorComponent::~ActorComponent(void)
{

}

// ========================================================================= //

void ActorComponent::init(void)
{
    SceneComponent::init();

    // Acquire the camera node from the parent SceneComponent class.
    m_rootNode = this->getSceneNode();

    // Create yaw node as the camera's top node.
    m_yawNode = m_rootNode->createChildSceneNode();

    // Create pitch node as the camera's middle node.
    m_pitchNode = m_yawNode->createChildSceneNode();

    // Create roll node as the camera's bottom node.
    m_rollNode = m_pitchNode->createChildSceneNode();

    // Set to identity rotations.
    m_yawNode->setOrientation(Ogre::Quaternion::IDENTITY);
    m_pitchNode->setOrientation(Ogre::Quaternion::IDENTITY);
    m_rollNode->setOrientation(Ogre::Quaternion::IDENTITY);

    // Create PhysX character controller.
    if (m_cc == CC::Kinematic){
        m_kcc = new KCC();
        m_kcc->init(this->getWorld());
    }
}

// ========================================================================= //

void ActorComponent::destroy(void)
{
    // Destroy character controller.
    if (m_cc == CC::Kinematic){
        delete m_kcc;
    }
    else if (m_cc == CC::Dynamic){
        delete m_dcc;
    }

    this->getWorld()->getSceneManager()->destroySceneNode(m_yawNode);
    this->getWorld()->getSceneManager()->destroySceneNode(m_pitchNode);
    this->getWorld()->getSceneManager()->destroySceneNode(m_rollNode);
    SceneComponent::destroy();
}

// ========================================================================= //

void ActorComponent::update(void)
{
    Ogre::Vector3 translate(Ogre::Vector3::ZERO);
    translate.x = m_controllerAxis->x1;
    translate.z = m_controllerAxis->y1;

    // Calculate movement vector.
    translate = m_yawOrientation * m_pitchOrientation * translate;

    // Calculate the forwards vector and use it to keep the player moving at
    // the same velocity despite the pitch of the camera.
    Ogre::Vector3 right, up, forwards;
    m_rollNode->_getDerivedOrientation().ToAxes(right, up, forwards);
    up.crossProduct(right);
    up.normalise();

    // Modify original movement vector.
    translate.x /= up.y;
    translate.z /= up.y;

    // Prevent faster movement when moving diagonally.
    //translate.normalise();

    // Apply actor's speed.
    translate *= m_speed * 0.0001f;

    const Ogre::Real mag = translate.length();
    //printf("Mag: %.2f\n", mag);
    const Ogre::Real max = 0.275f;
    if (mag > max){
        translate *= max / mag;
    }

    switch (m_mode){
    default:
        break;

    case Mode::Spectator:
        
        break;

    case Mode::Player:
        {
            if (m_cc == CC::Kinematic){
                // Update kinematic controller.
                translate.y = 0.f;
                m_kcc->move(translate);

                // Update scene node with controller's new position.
                PxExtendedVec3 pos = m_kcc->update(this->getWorld());
                m_rootNode->setPosition(Ogre::Real(pos.x),
                                          Ogre::Real(pos.y),
                                          Ogre::Real(pos.z));
            }
        }
        break;
    }

    Ogre::Real x = static_cast<Ogre::Real>(m_controllerAxis->x2);
    Ogre::Real y = static_cast<Ogre::Real>(m_controllerAxis->y2);

    x *= 0.0001f;
    y *= 0.0001f;

    const Ogre::Real deadzone = 0.60f;
    if (std::abs(x) < deadzone){
        x = 0.f;
    }
    if (std::abs(y) < deadzone){
        y = 0.f;
    }

    if (x == 0.f && y == 0.f){
        return;
    }

    printf("look: %.2f, %.2f\n", x, y);

    const Ogre::Real sens = 0.70f;

    m_yawNode->yaw(Ogre::Degree(-x * sens));
    m_pitchNode->pitch(Ogre::Degree(y * sens * 0.75f));
    {
        // Prevent the camera from pitching upside down.
        Ogre::Real pitchAngle = 0.f;
        Ogre::Real pitchAngleParity = 0.f;

        // Get the angle of rotation around the x-axis.
        pitchAngle = (2.f * Ogre::Degree(Ogre::Math::ACos(
            m_pitchNode->getOrientation().w)).valueDegrees());

        pitchAngleParity = m_pitchNode->getOrientation().x;

        // Limit pitch.
        // @TODO: Define this in data.
        if (pitchAngle > 80.f){
            if (pitchAngleParity > 0){
                m_pitchNode->setOrientation(Ogre::Quaternion(
                    Ogre::Math::Sqrt(0.5f),
                    Ogre::Math::Sqrt(0.5f) - 0.115f,
                    0.f,
                    0.f));
            }
            else if (pitchAngleParity < 0){
                m_pitchNode->setOrientation(Ogre::Quaternion(
                    Ogre::Math::Sqrt(0.5f),
                    -Ogre::Math::Sqrt(0.5f) + 0.115f,
                    0.f,
                    0.f));
            }
        }
    }

    // Store orientations for next call to applyInput().
    m_yawOrientation = m_yawNode->getOrientation();
    m_pitchOrientation = m_pitchNode->getOrientation();
}

// ========================================================================= //

void ActorComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::GetPosition:       
        msg.data = this->getPosition();
        break;

    case ComponentMessage::Type::SetPosition:
        this->setPosition(boost::get<Ogre::Vector3>(msg.data));
        break;

    case ComponentMessage::Type::GetOrientation:
        msg.data = this->getOrientation();
        break;

    case ComponentMessage::Type::SetOrientation:
        this->setOrientation(boost::get<Ogre::Quaternion>(msg.data));
        break;

    case ComponentMessage::Type::Move:
        {
            ControllerAxisMotion m = 
                boost::get<ControllerAxisMotion>(msg.data);

            this->move(m.x1, m.y1);
            m_controllerAxis->x2 = m.x2;
            m_controllerAxis->y2 = m.y2;
        }
        break;

    case ComponentMessage::Type::Look:
        {
            MouseMove mm = boost::get<MouseMove>(msg.data);
            this->look(mm.relx, mm.rely);
        }
        break;
        
    case ComponentMessage::Type::TransformUpdate:        
        {
            // Apply transform update.
            // @TODO: Interpolate.                                                    
            TransformUpdate transform = 
                boost::get<TransformUpdate>(msg.data);

            /*Ogre::Vector3 src = this->getPosition();
            Ogre::Vector3 dst = transform.position;         
            const Ogre::Real max = 25.f;
            printf("Squared dist: %.2f\n", src.squaredDistance(dst));
            if (src.squaredDistance(dst) < max){
                m_rootNode->translate((dst - src) * 0.1f);
            }
            else{*/
                this->setPosition(transform.position);
                //m_rootNode->_setDerivedOrientation(transform.orientation);
                //m_yawNode->setOrientation(transform.orientation);
                //m_pitchNode->setOrientation(transform.orientation2);
            //}
        }
        break;

    case ComponentMessage::Type::Command:       
        this->applyInput(boost::get<CommandType>(msg.data));
        break;
    }
}

// ========================================================================= //

// Component functions:

// ========================================================================= //

void ActorComponent::attachCamera(Ogre::Camera* camera)
{
    m_rollNode->attachObject(camera);
}

// ========================================================================= //

void ActorComponent::attachFlashlight(Ogre::Light* light)
{
    Assert(light->getType() == Ogre::Light::LT_SPOTLIGHT,
           "Non-spotlight added as flashlight");

    Ogre::SceneNode* flashlight = m_rollNode->createChildSceneNode();
    flashlight->translate(0.5f, -0.45f, -3.f);

    light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
    flashlight->attachObject(light);
}

// ========================================================================= //

void ActorComponent::applyInput(const CommandType& type)
{
    Ogre::Vector3 translate(Ogre::Vector3::ZERO);
    const Ogre::Real move = 1.f;

    // Determine which direction to move, or action to perform.
    switch (type){
    default:
        return;

    case CommandType::MoveForward:
        translate.z = -move;
        break;

    case CommandType::MoveBackward:
        translate.z = move;
        break;

    case CommandType::MoveRight:
        translate.x = move;
        break;

    case CommandType::MoveLeft:
        translate.x = -move;
        break;

    case CommandType::Action:
        this->action();
        return;

    case CommandType::Jump:
        m_kcc->jump();
        return;        
    }

    // Calculate movement vector.
    translate = m_yawOrientation * m_pitchOrientation * translate;

    // Calculate the forwards vector and use it to keep the player moving at
    // the same velocity despite the pitch of the camera.
    Ogre::Vector3 right, up, forwards;
    m_rollNode->_getDerivedOrientation().ToAxes(right, up, forwards);
    up.crossProduct(right);
    up.normalise();

    // Modify original movement vector.
    translate.x /= up.y;
    translate.z /= up.y;

    // Prevent faster movement when moving diagonally.
    translate.normalise();

    // Apply actor's speed.
    translate *= m_speed;

    switch (m_mode){
    default:
        break;

    case Mode::Player:
        {
            // Update kinematic controller.
            translate.y = 0.f;
            PxExtendedVec3 pos = m_kcc->move(translate);

            // Update scene node with controller's new position.
            m_rootNode->setPosition(Ogre::Real(pos.x),
                                    Ogre::Real(pos.y),
                                    Ogre::Real(pos.z));
        }
        break;

    case Mode::Spectator:
        m_rootNode->translate(translate, Ogre::SceneNode::TS_LOCAL);
        break;
    }    
}

// ========================================================================= //

void ActorComponent::move(const Sint16 x, const Sint16 y)
{
    const Sint16 deadzone = 5000;
    if (std::abs(x) > deadzone){
        m_controllerAxis->x1 = x;
    }
    else{
        m_controllerAxis->x1 = 0;
    }
    if (std::abs(y) > deadzone){
        m_controllerAxis->y1 = y;
    }
    else{
        m_controllerAxis->y1 = 0;
    }

    //Ogre::Vector3 translate(Ogre::Vector3::ZERO);
    //translate.x = x;
    //translate.z = y;
    ////translate.normalise();

    //// Calculate movement vector.
    //translate = m_yawOrientation * m_pitchOrientation * translate;

    //// Calculate the forwards vector and use it to keep the player moving at
    //// the same velocity despite the pitch of the camera.
    //Ogre::Vector3 right, up, forwards;
    //m_rollNode->_getDerivedOrientation().ToAxes(right, up, forwards);
    //up.crossProduct(right);
    //up.normalise();

    //// Modify original movement vector.
    //translate.x /= up.y;
    //translate.z /= up.y;

    //// Prevent faster movement when moving diagonally.
    //translate.normalise();

    //// Apply actor's speed.
    //translate *= m_speed;

    //// Update kinematic controller.
    //translate.y = 0.f;
    //PxExtendedVec3 pos = m_kcc->move(translate);

    //// Update scene node with controller's new position.
    //m_rootNode->setPosition(Ogre::Real(pos.x),
    //                        Ogre::Real(pos.y),
    //                        Ogre::Real(pos.z));
}

// ========================================================================= //

void ActorComponent::look(const Sint16 relx, const Sint16 rely)
{
    if (relx == 0 && rely == 0){
        return;
    }

    const Ogre::Real sens = 0.2f;

    m_yawNode->yaw(Ogre::Degree(-Ogre::Real(relx) * sens));
    m_pitchNode->pitch(Ogre::Degree(-Ogre::Real(rely) * sens));
    {
        // Prevent the camera from pitching upside down.
        Ogre::Real pitchAngle = 0.f;
        Ogre::Real pitchAngleParity = 0.f;

        // Get the angle of rotation around the x-axis.
        pitchAngle = (2.f * Ogre::Degree(Ogre::Math::ACos(
            m_pitchNode->getOrientation().w)).valueDegrees());

        pitchAngleParity = m_pitchNode->getOrientation().x;

        // Limit pitch.
        // @TODO: Define this in data.
        if (pitchAngle > 80.f){
            if (pitchAngleParity > 0){
                m_pitchNode->setOrientation(Ogre::Quaternion(
                    Ogre::Math::Sqrt(0.5f),
                    Ogre::Math::Sqrt(0.5f) - 0.115f,
                    0.f,
                    0.f));
            }
            else if (pitchAngleParity < 0){
                m_pitchNode->setOrientation(Ogre::Quaternion(
                    Ogre::Math::Sqrt(0.5f),
                    -Ogre::Math::Sqrt(0.5f) + 0.115f,
                    0.f,
                    0.f));
            }
        }
    }

    // Store orientations for next call to applyInput().
    m_yawOrientation = m_yawNode->getOrientation();
    m_pitchOrientation = m_pitchNode->getOrientation();
}

// ========================================================================= //

void ActorComponent::action(void)
{    
    // Construct a direction from the player's origin going forward in the 
    // direction they are looking.
    Ogre::Vector3 dir = //this->getPosition() +
        (m_yawNode->getOrientation() * m_pitchNode->getOrientation()) * 
        Ogre::Vector3::NEGATIVE_UNIT_Z;

    // Setup raycast data.
    PScene::Ray ray;
    ray.dir = Physics::toPx(dir);
    ray.dir.normalize();
    ray.dist = 100.f;
    ray.origin = Physics::toPx(this->getPosition());

    const PxU32 size = 5;
    PxRaycastHit hitBuffer[size];
    PxRaycastBuffer buf(hitBuffer, size);

    bool hasHit = this->getWorld()->getPScene()->getScene()->raycast(ray.origin,
                                                                     ray.dir,
                                                                     ray.dist,
                                                                     buf);
    if (hasHit){
        for (PxU32 i = 0; i < buf.nbTouches; ++i){
            if (buf.touches[i].distance < 4.f){
                const EntityID id = reinterpret_cast<const EntityID>(
                    static_cast<void*>(buf.touches[i].actor->userData));
                //printf("Hit EntityID %d, %.2f\n", id, buf.touches[i].distance);
                EntityPtr entity = this->getWorld()->getEntityPtr(id);
                ComponentMessage msg(ComponentMessage::Type::Action);
                entity->message(msg);
            }
        }
    }
    
    // Run the raycast and check results.
    //bool status = this->getWorld()->getPScene()->raycast(ray);
    //const PxReal minDistance = 5.f;
    //if (status){
    //    const PxReal dist = ray.hit.block.distance;
    //    if (dist > 0.f && dist < minDistance){
    //        // Get the EntityID of what was hit, stored in the actor's user data.
    //        const EntityID id = reinterpret_cast<const EntityID>(
    //            static_cast<void*>(ray.hit.block.actor->userData));           

    //        // Get EntityPtr from world.
    //        EntityPtr entity = this->getWorld()->getEntityPtr(id);
    //        Assert(entity != nullptr, 
    //               "Invalid EntityPtr retrieved from World in action");

    //        // Send entity an action message.
    //        ComponentMessage msg(ComponentMessage::Type::Action);
    //        entity->message(msg);
    //        printf("Action hit: %.2f\tEntityID: %d\n", dist, id);
    //    }
    //}
}

// ========================================================================= //

// Getters:

// ========================================================================= //

const Ogre::Vector3& ActorComponent::getPosition(void) const{
    return m_rootNode->getPosition();
}

// ========================================================================= //

const Ogre::Quaternion& ActorComponent::getOrientation(void) const{
    return m_rollNode->_getDerivedOrientation();    
}

// ========================================================================= //

const Ogre::Quaternion& ActorComponent::getYawOrientation(void) const
{
    return m_yawNode->getOrientation();
}

// ========================================================================= //

const Ogre::Quaternion& ActorComponent::getPitchOrientation(void) const
{
    return m_pitchNode->getOrientation();
}

// ========================================================================= //

KCC* ActorComponent::getKCC(void) const
{
    return m_kcc;
}

// ========================================================================= //

Ogre::SceneNode* ActorComponent::getRollNode(void) const
{
    return m_rollNode;
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void ActorComponent::setPosition(const Ogre::Vector3& pos)
{
    SceneComponent::setPosition(pos);
    m_kcc->setPosition(pos);
}

// ========================================================================= //

void ActorComponent::setOrientation(const Ogre::Quaternion& orientation)
{
    m_rollNode->_setDerivedOrientation(orientation);
}

// ========================================================================= //

void ActorComponent::setYawOrientation(const Ogre::Quaternion& orientation)
{
    m_yawOrientation = orientation;
}

// ========================================================================= //

void ActorComponent::setPitchOrientation(const Ogre::Quaternion& orientation)
{
    m_pitchOrientation = orientation;
}

// ========================================================================= //

void ActorComponent::setMode(const Mode& mode)
{
    m_mode = mode;

    switch (m_mode){
    default:
        break;

    case Mode::Player:
        // For debugging.
        m_kcc->setPosition(m_rootNode->getPosition());
        break;

    case Mode::Spectator:

        break;
    }
}

// ========================================================================= //