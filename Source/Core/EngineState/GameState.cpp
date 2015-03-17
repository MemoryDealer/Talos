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
#include "Component/ActorComponent.hpp"
#include "Component/CameraComponent.hpp"
#include "Component/CollisionComponent.hpp"
#include "Component/ComponentMessage.hpp"
#include "Component/LightComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/NetworkComponent.hpp"
#include "Component/PhysicsComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Core/EngineNotifications.hpp"
#include "GameState.hpp"
#include "Input/Input.hpp"
#include "Network/Network.hpp"
#include "Network/Update.hpp"
#include "Physics/PScene.hpp"
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
    m_world.init(true);
    m_world.getInput()->setMode(Input::Mode::Player);
    //m_world.getPScene()->loadDebugDrawer();

    // Add systems.
    m_world.addSystem(new CollisionSystem());
    m_world.addSystem(new PhysicsSystem());

    // Create player.
    EntityPtr player = m_world.createEntity();
    m_world.attachComponent<ActorComponent>(player);
    if (m_world.getNetwork()->getMode() == Network::Mode::Client){
        //player->getComponent<ActorComponent>()->setRemote(true);
    }
    m_world.attachComponent<CameraComponent>(player);
    m_world.attachComponent<ModelComponent>(player)->init(
        m_world, "Cylinder.mesh");
    m_world.attachComponent<NetworkComponent>(player);

    m_world.setPlayer(player);

    LightComponentPtr lightC = nullptr;
    ModelComponentPtr modelC = nullptr;
    PhysicsComponentPtr physicsC = nullptr;

    // Create basic plane.
    EntityPtr plane = m_world.createEntity();
    m_world.attachComponent<SceneComponent>(plane);
    modelC = m_world.attachComponent<ModelComponent>(plane);
    modelC->init(m_world, "Plane/Board", "Board");      
    m_world.attachComponent<CollisionComponent>(plane);
    ComponentMessage msg(ComponentMessage::Type::Translate);
    msg.data = Ogre::Vector3(0.f, -50.f, 0.f);
    plane->message(msg);

    plane = m_world.createEntity();
    m_world.attachComponent<SceneComponent>(plane);
    modelC = m_world.attachComponent<ModelComponent>(plane);
    modelC->init(m_world, "Plane/Board", "Board");
    m_world.attachComponent<CollisionComponent>(plane);
    msg.data = Ogre::Vector3(75.f, 0.f, 0.f);
    plane->message(msg);

    // Create ball.
    EntityPtr ball = m_world.createEntity();
    m_world.attachComponent<SceneComponent>(ball);
    modelC = m_world.attachComponent<ModelComponent>(ball);
    modelC->init(m_world, "icosphere.mesh");
    lightC = m_world.attachComponent<LightComponent>(ball);
    lightC->setType(LightComponent::Type::POINT);
    lightC->setColour(50.f, 0.f, 50.f);
    lightC->setRange(175.f);
    physicsC = m_world.attachComponent<PhysicsComponent>(ball);
    physicsC->setType(PhysicsComponent::Type::Sphere);
    msg.data = Ogre::Vector3(10.f, 0.f, 0.f);
    ball->message(msg);
    
    
    // Setup visual scene settings.
    m_world.getEnvironment()->setAmbientLight(255.f, 255.f, 255.f);
    m_world.getEnvironment()->setSunColour(20.f, 17.5f, 18.9f);
    m_world.getEnvironment()->setMoonColour(.50f, .50f, 255.f);

    // Create Ocean.
#ifdef _DEBUG
    m_world.getEnvironment()->loadOcean("Ocean2_HLSL_GLSL");
#else
    m_world.getEnvironment()->loadOcean("HydraxDemo.hdx");
#endif

    m_world.getEnvironment()->setOceanPosition(0.f, -100.f, 0.f);

    // Create sky.
    m_world.getEnvironment()->loadSky();

    // Network game setup.
    if (m_world.getNetwork()->initialized()){
        this->addNetworkPlayers();
        m_world.getNetwork()->setGameActive(true);
    }    

    if (!m_world.setupEntities()){
        throw std::exception("GameState entities reported uninitialized");
    }
}

