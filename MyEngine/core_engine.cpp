#pragma once 

#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <typindex>
#include <optional>
#include <cstdint>
#include <memory>
#include <unordered_map>  

#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/glm/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class World {

    std::unordered_map<Entity, Transform> transforms;
    std::unordered_map<Entity, Vel> velocities;
    std::unordered_map<Entity, MeshComponent> meshes;
    std::unordered_map<Entity> entities;
    Entity next_id = 0; 

    public:
        Entity create_entity( ) { return next_id++; }

        template<typename T>

        void add_component(Entity e, T comp);

        void update(double dt)
        {

            for(auto& [entity, vel] : velocities)
            {
                if(transforms.contains(entities))
                {
                    transforms.[entity].position += vel.value * (float)dt; 
                }
            }
        }

        auto& get_transforms() {
            return transforms;
        }
        auto& get_meshes() {
            return meshes; 
        }
};

class ResourceManager {

    std::unordered_map<std::string, GLuint> shaders;
    std::unordered_map<std::string, GLuint> textures;

    public: 
        GLuint load_shader(const std::string&, vert_path, const std::string& fragpath)
        {
            if(shaders.contains(vert_path)) return shaders[vert_path];

            GLuint prog = compile_shaders(vert_path, fragpath);
            shaders[vert_path] = prog;
            return prog; 
        }
};


class Renderer {

    ResourceManager& resources;

    public:
        void render(World world, double alpha)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPT_BUFFER_BIT);

            auto shader = resources.load_shader("vert", "frag");
            glUseProgram(shader);

            for(auto& [entity, mesh] : world.get_meshes())
            {
                auto& transform = world.get_transforms()[entity];

                glBindVertexArray(mesh.VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED, 0);
            }
        }
};

struct This_Engine {

    void run()
    {
        using clock = std::chrono::high_resolution_clock;
        auto last_time = clock::now();
        double accum = 0.0; 

        const double dtt = 1.00 / 60.00; 

        while(isrunning)
        {
            auto now = clock::now();
            double frame_time = std::chrono::duration<double>(now - last_time).count();
            last_time = now;
            accum += frame_time;

            process_input();

            while(accum >= dtt)
            {
                world.update(dtt);
                accum -= dtt; 
            }

            renderer.render(world, accum / dtt); 
        }
    }

    private:
        bool isrunning = true;
        World world;
        renderer render;

        void process_input();
};

using Entity = uint32_t;

struct Transform {

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale = {
        1, 1, 1
    };
};

struct Vel {

    glm::vec3 value;
};

struct MeshComponent {

    GLuint VAO;
    GLuint texture;
};
