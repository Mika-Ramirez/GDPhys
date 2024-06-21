    #include <algorithm>
    #include <iomanip> 
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
    #include "P6/PhysicsParticles.h"
    #include "P6/PhysicsWorld.h"

    #include <iostream>
    #include <string>
    #include <chrono>
    #include <cmath> // For std::sqrt

    #define TINYOBJLOADER_IMPLEMENTATION
    #include "tiny_obj_loader.h"

    using namespace std::chrono_literals;
    constexpr std::chrono::nanoseconds timestep(4ms);

    // Position
    float theta = 0.0f;

    // Rotation
    float axis_x = 0.0f;
    float axis_y = 1.0f;
    float axis_z = 0.0f;

    // Position and Scale
    float scale = 0.5f;

    static std::string ordinals(int num) {
        switch (num % 10) {
        case 1:
            return "st";

        case 2:
            return "nd";

        case 3:
            return "rd";

        default:
            return "th";
        }
    }

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
        float window_width = 700;
        float window_height = 700;

        if (!glfwInit())
            return -1;

        window = glfwCreateWindow(window_width, window_height, "PC01 Catrina Mikaela G. Ramirez", NULL, NULL);
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

        glViewport(0, 0, 700, 700);

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

        //4 Particles
        std::vector<P6::PhysicsParticles> particles = { P6::PhysicsParticles(), P6::PhysicsParticles(), P6::PhysicsParticles(), P6::PhysicsParticles() };

        glm::mat4 identity_matrix = glm::mat4(1.0f);
    
        //Ortho cam
        glm::mat4 projectionMatrix = glm::ortho(-350.f, 350.f, -350.f, 350.f, -350.f, 350.f);

        //Scale of the particles
        P6::MyVector scaleVec(10, 10, 10);

        using clock = std::chrono::high_resolution_clock;
        auto start_time = clock::now();
        auto curr_time = clock::now();
        auto prev_time = curr_time;

        std::chrono::nanoseconds curr_ns(0);
        std::map<float, int> arrivalTimes;

        P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    
        //Green Top-right
        particles[0].Position = P6::MyVector(200, 200, 173);
        particles[0].Velocity = particles[0].Position.direction().ScalarMultiplication(90.f).ScalarMultiplication(-1.f);
        particles[0].Acceleration = particles[0].Position.direction().ScalarMultiplication(8.f).ScalarMultiplication(-1.f);

        pWorld.AddParticle(&particles[0]);


        //Red Top-left
        particles[1].Position = P6::MyVector(-200, 200, 201);
        particles[1].Velocity = particles[1].Position.direction().ScalarMultiplication(80.f).ScalarMultiplication(-1.f);
        particles[1].Acceleration = particles[1].Position.direction().ScalarMultiplication(14.5f).ScalarMultiplication(-1.f);

        pWorld.AddParticle(&particles[1]);


        //Blue Bottom-left
        particles[2].Position = P6::MyVector(200, -200, -300);
        particles[2].Velocity = particles[2].Position.direction().ScalarMultiplication(130.f).ScalarMultiplication(-1.f);
        particles[2].Acceleration = particles[2].Position.direction().ScalarMultiplication(1.f).ScalarMultiplication(-1.f);

        pWorld.AddParticle(&particles[2]);


        //Yellow Bottom-right
        particles[3].Position = P6::MyVector(-200, -200, -150);
        particles[3].Velocity = particles[3].Position.direction().ScalarMultiplication(110.f).ScalarMultiplication(-1.f);
        particles[3].Acceleration = particles[3].Position.direction().ScalarMultiplication(3.f).ScalarMultiplication(-1.f);

        pWorld.AddParticle(&particles[3]);


        //Clock Start and end Time
        std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> startTime(4, clock::now());
        std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> endTime(4);

        //Colors of the Particles
        glm::vec3 colors[4] = {
            glm::vec3(0.0f, 1.0f, 0.0f),// Green
            glm::vec3(1.0f, 0.0f, 0.0f), // Red
            glm::vec3(0.0f, 0.0f, 1.0f), // Blue
            glm::vec3(1.0f, 1.0f, 0.0f)  // Yellow
        };


        std::vector<bool> finished(4, false);
        std::vector<float> MagVelocity(4, 0.0f);

        bool resultPrinted = false;
        bool all_finished = false;

        //Initial Velocity
        std::vector<P6::MyVector> initialVelocities = {
            particles[0].Velocity,
            particles[1].Velocity,
            particles[2].Velocity,
            particles[3].Velocity
        };

        float epsilon = 1.0f; 

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
                all_finished = true;

                for (int i = 0; i < 4; ++i) {
                    if (!finished[i]) {

                   
                        float pos_x = particles[i].Position.x;
                        float pos_y = particles[i].Position.y;
                        float pos_z = particles[i].Position.z;

                        pWorld.Update((float)ms.count() / 1000);

                    

                        if (pos_x == 0 && pos_y == 0 && pos_z == 0) {

                            particles[0].Destroy();
                            particles[1].Destroy();
                            particles[2].Destroy();
                            particles[3].Destroy();

                        }


                    

                        //Where particles stopped when it hits the 0,0,0
                        if (std::abs(pos_x) < epsilon && std::abs(pos_y) < epsilon && std::abs(pos_z) < epsilon) {

                            finished[i] = true;
                            endTime[i] = clock::now();
                            MagVelocity[i] = particles[i].Velocity.Magnitude();

                        } else {

                            all_finished = false;
                        }
                    }
                }
            }

            //Simulation Starts
            if (all_finished && !resultPrinted) {
                std::vector<int> index = { 0, 1, 2, 3 };

                //For Time
                for (int i = 0; i < index.size() - 1; i++) {
                    for (int j = 0; j < index.size() - i - 1; j++) {
                        auto time_diff_a = std::chrono::duration_cast<std::chrono::milliseconds>(endTime[index[j]] - startTime[index[j]]);
                        auto time_diff_b = std::chrono::duration_cast<std::chrono::milliseconds>(endTime[index[j + 1]] - startTime[index[j + 1]]);

                        if (time_diff_a.count() > time_diff_b.count()) {
                            std::swap(index[j], index[j + 1]);
                        }
                    }
                }

                //Ranking for the particles
                int rank = 1;
                for (int i : index) {
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime[i] - startTime[i]).count();

                    //Computing for the Average Velocity
                    float avgVelocityX = (initialVelocities[i].x + particles[i].Velocity.x) / 2.0f;
                    float avgVelocityY = (initialVelocities[i].y + particles[i].Velocity.y) / 2.0f;
                    float avgVelocityZ = (initialVelocities[i].z + particles[i].Velocity.z) / 2.0f;

                    //Displaying the Particles
                    std::cout << rank << ordinals(rank) << " : ";
                    if (colors[i] == glm::vec3(0.0f, 1.0f, 0.0f)) {
                        std::cout << "Green" << std::endl;
                    } else if (colors[i] == glm::vec3(1.0f, 0.0f, 0.0f)) {
                        std::cout << "Red" << std::endl;
                    } else if (colors[i] == glm::vec3(0.0f, 0.0f, 1.0f)) {
                        std::cout << "Blue" << std::endl;
                    } else if (colors[i] == glm::vec3(1.0f, 1.0f, 0.0f)) {
                        std::cout << "Yellow" << std::endl;
                    }           

                    //Displaying the Magnitude, Average Velocity and the seconds when it hits the 0,0,0
                    std::cout << "Mag. of Velocity: " << std::fixed << std::setprecision(2) << MagVelocity[i] << " m/s" << std::endl;
                    std::cout << "Average Velocity: (" << std::fixed << std::setprecision(2) << avgVelocityX << ", " << avgVelocityY << ", " << avgVelocityZ << ") m/s" << std::endl;
                    std::cout << static_cast<float>(elapsed) / 1000.f << " secs" << std::endl;
                    std::cout << "\n";
                    rank++;
                }
                resultPrinted = true;
            }

            glUseProgram(shaderProg);
            glBindVertexArray(VAO);

            for (int i = 0; i < 4; ++i) {
                glm::mat4 transformation_matrix = glm::translate(identity_matrix, (glm::vec3)particles[i].Position);
                transformation_matrix = glm::scale(transformation_matrix, (glm::vec3)scaleVec);
                transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));

                unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
                unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
                glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

                unsigned int colorLoc = glGetUniformLocation(shaderProg, "objectColor");
                glUniform3fv(colorLoc, 1, glm::value_ptr(colors[i]));

                glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
