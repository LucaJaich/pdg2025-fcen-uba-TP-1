//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-04 22:12:21 gtaubin>
//------------------------------------------------------------------------
//
// LoaderStl.cpp
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

#include <stdio.h>
#include "TokenizerFile.hpp"
#include "LoaderStl.hpp"
#include "StrException.hpp"

#include "wrl/Shape.hpp"
#include "wrl/Appearance.hpp"
#include "wrl/Material.hpp"
#include "wrl/IndexedFaceSet.hpp"

// reference
// https://en.wikipedia.org/wiki/STL_(file_format)

const char* LoaderStl::_ext = "stl";

bool LoaderStl::parseFace(TokenizerFile& tkn, 
                           vector<float>& normal,
                           vector<float>& coord,
                           vector<int>& coordIndex) {
  if (!tkn.equals("facet"))
    throw new StrException("expecting facet, found " + tkn);

  if (!tkn.expecting("normal"))
    throw new StrException("expecting normal, found " + tkn);

  Vec3f faceNormal;
  if (!tkn.getVec3f(faceNormal))
    throw new StrException("expecting 3d vector for normal, found " + tkn);

  normal.push_back(faceNormal.x);
  normal.push_back(faceNormal.y);
  normal.push_back(faceNormal.z);

  if (!(tkn.expecting("outer") && tkn.expecting("loop")))
    throw new StrException("expecting outer loop, found " + tkn);

  while (tkn.get() && !tkn.equals("endloop")) {
    if (!tkn.equals("vertex"))
      throw new StrException("expecting vertex, found " + tkn);

    Vec3f v;
    if (!tkn.getVec3f(v))
      throw new StrException("expecting 3d vector for vertex, found " + tkn);

    coord.push_back(v.x);
    coord.push_back(v.y);
    coord.push_back(v.z);

    coordIndex.push_back((int)(coord.size() / 3 - 1));
  }

  coordIndex.push_back(-1);

  if (!tkn.expecting("endfacet"))
    throw new StrException("expecting endfacet, found " + tkn);

  return true;
}

bool LoaderStl::load(const char* filename, SceneGraph& wrl) {
  bool success = false;

  // clear the scene graph
  wrl.clear();
  wrl.setUrl("");

  FILE* fp = (FILE*)0;
  try {

    if(filename==(char*)0) throw new StrException("filename==null");
    fp = fopen(filename,"r");
    if(fp==(FILE*)0) throw new StrException("fp==(FILE*)0");

    TokenizerFile tkn(fp);
    if(tkn.expecting("solid") && tkn.get()) {

      Shape* shape = new Shape();
      wrl.addChild(shape);

      Appearance* a = new Appearance();
      shape->setAppearance(a);

      Material* m = new Material();
      a->setMaterial(m);

      IndexedFaceSet* ifs = new IndexedFaceSet();
      shape->setGeometry(ifs);

      vector<int>& coordIndex = ifs->getCoordIndex();
      vector<float>& coord = ifs->getCoord();
      vector<float>& normal = ifs->getNormal();

      ifs->setNormalPerVertex(false);

      while (tkn.get() && !tkn.equals("endsolid")) {
        if (!parseFace(tkn, normal, coord, coordIndex)) {
          throw new StrException("failed to parse face");
        }
      }
      success = true;
    }

    fclose(fp);
    
  } catch(StrException* e) { 
    if(fp!=(FILE*)0) fclose(fp);
    fprintf(stderr,"ERROR | %s\n",e->what());
    delete e;
  }

  return success;
}
