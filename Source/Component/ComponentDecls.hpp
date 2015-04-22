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
// File: ComponentDecls.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Forward declares classes for each type of Component and the typedefs for
// pointers to them.
// ========================================================================= //

#ifndef __COMPONENTDECLS_HPP__
#define __COMPONENTDECLS_HPP__

// ========================================================================= //

class Component;
class ActorComponent;
class CameraComponent;
class CollisionComponent;
class LightComponent;
class LinkComponent;
class ModelComponent;
class MultiModelComponent;
class NetworkComponent;
class ParticleComponent;
class PhysicsComponent;
class RotationComponent;
class SceneComponent;
class SoundComponent;
class StatComponent;
class TrackComponent;
class WeaponComponent;

// Other forward declarations.

struct ComponentMessage;

// All component pointer typedefs.

typedef Component* ComponentPtr;
typedef ActorComponent* ActorComponentPtr;
typedef CameraComponent* CameraComponentPtr;
typedef CollisionComponent* CollisionComponentPtr;
typedef LightComponent* LightComponentPtr;
typedef LinkComponent* LinkComponentPtr;
typedef ModelComponent* ModelComponentPtr;
typedef MultiModelComponent* MultiModelComponentPtr;
typedef NetworkComponent* NetworkComponentPtr;
typedef ParticleComponent* ParticleComponentPtr;
typedef PhysicsComponent* PhysicsComponentPtr;
typedef RotationComponent* RotationComponentPtr;
typedef SceneComponent* SceneComponentPtr;
typedef SoundComponent* SoundComponentPtr;
typedef StatComponent* StatComponentPtr;
typedef TrackComponent* TrackComponentPtr;
typedef WeaponComponent* WeaponComponentPtr;

// ========================================================================= //

#endif

// ========================================================================= //