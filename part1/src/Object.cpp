#include "Object.hpp"
#include "Error.hpp"
#include <algorithm>
#include "Camera.hpp"

using namespace std;
Object::Object(){
    m_wireframe = false; // object starts not in wireframe mode
    m_rotate = false;
}

Object::~Object(){
}


void Object::Update(unsigned int screenWidth, unsigned int screenHeight){
    m_textureDiffuse.Unbind();
    // Make sure we are updating the correct 'buffers'
    m_shader.Bind();
    m_vertexBufferLayout.Bind();
    m_textureDiffuse.Bind(0);

    m_shader.SetUniformMatrix1i("u_Texture",0);
    m_transform.Translate(0.0f, -0.7f, 5.0f);

    static float rot = 0;
    if(rot>360){rot=0;}
    if (m_rotate) {
        rot+=0.035f;
    }
    m_transform.Rotate(rot,0.0f,1.0f,0.0f );

    m_projectionMatrix = glm::perspective(glm::radians(60.0f),(float)screenWidth/(float)screenHeight,0.1f,100.0f);
    
    // Set the uniforms in our current shader
    glm::mat4 model        = m_transform.GetInternalMatrix();
    glm::mat4 view         = Camera::Instance().GetWorldToViewmatrix();
    // create our normal matrix here, that is the transpose of the inverse of the model view matrix
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(view * model));

    m_shader.SetUniformMatrix4fv("model", &model[0][0]);
    m_shader.SetUniformMatrix4fv("view", &view[0][0]);
    m_shader.SetUniformMatrix4fv("projection", &m_projectionMatrix[0][0]);

    m_shader.SetUniformMatrix4fv("normal_matrix", &normalMatrix[0][0]);
    
	m_shader.SetUniform3f("lightPos", Camera::Instance().GetEyeXPosition() + Camera::Instance().GetViewXDirection(),
                                      Camera::Instance().GetEyeYPosition() + Camera::Instance().GetViewYDirection(),
                                      Camera::Instance().GetEyeZPosition() + Camera::Instance().GetViewZDirection());

    m_shader.SetUniform3f("viewPos",Camera::Instance().GetEyeXPosition(),
                                    Camera::Instance().GetEyeYPosition(),
                                    Camera::Instance().GetEyeZPosition());

}

void Object::Render(){
    // Select this objects buffer to render
    m_vertexBufferLayout.Bind();
    // Select this objects shader to render
    m_shader.Bind();
    //Render data
    glDrawElements(GL_TRIANGLES,
                    m_geometry.GetIndicesSize(),  // The number of indicies, not triangles.
                    GL_UNSIGNED_INT,   // Make sure the data type matches
                    nullptr);          // Offset pointer to the data. nullptr
                    // because we are currently bound:
}

void AddTokensToVector(vector<float> &vectorToAdd, vector<string> tokens) {
    for (int ii = 1; ii < tokens.size(); ii++) {
            vectorToAdd.push_back(stof(tokens[ii]));
    }
}

