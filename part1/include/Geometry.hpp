/** @file Geometry.hpp
 *  @brief Organizes vertex and triangle information.
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
using namespace std;

// Purpose of this class is to store vertice and triangle information
class Geometry{
public:
	// Constructor
	Geometry();
	// Destructor
	~Geometry();
	
	// Functions for working with individual vertices
	unsigned int GetBufferSizeInBytes();
    // Retrieve the 
	unsigned int GetBufferDataSize();
    // Retrieve the Buffer Data Size
	float* GetBufferDataPtr();
	// Add a new vertex 
	void AddVertex(float x, float y, float z, float xn, float yn, float zn, float s, float t);
    // gen pushes all attributes into a single vector
	void Gen();
	// Functions for working with Indices
	// Creates a triangle from 3 indicies
	// When a triangle is made, the tangents and bi-tangents are also
	// computed
	void MakeTriangle(vector<unsigned int> &verts);  
    // Retrieve how many indicies there are
	unsigned int GetIndicesSize();
    // Retrieve the pointer to the indices
	unsigned int* GetIndicesDataPtr();

private:
	// m_bufferData stores all of the vertexPositons, coordinates, normals, etc.
	// This is all of the information that should be sent to the vertex Buffer Object
	vector<float> m_bufferData;

    // Individual components of 
	vector<float> m_vertexPositions;
	vector<float> m_textureCoords;
	vector<float> m_normals;
	vector<float> m_tangents;
	vector<float> m_biTangents;

	// The indices for a indexed-triangle mesh
	vector<unsigned int> m_indices;
};





#endif
