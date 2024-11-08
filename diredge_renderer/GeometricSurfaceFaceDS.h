///////////////////////////////////////////////////
//
//	Hamish Carr
//	January, 2018
//
//	------------------------
//	GeometricSurfaceFaceDS.h
//	------------------------
//
//	Base code for geometric assignments.
//
//	This is the minimalistic Face-based D/S for storing
//	surfaces, to be used as the basis for fuller versions
//
//	It will include object load / save code & render code
//
///////////////////////////////////////////////////

#ifndef _GEOMETRIC_SURFACE_FACE_DS_H
#define _GEOMETRIC_SURFACE_FACE_DS_H

#include <vector>

#include "Cartesian3.h"

#include "Defines.h"

#if DIREDGE // DIREDGE
using IndexType = size_t;

struct Mesh {
    std::vector<Cartesian3> vertices;
    std::vector<IndexType> faceVertices;
    std::vector<IndexType> firstDirectedEdges;
    std::vector<IndexType> otherHalfs;
};
#endif // DIREDGE

class GeometricSurfaceFaceDS { // class GeometricSurfaceFaceDS
  public:
    // vectors to store vertex and triangle information - relying on POD rule
    std::vector<Cartesian3> vertices;

    // bounding sphere size
    float boundingSphereSize;

    // midpoint of object
    Cartesian3 midPoint;

    // constructor will initialise to safe values
    GeometricSurfaceFaceDS();

#if DIREDGE // DIREDGE
    bool ReadDirectedEdgeFile(const char *filenameStr);

    bool ValidateTriangleSoup();
#endif // DIREDGE

    // read routine returns true on success, failure otherwise
    bool ReadFileTriangleSoup(char *fileName);

    // routine to render
    void Render();

#if DIREDGE // DIREDGE
  private:
    Mesh mMesh;

    std::vector<Cartesian3> mTriangleSoup;

#endif // DIREDGE
}; // class GeometricSurfaceFaceDS

#endif
