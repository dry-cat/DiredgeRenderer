///////////////////////////////////////////////////
//
//	Hamish Carr
//	January, 2018
//
//	------------------------
//	GeometricSurfaceFaceDS.cpp
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

#include "GeometricSurfaceFaceDS.h"
#include <fstream>
#include <iostream>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#if DIREDGE // DIREDGE
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>
#endif // DIREDGE

#if DIREDGE // DIREDGE
namespace {
Mesh CalcMesh(std::ifstream &inf) {
    Mesh mesh;

    // Read in each line
    for (std::string str; inf >> str;) {
        if (str == "Vertex") {
            // Read in the vertex index and vertex
            IndexType index = 0;
            Cartesian3 vertex;
            inf >> index >> vertex.x >> vertex.y >> vertex.z;

            mesh.vertices.push_back(vertex);
        } else if (str == "Face") {
            // Read in the face index and face vertex indices
            IndexType index = 0;
            IndexType v0 = 0;
            IndexType v1 = 0;
            IndexType v2 = 0;
            inf >> index >> v0 >> v1 >> v2;

            mesh.faceVertices.push_back(v0);
            mesh.faceVertices.push_back(v1);
            mesh.faceVertices.push_back(v2);
        } else if (str == "FirstDirectedEdge") {
            // Read in the first directed edge index and vertex index
            IndexType index = 0;
            IndexType vertexIndex = 0;
            inf >> index >> vertexIndex;

            mesh.firstDirectedEdges.push_back(vertexIndex);
        } else if (str == "OtherHalf") {
            // Read in the other half edge index and vertex index
            IndexType index = 0;
            IndexType vertexIndex = 0;
            inf >> index >> vertexIndex;

            mesh.otherHalfs.push_back(vertexIndex);
        }
    }

    return mesh;
}

void PrintVertices(const std::vector<Cartesian3> &vertices) {
    IndexType vi = 0;
    for (auto v : vertices) {
        std::cout << "Vertex " << vi++ << " " << v << '\n';
    }
}

void PrintFirstDirectedEdges(const std::vector<IndexType> &firstDirectedEdges) {
    IndexType fdei = 0;
    for (auto fde : firstDirectedEdges) {
        std::cout << "FirstDirectedEdge " << fdei++ << " " << fde << '\n';
    }
}

void PrintOtherHalfEdges(const std::vector<IndexType> &otherHalfs) {
    IndexType ohi = 0;
    for (auto otherHalf : otherHalfs) {
        std::cout << "OtherHalf " << ohi++ << " " << otherHalf << '\n';
    }
}

void PrintFaceVertices(const std::vector<IndexType> &faceVertices) {
    IndexType fi = 0;
    for (auto it = faceVertices.begin(); it != faceVertices.end();) {
        std::cout << "Face " << fi++ << " " << *it++ << " " << *it++ << " "
                  << *it++ << '\n';
    }
}

void PrintMesh(const Mesh &mesh) {
    PrintVertices(mesh.vertices);
    PrintFirstDirectedEdges(mesh.firstDirectedEdges);
    PrintFaceVertices(mesh.faceVertices);
    PrintOtherHalfEdges(mesh.otherHalfs);
}
} // namespace

std::vector<Cartesian3> CalcTriangleSoup(const Mesh &mesh) {
    std::vector<Cartesian3> triangleSoup;

    // Create indirection through otherHalfs to cross-check

    // Create triangle soup
    for (IndexType f = 0; f < mesh.faceVertices.size() / 3; f++) {
        // Container indices represent indices into the array, i.e., position i
        // in the array. Indices otherwise represent actual index values stored
        // in the array, i.e. at a position in the array we store index i.

        // Calculate the container indices for where the half edges of the face
        // live.
        IndexType halfEdgeContainerIndex0 = 3 * f + 0;
        IndexType halfEdgeContainerIndex1 = 3 * f + 1;
        IndexType halfEdgeContainerIndex2 = 3 * f + 2;

        // Calculate the container indices for where the other half edges of the
        // face live.
        IndexType otherHalfEdgeContainerIndex0 =
            mesh.otherHalfs[halfEdgeContainerIndex0];
        IndexType otherHalfEdgeContainerIndex1 =
            mesh.otherHalfs[halfEdgeContainerIndex1];
        IndexType otherHalfEdgeContainerIndex2 =
            mesh.otherHalfs[halfEdgeContainerIndex2];

        // Get the contents of the other half edge location in the array.
        // I.e., the container indices for where the half edges of the face
        // live.
        IndexType otherHalfEdgeIndex0 =
            mesh.otherHalfs[otherHalfEdgeContainerIndex0];
        IndexType otherHalfEdgeIndex1 =
            mesh.otherHalfs[otherHalfEdgeContainerIndex1];
        IndexType otherHalfEdgeIndex2 =
            mesh.otherHalfs[otherHalfEdgeContainerIndex2];

        // Cross-check the edge indices
        assert(halfEdgeContainerIndex0 == otherHalfEdgeIndex0 &&
               halfEdgeContainerIndex1 == otherHalfEdgeIndex1 &&
               halfEdgeContainerIndex2 == otherHalfEdgeIndex2);

        // Index into the face vertices array with the edge indices to get the
        // start vertices of the edges of the face.
        IndexType vertexIndex0 = mesh.faceVertices[otherHalfEdgeIndex0];
        IndexType vertexIndex1 = mesh.faceVertices[otherHalfEdgeIndex1];
        IndexType vertexIndex2 = mesh.faceVertices[otherHalfEdgeIndex2];

        triangleSoup.push_back(
            mesh.vertices[vertexIndex0]);
        triangleSoup.push_back(
            mesh.vertices[vertexIndex1]);
        triangleSoup.push_back(
            mesh.vertices[vertexIndex2]);
    }

    return triangleSoup;
}
#endif // DIREDGE