// Get the vertex index, then the texture index, then the normal index
// Then get the corresponding values 
unsigned int Object::GetIndicesFromToken(string token) {

    vector<int> indexTokens;
    stringstream tokenStream(token);
    string tokenized;
    int vIndex, vtIndex, nIndex;
    unsigned int index;

    if(token.find("//") != string::npos) {
        size_t pos = 0;
        string delimiter = "//";
        string new_token;
        while ((pos = token.find(delimiter)) != string::npos) {
            new_token = token.substr(0, pos);
            indexTokens.push_back(stoi(new_token) - 1);
            token.erase(0, pos + delimiter.length());
        }
        indexTokens.push_back(stoi(token) - 1);
        
        vIndex = indexTokens[0] * 3;
        vtIndex = 1;
        nIndex  = indexTokens[1] * 3; // the normal index

        // The vertex at vIndex
        float x  = m_objectVertices[vIndex];
        float y  = m_objectVertices[vIndex + 1];
        float z  = m_objectVertices[vIndex + 2];
        // The normal at nIndex
        float xn = m_objectNormals[nIndex];
        float yn = m_objectNormals[nIndex + 1];
        float zn = m_objectNormals[nIndex + 2];

        VertexData vd = VertexData(x, y, z, xn, yn, zn);

        vector<VertexData>::iterator it = find(m_vertexDataVector.begin(), m_vertexDataVector.end(), vd);
        if (it != m_vertexDataVector.end()) {
            index = it - m_vertexDataVector.begin();
        }
        else {
            m_vertexDataVector.push_back(vd);
            m_geometry.AddVertex(x, y, z, xn, yn, zn, 0.0f, 0.0f);
            index = m_vertexDataVector.size() - 1;
        } 
        return index;
        
    }
    else if (token.find("/") != string::npos) {
        while(getline(tokenStream, tokenized, '/')) {
            indexTokens.push_back(stoi(tokenized) - 1);
        }
        vIndex  = indexTokens[0] * 3; // the vertex index
        vtIndex = indexTokens[1] * 2;
        nIndex  = indexTokens[2] * 3; // the normal index

        // The vertex at vIndex
        float x  = m_objectVertices[vIndex];
        float y  = m_objectVertices[vIndex + 1];
        float z  = m_objectVertices[vIndex + 2];
        // The normal at nIndex
        float xn = m_objectNormals[nIndex];
        float yn = m_objectNormals[nIndex + 1];
        float zn = m_objectNormals[nIndex + 2];
        // The texture at vtIndex
        float s  = m_objectTextures[vtIndex];
        float t  = m_objectTextures[vtIndex + 1];
        
        VertexData vd = VertexData(x, y, z, xn, yn, zn, s, t);

        vector<VertexData>::iterator it = find(m_vertexDataVector.begin(), m_vertexDataVector.end(), vd);
        if (it != m_vertexDataVector.end()) {
            index = it - m_vertexDataVector.begin();
        }
        else {
            m_vertexDataVector.push_back(vd);
            m_geometry.AddVertex(x, y, z, xn, yn, zn, s, t);
            index = m_vertexDataVector.size() - 1;
        } 
        return index;
    }
    else {
        vIndex = (stoi(token) - 1) * 3;
        float x  = m_objectVertices[vIndex];
        float y  = m_objectVertices[vIndex + 1];
        float z  = m_objectVertices[vIndex + 2];
        VertexData vd = VertexData(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        vector<VertexData>::iterator it = find(m_vertexDataVector.begin(), m_vertexDataVector.end(), vd);
        if (it != m_vertexDataVector.end()) {
            index = it - m_vertexDataVector.begin();
        }
        else {
            m_vertexDataVector.push_back(vd);
            m_geometry.AddVertex(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            index = m_vertexDataVector.size() - 1;
        } 
        return index;
    }
}

string Object::getFilePath(string fileName) {
    // Like in GetIndices from Token, push all of the tokens onto the vector except for the last one
    vector<string> indexTokens;
    stringstream tokenStream(fileName);
    string tokenized;
    string filePath;
    while(getline(tokenStream, tokenized, '/')) {
        indexTokens.push_back(tokenized);
    }
    for (int ii = 0; ii < indexTokens.size() - 1; ii++) {
        filePath += indexTokens[ii] + "/";
    }
    return filePath;
}

void Object::CreateObject(string fileName) {
    ifstream objFile;
    objFile.open(fileName);

    // Obj file keywords
    const string VERTEX         = "v";
    const string VERTEX_NORMAL  = "vn";
    const string VERTEX_TEXTURE = "vt";
    const string FACE           = "f";
    const string MTL_FILE       = "mtllib";
    // get the prepended path of the fileName, this assumes the mtl file is 
    // in the same relative path as the obj file
    m_filePath = getFilePath(fileName);
    // Set the way we are going to render this object.
    // 0 means that we are not rendering from a mtl file
    // 1 means that we are
    if(objFile.is_open()) {
        string line;
        
        while (getline(objFile, line)) {
            // Check for vertices, texture coords, face, or mtl file
            if(line[0] == 'v' || line[0] == 'f' || line[0] == 'm') {
                // Only concerned with lines describing a vertex or face
                vector<string> tokens = SplitString(line);

                // need to make sure we aren't adding the vertex normals as well
                if(tokens[0] == VERTEX) {
                    AddTokensToVector(m_objectVertices, tokens);
                }
                if(tokens[0] == VERTEX_TEXTURE) {
                    AddTokensToVector(m_objectTextures, tokens);
                }
                else if (tokens[0] == VERTEX_NORMAL) {
                    AddTokensToVector(m_objectNormals, tokens);
                }
                
                // Finally build our index buffer here
                else if (tokens[0] == FACE) {
                    vector<unsigned int> indices;
                    for (int ii = 1; ii < tokens.size(); ii++) {
                        indices.push_back(GetIndicesFromToken(tokens[ii]));
                    }
                    assert(indices.size() == 3);
                    m_geometry.MakeTriangle(indices);

                }
                else if (tokens[0] == MTL_FILE) {
                    // Format is mtllib [MTL_FILENAME]
                    LoadTextureFile(m_filePath + tokens[1]);
                }
            }
        } 
    }
    objFile.close();
    m_geometry.Gen();
    m_vertexBufferLayout.CreateNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                m_geometry.GetIndicesSize(),
                                m_geometry.GetBufferDataPtr(),
                                m_geometry.GetIndicesDataPtr());
    
    std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
    std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");

    // Actually create our shader
    m_shader.CreateShader(vertexShader,fragmentShader);
}

void Object::LoadTextureFile(string mtlFileName) {
    ifstream mtlFile; 
    const string DIFFUSE = "map_Kd";
    const string NORMAL  = "map_Bump";
    mtlFile.open(mtlFileName);
    string ppmFile_Diffuse;
    string ppmFile_Normal;

    if(mtlFile.is_open()) {
        string line;
        while(getline(mtlFile, line)) {
            if (line[0] == 'm') {
                vector<string> tokens = SplitString(line);
                if (tokens[0] == DIFFUSE) {
                    ppmFile_Diffuse = tokens[1];
                }
                else if (tokens[0] == NORMAL) {
                    ppmFile_Normal = tokens[1];
                }
            }
        }
    }
    // check for the ppm file extension
    if (ppmFile_Diffuse.find(".ppm") != std::string::npos) {
       m_textureDiffuse.LoadTexture((m_filePath + ppmFile_Diffuse).c_str());
    }
    else {
       m_textureDiffuse.LoadTexture((m_filePath + "Base_Color.ppm").c_str());
    }
    mtlFile.close();
}

// Split the string based on spaces
vector<string> Object::SplitString(string &line) {
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> tokens(beg, end);
    return tokens;    
}

// Returns the actual transform stored in our object
// which can then be modified
Transform& Object::GetTransform(){
     return m_transform; 
}

void Object::LoadTexture(const std::string filePath) {
    m_textureDiffuse.LoadTexture(filePath);
}
