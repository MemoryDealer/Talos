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
#include "Command/NullCommand.hpp"
#include "Command/Actor/Look.hpp"
#include "Command/Actor/MoveBackward.hpp"
#include "Command/Actor/MoveForward.hpp"
#include "Command/Actor/MoveLeft.hpp"
#include "Command/Actor/MoveRight.hpp"
#include "Command/Actor/Debug/Spectator.hpp"
#include "Component/ComponentMessage.hpp"
#include "Entity/Entity.hpp"
#include "Input.hpp"
#include "UI/UI.hpp"

// ========================================================================= //

Input::Input(void) :
m_commandRepo(new CommandRepository()),
m_keymap(),
m_mode(Mode::UI)
{
    // @TODO: For every key on keyboard read from keymap file
    // (add mapKey() function which is a switch-case on the command).
    for (unsigned int i = 0; i < SDLK_z; ++i){
        //m_keymap.insert(KM_VT(i, m_commandRepo->NullCommand));
        m_keymap[i] = m_commandRepo->getCommand(CommandType::Null);
    }
    m_keymap[SDLK_LALT] = m_commandRepo->getCommand(CommandType::Null);

    m_keymap[SDLK_w] = m_commandRepo->getCommand(CommandType::MoveForward);
    m_keymap[SDLK_s] = m_commandRepo->getCommand(CommandType::MoveBackward);
    m_keymap[SDLK_a] = m_commandRepo->getCommand(CommandType::MoveLeft);
    m_keymap[SDLK_d] = m_commandRepo->getCommand(CommandType::MoveRight);
    m_keymap[SDLK_SPACE] = m_commandRepo->getCommand(CommandType::Spectator);
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
                return m_commandRepo->getCommand(CommandType::Null);

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
            LookCommand* lc = static_cast<LookCommand*>(
                m_commandRepo->getCommand(CommandType::Look));
            lc->setXY(e.motion.xrel, e.motion.yrel);
            return lc;
        }
        break;

    case SDL_KEYDOWN:
        if (m_mode == Mode::UI){
            CEGUI::Key::Scan kc = SDLKeyToCEGUIKey(e.key.keysym.sym);
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectKeyDown(kc);
            
            
        }
        else{
            switch (e.key.keysym.sym){
            default:
                //return m_keymap.find(e.key.keysym.sym)->second;
                return m_keymap[e.key.keysym.sym];

            case SDLK_ESCAPE:
                return nullptr;
            }
        }
        break;

    case SDL_TEXTINPUT:
        if (m_mode == Mode::UI){
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectChar(e.text.text[0]);
        }
        break;

    case SDL_KEYUP:
        if (m_mode == Mode::UI){
            CEGUI::Key::Scan kc = SDLKeyToCEGUIKey(e.key.keysym.sym);
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectKeyUp(kc);
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

    return nullptr;
}

// ========================================================================= //

void Input::setMode(const Mode mode)
{
    m_mode = mode;
    if (m_mode == Mode::Player){
        SDL_SetRelativeMouseMode(SDL_TRUE);
        CEGUI::System::getSingleton().
            getDefaultGUIContext().getMouseCursor().hide();
    }
    else if (m_mode == Mode::UI){
        SDL_SetRelativeMouseMode(SDL_FALSE);
        CEGUI::System::getSingleton().
            getDefaultGUIContext().getMouseCursor().show();
    }
}

// ========================================================================= //