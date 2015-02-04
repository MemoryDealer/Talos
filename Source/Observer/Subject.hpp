// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Subject.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Subject class.
// ========================================================================= //

#ifndef __SUBJECT_HPP__
#define __SUBJECT_HPP__

// ========================================================================= //

#include "Observer.hpp"

// ========================================================================= //

class Subject
{
public:
	// Reserves a buffer of one Observer.
	explicit Subject(void);

	// Empty destructor.
	~Subject(void);

	void addObserver(const ObserverPtr);

	void removeObserver(const ObserverPtr);

	// Notifies all attached Observers.
	void notify(const unsigned int);

private:
	ObserverList m_observers;
};

// ========================================================================= //

inline void Subject::addObserver(const ObserverPtr obs){
	m_observers.push_back(obs);
}

inline void Subject::removeObserver(const ObserverPtr obs){
	for (ObserverList::iterator itr = m_observers.begin();
		 itr != m_observers.end();){
		if (*itr == obs){
			itr = m_observers.erase(itr);
		}
		else{
			++itr;
		}
	}
}

// ========================================================================= //

#endif

// ========================================================================= //