// constructor will initialise to safe values
GeometricSurfaceFaceDS::GeometricSurfaceFaceDS() { // GeometricSurfaceFaceDS::GeometricSurfaceFaceDS()
    // force the size to nil (should not be necessary, but . . .)
    vertices.resize(0);

    // set this to something reasonable
    boundingSphereSize = 1.0;

    // set the midpoint to the origin
    midPoint = Cartesian3(0.0, 0.0, 0.0);
} // GeometricSurfaceFaceDS::GeometricSurfaceFaceDS()



#if DIREDGE // DIREDGE
bool GeometricSurfaceFaceDS::ReadDirectedEdgeFile(const char *filenameStr) {
    const std::filesystem::path filename{filenameStr};
    std::ifstream inf{filename};

    // Check file extension
    if (filename.extension() != ".diredge") {
        std::cerr << "Wrong file extension for .diredge file.";
        return false;
    }

    if (!inf) {

        return false;
    }

    mMesh = CalcMesh(inf);

    std::puts("Mesh data generated from .diredge file.");
    PrintMesh(mMesh);
    std::cout << '\n';

    mTriangleSoup = CalcTriangleSoup(mMesh);

    // Copy over the code to calculate the bounding box and bounding sphere of
    // the object. NOTE: the calculation of the midpoint could be kept track of
    // as the vertices are read in as is done in ReadFileTriangleSoup.
    //
    // these are for accumulating a bounding box for the object
    Cartesian3 minCoords(1000000.0, 1000000.0, 1000000.0);
    Cartesian3 maxCoords(-1000000.0, -1000000.0, -1000000.0);

    // set the midpoint to the origin
    midPoint = Cartesian3(0.0, 0.0, 0.0);

    // now loop to read the vertices in, and hope nothing goes wrong
    for (const auto& vertex : mTriangleSoup) {
        // keep running track of midpoint, &c.
        midPoint = midPoint + vertex;
        if (vertex.x < minCoords.x)
            minCoords.x = vertex.x;
        if (vertex.y < minCoords.y)
            minCoords.y = vertex.y;
        if (vertex.z < minCoords.z)
            minCoords.z = vertex.z;

        if (vertex.x > maxCoords.x)
            maxCoords.x = vertex.x;
        if (vertex.y > maxCoords.y)
            maxCoords.y = vertex.y;
        if (vertex.z > maxCoords.z)
            maxCoords.z = vertex.z;
    }

    // now sort out the size of a bounding sphere for viewing
    // and also set the midpoint's location
    midPoint = midPoint / mTriangleSoup.size();

    // now go back through the vertices, subtracting the mid point
    for (auto& vertex : mTriangleSoup) { // per vertex
        vertex = vertex - midPoint;
    } // per vertex

    // the bounding sphere radius is just half the distance between these
    boundingSphereSize = sqrt((maxCoords - minCoords).length()) * 1.0;

    return true;
}

bool GeometricSurfaceFaceDS::ValidateTriangleSoup() {
    std::puts("Triangle soup generated from .tri file.");
    PrintVertices(vertices);
    std::cout << '\n';

    std::puts(
        "Triangle soup generated from vertices and faces from .diredge file.");
    PrintVertices(mTriangleSoup);
    std::cout << '\n';

    bool isTriangleSoupEqual = std::equal(
        mTriangleSoup.begin(), mTriangleSoup.end(), vertices.begin());
    if (isTriangleSoupEqual) {
        std::puts(
            "Triangle soup generated from .diredge vertices and faces is "
            "equal to triangle soup generated from .tri file. This assumes "
            "that the faces indexed in the .diredge file are in the same order "
            "as the faces implicitly defined in the .tri file.");
    }

    return isTriangleSoupEqual;
}
#endif // DIREDGE

