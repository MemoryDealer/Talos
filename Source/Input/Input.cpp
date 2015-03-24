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
#include "UI/UI.hpp"

// ========================================================================= //

static const Uint8* keystate = SDL_GetKeyboardState(nullptr);

// ========================================================================= //

Input::Input(void) :
m_commandRepo(new CommandRepository()),
m_keymap(),
m_commands(),
m_gamepads(),
m_mode(Mode::UI)
{
    // @TODO: For every key on keyboard read from keymap file
    // (add mapKey() function which is a switch-case on the command).
    for (unsigned int i = 0; i < SDLK_z; ++i){
        //m_keymap.insert(KM_VT(i, m_commandRepo->NullCommand));
        m_keymap[i] = m_commandRepo->getCommand(CommandType::Null);
    }
    m_keymap[SDLK_LALT] = m_commandRepo->getCommand(CommandType::Null);

    // @TODO: Store movement keys (and anything that must be held down in a 
    // list for processing in update.
    m_keymap[SDLK_w] = m_commandRepo->getCommand(CommandType::MoveForward);
    m_keymap[SDLK_s] = m_commandRepo->getCommand(CommandType::MoveBackward);
    m_keymap[SDLK_a] = m_commandRepo->getCommand(CommandType::MoveLeft);
    m_keymap[SDLK_d] = m_commandRepo->getCommand(CommandType::MoveRight);
    m_keymap[SDLK_e] = m_commandRepo->getCommand(CommandType::Action);
    m_keymap[SDLK_f] = m_commandRepo->getCommand(CommandType::Flashlight);
    m_keymap[SDLK_SPACE] = m_commandRepo->getCommand(CommandType::Jump);
    m_keymap[SDLK_LSHIFT] = m_commandRepo->getCommand(CommandType::Spectator);

    // Gamepad map.
    m_gamepadMap[0] = m_commandRepo->getCommand(CommandType::Jump);
    m_gamepadMap[2] = m_commandRepo->getCommand(CommandType::Action);
    m_gamepadMap[10] = m_commandRepo->getCommand(CommandType::Flashlight);
}

// ========================================================================= //

Input::~Input(void)
{

}

// ========================================================================= //

void Input::handle(const SDL_Event& e)
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
                break;

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

    case SDL_KEYDOWN:
        if (m_mode == Mode::UI){
            CEGUI::Key::Scan kc = SDLKeyToCEGUIKey(e.key.keysym.sym);
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectKeyDown(kc);
        }
        else{
            switch (e.key.keysym.sym){
            default:
                {
                    CommandPtr command = m_keymap[e.key.keysym.sym];
                    if (command){
                        this->pushCommand(command);
                    }
                }
                break;

            case SDLK_w:
            case SDLK_s:
            case SDLK_a:
            case SDLK_d:
            case SDLK_ESCAPE:
                break;
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
                break;
            }
        }
        break;

    case SDL_CONTROLLERBUTTONDOWN:
        printf("Controller: %d\n", e.cbutton.button);        
        {
            CommandPtr command = m_gamepadMap[e.cbutton.button];
            this->pushCommand(command);
        }
        break;

    case SDL_CONTROLLERBUTTONUP:

        break;
    }
}

// ========================================================================= //

const MouseMove Input::handleMouse(const SDL_Event& e)
{
    MouseMove mm;
    mm.relx = mm.rely = 0;

    if (m_mode == Mode::UI){
        CEGUI::System::getSingleton().getDefaultGUIContext().
            injectMousePosition(static_cast<float>(e.motion.x),
            static_cast<float>(e.motion.y));
    }
    else{
        mm.relx = e.motion.xrel;
        mm.rely = e.motion.yrel;
    }

    return mm;
}

// ========================================================================= //

const ControllerAxisMotion Input::handleControllerAxisMotion(const SDL_Event& e)
{
    ControllerAxisMotion m;
    m.x1 = m.y1 = m.x2 = m.y2 = 0;

    Assert(m_gamepads.count(e.cdevice.which) != 0, "Invalid controller input");

    // Get the connected gamepad instance.
    SDL_GameController* controller = m_gamepads[e.cdevice.which].controller;
    

    if (m_mode == Mode::UI){

    }
    else{
        m.x1 = SDL_GameControllerGetAxis(controller,
                                        static_cast<SDL_GameControllerAxis>(0));
        m.y1 = SDL_GameControllerGetAxis(controller,
                                        static_cast<SDL_GameControllerAxis>(1));
        m.x2 = SDL_GameControllerGetAxis(controller,
                                         static_cast<SDL_GameControllerAxis>(2));
        m.y2 = SDL_GameControllerGetAxis(controller,
                                         static_cast<SDL_GameControllerAxis>(3));
    }

    return m;
}

// ========================================================================= //

void Input::update(void)
{
    /*if (keystate[SDL_SCANCODE_W]){
        this->pushCommand(m_keymap[SDLK_w]);
    }
    if (keystate[SDL_SCANCODE_S]){
        this->pushCommand(m_keymap[SDLK_s]);
    }
    if (keystate[SDL_SCANCODE_A]){
        this->pushCommand(m_keymap[SDLK_a]);
    }
    if (keystate[SDL_SCANCODE_D]){
        this->pushCommand(m_keymap[SDLK_d]);
    }*/
}

// ========================================================================= //

// Command queue:

// ========================================================================= //

void Input::pushCommand(CommandPtr command)
{
    m_commands.push(command);
}

// ========================================================================= //

const bool Input::hasPendingCommand(void) const
{
    return (m_commands.empty() == false);
}

// ========================================================================= //

CommandPtr Input::getNextCommand(void)
{
    CommandPtr command = m_commands.front();
    m_commands.pop();
    return command;
}

// ========================================================================= //

// Gamepads:

// ========================================================================= //

void Input::addGamepad(const Sint32 id)
{
    // See if this is a gamepad.
    if (SDL_IsGameController(id)){
        Gamepad pad;

        pad.controller = SDL_GameControllerOpen(id);
        if (pad.controller){
            // Get the instance ID to store.
            SDL_Joystick* joy = SDL_JoystickOpen(id);
            pad.id = SDL_JoystickInstanceID(joy);

            // Insert into gamepad table.
            m_gamepads[id] = pad;

            Talos::Log::getSingleton().log("Added gamepad with ID " + 
                                           toString(pad.id));
        }
    }
}

// ========================================================================= //

void Input::removeGamepad(const Sint32 id)
{
    if (m_gamepads.count(id) != 0){
        SDL_GameControllerClose(m_gamepads[id].controller);
        m_gamepads.erase(id);
    }
}

// ========================================================================= //

void Input::addAllConnectedGamepads(void)
{
    for (Sint32 i = 0; i < SDL_NumJoysticks(); ++i){
        this->addGamepad(i);
    }
}

// ========================================================================= //

// Setters:

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