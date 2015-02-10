// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
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
}

// ========================================================================= //