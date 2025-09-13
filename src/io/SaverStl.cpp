//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-04 22:14:44 gtaubin>
//------------------------------------------------------------------------
//
// SaverStl.cpp
//
// Written by: Luca Jaichenco
//
// Software developed for the course
// Digital Geometry Processing
// Copyright (c) 2025, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "SaverStl.hpp"

#include "wrl/Shape.hpp"
#include "wrl/Appearance.hpp"
#include "wrl/Material.hpp"
#include "wrl/IndexedFaceSet.hpp"

#include "core/Faces.hpp"

const char* SaverStl::_ext = "stl";

//////////////////////////////////////////////////////////////////////
bool SaverStl::save(const char* filename, SceneGraph& wrl) const {
  bool success = false;
  if(filename!=(char*)0) {

    // Check these conditions

    // 1) the SceneGraph should have a single child
    // 2) the child should be a Shape node
    // 3) the geometry of the Shape node should be an IndexedFaceSet node

    Node* node = wrl[0];

    // if (wrl.getNumberOfChildren() > 1)
    //   return false;

    // if (!node->isShape())
    //   return false;

    // if (!node->isIndexedFaceSet())
    //   return false;

    // - construct an instance of the Faces class from the IndexedFaceSet
    // - remember to delete it when you are done with it (if necessary)
    //   before returning

    // 4) the IndexedFaceSet should be a triangle mesh
    // 5) the IndexedFaceSet should have normals per face
    Shape *shape = (Shape*) node;
    node = shape->getGeometry();
    IndexedFaceSet* indexedFaceSet = (IndexedFaceSet*)node;

    // if (indexedFaceSet->isTriangleMesh())
    //   return false;

    // if (indexedFaceSet->getNumberOfNormal() == indexedFaceSet->getNumberOfFaces())
    //   return false;
    
    // if (all the conditions are satisfied) {

    Faces* faces = new Faces(2, indexedFaceSet->getCoordIndex());
    int nFaces = indexedFaceSet->getNumberOfFaces();
    vector<float> allCords = indexedFaceSet->getCoord();
    
    // Get the computed normals
    vector<float> normals = indexedFaceSet->getNormal();

    FILE* fp = fopen(filename,"w");
    if(	fp!=(FILE*)0) {

      // if set, use ifs->getName()
      // otherwise use filename,
      // but first remove directory and extension

      fprintf(fp,"solid %s\n",filename);

      for (int i = 0; i < nFaces; i++) {
        float nx = normals[i*3 + 0];
        float ny = normals[i*3 + 1];
        float nz = normals[i*3 + 2];
        
        fprintf(fp,"facet normal %f %f %f\n", nx, ny, nz);
        fprintf(fp,"outer loop\n");

        for (int j = 0; j < 3; j++) {
          int vertex = faces->getFaceVertex(i, j);
          float coord0 = allCords[vertex*3+0];
          float coord1 = allCords[vertex*3+1];
          float coord2 = allCords[vertex*3+2];

          fprintf(fp,"vertex %f %f %f\n", coord0, coord1, coord2);
        }
        fprintf(fp,"endloop\n");
        fprintf(fp,"endfacet\n");
      }
      fprintf(fp,"endsolid %s\n", filename);
      fclose(fp);
      success = true;
    }

    // } endif (all the conditions are satisfied)

  }
  return success;
}
