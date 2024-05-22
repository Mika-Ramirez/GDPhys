#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "P6/PhysicsParticles.h"

#include <iostream>
#include <string>
#include <chrono>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms);

//Position
float theta = 0.0f;

//Rotation
float axis_x = 0.0f;
float axis_y = 1.0f;
float axis_z = 0.0f;

// Position and Scale
float x_mod = 0.0f;
float y_mod = 0.0f;
float z_mod = -3.0f;

float scale = 0.5f;

int main(void)
{
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLFWwindow* window;
    float window_width = 800;
    float window_height = 800;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(window_width, window_height, "Ramirez, Catrina Mikaela G. & Jacinto, Gracielle Ann R.", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f, NULL);
    glCompileShader(fragmentShader);
    GLuint shaderProg = glCreateProgram();


    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader);
    glLinkProgram(shaderProg);

    glViewport(0, // Min x
        0,//Min y
        800,//Width
        800); // Height
    

    std::string path = "3D/sphere.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    GLfloat vertices[]
    {
        //x  //y   //z
        0.f, 0.5f, 0.f, //0
        -0.5f, -0.5f, 0.f, //1
        0.5f, -0.5f, 0.f //2

    };

    GLuint indices[]
    {
        0,1,2
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size(),
        attributes.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    P6::PhysicsParticles particle = P6::PhysicsParticles();

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    glm::mat4 projectionMatrix = glm::ortho( -400.f, 400.f, -400.f, 400.f, -1.f, 100.f);   
   
    P6::MyVector scale(50, 50, 50);

    int px, py, pz;
    std::cout << "X Velocity: ";
    std::cin >> px;
    std::cout << "Y Velocity: ";
    std::cin >> py;
    std::cout << "Z Velocity: ";
    std::cin >> pz;

    using clock = std::chrono::high_resolution_clock;
    auto start_time = clock::now();
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    particle.Position = P6::MyVector(0, -350, 0);
    particle.Velocity = P6::MyVector(px, py, pz);
    particle.Acceleration = P6::MyVector(0, -50, 0);

    bool end = false;

    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);

        curr_time = clock::now();

        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;

        curr_ns += dur;

        if (curr_ns >= timestep) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            curr_ns -= curr_ns;
            particle.Update((float)ms.count() / 1000);
        }
        
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, (glm::vec3)particle.Position);
        transformation_matrix = glm::scale(transformation_matrix, (glm::vec3) scale);
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix)); 
        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        if (particle.Position.y <= -351.f && !end) {
            auto end_time = clock::now();
            auto land = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            std::cout << "It took " << land.count() / 1000.f << " seconds for it to land";
            end = true;
        }
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}