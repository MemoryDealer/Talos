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
#include "Command/CommandTypes.hpp"
#include "ComponentMessage.hpp"
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
m_speed(0.20f),
m_remote(false),
m_cc(CC::Kinematic),
m_dcc(nullptr),
m_kcc(nullptr),
m_mode(Mode::Player),
m_state()
{
    
}

// ========================================================================= //

ActorComponent::~ActorComponent(void)
{

}

// ========================================================================= //

void ActorComponent::init(World& world)
{
    SceneComponent::init(world);

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
        m_kcc->init(world);
    }
}

// ========================================================================= //

void ActorComponent::destroy(World& world)
{
    // Destroy character controller.
    if (m_cc == CC::Kinematic){
        delete m_kcc;
    }
    else if (m_cc == CC::Dynamic){
        delete m_dcc;
    }

    world.getSceneManager()->destroySceneNode(m_yawNode);
    SceneComponent::destroy(world);
}

// ========================================================================= //

void ActorComponent::update(World& world)
{
    m_kcc->update(world, 0.f, 0.f, 0.f);
    return;

    if (m_remote){
        return;
    }

    printf("Forward: %d\tBack: %d\n", m_state.input.forward, m_state.input.back);

    Ogre::Vector3 translate(Ogre::Vector3::ZERO);
    const Ogre::Real move = 1.f;
    if (m_state.input.forward){
        translate.z = -move;
    }
    else if (m_state.input.back){
        translate.z = move;
    }
    if (m_state.input.right){
        translate.x = move;
    }
    else if (m_state.input.left){
        translate.x = -move;
    }

    // Calculate movement vector.
    translate = m_yawNode->getOrientation() *
        m_pitchNode->getOrientation() *
        translate;

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

    translate *= m_speed;

    switch (m_mode){
    default:
        break;

    case Mode::Spectator:
        m_rootNode->translate(translate, Ogre::SceneNode::TS_LOCAL);
        break;

    case Mode::Player:
        {
            if (m_cc == CC::Kinematic){
                PxExtendedVec3 pos = m_kcc->update(world,
                                                   translate.x,
                                                   translate.y,
                                                   translate.z);
                m_rootNode->setPosition(Ogre::Real(pos.x),
                                          Ogre::Real(pos.y),
                                          Ogre::Real(pos.z));
            }
        }
        break;
    }

    // Reset data for next frame. This should be bit values for input that
    // must be held down, such as movement. 
    /*m_state.input.back = m_state.input.forward =
        m_state.input.right = m_state.input.left = false;
    m_state.position = m_rollNode->_getDerivedPosition();
    m_state.orientation = m_rollNode->_getDerivedOrientation();*/
}

// ========================================================================= //

void ActorComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::GetPosition:       
        msg.data = m_rootNode->_getDerivedPosition();
        break;

    case ComponentMessage::Type::SetPosition:
        this->setPosition(boost::get<Ogre::Vector3>(msg.data));
        break;

    case ComponentMessage::Type::GetOrientation:
        msg.data = m_rootNode->_getDerivedOrientation();
        break;

    case ComponentMessage::Type::SetOrientation:
        this->setOrientation(boost::get<Ogre::Quaternion>(msg.data));
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
            //}
        }
        break;

    case ComponentMessage::Type::Command:       
        {
            // Get command type and process it.
            CommandType type = boost::get<CommandType>(msg.data);
            switch (type){
            default:
                break;

            case CommandType::MoveForward:
                m_state.input.forward = true;
                {
                    Ogre::Vector3 translate(Ogre::Vector3::ZERO);
                    translate.z = -1.f;
                    translate = m_yawNode->getOrientation() *
                        m_pitchNode->getOrientation() *
                        translate;
                    translate.normalise();

                    translate *= m_speed;

                    PxExtendedVec3 pos = m_kcc->update(*m_world,
                                                       translate.x,
                                                       translate.y,
                                                       translate.z);
                    m_rootNode->setPosition(Ogre::Real(pos.x),
                                            Ogre::Real(pos.y),
                                            Ogre::Real(pos.z));
                }
                break;

            case CommandType::MoveBackward:
                m_state.input.back = true;
                break;

            case CommandType::MoveRight:
                m_state.input.right = true;
                break;

            case CommandType::MoveLeft:
                m_state.input.left = true;
                break;

            case CommandType::Jump:
                m_kcc->jump();
                break;
            }
        }
        break;
    }
}



// ========================================================================= //

void ActorComponent::onComponentAttached(ComponentPtr component)
{
    
}

// ========================================================================= //

void ActorComponent::attachCamera(Ogre::Camera* camera)
{
    m_rollNode->attachObject(camera);
}

// ========================================================================= //

void ActorComponent::look(const int relx, const int rely)
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
}

// ========================================================================= //

void ActorComponent::setMode(const Mode mode)
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

void ActorComponent::setPosition(const Ogre::Vector3& pos)
{
    SceneComponent::setPosition(pos);
    m_kcc->setPosition(pos);
}

// ========================================================================= //