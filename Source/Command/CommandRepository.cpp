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
// File: CommandRepository.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements CommandRepository struct.
// ========================================================================= //

#include "CommandRepository.hpp"
#include "Command/NullCommand.hpp"
#include "Command/Actor/Jump.hpp"
#include "Command/Actor/MoveBackward.hpp"
#include "Command/Actor/MoveForward.hpp"
#include "Command/Actor/MoveLeft.hpp"
#include "Command/Actor/MoveRight.hpp"
#include "Command/Actor/Debug/Spectator.hpp"

// ========================================================================= //

CommandRepository::CommandRepository(void) :
m_commands()
{
    CommandPtr command = nullptr;

    command = new NullCommand();
    this->addCommand(command);

    command = new MoveBackwardCommand();
    this->addCommand(command);

    command = new MoveForwardCommand();
    this->addCommand(command);

    command = new MoveLeftCommand();
    this->addCommand(command);

    command = new MoveRightCommand();
    this->addCommand(command);

    command = new JumpCommand();
    this->addCommand(command);

    command = new SpectatorCommand();
    this->addCommand(command);
}

// ========================================================================= //

CommandRepository::~CommandRepository(void)
{
    for (auto& i : m_commands){
        delete i.second;
    }

    m_commands.clear();
}

// ========================================================================= //

void CommandRepository::addCommand(CommandPtr command)
{
    m_commands[command->type] = command;
}

// ========================================================================= //

CommandPtr CommandRepository::getCommand(const CommandType type)
{
    if (m_commands.count(type) != 0){
        return m_commands[type];
    }

    return m_commands[CommandType::Null];
}

// ========================================================================= //