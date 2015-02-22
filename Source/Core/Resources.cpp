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
// File: Resources.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements helper function(s) to manage resources.
// ========================================================================= //

#include "stdafx.hpp"
#include "Resources.hpp"

// ========================================================================= //

void loadOgreResources(void)
{
    Ogre::String sec, type, arch;
    Ogre::ConfigFile cf;
#ifdef _DEBUG
    cf.load("resources_d.cfg");
#else
    cf.load("resources.cfg");
#endif

    // Parse each resource location in resource config file.
    Ogre::ConfigFile::SectionIterator i = cf.getSectionIterator();
    while (i.hasMoreElements()){
        sec = i.peekNextKey();

        Ogre::ConfigFile::SettingsMultiMap* settings = i.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator si;
        for (si = settings->begin(); si != settings->end(); ++si){
            type = si->first;
            arch = si->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                arch,
                type,
                sec);
        }
    }

    // Now initialize the parsed resource groups.
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Also set the default texture settings here.
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
}

// ========================================================================= //

void loadMeshes(void)
{
    // Planes.
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

    Ogre::MeshManager::getSingleton().
        createPlane("Plane/Board", 
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,                                  
        plane, 
        150.f, 
        150.f, 
        20, 
        20, 
        true, 
        1, 
        12.f, 
        12.f, 
        Ogre::Vector3::UNIT_Z);
}

// ========================================================================= //

void loadCEGUIResources(void)
{    
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    // Setup default CEGUI skins.
    // @TODO: Load from config file.
    CEGUI::SchemeManager::getSingleton().createFromFile(
        "AlfiskoSkin.scheme");
    /*CEGUI::SchemeManager::getSingleton().createFromFile(
    "GameMenu.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile(
    "Generic.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile(
    "VanillaSkin.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile(
    "TaharezLook.scheme");*/
    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    CEGUI::System::getSingleton().getDefaultGUIContext().
        setDefaultFont("DejaVuSans-10");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().
        setDefaultImage("AlfiskoSkin/MouseArrow");
}

// ========================================================================= //