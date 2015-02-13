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
// File: Input.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Input class.
// ========================================================================= //

#include "Command/CommandRepository.hpp"
#include "Component/ComponentMessage.hpp"
#include "Entity/Entity.hpp"
#include "Input.hpp"

// ========================================================================= //

Input::Input(void) :
m_commandRepo(new CommandRepository()),
m_keymap(),
m_mode(Mode::UI)
{
    // @TODO: For every key on keyboard read from keymap file
    // (add mapKey() function which is a switch-case on the command).
    for (unsigned int i = 0; i < SDLK_z; ++i){
        m_keymap.insert(KM_VT(i, m_commandRepo->NullCommand));
    }
    m_keymap.insert(KM_VT(SDLK_LALT, m_commandRepo->NullCommand));

    m_keymap.find(SDLK_w)->second = m_commandRepo->MoveForwardCommand;
    m_keymap.find(SDLK_s)->second = m_commandRepo->MoveBackwardCommand;
    m_keymap.find(SDLK_a)->second = m_commandRepo->MoveLeftCommand;
    m_keymap.find(SDLK_d)->second = m_commandRepo->MoveRightCommand;
    m_keymap.find(SDLK_SPACE)->second = m_commandRepo->SpectatorCommand;
}

// ========================================================================= //

Input::~Input(void)
{

}

// ========================================================================= //

const CommandPtr Input::handle(const SDL_Event& e)
{
    ComponentMessage msg;

    switch (e.type){
    default:
        break;

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        if (m_mode == Mode::UI){
            // Convert SDL button to CEGUI button.
            CEGUI::MouseButton button;
            switch (e.button.button){
            default:
                return m_commandRepo->NullCommand;

            case SDL_BUTTON_LEFT:
                button = CEGUI::MouseButton::LeftButton;
                break;

            case SDL_BUTTON_RIGHT:
                button = CEGUI::MouseButton::RightButton;
                break;

            case SDL_BUTTON_MIDDLE:
                button = CEGUI::MouseButton::MiddleButton;
                break;
            }

            // Inject the mouse button event.
            if (e.type == SDL_MOUSEBUTTONDOWN){
                CEGUI::System::getSingleton().getDefaultGUIContext().
                    injectMouseButtonDown(button);
            }
            else if (e.type == SDL_MOUSEBUTTONUP){
                CEGUI::System::getSingleton().getDefaultGUIContext().
                    injectMouseButtonUp(button);
            }
        }
        else{

        }
        break;

    case SDL_MOUSEMOTION:
        if (m_mode == Mode::UI){
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectMousePosition(static_cast<float>(e.motion.x), 
                static_cast<float>(e.motion.y));
        }
        else{
            // Return modified MouseMoveCommand.
            LookCommandPtr lc = m_commandRepo->LookCommand;
            lc->setXY(e.motion.xrel, e.motion.yrel);
            return lc;
        }
        break;

    case SDL_KEYDOWN:
        if (m_mode == Mode::UI){
            
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectKeyDown((CEGUI::Key::Scan)e.key.keysym.scancode);
        }
        else{
            switch (e.key.keysym.sym){
            default:
                return m_keymap.find(e.key.keysym.sym)->second;

            case SDLK_ESCAPE:
                return nullptr;
            }
        }
        break;

    case SDL_KEYUP:
        if (m_mode == Mode::UI){
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectKeyUp((CEGUI::Key::Scan)e.key.keysym.scancode);
        }
        else{
            switch (e.key.keysym.sym){
            default:
                return m_keymap.find(e.key.keysym.sym)->second;

            case SDLK_ESCAPE:
                return nullptr;
            }
        }
        break;
    }

    return 0;
}

// ========================================================================= //