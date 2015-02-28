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
// File: LobbyState.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements LobbyState class.
// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/CameraComponent.hpp"
#include "Component/LightComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Core/EngineNotifications.hpp"
#include "Core/EngineState/EngineStateID.hpp"
#include "Input/Input.hpp"
#include "LobbyState.hpp"
#include "UI/LobbyUI.hpp"
#include "World/Environment.hpp"

// ========================================================================= //

LobbyState::LobbyState(void)
{
    this->setID(EngineStateID::Lobby);
}

// ========================================================================= //

LobbyState::~LobbyState(void)
{

}

// ========================================================================= //

void LobbyState::enter(void)
{
    m_world.init();
    m_world.getInput()->setMode(Input::Mode::UI);

    // Setup visual scene settings.
    m_world.getEnvironment()->setAmbientLight(255.f, 255.f, 255.f);
    m_world.getEnvironment()->setSunColour(200.f, 175.f, 189.f);
    m_world.getEnvironment()->setMoonColour(.50f, .50f, 255.f);

    // Create camera.
    EntityPtr camera = m_world.createEntity();
    m_world.attachComponent<SceneComponent>(camera);
    m_world.attachComponent<CameraComponent>(camera);

    m_world.setMainCamera(camera->getComponent<CameraComponent>());

    // Create sky.
    m_world.getEnvironment()->loadSky();

    // Load UI.
    m_ui.reset(new LobbyUI());
    m_ui->init();

    if (m_world.checkEntities() == false){
        throw std::exception("LobbyState entities reported uninitialized");
    }
}

// ========================================================================= //

void LobbyState::exit(void)
{
    m_ui->destroy();
    m_world.destroy();
}

// ========================================================================= //

void LobbyState::pause(void)
{
    m_ui->setVisible(false);
}

// ========================================================================= //

void LobbyState::resume(void)
{
    m_world.resume();
    m_ui->setVisible(true);
}

// ========================================================================= //

void LobbyState::update(void)
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
        if (m_ui->update() == true){
            this->handleUIEvents();
        }     
    }
}

// ========================================================================= //

void LobbyState::handleUIEvents(void)
{
    int uiEvent = 0;
    while ((uiEvent = m_ui->getNextEvent())){
        switch (uiEvent){
        default:
            break;

        case LobbyUI::Event::Exit:

            break;
        }
    }
}

// ========================================================================= //