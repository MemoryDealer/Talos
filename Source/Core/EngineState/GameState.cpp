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
// File: GameState.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements GameState class.
// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/AllComponents.hpp"
#include "Core/EngineNotifications.hpp"
#include "GameState.hpp"
#include "Input/Input.hpp"
#include "Loader/DotSceneLoader.hpp"
#include "Network/Network.hpp"
#include "Network/Update.hpp"
#include "Physics/PScene.hpp"
#include "Rendering/Sky/Sky.hpp"
#include "Rendering/Sky/SkyPresets.hpp"
#include "System/CollisionSystem.hpp"
#include "System/PhysicsSystem.hpp"
#include "World/Environment.hpp"

// ========================================================================= //

GameState::GameState(void)
{

}

// ========================================================================= //

GameState::~GameState(void)
{

}

// ========================================================================= //

void GameState::enter(void)
{
    this->createScene();
    
    // Network game setup.
    if (m_world->getNetwork()->initialized()){
        this->addNetworkPlayers();
        m_world->getNetwork()->setGameActive(true);
    }    

    if (!m_world->setupEntities()){
        throw std::exception("GameState entities reported uninitialized");
    }
}

// ========================================================================= //

void GameState::exit(void)
{
    m_world->destroy();
    
}

// ========================================================================= //

void GameState::pause(void)
{
    m_world->pause();
}

// ========================================================================= //

void GameState::resume(void)
{
    m_world->resume();
}

// ========================================================================= //

void GameState::update(void)
{
    if (m_active){
        SDL_Event e;
        while (SDL_PollEvent(&e)){
            switch (e.type){
            default:
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_KEYDOWN:
            case SDL_TEXTINPUT:
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                // Temporary exit handling.
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                    m_world->getNetwork()->endGame();
                    m_subject.notify(EngineNotification::Pop);
                    return;
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_b){
                    static bool bb = false;
                    bb = !bb;

                    Ogre::SceneNode::ChildNodeIterator itr = 
                        m_world->getSceneManager()->getRootSceneNode()->getChildIterator();
                    for (; itr.hasMoreElements();){
                        Ogre::SceneNode* child = static_cast<Ogre::SceneNode*>(itr.getNext());
                        child->showBoundingBox(bb);
                    }
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_l){
                    static bool al = false;
                    al = !al;
                    if (al){
                        m_world->getEnvironment()->setAmbientLight(0.6f, 0.6f, 0.6f);
                    }
                    else{
                        m_world->getEnvironment()->setAmbientLight(0.f, 0.f, 0.f);
                    }
                }
                
                // Send input commands to the player.
                m_world->handleInput(e);
                break;

            case SDL_KEYUP:
                //m_world->handleInput(e);
                break;

            case SDL_MOUSEMOTION:
                {
                    MouseMove mm = m_world->getInput()->handleMouse(e);
                    ComponentMessage msg(ComponentMessage::Type::Look);
                    msg.data = mm;
                    m_world->getPlayer()->message(msg);
                    m_world->getNetwork()->sendMouseMove(mm.relx, mm.rely);
                }
                break;

            case SDL_CONTROLLERAXISMOTION:                
                {
                    ControllerAxisMotion m = m_world->getInput()->handleControllerAxisMotion(e);
                    //printf("AxisMotion:\n %d, %d\n%d, %d\n", m.x1, m.y1, m.x2, m.y2);

                    ComponentMessage msg(ComponentMessage::Type::Move);
                    msg.data = m;
                    m_world->getPlayer()->message(msg);
                }
                break;

            case SDL_CONTROLLERDEVICEADDED:
                m_world->getInput()->addGamepad(e.cdevice.which);
                break;

            case SDL_CONTROLLERDEVICEREMOVED:
                m_world->getInput()->removeGamepad(e.cdevice.which);
                break;

            case SDL_QUIT:
                m_subject.notify(EngineNotification::Pop);
                return;
            }
        }

        // Step through all World components.
        m_world->update();
        // Process network updates.
        if (m_world->getNetwork()->hasPendingEvent()){
            this->handleNetEvents();
            // Process server reconciliation if needed.
            m_world->getPlayer()->getComponent<NetworkComponent>()->update();
        }

        // Process local player input.
        m_world->getInput()->update();
        while (m_world->getInput()->hasPendingCommand()){
            CommandPtr command = m_world->getInput()->getNextCommand();
            m_world->getNetwork()->sendCommand(command);
            command->execute(m_world->getPlayer());
        }
        m_world->getPlayer()->getComponent<ActorComponent>()->update();
        m_world->getPlayer()->getComponent<WeaponComponent>()->update();
                
        /*if (m_ui->update() == true){
            this->handleUIEvents();
        } */
    }
}

// ========================================================================= //

