// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: PDebugDrawer.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines PDebugDrawer class.
// ========================================================================= //

#include "Component/SceneComponent.hpp"
#include "Entity/Entity.hpp"
#include "PDebugDrawer.hpp"
#include "Rendering/DynamicLines.hpp"

// ========================================================================= //

PDebugDrawer::PDebugDrawer(void) :
m_nodes()
{
	m_nodes.reserve(256);
}

// ========================================================================= //

PDebugDrawer::~PDebugDrawer(void)
{

}

// ========================================================================= //

void PDebugDrawer::add(PxRigidActor* actor, PxGeometry& geometry)
{
	Node node;
	node.actor = actor;

	// Allocate a new set of DynamicLines. Create points for the type of 
	// geometry on this actor.
	switch (geometry.getType()){
	default:
		break;

	case PxGeometryType::eBOX:

		break;

	case PxGeometryType::eSPHERE:
		{
			PxSphereGeometry sphere = static_cast<PxSphereGeometry&>(geometry);
			const PxReal offset = sphere.radius * 2;
			const int numPoints = 24;
			const Ogre::Vector3 points[numPoints] = {
				{ 0.f, offset, 0.f }, { offset, 0.f, 0.f },
				{ 0.f, offset, 0.f }, { -offset, 0.f, 0.f },
				{ 0.f, offset, 0.f }, { 0.f, 0.f, offset },
				{ 0.f, offset, 0.f }, { 0.f, 0.f, -offset },
				{ 0.f, -offset, 0.f }, { offset, 0.f, 0.f },
				{ 0.f, -offset, 0.f }, { -offset, 0.f, 0.f },
				{ 0.f, -offset, 0.f }, { 0.f, 0.f, offset },
				{ 0.f, -offset, 0.f }, { 0.f, 0.f, -offset },
				{ offset, 0.f, 0.f }, { 0.f, 0.f, offset },
				{ offset, 0.f, 0.f }, { 0.f, 0.f, -offset },
				{ -offset, 0.f, 0.f }, { 0.f, 0.f, offset },
				{ -offset, 0.f, 0.f }, { 0.f, 0.f, -offset }
			};

			node.lines.reset(new DynamicLines());

			for (int i = 0; i < numPoints; ++i){
				node.lines->addPoint(points[i]);
			}
		}
		break;
	}

	// Setup DynamicLines.
	if (node.lines != nullptr){
		// Get EntityPtr from actor's user data.
		EntityPtr entity = static_cast<EntityPtr>(node.actor->userData);
		// Get the Ogre::SceneNode and attach the DynamicLines to it.
		SceneComponentPtr sceneC = static_cast<SceneComponentPtr>(
			entity->getComponentPtr("SceneComponent"));
		Ogre::SceneNode* lnode = sceneC->getSceneNode();
		lnode->attachObject(node.lines.get());

		// Set line colour to red for dynamic actors.
		if (node.actor->getType() == PxActorType::eRIGID_DYNAMIC){
			node.lines->setColour(DynamicLines::Colour::RED);
		}

		// Add to list of debug nodes.
		m_nodes.push_back(node);
	}
}

// ========================================================================= //

void PDebugDrawer::update(void)
{
	for (std::vector<Node>::iterator itr = m_nodes.begin();
		 itr != m_nodes.end();
		 ++itr){
		itr->lines->update();
	}
}

// ========================================================================= //