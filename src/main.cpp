
#include <iostream>
using namespace std;
#include <GL/glew.h>
#include <GLFW/glfw3.h>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common/utils.hpp"
#include <fstream>
#include <sstream>
#include <common/controls.hpp>
#include <limits>

GLFWwindow* window;
static const int window_width = 1920;
static const int window_height = 1080;
unsigned int nIndices;
unsigned int nIndices2;
GLuint textureID;
GLuint hightID;
GLuint roughID;
GLuint normalID;
GLuint snowID;
GLuint snowMID;
GLuint grassID;
GLuint grassMID;
GLuint snowNID;
GLuint grassNID;
GLuint WaterTextureID;
GLuint flowerID;
GLuint flower2ID;



bool initializeGL()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        cerr << "Failed to initialize GLFW" << endl;
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 1); //no anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy;
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(window_width, window_height, "OpenGLRenderer", NULL, NULL);

    if (window == NULL)
    {
        cerr << "Failed to open GLFW window. If you have an Intel GPU, they may not be 4.5 compatible." << endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        cerr << "Failed to initialize GLEW" << endl;
        glfwTerminate();
        return false;
    }

    if (!GLEW_ARB_debug_output) return 1;


    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, window_width / 2, window_height / 2);
    return true;
}

//New variables to add
static const int n_points = 600; //minimum 2
static const float m_scale = 5;

// Include GLM

using namespace glm;
#include <vector>

GLuint VertexArrayID;
//Buffers for VAO
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint elementbuffer;


GLuint waterArrayID;
//Buffers for VAO
GLuint waterbuffer;
GLuint wateruvbuffer;
GLuint waternormalbuffer;
GLuint waterelementbuffer;

