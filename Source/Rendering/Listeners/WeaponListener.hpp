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
// File: ParticleComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines ParticleComponent class.
// ========================================================================= //

// Listener for rendering the weapon in front of everything else, so it doesn't
// go through objects when player gets close.
class WeaponRenderListener : public Ogre::RenderQueueListener
{
public:
    ~WeaponRenderListener(void) { }

    void renderQueueStarted(Ogre::uint8 queueGroupID,
                            const Ogre::String& invocation,
                            bool& repeatThisInvocation){
        // Clear the depth buffer for entities in queue 9, forcing them to 
        // render in front of other objects.
        if (queueGroupID == Ogre::RenderQueueGroupID::RENDER_QUEUE_9){
            Ogre::Root::getSingleton().
                getRenderSystem()->clearFrameBuffer(Ogre::FBT_DEPTH);
        }
    }
};

// ========================================================================= //