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
// File: Cooker.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Cooker class.
// ========================================================================= //

#include "Cooker.hpp"

// ========================================================================= //

Cooker::Cooker(PxPhysics* physx, PxCooking* cookingInterface) :
m_physx(physx),
m_cookingInterface(cookingInterface),
m_defaultMaterial(nullptr)
{

}

// ========================================================================= //

Cooker::~Cooker(void)
{

}

// ========================================================================= //

void Cooker::getMeshInfo(Ogre::MeshPtr mesh, Params& params, MeshInfo& outInfo)
{
    //First, we compute the total number of vertices and indices and init the buffers.
    unsigned int numVertices = 0;
    unsigned int numIndices = 0;

    if (mesh->sharedVertexData) numVertices += mesh->sharedVertexData->vertexCount;
    Ogre::Mesh::SubMeshIterator subMeshIterator = mesh->getSubMeshIterator();
    bool indices32 = true;
    while (subMeshIterator.hasMoreElements())
    {
        Ogre::SubMesh *subMesh = subMeshIterator.getNext();
        if (subMesh->vertexData) numVertices += subMesh->vertexData->vertexCount;
        if (params.addBackfaces)
            numIndices += subMesh->indexData->indexCount * 2;
        else
            numIndices += subMesh->indexData->indexCount;

        //We assume that every submesh uses the same index format
        indices32 = (subMesh->indexData->indexBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
    }

    outInfo.vertices.resize(numVertices);
    outInfo.indices.resize(numIndices);
    outInfo.materials.resize(numIndices / 3);

    unsigned int addedVertices = 0;
    unsigned int addedIndices = 0;
    unsigned int addedMaterialIndices = 0;

    /*
    Read shared vertices
    */
    unsigned int shared_index_offset = 0;
    Ogre::VertexData *shared_vertex_data = mesh->sharedVertexData;
    if (shared_vertex_data)
    {
        const Ogre::VertexElement* posElem =
            shared_vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
        Ogre::HardwareVertexBufferSharedPtr vbuf =
            shared_vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

        shared_index_offset = shared_vertex_data->vertexCount;

        unsigned char* pVertices = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        Ogre::Real* pReal;
        for (size_t i = addedVertices; i < shared_vertex_data->vertexCount; i++)
        {
            posElem->baseVertexPointerToElement(pVertices, &pReal);
            Ogre::Vector3 vec;
            vec.x = (*pReal++) * params.scale.x;
            vec.y = (*pReal++) * params.scale.y;
            vec.z = (*pReal++) * params.scale.z;
            outInfo.vertices[i] = vec;
            pVertices += vbuf->getVertexSize();
        }
        vbuf->unlock();
        addedVertices += shared_vertex_data->vertexCount;

    }

    unsigned int index_offset = 0;

    /*
    Read submeshes
    */
    subMeshIterator = mesh->getSubMeshIterator();
    while (subMeshIterator.hasMoreElements())
    {
        Ogre::SubMesh *subMesh = subMeshIterator.getNext();

        //Read vertex data
        Ogre::VertexData *vertex_data = subMesh->vertexData;
        if (vertex_data)
        {
            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* pVertices = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            Ogre::Real* pReal;
            for (size_t i = addedVertices; i < addedVertices + vertex_data->vertexCount; i++)
            {
                posElem->baseVertexPointerToElement(pVertices, &pReal);
                Ogre::Vector3 vec;
                vec.x = (*pReal++) * params.scale.x;
                vec.y = (*pReal++) * params.scale.y;
                vec.z = (*pReal++) * params.scale.z;
                outInfo.vertices[i] = vec;
                pVertices += vbuf->getVertexSize();
            }
            addedVertices += vertex_data->vertexCount;

            vbuf->unlock();
        }

        //Read index data
        Ogre::IndexData *index_data = subMesh->indexData;
        if (index_data)
        {
            Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

            PxU32 *pIndices = 0;
            if (indices32)
            {
                pIndices = static_cast<PxU32*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            }
            else
            {
                PxU16 *pShortIndices = static_cast<PxU16*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                pIndices = new PxU32[index_data->indexCount];
                for (size_t k = 0; k < index_data->indexCount; k++) pIndices[k] = static_cast<PxU32>(pShortIndices[k]);
            }
            unsigned int bufferIndex = 0;
            if (params.addBackfaces)
            {
                size_t numTris = index_data->indexCount / 3;
                size_t i = addedIndices;
                for (unsigned int x = 0; x < numTris; x++)
                {
                    if (subMesh->useSharedVertices)
                    {
                        if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
                        else outInfo.indices[i] = pIndices[bufferIndex];
                        bufferIndex++;
                        if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i + 1] = pIndices[bufferIndex] + index_offset;
                        else outInfo.indices[i + 1] = pIndices[bufferIndex];
                        bufferIndex++;
                        if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i + 2] = pIndices[bufferIndex] + index_offset;
                        else outInfo.indices[i + 2] = pIndices[bufferIndex];
                        bufferIndex++;
                    }
                    else
                    {
                        outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
                        bufferIndex++;
                        outInfo.indices[i + 1] = pIndices[bufferIndex] + index_offset;
                        bufferIndex++;
                        outInfo.indices[i + 2] = pIndices[bufferIndex] + index_offset;
                        bufferIndex++;
                    }
                    outInfo.indices[i + 3] = outInfo.indices[i + 2];
                    outInfo.indices[i + 4] = outInfo.indices[i + 1];
                    outInfo.indices[i + 5] = outInfo.indices[i];
                    i += 6;
                }
                addedIndices += index_data->indexCount * 2;
            }
            else
            {
                for (size_t i = addedIndices; i < addedIndices + index_data->indexCount; i++)
                {
                    if (subMesh->useSharedVertices)
                    {
                        if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
                        else outInfo.indices[i] = pIndices[bufferIndex];
                    }
                    else outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
                    bufferIndex++;
                }
                addedIndices += index_data->indexCount;
            }
            if (!indices32) delete[] pIndices;

            ibuf->unlock();

            //All triangles of a submesh have the same material
            unsigned int numTris = index_data->indexCount / 3;
            if (params.addBackfaces) numTris *= 2;
            for (size_t i = addedMaterialIndices; i < addedMaterialIndices + numTris; i++)
                outInfo.materials[i] = subMesh->getMaterialName();
            addedMaterialIndices += numTris;
        }

        if (vertex_data) index_offset += vertex_data->vertexCount;

    }
}

