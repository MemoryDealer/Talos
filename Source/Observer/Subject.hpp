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