void LoadModel()
{

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> indices;
    for (int i = 0; i < n_points; i++)
    {
        float x = (m_scale) * ((i / float(n_points - 1)) - 0.5f) * 2.0f;
        for (int j = 0; j < n_points; j++)
        {
            float z = (m_scale) * ((j / float(n_points - 1)) - 0.5f) * 2.0f;
            vertices.push_back(vec3(x, 0, z));
            uvs.push_back(vec2(float(i + 0.5f) / float(n_points - 1),
                float(j + 0.5f) / float(n_points - 1)));
        }
    }

    glEnable(GL_PRIMITIVE_RESTART);
    constexpr unsigned int restartIndex = std::numeric_limits<std::uint32_t>::max();
    glPrimitiveRestartIndex(restartIndex);
    int n = 0;





    for (int i = 0; i < n_points - 1; i += 1)
    {
        for (int j = 0; j < n_points - 1; j += 1)
        {
            unsigned int topLeft = n;
            unsigned int topRight = n + 1;
            unsigned int bottomLeft = n + n_points;
            unsigned int bottomRight = n + n_points + 1;


            // First tri
            indices.push_back(topLeft);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);

            // Second tri
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(bottomRight);

            n += 1;
        }
        n += 1;
    }

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    glEnableVertexAttribArray(0);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(
        0, // attribute
        3, // size (we have x,y,z)
        GL_FLOAT, // type of each individual element
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );



    glEnableVertexAttribArray(1);
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    //declare at file scope

    //inside function
    nIndices = indices.size();
    cout << "load model" << "\n";
}
GLuint VertexArrayID2;
//Buffers for VAO
GLuint vertexbuffer2;
GLuint uvbuffer2;
GLuint normalbuffer2;
GLuint elementbuffer2;
void LoadModel2()
{

    std::vector<glm::vec3> vertices2;
    std::vector<glm::vec2> uvs2;
    std::vector<unsigned int> indices2;
    for (int i = 0; i < n_points; i++)
    {
        float x = (m_scale) * ((i / float(n_points - 1)) - 0.5f) * 2.0f;
        for (int j = 0; j < n_points; j++)
        {
            float z = (m_scale) * ((j / float(n_points - 1)) - 0.5f) * 2.0f;
            vertices2.push_back(vec3(x, 0, z));
            uvs2.push_back(vec2(float(i + 0.5f) / float(n_points - 1),
                float(j + 0.5f) / float(n_points - 1)));
        }
    }

    glEnable(GL_PRIMITIVE_RESTART);
    constexpr unsigned int restartIndex = std::numeric_limits<std::uint32_t>::max();
    glPrimitiveRestartIndex(restartIndex);
    int n = 0;
    for (int i = 0; i < n_points - 1; i++)
    {
        for (int j = 0; j < n_points; j++)
        {
            unsigned int topLeft = n;
            unsigned int bottomLeft = topLeft + n_points;
            indices2.push_back(bottomLeft);
            indices2.push_back(topLeft);
            n++;
        }
        indices2.push_back(restartIndex);
    }

    //VAO

    glGenVertexArrays(1, &VertexArrayID2);
    glBindVertexArray(VertexArrayID2);


    glEnableVertexAttribArray(0);
    glGenBuffers(1, &vertexbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
    glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec3), &vertices2[0], GL_STATIC_DRAW);
    glVertexAttribPointer(
        0, // attribute
        3, // size (we have x,y,z)
        GL_FLOAT, // type of each individual element
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );



    glEnableVertexAttribArray(1);
    glGenBuffers(1, &uvbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
    glBufferData(GL_ARRAY_BUFFER, uvs2.size() * sizeof(glm::vec2), &uvs2[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices2.size() * sizeof(unsigned int), &indices2[0], GL_STATIC_DRAW);
    //declare at file scope

    //inside function
    nIndices2 = indices2.size();
    cout << "load model" << "\n";
}



void LoadTextures()
{
    int width, height;
    unsigned char* data = nullptr;
    loadBMP_custom("rocks.bmp", width, height, data);
    cout << width << ": height";
    //add as a file variable

    //add to function
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    width, height;
    data = nullptr;
    loadBMP_custom("mountains_height.bmp", width, height, data);
    cout << width << ": height";
    //add as a file variable

    //add to function
    glGenTextures(1, &hightID);
    glBindTexture(GL_TEXTURE_2D, hightID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    width, height;
    data = nullptr;
    loadBMP_custom("rocks-r.bmp", width, height, data);
    cout << width << ": height";
    //add as a file variable

    //add to function
    glGenTextures(1, &roughID);
    glBindTexture(GL_TEXTURE_2D, roughID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    width, height;
    data = nullptr;
    loadBMP_custom("rocks-n.bmp", width, height, data);
    cout << width << ": height";
    //add as a file variable

    //add to function
    glGenTextures(1, &normalID);
    glBindTexture(GL_TEXTURE_2D, normalID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    width, height;
    data = nullptr;
    loadBMP_custom("snow.bmp", width, height, data);
    cout << width << ": height";
    //add as a file variable

    //add to function
    glGenTextures(1, &snowID);
    glBindTexture(GL_TEXTURE_2D, snowID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    width, height;
    data = nullptr;
    loadBMP_custom("grass.bmp", width, height, data);
    cout << width << ": height";
    //add to function
    glGenTextures(1, &grassID);
    glBindTexture(GL_TEXTURE_2D, grassID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    width, height;
    data = nullptr;
    loadBMP_custom("grass-r.bmp", width, height, data);
    cout << width << ": height";
    //add to function
    glGenTextures(1, &grassMID);
    glBindTexture(GL_TEXTURE_2D, grassMID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    width, height;
    data = nullptr;
    loadBMP_custom("snow-r.bmp", width, height, data);
    cout << width << ": height";
    //add to function
    glGenTextures(1, &snowMID);
    glBindTexture(GL_TEXTURE_2D, snowMID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    width, height;
    data = nullptr;
    loadBMP_custom("snow-n.bmp", width, height, data);
    cout << width << ": height";
    //add to function
    glGenTextures(1, &snowNID);
    glBindTexture(GL_TEXTURE_2D, snowNID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    width, height;
    data = nullptr;
    loadBMP_custom("grass-n.bmp", width, height, data);
    cout << width << ": height";
    //add to function
    glGenTextures(1, &grassNID);
    glBindTexture(GL_TEXTURE_2D, grassNID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

}

//and flowers
void LoadWaterTextures()
{
    int width, height;
    unsigned char* data = nullptr;
    loadBMP_custom("foam.bmp", width, height, data);
    cout << width << ": height";
    //add as a file variable

    //add to function
    glGenTextures(1, &WaterTextureID);
    glBindTexture(GL_TEXTURE_2D, WaterTextureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);



    loadBMP_custom("flower.bmp", width, height, data);
    cout << width << ": height";
    //add as a file variable

    //add to function
    glGenTextures(1, &flowerID);
    glBindTexture(GL_TEXTURE_2D, flowerID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    loadBMP_custom("OIP.bmp", width, height, data);
    cout << width << ": height";
    //add as a file variable

    //add to function
    glGenTextures(1, &flower2ID);
    glBindTexture(GL_TEXTURE_2D, flower2ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);
}
bool readAndCompileShader(const char* shader_path, const GLuint& id)
{

    string shaderCode;
    ifstream shaderStream(shader_path, std::ios::in);
    if (shaderStream.is_open())
    {
        stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    }
    else
    {
        cout << "Impossible to open " << shader_path << ". Are you in the right directory?" << endl;
        return false;
    }
    cout << "Compiling shader :" << shader_path << endl;
    char const* sourcePointer = shaderCode.c_str();
    glShaderSource(id, 1, &sourcePointer, NULL);
    glCompileShader(id);
    GLint Result = GL_FALSE;
    int InfoLogLength;
    glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        vector<char> shaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(id, InfoLogLength, NULL, &shaderErrorMessage[0]);
        cout << &shaderErrorMessage[0] << endl;
    }
    cout << "Compilation of Shader: " << shader_path << " " << (Result == GL_TRUE ? "Success" : "Failed!") << endl;
    return Result == 1;
}
void LoadShaders(GLuint& program, const char* vertex_file_path, const char* fragment_file_path)
{
    // Create the shaders - tasks 1 and 2
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    bool vok = readAndCompileShader(vertex_file_path, VertexShaderID);
    bool fok = readAndCompileShader(fragment_file_path, FragmentShaderID);
    if (vok && fok)
    {
        GLint Result = GL_FALSE;
        int InfoLogLength;
        cout << "Linking program" << endl;
        program = glCreateProgram();
        glAttachShader(program, VertexShaderID);
        glAttachShader(program, FragmentShaderID);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &Result);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            cout << &ProgramErrorMessage[0];
        }
        std::cout << "Linking program: " << (Result == GL_TRUE ? "Success" : "Failed!") << std::endl;
    }
    else
    {
        std::cout << "Program will not be linked: one of the shaders has an error" << std::endl;
    }
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
}
void LoadShadersGeo(GLuint& program, const char* vertex_file_path, const char* fragment_file_path, const char* geo_file_path)
{
    // Create the shaders - tasks 1 and 2
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint GeoShaderID = glCreateShader(GL_GEOMETRY_SHADER);
    bool vok = readAndCompileShader(vertex_file_path, VertexShaderID);
    bool fok = readAndCompileShader(fragment_file_path, FragmentShaderID);
    bool gok = readAndCompileShader(geo_file_path, GeoShaderID);
    if (vok && fok && gok)
    {
        GLint Result = GL_FALSE;
        int InfoLogLength;
        cout << "Linking program" << endl;
        program = glCreateProgram();
        glAttachShader(program, VertexShaderID);
        glAttachShader(program, FragmentShaderID);
        glAttachShader(program, GeoShaderID);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &Result);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            cout << &ProgramErrorMessage[0];
        }
        std::cout << "Linking program: " << (Result == GL_TRUE ? "Success" : "Failed!") << std::endl;
    }
    else
    {
        std::cout << "Program will not be linked: one of the shaders has an error" << std::endl;
    }
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    glDeleteShader(GeoShaderID);
}

void LoadShadersTess(GLuint& program, const char* vertex_file_path, const char* fragment_file_path, const char* eval_path, const char* control_path)
{
    // Create the shaders - tasks 1 and 2
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint controlTessID = glCreateShader(GL_TESS_CONTROL_SHADER);
    GLuint evalTessID = glCreateShader(GL_TESS_EVALUATION_SHADER);
    bool vok = readAndCompileShader(vertex_file_path, VertexShaderID);
    bool fok = readAndCompileShader(fragment_file_path, FragmentShaderID);
    bool cok = readAndCompileShader(control_path, controlTessID);
    bool eok = readAndCompileShader(eval_path, evalTessID);

    if (vok && fok && cok && eok)
    {
        GLint Result = GL_FALSE;
        int InfoLogLength;
        cout << "Linking program" << endl;
        program = glCreateProgram();
        glAttachShader(program, VertexShaderID);
        glAttachShader(program, FragmentShaderID);
        glAttachShader(program, controlTessID);
        glAttachShader(program, evalTessID);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &Result);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            cout << &ProgramErrorMessage[0];
        }
        std::cout << "Linking program: " << (Result == GL_TRUE ? "Success" : "Failed!") << std::endl;
    }
    else
    {
        std::cout << "Program will not be linked: one of the shaders has an error" << std::endl;
    }
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    glDeleteShader(evalTessID);
    glDeleteShader(controlTessID);

}
void LoadShadersGeoTess(GLuint& program, const char* vertex_file_path, const char* fragment_file_path, const char* eval_path, const char* control_path, const char* geoPath)
{
    // Create the shaders - tasks 1 and 2
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint controlTessID = glCreateShader(GL_TESS_CONTROL_SHADER);
    GLuint evalTessID = glCreateShader(GL_TESS_EVALUATION_SHADER);
    GLuint geoID = glCreateShader(GL_GEOMETRY_SHADER);
    bool vok = readAndCompileShader(vertex_file_path, VertexShaderID);
    bool fok = readAndCompileShader(fragment_file_path, FragmentShaderID);
    bool cok = readAndCompileShader(control_path, controlTessID);
    bool eok = readAndCompileShader(eval_path, evalTessID);
    bool gok = readAndCompileShader(geoPath, geoID);

    if (vok && fok && cok && eok && gok)
    {
        GLint Result = GL_FALSE;
        int InfoLogLength;
        cout << "Linking program" << endl;
        program = glCreateProgram();
        glAttachShader(program, VertexShaderID);
        glAttachShader(program, FragmentShaderID);
        glAttachShader(program, controlTessID);
        glAttachShader(program, evalTessID);
        glAttachShader(program, geoID);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &Result);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0)
        {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            cout << &ProgramErrorMessage[0];
        }
        std::cout << "Linking program: " << (Result == GL_TRUE ? "Success" : "Failed!") << std::endl;
    }
    else
    {
        std::cout << "Program will not be linked: one of the shaders has an error" << std::endl;
    }
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    glDeleteShader(evalTessID);
    glDeleteShader(controlTessID);

}

unsigned int skyboxVAO, skyboxVBO;
void LoadCube()
{
    float Vertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
unsigned int cubeID;
void loadCubemap()
{
    vector<std::string> faces
    {
        "skybox/right.bmp",
        "skybox/left.bmp",
        "skybox/top.bmp",
        "skybox/bottom.bmp",
        "skybox/back.bmp",
        "skybox/front.bmp",
    };

    unsigned char* data = nullptr;
    glGenTextures(1, &cubeID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeID);

    int width, height;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        loadBMP_custom(faces[i].c_str(), width, height, data);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            delete[] data;
        }

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


}
int main()
{
    float scaleFactor = 1.0f;
    if (!initializeGL()) return -1;
    cout << "init gl" << "\n";
    LoadModel();
    LoadModel2();


    LoadTextures();
    // LoadModelTess();
    LoadCube();
    loadCubemap();
    LoadWaterTextures();

    cout << "load model" << "\n";
    GLuint programID = glCreateProgram();
    GLuint programID2 = glCreateProgram();
    GLuint programID3 = glCreateProgram();
    GLuint programID4 = glCreateProgram();
    GLuint programID5 = glCreateProgram();
    LoadShaders(programID, "shaders/Basic.vert", "shaders/Texture.frag");
    LoadShaders(programID2, "shaders/skybox.vert", "shaders/skybox.frag");
    LoadShaders(programID3, "shaders/water.vert", "shaders/water.frag");
    LoadShadersGeo(programID4, "shaders/bill.vert", "shaders/bill.frag", "shaders/bill.geom");
    LoadShadersTess(programID5, "shaders/tess.vert", "shaders/tessShade.frag", "shaders/eval.tese", "shaders/control.tesc");

    glClearColor(0.7f, 0.8f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glm::vec4 lightPos = glm::vec4(0, 0, -0.5, 1.0);




    do {

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            LoadShaders(programID, "shaders/Basic.vert", "shaders/Texture.frag");
            LoadShaders(programID2, "shaders/skybox.vert", "shaders/skybox.frag");
            LoadShaders(programID3, "shaders/water.vert", "shaders/water.frag");
            LoadShadersGeo(programID4, "shaders/bill.vert", "shaders/bill.frag", "shaders/bill.geom");
            LoadShadersTess(programID5, "shaders/tess.vert", "shaders/tessShade.frag", "shaders/eval.tese", "shaders/control.tesc");


        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            // Rotate lightPos around the X-axis (pitch)
            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::rotate(rot, glm::radians(0.5f), glm::vec3(0.0, 0.0, 1.0)); // roate around z aixs
            lightPos = rot * lightPos;

        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // Rotate lightPos around the X-axis (pitch)

            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::rotate(rot, glm::radians(-0.5f), glm::vec3(0.0, 0.0, 1.0)); // roate around z aixs
            lightPos = rot * lightPos;

        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // Rotate lightPos around the X-axis (pitch)



            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::rotate(rot, glm::radians(0.5f), glm::vec3(1.0, 0.0, 0.0)); // roate around z aixs
            lightPos = rot * lightPos;


        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // Rotate lightPos around the X-axis (pitch)



            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::rotate(rot, glm::radians(-0.5f), glm::vec3(1.0, 0.0, 0.0)); // roate around z aixs
            lightPos = rot * lightPos;


        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            // Rotate lightPos around the X-axis (pitch)



            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::rotate(rot, glm::radians(0.5f), glm::vec3(0.0, 1.0, 0.0)); // roate around z aixs
            lightPos = rot * lightPos;


        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            // Rotate lightPos around the X-axis (pitch)



            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::rotate(rot, glm::radians(-0.5f), glm::vec3(0.0, 1.0, 0.0)); // roate around z aixs
            lightPos = rot * lightPos;


        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {

            scaleFactor += 0.1;
            // Rotate lightPos around the X-axis (pitch)


        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {

            scaleFactor -= 0.1;
            // Rotate lightPos around the X-axis (pitch)


        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS) {

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 scale = glm::mat4(1.0f);
        //glm::mat4 ScaleMatrix = glm::scale(scale, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;


        //skybox Pass------------------------------------------
        glUseProgram(programID2);
        glDepthMask(GL_FALSE);
        glm::mat4 skyView = ProjectionMatrix * glm::mat4(glm::mat3(ViewMatrix));
        GLuint skyID = glGetUniformLocation(programID2, "MVP");

        glUniformMatrix4fv(skyID, 1, GL_FALSE, &skyView[0][0]);


        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeID);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthMask(GL_TRUE);

        float currentTime = glfwGetTime();


        //terrian geo pass-------------------------------------
        glUseProgram(programID5);
        GLuint MatrixID = glGetUniformLocation(programID5, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


        GLuint lightID = glGetUniformLocation(programID5, "lightPos");
        glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

        GLuint postionID = glGetUniformLocation(programID5, "postionCam");
        glUniform3f(postionID, getCameraPosition().x, getCameraPosition().y, getCameraPosition().z);

        GLuint viewID = glGetUniformLocation(programID5, "ViewMatrix");
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &ViewMatrix[0][0]);

        GLuint scaleID = glGetUniformLocation(programID5, "scale");
        glUniform1f(scaleID, scaleFactor);

        GLuint pointId = glGetUniformLocation(programID5, "pointsnum");
        glUniform1f(pointId, n_points);

        glBindVertexArray(VertexArrayID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, hightID);


        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, roughID);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, normalID);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, snowID);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, grassID);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, grassMID);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, snowMID);

        glActiveTexture(GL_TEXTURE8);
        glBindTexture(GL_TEXTURE_2D, grassNID);
        glActiveTexture(GL_TEXTURE9);
        glBindTexture(GL_TEXTURE_2D, snowNID);

        glPatchParameteri(GL_PATCH_VERTICES, 3);
        glDrawElements(
            GL_PATCHES,
            // mode
            (GLsizei)nIndices,
            // count
            GL_UNSIGNED_INT, // type
            (void*)0
            // element array buffer offset
        );



        //bill board pass ---------------------------------
        glUseProgram(programID4);
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        MatrixID = glGetUniformLocation(programID4, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


        lightID = glGetUniformLocation(programID4, "lightPos");
        glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

        postionID = glGetUniformLocation(programID4, "postionCam");
        glUniform3f(postionID, getCameraPosition().x, getCameraPosition().y, getCameraPosition().z);

        viewID = glGetUniformLocation(programID4, "ViewMatrix");
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &ViewMatrix[0][0]);

        GLuint projID = glGetUniformLocation(programID4, "projectionMatrix");
        glUniformMatrix4fv(projID, 1, GL_FALSE, &ProjectionMatrix[0][0]);

        scaleID = glGetUniformLocation(programID4, "scale");
        glUniform1f(scaleID, scaleFactor);


        GLuint timeID = glGetUniformLocation(programID4, "time");
        glUniform1f(timeID, currentTime);


        skyID = glGetUniformLocation(programID4, "sky");
        glUniformMatrix4fv(skyID, 1, GL_FALSE, &skyView[0][0]);

        glActiveTexture(GL_TEXTURE12);
        glBindTexture(GL_TEXTURE_2D, flowerID);
        glActiveTexture(GL_TEXTURE13);
        glBindTexture(GL_TEXTURE_2D, flower2ID);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, grassID);

        glBindVertexArray(VertexArrayID2);
        glDrawElements(
            GL_TRIANGLE_STRIP,
            // mode
            (GLsizei)nIndices2,
            // count
            GL_UNSIGNED_INT, // type
            (void*)0
            // element array buffer offset
        );
        glUseProgram(programID3);
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        MatrixID = glGetUniformLocation(programID3, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


        lightID = glGetUniformLocation(programID3, "lightPos");
        glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

        postionID = glGetUniformLocation(programID3, "postionCam");
        glUniform3f(postionID, getCameraPosition().x, getCameraPosition().y, getCameraPosition().z);

        viewID = glGetUniformLocation(programID3, "ViewMatrix");
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &ViewMatrix[0][0]);

        scaleID = glGetUniformLocation(programID3, "scale");
        glUniform1f(scaleID, scaleFactor);




        GLuint timeID2 = glGetUniformLocation(programID3, "time");
        glUniform1f(timeID2, currentTime);

        skyID = glGetUniformLocation(programID3, "sky");
        glUniformMatrix4fv(skyID, 1, GL_FALSE, &skyView[0][0]);

        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, WaterTextureID);


        glBindVertexArray(VertexArrayID2);
        glDrawElements(
            GL_TRIANGLE_STRIP,
            // mode
            (GLsizei)nIndices2,
            // count
            GL_UNSIGNED_INT, // type
            (void*)0
            // element array buffer offset
        );
        //end of render -----------------------------------------

        // Swap buffers
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
    return 0;
}