// ========================================================================= //

struct OctreeNode
{
    OctreeNode(){
        vPos.x = 0.0f; vPos.y = 0.0f; vPos.z = 0.0f;
        aSubNodes[0] = 0; aSubNodes[1] = 0; aSubNodes[2] = 0; aSubNodes[3] = 0; aSubNodes[4] = 0; aSubNodes[5] = 0; aSubNodes[6] = 0; aSubNodes[7] = 0;
    }
    Ogre::Vector3 vPos;
    OctreeNode* aSubNodes[8];
    std::list<int> liIndices;
};

struct STri
{
    STri(){ i1 = -1; i2 = -1; i3 = -1; }
    STri(int iIndex1, int iIndex2, int iIndex3, Ogre::String material, bool bSort = true)
    {
        if (!bSort)
        {
            i1 = iIndex1;
            i2 = iIndex2;
            i3 = iIndex3;
            return;
        }
        //rotate indices
        if (iIndex2<iIndex1)
        {
            if (iIndex3<iIndex2)
            {//index 3 is the smallest
                i1 = iIndex3;
                i2 = iIndex1;
                i3 = iIndex2;
            }
            else
            {
                i1 = iIndex2;
                i2 = iIndex3;
                i3 = iIndex1;
            }
        }
        else
        {
            i1 = iIndex1;
            i2 = iIndex2;
            i3 = iIndex3;
        }
        mat = material;
    }
    bool operator !=(STri& op){ if (op.i1 != i1 || op.i2 != i2 || op.i3 != i3) return true; return false; }
    bool operator <(STri& op)
    {
        if (op.i1 != i1)
            return i1<op.i1;
        if (op.i2 != i2)
            return i2<op.i2;
        return i3<op.i3;
    }
    int i1, i2, i3;
    Ogre::String mat;
};

//returns current vertex count
int ExtractOctree(OctreeNode* pNode, int iVertexCount, int* aiIndexTable, Ogre::Vector3* aNewVertices)
{
    for (std::list<int>::const_iterator it = pNode->liIndices.begin();
         it != pNode->liIndices.end(); it++)
         aiIndexTable[*it] = iVertexCount;
    aNewVertices[iVertexCount++] = pNode->vPos;
    for (int iSubNode = 0; iSubNode<8; iSubNode++)
    if (pNode->aSubNodes[iSubNode])
    {
        iVertexCount = ExtractOctree(pNode->aSubNodes[iSubNode], iVertexCount, aiIndexTable, aNewVertices);
        delete pNode->aSubNodes[iSubNode];
        pNode->aSubNodes[iSubNode] = nullptr;
    }
    return iVertexCount;
}

#define IS_IN_BOX(v1,v2,d) ((v1.x<=v2.x+d) && (v1.x>=v2.x-d) && (v1.y<=v2.y+d) && (v1.y>=v2.y-d) && (v1.z<=v2.z+d) && (v1.z>=v2.z-d))

#define EIGHTH_SPACE_INDEX(v1,v2) (((v1.x>v2.x)?4:0)+((v1.y>v2.y)?2:0)+((v1.z>v2.z)?1:0))

