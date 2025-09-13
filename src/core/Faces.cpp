//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-09-13 22:09:56 gtaubin>
//------------------------------------------------------------------------
//
// Faces.cpp
//
// Written by: Luca Jaichenco (corrected minimal version)
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

#include <math.h>
#include "Faces.hpp"
  
Faces::Faces(const int nV, const vector<int>& coordIndex) {
  this->coordIndex = coordIndex;

  // compute number of vertices = max index + 1
  int maxIndex = nV - 1;
  for (int i = 0; i < (int)coordIndex.size(); i++) {
    if (coordIndex[i] >= 0 && coordIndex[i] > maxIndex) {
      maxIndex = coordIndex[i];
    }
  }
  _nV = maxIndex + 1;
}

int Faces::getNumberOfVertices() const {
  return _nV;
}

int Faces::getNumberOfFaces() const {
  int result = 0;
  for (int i = 0; i < (int)coordIndex.size(); i++) {
    if (coordIndex[i] == -1) {
      result++;
    }
  }
  return result;
}

int Faces::getNumberOfCorners() const {
  return (int)coordIndex.size();
}

int Faces::getFaceSize(const int iF) const {
  if (iF < 0 || iF >= getNumberOfFaces()) {
    return 0;
  }

  int currentFace = 0;
  int result = 0;
  for (int i = 0; i < (int)coordIndex.size(); i++) {
    if (coordIndex[i] == -1) {
      currentFace++;
      if (currentFace > iF) break;
    } else if (currentFace == iF) {
      result++;
    }
  }
  return result;
}

int Faces::getFaceFirstCorner(const int iF) const {
  if (iF < 0 || iF >= getNumberOfFaces()) {
    return -1;
  }

  int currentFace = 0;
  for (int i = 0; i < (int)coordIndex.size(); i++) {
    if (coordIndex[i] == -1) {
      currentFace++;
      continue;
    }
    if (currentFace == iF) {
      return i; 
    }
  }
  return -1;
}

int Faces::getFaceVertex(const int iF, const int j) const {
  if (iF < 0 || iF >= getNumberOfFaces())
    return -1;
  if (j < 0 || j >= getFaceSize(iF))
    return -1;

  int first = getFaceFirstCorner(iF);
  if (first < 0) return -1;

  return coordIndex[first + j]; 
}

int Faces::getCornerFace(const int iC) const {
  if (iC < 0 || iC >= getNumberOfCorners())
    return -1;
  if (coordIndex[iC] == -1)
    return -1;

  int currentFace = 0;
  for (int i = 0; i < (int)coordIndex.size(); i++) {
    if (coordIndex[i] == -1) {
      currentFace++;
    }
    if (i == iC) {
      return currentFace;
    }
  }
  return -1;
}

int Faces::getNextCorner(const int iC) const {
  if (iC < 0 || iC >= getNumberOfCorners())
    return -1;
  if (coordIndex[iC] == -1)
    return -1;

  int faceIndex = getCornerFace(iC);
  if (faceIndex < 0) return -1;

  int faceSize = getFaceSize(faceIndex);
  int firstCorner = getFaceFirstCorner(faceIndex);
  if (firstCorner < 0) return -1;

  return (iC - firstCorner + 1) % faceSize + firstCorner;
}