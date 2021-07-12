#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <vector>
#include <glad/glad.h>
#include <KHR/khrplatform.h>

#include "Transform.hpp"
#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <map>
#include "Geometry.hpp"


using namespace std;

struct VertexData{
	float x,y,z;	// x,y,z positions
	float xn,yn,zn; // x,y,z normals
	float s,t;		// s,t texture coordinates

	VertexData(float _x, float _y, float _z,float _xn, float _yn, float _zn, float _s, float _t):
                 x(_x),y(_y),z(_z),xn(_xn),yn(_yn),zn(_zn),s(_s),t(_t) { }

    
	VertexData(float _x, float _y, float _z,float _xn, float _yn, float _zn):
                 x(_x),y(_y),z(_z),xn(_xn),yn(_yn),zn(_zn),s(0.0f),t(0.0f) { }
	
	// Tests if two VertexData are equal
	bool operator== (const VertexData &rhs){
        const float epsilon = 0.005f;
        return abs(x - rhs.x) < epsilon &&
               abs(y - rhs.y) < epsilon &&
               abs(z - rhs.z) < epsilon &&
               abs(xn - rhs.xn) < epsilon &&
               abs(yn - rhs.yn) < epsilon &&
               abs(zn - rhs.zn) < epsilon &&
               abs(s - rhs.s) < epsilon &&
               abs(t - rhs.t) < epsilon;
	}
};

// Class representing an Object
class Object{
public:
    // Object Constructor
    Object();
    // Object destructor
    ~Object();
    // Updates and transformatinos applied to object
    void Update(unsigned int screenWidth, unsigned int screenHeight);


    // How to draw the object
    void Render();
    // Returns an objects transform
    Transform& GetTransform();

    // create the object
    void CreateObject(string fileName);
    bool m_wireframe;
    bool m_rotate;    
    // For now we have one shader per object.
    Shader m_shader;

    void LoadTexture(const std::string filepath);

private:

    unsigned int GetIndicesFromToken(string token);
    // Split a given string by spaces
    vector<string> SplitString(string &line);

    // These temp vectors are from the initial reading of the 
    // object file. Once we read the indices, we access these
    // and add them to the geometry member variable
    vector<GLfloat> m_objectVertices;
    vector<GLfloat> m_objectNormals;
    // Object texture coordinates
    vector<GLfloat> m_objectTextures;

    vector<VertexData> m_vertexDataVector;
    void LoadTextureFile(string mtlFileName);
    string getFilePath(string fileName);

    // The file path for all of our objects
    string m_filePath;

    Texture m_textureDiffuse;


    // For now we have one buffer per object.
    VertexBufferLayout m_vertexBufferLayout;
    // Store the objects transformations
    Transform m_transform; 
    // Store the 'camera' projection
    glm::mat4 m_projectionMatrix;
    // The object's geometry
    Geometry m_geometry;
};


#endif