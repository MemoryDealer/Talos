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
// File: ParticleComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements ParticleComponent class.
// ========================================================================= //

#include "ParticleComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

ParticleComponent::ParticleComponent(void) :
m_ps(nullptr)
{

}

// ========================================================================= //

ParticleComponent::~ParticleComponent(void)
{

}

// ========================================================================= //

void ParticleComponent::init(void)
{
    m_ps = this->getWorld()->getSceneManager()->createParticleSystem("Test",
                                                                     "PurpleFountain");
}

// ========================================================================= //

void ParticleComponent::destroy(void)
{

}

// ========================================================================= //

void ParticleComponent::update(void)
{

}

// ========================================================================= //

void ParticleComponent::message(ComponentMessage& msg)
{

}

// ========================================================================= //

void ParticleComponent::setup(Ogre::SceneNode* node)
{
    node->attachObject(m_ps);
}

// ========================================================================= //