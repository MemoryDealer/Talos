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
// File: Typedefs.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines typedefs and forward declarations used across engine. Anything
// that must be forward declared/typedef'd in more than once place should go
// here.
// ========================================================================= //

#ifndef __TYPEDEFS_HPP__
#define __TYPEDEFS_HPP__

// ========================================================================= //

#include <cstdint>
#include <memory>

// ========================================================================= //

class ActorComponent;
class Client;
class Command;
class CommandRepository;
class ComponentFactory;
class DCC;
class DynamicLines;
class EngineState;
class Entity;
class EntityPool;
class Environment;
class Input;
class KCC;
struct MouseMove;
class Network;
class Ocean;
class PDebugDrawer;
class Physics;
template<typename T> class Pool;
class PScene;
class Server;
class Sky;
struct SkyPreset;
class System;
class SystemManager;
struct TransformUpdate;
class UI;
class World;

typedef ActorComponent* ActorComponentPtr;
typedef Command* CommandPtr;
typedef std::shared_ptr<EngineState> EngineStatePtr;
typedef uint32_t EntityID;
typedef Entity* EntityPtr;
typedef uint32_t NetworkID;
typedef std::shared_ptr<World> WorldPtr;

// ========================================================================= //

#endif

// ========================================================================= //