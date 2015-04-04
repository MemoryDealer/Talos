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
// File: Cooker.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Cooker class.
// ========================================================================= //

#ifndef __COOKER_HPP__
#define __COOKER_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

using namespace physx;

// ========================================================================= //
// Handles mesh cooking for PhysX.
class Cooker final
{
public:
    // Stores PxPhysics and cooking interface pointers.
    explicit Cooker(PxPhysics* physx, PxCooking* cookingInterface);

    ~Cooker(void);

    // Structs:

    struct Params{
        explicit Params(void) :
        scale(Ogre::Vector3(1.f, 1.f, 1.f)),
        materialBindings(),
        addBackfaces(false) { }

        Ogre::Vector3 scale;
        std::map<Ogre::String, PxMaterial*> materialBindings;
        bool addBackfaces;
    };

    struct AddedMaterials{
        explicit AddedMaterials(void) :
        materials(nullptr),
        materialCount(0.f) { }

        ~AddedMaterials(void){
            if (materials){
                delete[] materials;
            }
        }

        PxMaterial** materials;
        PxU32 materialCount;
    };

    struct MeshInfo{
        std::vector<Ogre::Vector3> vertices;
        std::vector<int32_t> indices;
        std::vector<Ogre::String> materials;
    };

    // Cooking functions:

    void getMeshInfo(Ogre::MeshPtr mesh, Params& params, MeshInfo& outInfo);

    void mergeVertices(MeshInfo& info, float mergeDist = 1e-3f);

    //void insetMesh(MeshInfo& info, float amount);

    void cookTriangleMesh(Ogre::MeshPtr mesh,
                          PxOutputStream& outputStream,
                          Params& params = Params(),
                          AddedMaterials* addedMaterials = nullptr);

    void cookConvexMesh(Ogre::MeshPtr mesh,
                        PxOutputStream& outputStream,
                        Params& params = Params());

    PxTriangleMesh* createTriangleMesh(Ogre::MeshPtr mesh,
                                       Params& params = Params(),
                                       AddedMaterials* addedMaterials = nullptr);

    PxConvexMesh* createConvexMesh(Ogre::MeshPtr mesh,
                                   Params& params = Params());

    // Setters:

    void setDefaultMaterial(PxMaterial* material);

private:
    PxPhysics* m_physx;
    PxCooking* m_cookingInterface;
    PxMaterial* m_defaultMaterial;
};

// ========================================================================= //

#endif

// ========================================================================= //