void GameState::createScene(void)
{
    m_world->init(true);

    m_world->getInput()->setMode(Input::Mode::Player);
    //m_world->getPScene()->loadDebugDrawer();

    // Add systems.
    m_world->addSystem(new CollisionSystem());
    m_world->addSystem(new PhysicsSystem());

    LightComponentPtr lightC = nullptr;
    ModelComponentPtr modelC = nullptr;
    PhysicsComponentPtr physicsC = nullptr;
    TrackComponentPtr track = nullptr;

    // Create player.
    EntityPtr player = m_world->createEntity();
    m_world->attachComponent<ActorComponent>(player)->setPosition(Ogre::Vector3(0.f, 100.f, 0.f));
    m_world->attachComponent<CameraComponent>(player);
    m_world->attachComponent<ModelComponent>(player)->setMesh("Cylinder.mesh");
    m_world->attachComponent<NetworkComponent>(player);
    m_world->attachComponent<WeaponComponent>(player)->setClearDepth(true);
    lightC = m_world->attachComponent<LightComponent>(player);
    lightC->setType(LightComponent::Type::Spotlight);
    lightC->setColour(1.f, 1.f, 1.f);
    lightC->setRange(125.f);

    m_world->setPlayer(player);

    m_world->getEnvironment()->loadEffects();

    //EntityPtr chopper = m_world->createEntity();
    //m_world->attachComponent<SceneComponent>(chopper);
    //m_world->attachComponent<MultiModelComponent>(chopper)->setMesh("Bumblebee.scene", "Board");
    //m_world->attachComponent<PhysicsComponent>(chopper)->setKinematic(true);
    ////chopper->getComponent<PhysicsComponent>()->setType(PhysicsComponent::Type::TriangleMesh);
    //RotationComponentPtr rot = m_world->attachComponent<RotationComponent>(chopper);
    //rot->addRotation(Ogre::Vector3::UNIT_Y, 100.f, "Cylinder.024");
    //rot->addRotation(Ogre::Vector3::UNIT_Y, 100.f, "Cylinder.008");

    // Create basic plane.
    EntityPtr plane = m_world->createEntity();
    m_world->attachComponent<SceneComponent>(plane);
    m_world->attachComponent<ModelComponent>(plane)->setMesh("Plane/Board", "MarbleWall");
    m_world->attachComponent<RotationComponent>(plane)->addRotation(
        Ogre::Vector3::UNIT_Y, 0.3f);
    m_world->attachComponent<PhysicsComponent>(plane)->setKinematic(true);

    // Setup visual scene settings.
    m_world->getEnvironment()->setAmbientLight(0.6f, 0.6f, 0.6f);
    //m_world->getEnvironment()->setSunColour(2.f, 1.75f, 1.89f);
    //m_world->getEnvironment()->setMoonColour(.50f, .50f, 5.f);

    // Create Ocean.
#ifdef _DEBUG
    m_world->getEnvironment()->loadOcean("Ocean2_HLSL_GLSL");
#else
    m_world->getEnvironment()->loadOcean("HydraxDemo.hdx");
#endif

    m_world->getEnvironment()->setOceanPosition(0.f, -100.f, 0.f);

    // Create sky.
    /*m_world->getEnvironment()->loadSky();
    m_world->getEnvironment()->getSky()->loadPreset(
        SkyPresets[SkyPreset::Thunderstorm2]);*/
}

// ========================================================================= //

void GameState::handleNetEvents(void)
{
    NetEvent e = m_world->getNetwork()->getNextEvent();
    for (; 
         m_world->getNetwork()->hasPendingEvent();
         e = m_world->getNetwork()->getNextEvent()){
        switch (e.type){
        default:
            break;

        case NetMessage::ClientDisconnect:
            // @TODO: Remove entity from world.
            break;

        case NetMessage::EndGame:
            m_world->getNetwork()->endGame();
            m_subject.notify(EngineNotification::Pop);
            break;

        case NetMessage::PlayerUpdate:
            {
                // Get player's EntityID.
                EntityID id = boost::get<TransformUpdate>(e.data).id;

                // Get the player's Entity.
                EntityPtr entity = m_world->getEntityPtr(id);
                Assert(entity != nullptr, 
                       "Null player entity in net player update");

                // Send messages to player entity to update transform.
                ComponentMessage msg(ComponentMessage::Type::TransformUpdate);
                msg.data = boost::get<TransformUpdate>(e.data);
             
                if (id == m_world->getPlayer()->getID()){
                    entity->getComponent<NetworkComponent>()->message(msg);
                }
                else{
                    entity->message(msg);
                }
            }
            break;
        }
    }
}

// ========================================================================= //

void GameState::handleUIEvents(void)
{
    /*UIEvent e = m_ui->getNextEvent();
    for (; e.type != UIEvent::None; e = m_ui->getNextEvent()){
        switch (e.type){
        default:
            break;
        
        }
    }*/
}

// ========================================================================= //

void GameState::addNetworkPlayers(void)
{
    Network::PlayerList& players = m_world->getNetwork()->getPlayerList();
    for (auto& i : players){
        // Prevent assigning new entity to local player.
        if (i.second.entity != nullptr){
            continue;
        }

        // Create entity.
        EntityPtr e = m_world->createEntity();

        // Attach player components.
        m_world->attachComponent<ActorComponent>(e);
        m_world->attachComponent<ModelComponent>(e)->setMesh("Cylinder.mesh", "Board.jpg");
        LightComponentPtr lightC = 
            m_world->attachComponent<LightComponent>(e);
        lightC->setType(LightComponent::Type::Spotlight);
        lightC->setColour(1.f, 1.f, 1.f);
        lightC->setRange(1000.f);
        m_world->attachComponent<WeaponComponent>(e);

        i.second.entity = e;
    }
}

// ========================================================================= //