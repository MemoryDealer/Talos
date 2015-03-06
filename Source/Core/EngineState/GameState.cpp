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
#include "Component/LightComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/PhysicsComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Core/EngineNotifications.hpp"
#include "GameState.hpp"
#include "Input/Input.hpp"
#include "Network/Network.hpp"
#include "Physics/PScene.hpp"
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
    m_world.getInput()->setMode(Input::Mode::PLAYER);
    //m_world.getPScene()->loadDebugDrawer();

    // Add systems.
    m_world.addSystem(new PhysicsSystem());

    // Create player.
    EntityPtr player = m_world.createEntity();
    m_world.attachComponent<ActorComponent>(player);
    m_world.attachComponent<CameraComponent>(player);

    m_world.setPlayer(player);

    LightComponentPtr lightC = nullptr;
    ModelComponentPtr modelC = nullptr;
    PhysicsComponentPtr physicsC = nullptr;

    // Create basic plane.
    EntityPtr plane = m_world.createEntity();
    m_world.attachComponent<SceneComponent>(plane);
    modelC = m_world.attachComponent<ModelComponent>(plane);
    modelC->init(m_world, "Plane/Board", "Board");      
    physicsC = m_world.attachComponent<PhysicsComponent>(plane);
    physicsC->init(m_world, plane->getID(), PhysicsComponent::Type::Static,
                   physx::PxBoxGeometry(75.f, 5.f, 75.f));
    physicsC->translate(0.f, -50.f, 0.f);

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
    physicsC->init(m_world, ball->getID(), PhysicsComponent::Type::Dynamic,
                   PxSphereGeometry(5.5f),
                   0.2f, 0.2f, 0.1f);
    physicsC->getDynamicActor()->addForce(PxVec3(500.f, 0.f, 0.f));
    
    // Setup visual scene settings.
    //m_world.getEnvironment()->setAmbientLight(255.f, 255.f, 255.f);
    //m_world.getEnvironment()->setSunColour(20.f, 17.5f, 18.9f);
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
    

    if (m_world.setupEntities() == false){
        throw std::exception("MainMenu entities reported uninitialized");
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
            case SDL_MOUSEMOTION:
            case SDL_KEYDOWN:
            case SDL_TEXTINPUT:
                {
                    // Temporary exit handling.
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                        m_subject.notify(EngineNotification::Pop);
                        return;
                    }

                    // Send input commands to the player.
                    CommandPtr command = m_world.getInput()->handle(e);
                    if (command){
                        command->execute(m_world.getPlayer());
                        printf("Command: %d\t%d\n", sizeof(command), typeid(*command).hash_code());
                    }
                }
                break;

            case SDL_KEYUP:
                {
                    CommandPtr command = m_world.getInput()->handle(e);
                    if (command){
                        command->unexecute(m_world.getPlayer());
                    }
                }
                break;

            case SDL_QUIT:
                m_subject.notify(EngineNotification::Pop);
                return;
            }
        }

        m_world.update();
        if (m_world.getNetwork()){
            if (m_world.getNetwork()->hasPendingEvent()){
                this->handleNetEvents();
            }
        }
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
         e.type != NetMessage::Null;
         e = m_world.getNetwork()->getNextEvent()){
        /*switch (e.type){
        default:
            break;
        }*/
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