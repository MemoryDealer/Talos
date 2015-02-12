// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: LightComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines LightComponent class.
// ========================================================================= //

#ifndef __LIGHTCOMPONENT_HPP__
#define __LIGHTCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Lights up objects in the World within its range. Either a point light or a
// spotlight.
class LightComponent : public Component
{
public:
	enum Type{
		POINT = 0,
		SPOTLIGHT
	};

	// Sets type.
	explicit LightComponent(void);

	// Empty destructor.
	virtual ~LightComponent(void) override;

	// Empty.
	virtual void init(EntityPtr, World&) override;

	// Creates Ogre::Light and sets its type.
	virtual void init(World&, const Type);

	// Destroys Ogre::Light using World's Ogre::SceneManager.
	virtual void destroy(EntityPtr, World&) override;

	// Empty.
	virtual void update(EntityPtr, World&) override;

	// Empty.
	virtual void message(const ComponentMessage&) override;

	// Light functions:

	// Sets colour of light.
	void setColour(const Ogre::Real, const Ogre::Real, const Ogre::Real);

	// Sets range of light using linear and quadratic values internally.
	void setRange(const Ogre::Real);

	

	// Getters:

	// Returns pointer to internal Ogre::Light.
	Ogre::Light* getLight(void) const;

private:
	Ogre::Light* m_light;
	Ogre::Real m_intensity;
};

// ========================================================================= //

inline void LightComponent::setColour(const Ogre::Real r, 
									  const Ogre::Real g, 
									  const Ogre::Real b){
	m_light->setDiffuseColour(r, g, b);
	m_light->setSpecularColour(r, g, b);
}

inline void LightComponent::setRange(const Ogre::Real range){
	if (m_light->getType() == Ogre::Light::LT_POINT){
		m_light->setAttenuation(range,
								1.f,
								//4.5f / range,
								//75.f / (range * range));
								255.f / range,
								0.f);
	}
	else{
		m_light->setSpotlightRange(Ogre::Degree(range - 15),
								   Ogre::Degree(range));
	}
}

// Getters:

inline Ogre::Light* LightComponent::getLight(void) const{
	return m_light;
}

// ========================================================================= //

#endif

// ========================================================================= //