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
#include "Component/ComponentMessage.hpp"
#include "Component/LightComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Core/EngineNotifications.hpp"
#include "Core/EngineState/EngineStateID.hpp"
#include "Input/Input.hpp"
#include "LobbyState.hpp"
#include "Network/Client/Client.hpp"
#include "Network/Server/Server.hpp"
#include "Rendering/Sky/SkyHighGraphics.hpp"
#include "Rendering/Sky/SkyPresets.hpp"
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
    m_world->init();
    m_world->getInput()->setMode(Input::Mode::UI);

    m_world->getEnvironment()->loadEffects();

    // Setup visual scene settings.
    m_world->getEnvironment()->setAmbientLight(255.f, 255.f, 255.f);
    m_world->getEnvironment()->setSunColour(200.f, 175.f, 189.f);
    m_world->getEnvironment()->setMoonColour(.50f, .50f, 255.f);

    // Create camera.
    EntityPtr camera = m_world->createEntity();
    m_world->attachComponent<SceneComponent>(camera);
    m_world->attachComponent<CameraComponent>(camera);

    m_world->setMainCamera(camera->getComponent<CameraComponent>());

    // Create sky.
    m_world->getEnvironment()->loadSky();
    m_world->getEnvironment()->getSky()->loadPreset(
        SkyPresets[SkyPreset::Thunderstorm2]);

    // Load UI.
    m_ui.reset(new LobbyUI());
    m_ui->init();

    // Setup initial UI data.
    m_ui->insertListboxItem("PlayerList", 
        m_world->getNetwork()->getLocalPlayer()->username);
    m_ui->setUsername(
        m_world->getNetwork()->getLocalPlayer()->username);

    m_world->getNetwork()->unlockEventQueue();

    if (!m_world->setupEntities()){
        throw std::exception("LobbyState entities reported uninitialized");
    }
}

// ========================================================================= //

void LobbyState::exit(void)
{
    // Close network connections.
    m_world->getNetwork()->destroy();

    m_ui->destroy();
    m_world->destroy();
}

// ========================================================================= //

void LobbyState::pause(void)
{
    m_world->pause();
    m_ui->setVisible(false);
}

// ========================================================================= //

void LobbyState::resume(void)
{
    m_world->getInput()->setMode(Input::Mode::UI);
    m_world->resume();
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
            case SDL_KEYDOWN:
            case SDL_TEXTINPUT:
                {
                    // Temporary exit handling.
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                        m_subject.notify(EngineNotification::Pop);
                        return;
                    }

                    // Send input commands to the player.
                    m_world->handleInput(e);
                }
                break;

            case SDL_KEYUP:
                {
                    m_world->handleInput(e);
                }
                break;

            case SDL_MOUSEMOTION:
                m_world->getInput()->handleMouse(e);
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

        m_world->update();
        if (m_world->getNetwork()->hasPendingEvent()){
            this->handleNetEvents();
        }
        if (m_ui->update()){
            this->handleUIEvents();
        }     
    }
}

// ========================================================================= //

void LobbyState::handleNetEvents(void)
{
    NetEvent e = m_world->getNetwork()->getNextEvent();
    for (;
         e.type != NetMessage::Null;
         e = m_world->getNetwork()->getNextEvent()){
        switch (e.type){
        default:
            break;

        case ID_CONNECTION_LOST:
            m_subject.notify(EngineNotification::Pop);
            return;

        case NetMessage::Register:
            {
                m_ui->insertListboxItem("PlayerList", 
                                        boost::get<std::string>(e.data));
            }
            break;

        case NetMessage::ClientDisconnect:
            m_ui->removeListboxItem("PlayerList", 
                                    boost::get<std::string>(e.data));
            break;

        case NetMessage::Chat:
            {
                CEGUI::Window* chat = m_ui->getWindow(
                    LobbyUI::Layer::Root, "Chat");

                chat->appendText(boost::get<std::string>(e.data));
            }
            break;

        case NetMessage::PlayerList:
            {
                m_ui->clearListbox("PlayerList");
            }
            break;

        case NetMessage::StartGame:
            m_subject.notify(EngineNotification::Push, EngineStateID::Game);
            break;
        }
    }
}

// ========================================================================= //

void LobbyState::handleUIEvents(void)
{
    UIEvent e = m_ui->getNextEvent();
    for (; e.type != UIEvent::None; e = m_ui->getNextEvent()){
        switch (e.type){
        default:
            break;

        case LobbyUI::Event::Exit:

            break;

        case LobbyUI::Event::Chat:
            m_world->getNetwork()->chat(e.s1);
            break;

        case LobbyUI::Event::Start:
            if (m_world->getNetwork()->getMode() == Network::Mode::Server){
                m_world->getNetwork()->startGame();
                m_subject.notify(EngineNotification::Push, EngineStateID::Game);
            }
            break;
        }
    }
}

// ========================================================================= //