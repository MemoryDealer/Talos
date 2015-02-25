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
// File: MainMenuState.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements MainMenuState class.
// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/CameraComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Core/EngineNotifications.hpp"
#include "Input/Input.hpp"
#include "MainMenuState.hpp"
#include "UI/MainMenuUI.hpp"
#include "World/Environment.hpp"

// ========================================================================= //

MainMenuState::MainMenuState(void)
{

}

// ========================================================================= //

MainMenuState::~MainMenuState(void)
{

}

// ========================================================================= //

void MainMenuState::enter(void)
{
    m_world.init();
    m_world.getInput()->setMode(Input::Mode::UI);

    // Setup visual scene settings.
    m_world.getEnvironment()->setAmbientLight(255.f, 255.f, 255.f);
    m_world.getEnvironment()->setSunColour(200.f, 175.f, 189.f);
    m_world.getEnvironment()->setMoonColour(.50f, .50f, 255.f);

    // Create camera.
    EntityPtr camera = m_world.createEntity();
    SceneComponentPtr sceneC = m_world.createSceneComponent();
    sceneC->init(camera, m_world);
    camera->attachComponent(sceneC);
    CameraComponentPtr cameraC = m_world.createCameraComponent();
    cameraC->init(camera, m_world);
    camera->attachComponent(cameraC);

    m_world.setMainCamera(cameraC->getCamera());

    // Create Ocean.
#ifdef _DEBUG
    m_world.getEnvironment()->loadOcean("Ocean2_HLSL_GLSL");
#else
    m_world.getEnvironment()->loadOcean("HydraxDemo.hdx");
#endif

    m_world.getEnvironment()->setOceanPosition(0.f, -100.f, 0.f);


    // Create sky.
    m_world.getEnvironment()->loadSky();

    // Load GUI.
    m_ui.reset(new MainMenuUI());
    m_ui->init();

    if (m_world.checkEntities() == false){
        throw std::exception("MainMenu entities reported uninitialized");
    }
}

// ========================================================================= //

void MainMenuState::exit(void)
{
    m_ui->destroy(); // m_ui deallocated automatically.
    m_world.destroy();
}

// ========================================================================= //

void MainMenuState::update(void)
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
                {
                    // Temporary exit handling.
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                        m_subject.notify(EngineNotification::Pop);
                        return;
                    }

                    // Send input commands to the player.
                    m_world.getInput()->handle(e);
                }
                break;

            case SDL_KEYUP:
                {
                    CommandPtr command = m_world.getInput()->handle(e);
                }
                break;

            case SDL_QUIT:
                m_subject.notify(EngineNotification::Pop);
                return;
            }
        }

        m_world.update();
        m_ui->update();
    }
}

// ========================================================================= //