void Cooker::mergeVertices(MeshInfo& meshInfo, float mergeDist)
{
    OctreeNode root;
    root.vPos = meshInfo.vertices[0];
    int iVertex = 0;
    int numAdded = 0;

    for (; iVertex<(int)meshInfo.vertices.size(); iVertex++)
    {
        OctreeNode* pCurrNode = &root;
        while (true)
        {
            if (IS_IN_BOX(meshInfo.vertices[iVertex], pCurrNode->vPos, mergeDist))
            {
                pCurrNode->liIndices.push_back(iVertex);
                break;
            }
            else
            {//vertex is not in merge distance to this node
                int iSubNode = EIGHTH_SPACE_INDEX(pCurrNode->vPos, meshInfo.vertices[iVertex]);
                if (pCurrNode->aSubNodes[iSubNode])
                    //proceed deeper into the tree
                    pCurrNode = pCurrNode->aSubNodes[iSubNode];
                else
                {//there is no branch so make one
                    pCurrNode->aSubNodes[iSubNode] = new OctreeNode;
                    pCurrNode = pCurrNode->aSubNodes[iSubNode];
                    pCurrNode->vPos = meshInfo.vertices[iVertex];
                    numAdded++;
                }
            }//pCurrNode is now one level lower in the tree
        }
    }
    int* aiIndexTable = new int[meshInfo.vertices.size()];		//maps old indices to new 		
    Ogre::Vector3* aNewVertices = new Ogre::Vector3[meshInfo.vertices.size()];
    //extract indextable and vertex list
    int nNewVertices = ExtractOctree(&root, 0, aiIndexTable, aNewVertices);
    for (unsigned int iIndex = 0; iIndex< meshInfo.indices.size(); ++iIndex)
    {
        assert(meshInfo.indices[iIndex] < (int)meshInfo.indices.size());
        assert(meshInfo.indices[iIndex] >= 0);
        meshInfo.indices[iIndex] = aiIndexTable[meshInfo.indices[iIndex]];
    }

    meshInfo.vertices.resize(nNewVertices);
    for (iVertex = 0; iVertex<nNewVertices; iVertex++)
        meshInfo.vertices[iVertex] = aNewVertices[iVertex];

    delete[] aiIndexTable;
    delete[] aNewVertices;

    //search for duplicated and degenerate tris
    std::vector<STri> vTris;
    vTris.resize(meshInfo.indices.size() / 3);
    int nTrisCopied = 0;
    int iTri = 0;
    for (; iTri<(int)meshInfo.indices.size() / 3; iTri++)
    {//check if this tri is degenerate
        int index1 = meshInfo.indices[iTri * 3 + 0],
            index2 = meshInfo.indices[iTri * 3 + 1],
            index3 = meshInfo.indices[iTri * 3 + 2];
        if (index1 == index2 || index3 == index2 || index1 == index3)
            //degenerate tri: two or more vertices are the same
            continue;
        vTris[nTrisCopied++] = STri(index1, index2, index3, meshInfo.materials[iTri]);
    }
    vTris.resize(nTrisCopied);
    std::sort(vTris.begin(), vTris.end());//sort tris to find duplicates easily
    nTrisCopied = 0;
    STri lastTri;
    for (iTri = 0; iTri<(int)vTris.size(); iTri++)
    {
        if (lastTri != vTris[iTri])
        {
            meshInfo.indices[nTrisCopied * 3 + 0] = vTris[iTri].i1;
            meshInfo.indices[nTrisCopied * 3 + 1] = vTris[iTri].i2;
            meshInfo.indices[nTrisCopied * 3 + 2] = vTris[iTri].i3;
            meshInfo.materials[nTrisCopied] = vTris[iTri].mat;
            lastTri = vTris[iTri];
            nTrisCopied++;
        }
    }
    meshInfo.materials.resize(nTrisCopied);
    meshInfo.indices.resize(nTrisCopied * 3);
}

// ========================================================================= //

