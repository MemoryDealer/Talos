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
// File: SystemManager.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines SystemManager class.
// ========================================================================= //

#ifndef __SYSTEMMANAGER_HPP__
#define __SYSTEMMANAGER_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

typedef std::unordered_map<const std::type_info*, System*> SystemTable;

// ========================================================================= //
// Holds a table of Systems which can be added/removed and accessed.
class SystemManager final
{
public:
    // Default initializes SystemTable.
    explicit SystemManager(std::shared_ptr<World> world);

    // Empty destructor.
    ~SystemManager(void);

    // Deletes every System in table.
    void destroy(void);

    // Inserts new System into table.
    void addSystem(System* system);

    // Tests if Entity meets requirements for any Systems in the table and adds
    // it to that System if so.
    void processEntity(EntityPtr entity);

    // Updates each System in table.
    void update(void);

    // Returns true if System exists.
    template<typename T>
    bool hasSystem(void){
        return (m_systems.count(&typeid(T)) != 0);
    }
    
    // Returns System pointer if type.
    template<typename T>
    T* getSystem(void){
        if (m_systems.count(&typeid(T)) != 0){
            return static_cast<T*>(m_systems[&typeid(T)]);
        }
        else{
            return nullptr;
        }
    }

private:
    SystemTable m_systems;
    std::shared_ptr<World> m_world;
};

// ========================================================================= //

#endif

// ========================================================================= //