// read routine returns true on success, failure otherwise
bool GeometricSurfaceFaceDS::ReadFileTriangleSoup(
    char *fileName) { // GeometricSurfaceFaceDS::ReadFileTriangleSoup()
    // these are for accumulating a bounding box for the object
    Cartesian3 minCoords(1000000.0, 1000000.0, 1000000.0);
    Cartesian3 maxCoords(-1000000.0, -1000000.0, -1000000.0);

#if DIREDGE // Change this to catch errors properly
    const std::filesystem::path fp{fileName};
    std::ifstream inFile{fp};

    // Check file extension
    if (fp.extension() != ".tri") {
        std::cerr << "Wrong file extension for .tri file.";
        return false;
    }

    if (!inFile) {
        return false;
    }
#else // DIREDGE
    // open the input file
    std::ifstream inFile(fileName);
    if (inFile.bad())
        return false;
#endif // DIREDGE

    // set the number of vertices and faces
    long nTriangles = 0, nVertices = 0;

    // set the midpoint to the origin
    midPoint = Cartesian3(0.0, 0.0, 0.0);

    // read in the number of vertices
    inFile >> nTriangles;
    nVertices = nTriangles * 3;

    // now allocate space for them all
    vertices.resize(nVertices);

    // now loop to read the vertices in, and hope nothing goes wrong
    for (int vertex = 0; vertex < nVertices; vertex++) { // for each vertex
        inFile >> vertices[vertex].x >> vertices[vertex].y >> vertices[vertex].z;

        // keep running track of midpoint, &c.
        midPoint = midPoint + vertices[vertex];
        if (vertices[vertex].x < minCoords.x)
            minCoords.x = vertices[vertex].x;
        if (vertices[vertex].y < minCoords.y)
            minCoords.y = vertices[vertex].y;
        if (vertices[vertex].z < minCoords.z)
            minCoords.z = vertices[vertex].z;

        if (vertices[vertex].x > maxCoords.x)
            maxCoords.x = vertices[vertex].x;
        if (vertices[vertex].y > maxCoords.y)
            maxCoords.y = vertices[vertex].y;
        if (vertices[vertex].z > maxCoords.z)
            maxCoords.z = vertices[vertex].z;
    } // for each vertex

    // now sort out the size of a bounding sphere for viewing
    // and also set the midpoint's location
    midPoint = midPoint / vertices.size();

    // now go back through the vertices, subtracting the mid point
    for (int vertex = 0; vertex < nVertices; vertex++) { // per vertex
        vertices[vertex] = vertices[vertex] - midPoint;
    } // per vertex

#if !DIREDGE // !DIREDGE
    // the bounding sphere radius is just half the distance between these
    boundingSphereSize = sqrt((maxCoords - minCoords).length()) * 1.0;
#endif // !DIREDGE

    return true;
} // GeometricSurfaceFaceDS::ReadFileTriangleSoup()

// routine to render
void GeometricSurfaceFaceDS::Render() { // GeometricSurfaceFaceDS::Render()
    // walk through the faces rendering each one
    glBegin(GL_TRIANGLES);

#if DIREDGE // DIREDGE
    // we will loop in 3's, assuming CCW order
    for (unsigned int vertex = 0; vertex < mTriangleSoup.size();) { // per triangle
        // use increment to step through them
        Cartesian3 *v0 = &(mTriangleSoup[vertex++]);
        Cartesian3 *v1 = &(mTriangleSoup[vertex++]);
        Cartesian3 *v2 = &(mTriangleSoup[vertex++]);
        // now compute the normal vector
        Cartesian3 uVec = *v1 - *v0;
        Cartesian3 vVec = *v2 - *v0;
        Cartesian3 normal = uVec.cross(vVec).normalise();

        glNormal3fv(&normal.x);
        glVertex3fv(&v0->x);
        glVertex3fv(&v1->x);
        glVertex3fv(&v2->x);
    } // per triangle
#else // DIREDGE
    // we will loop in 3's, assuming CCW order
    for (unsigned int vertex = 0; vertex < vertices.size();) { // per triangle
        // use increment to step through them
        Cartesian3 *v0 = &(vertices[vertex++]);
        Cartesian3 *v1 = &(vertices[vertex++]);
        Cartesian3 *v2 = &(vertices[vertex++]);
        // now compute the normal vector
        Cartesian3 uVec = *v1 - *v0;
        Cartesian3 vVec = *v2 - *v0;
        Cartesian3 normal = uVec.cross(vVec).normalise();

        glNormal3fv(&normal.x);
        glVertex3fv(&v0->x);
        glVertex3fv(&v1->x);
        glVertex3fv(&v2->x);
    } // per triangle
#endif // DIREDGE

    glEnd();
} // GeometricSurfaceFaceDS::Render()
