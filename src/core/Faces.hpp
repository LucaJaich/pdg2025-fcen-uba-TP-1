//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-04 22:10:14 gtaubin>
//------------------------------------------------------------------------
//
// Faces.hpp
//
// Written by: <Your Name>
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

#ifndef _FACES_HPP_
#define _FACES_HPP_

#include <vector>

using namespace std;

class Faces {
  
public:
          Faces(const int nV, const vector<int>& coordIndex);

  // The constructor should compare the nV value passed as a parameter
  // with the non-negative values in stored in the coordIndex index
  // array, and update the value of nV stored internally if
  // necessary. This value returns the updated value;
  int     getNumberOfVertices()                    const;

  // The faces are conted in the constructor by counting the number of
  // -1's in the coordIndex array. If coordIndex is not empty, the
  // last value of coordIndex should be -1.
  int     getNumberOfFaces()                       const;

  // The number of corners is defined as the size of the coordIndex
  // array.  Including the -1 face separators as corners simplify many
  // of the algorithms.
  int     getNumberOfCorners()                     const;

  // If iF is a valid face index, this method returns the number of
  // corners of the face iF. Otherwise it returns 0.
  int     getFaceSize(const int iF)                const;

  // If iF is a valid face index, this method returns the index of the
  // coordIndex entry corresponding to the first corner of the face
  // iF. Otherwise it returns -1.
  int     getFaceFirstCorner(const int iF)         const;

  // If iF is a valid face index, and j is a valid corner index for
  // face iF, this method returns the value stored in the
  // corresponding coordIndex entry.
  int     getFaceVertex(const int iF, const int j) const;

  // If iC is a valid corner index, and it does not correspond to a -1
  // separator, this method returns the index of the face which
  // contains the given corner. Otherwise it returns -1.
  int     getCornerFace(const int iC)              const;

  // If iC is a valid corner index, and it does not correspond to a -1
  // separator, this method returns the next corner index within the
  // cyclical order of the face which contains the given
  // corner. Otherwise it returns -1.
  int     getNextCorner(const int iC)              const;

private:
  int _nV;
  vector<int> coordIndex;
};

#endif /* _FACES_HPP_ */