// ========================================================================= //

void GameState::exit(void)
{
    m_world.destroy();
    
}

// ========================================================================= //

void GameState::pause(void)
{
    m_world.pause();
}

// ========================================================================= //

void GameState::resume(void)
{
    m_world.resume();
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
                // Temporary exit handling.
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                    m_world.getNetwork()->endGame();
                    m_subject.notify(EngineNotification::Pop);
                    return;
                }

                // Send input commands to the player.
                m_world.handleInput(e);
                break;

            case SDL_KEYUP:
                //m_world.handleInput(e);
                break;

            case SDL_MOUSEMOTION:
                {
                    MouseMove mm = m_world.getInput()->handleMouse(e);
                    ComponentMessage msg(ComponentMessage::Type::Look);
                    msg.data = mm;
                    m_world.getPlayer()->message(msg);
                    m_world.getNetwork()->sendMouseMove(mm.relx, mm.rely);
                }
                break;

            case SDL_QUIT:
                m_subject.notify(EngineNotification::Pop);
                return;
            }
        }

        

        

        m_world.update();
        if (m_world.getNetwork()->hasPendingEvent()){
            this->handleNetEvents();
            m_world.getPlayer()->getComponent<NetworkComponent>()->update(m_world);
        }

        // ^ ? //

        m_world.getInput()->update();
        while (m_world.getInput()->hasPendingCommand()){
            CommandPtr command = m_world.getInput()->getNextCommand();
            m_world.getNetwork()->sendCommand(command);
            command->execute(m_world.getPlayer());
        }
        m_world.getPlayer()->getComponent<ActorComponent>()->update(m_world);

        /*m_world.update();
        if (m_world.getNetwork()->hasPendingEvent()){
            this->handleNetEvents();
            m_world.getPlayer()->getComponent<NetworkComponent>()->update(m_world);
        }

        m_world.getInput()->update();
        while (m_world.getInput()->hasPendingCommand()){
            CommandPtr command = m_world.getInput()->getNextCommand();
            m_world.getNetwork()->sendCommand(command);
            command->execute(m_world.getPlayer());

        }*/

        
        
        /*if (m_ui->update() == true){
            this->handleUIEvents();
        } */
    }
}

// ========================================================================= //

void GameState::handleNetEvents(void)
{
    NetEvent e = m_world.getNetwork()->getNextEvent();
    for (; 
         m_world.getNetwork()->hasPendingEvent();
         e = m_world.getNetwork()->getNextEvent()){
        switch (e.type){
        default:
            break;

        case NetMessage::ClientDisconnect:
            // @TODO: Remove entity from world.
            break;

        case NetMessage::EndGame:
            m_world.getNetwork()->endGame();
            m_subject.notify(EngineNotification::Pop);
            break;

        case NetMessage::PlayerUpdate:
            {
                // Get player's EntityID.
                EntityID id = boost::get<TransformUpdate>(e.data).id;

                // Get the player's Entity.
                EntityPtr entity = m_world.getEntityPtr(id);
                Assert(entity != nullptr, 
                       "Null player entity in net player update");

                // Send messages to player entity to update transform.
                ComponentMessage msg(ComponentMessage::Type::TransformUpdate);
                msg.data = boost::get<TransformUpdate>(e.data);
             
                if (id == m_world.getPlayer()->getID()){
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
    Network::PlayerList& players = m_world.getNetwork()->getPlayerList();
    for (auto& i : players){
        // Prevent assigning new entity to local player.
        if (i.second.entity != nullptr){
            continue;
        }

        // Create entity.
        EntityPtr e = m_world.createEntity();

        // Attach player components.
        m_world.attachComponent<ActorComponent>(e);
        m_world.attachComponent<ModelComponent>(e)->init(
            m_world, "Cylinder.mesh");

        i.second.entity = e;
    }
}

// ========================================================================= //