void Cooker::cookTriangleMesh(Ogre::MeshPtr mesh,
                              PxOutputStream& outputStream,
                              Params& params,
                              AddedMaterials* addedMaterials)
{
    MeshInfo meshInfo;
    getMeshInfo(mesh, params, meshInfo);
    mergeVertices(meshInfo);

    PxTriangleMeshDesc desc;
    desc.setToDefault();

    desc.points.count = meshInfo.vertices.size();
    desc.points.stride = 12;
    float *fVertices = new float[meshInfo.vertices.size() * 3];
    for (unsigned int i = 0; i < meshInfo.vertices.size(); ++i)
    {
        fVertices[i * 3] = meshInfo.vertices[i].x;
        fVertices[i * 3 + 1] = meshInfo.vertices[i].y;
        fVertices[i * 3 + 2] = meshInfo.vertices[i].z;
    }
    desc.points.data = fVertices;

    desc.triangles.count = meshInfo.indices.size() / 3;
    desc.triangles.stride = 12;
    PxU32 *iIndices = new PxU32[meshInfo.indices.size()];
    for (unsigned int i = 0; i < meshInfo.indices.size(); ++i)
        iIndices[i] = meshInfo.indices[i];
    desc.triangles.data = iIndices;

    std::unordered_map<PxMaterial*, PxMaterialTableIndex> materialIndicesMap;
    std::vector<PxMaterial*> orderedMaterials;
    materialIndicesMap.insert(std::make_pair<PxMaterial*, PxMaterialTableIndex>(&(*m_defaultMaterial), 0));	//add default material at index 0
    orderedMaterials.push_back(m_defaultMaterial);

    PxMaterialTableIndex *materialIndices = nullptr;
    if (!params.materialBindings.empty())
    {
        desc.materialIndices.stride = sizeof(PxMaterialTableIndex);
        materialIndices = new PxMaterialTableIndex[meshInfo.indices.size() / 3];
        for (unsigned int i = 0; i < meshInfo.indices.size() / 3; ++i)
        {
            auto mat = params.materialBindings.find(meshInfo.materials[i]);
            if (mat == params.materialBindings.end())
            {
                materialIndices[i] = 0;		//default material
            }
            else
            {
                auto matIndex = materialIndicesMap.find(mat->second);
                PxMaterialTableIndex index;
                if (matIndex == materialIndicesMap.end())
                {	//create material index if not existing
                    index = materialIndicesMap.size();
                    materialIndicesMap.insert(std::make_pair/*<PxMaterial*, PxMaterialTableIndex>*/(mat->second, index));
                    orderedMaterials.push_back(mat->second);
                }
                else index = matIndex->second;
                materialIndices[i] = index;
            }
        }
    }
    desc.materialIndices.data = materialIndices;

    if (addedMaterials)
    {
        addedMaterials->materialCount = materialIndicesMap.size();
        addedMaterials->materials = new PxMaterial*[addedMaterials->materialCount];
        for (unsigned int i = 0; i < orderedMaterials.size(); ++i)
            addedMaterials->materials[i] = orderedMaterials[i];
    }

    m_cookingInterface->cookTriangleMesh(desc, outputStream);

    delete[] fVertices;
    delete[] iIndices;
    if (materialIndices) delete[] materialIndices;
}

// ========================================================================= //

void Cooker::cookConvexMesh(Ogre::MeshPtr mesh,
                            PxOutputStream& outputStream,
                            Params& params)
{
    MeshInfo meshInfo;
    getMeshInfo(mesh, params, meshInfo);
    mergeVertices(meshInfo, 0.1f);

    PxConvexMeshDesc desc;
    desc.points.count = meshInfo.vertices.size();
    desc.points.stride = 12;
    desc.flags |= PxConvexFlag::eCOMPUTE_CONVEX;

    float *fVertices = new float[meshInfo.vertices.size() * 3];
    for (unsigned int i = 0; i < meshInfo.vertices.size(); ++i)
    {
        fVertices[i * 3] = meshInfo.vertices[i].x;
        fVertices[i * 3 + 1] = meshInfo.vertices[i].y;
        fVertices[i * 3 + 2] = meshInfo.vertices[i].z;
    }
    desc.points.data = fVertices;

    desc.triangles.count = meshInfo.indices.size() / 3;
    desc.triangles.stride = 12;
    int *iIndices = new int[meshInfo.indices.size()];
    for (unsigned int i = 0; i < meshInfo.indices.size(); ++i)
        iIndices[i] = meshInfo.indices[i];
    desc.triangles.data = iIndices;

    m_cookingInterface->cookConvexMesh(desc, outputStream);

    delete[] fVertices;
    delete[] iIndices;
}

// ========================================================================= //

PxTriangleMesh* Cooker::createTriangleMesh(Ogre::MeshPtr mesh,
                                           Params& params,
                                           AddedMaterials* addedMaterials)
{
    PxDefaultMemoryOutputStream stream;
    this->cookTriangleMesh(mesh, stream, params, addedMaterials);
    if (stream.getData() == nullptr) return nullptr;

    return m_physx->createTriangleMesh(PxDefaultMemoryInputData(stream.getData(), stream.getSize()));
}

// ========================================================================= //

PxConvexMesh* Cooker::createConvexMesh(Ogre::MeshPtr mesh,
                                       Params& params)
{
    PxDefaultMemoryOutputStream stream;
    this->cookConvexMesh(mesh, stream, params);
    if (stream.getData() == nullptr) return nullptr;

    return m_physx->createConvexMesh(PxDefaultMemoryInputData(stream.getData(), stream.getSize()));
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void Cooker::setDefaultMaterial(PxMaterial* material)
{
    m_defaultMaterial = material;
}

// ========================================================================= //