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

#include <glm/glm/matrix_transform.hpp>
#include <GLFW/glfw3.h>

struct Vec3 {

    float x;
    float y;
    float z;  
};


struct Transform {

    Vec3 position = {
        0,0,0 
    };

    Vec3 rotation = {
        0,0,0 
    };

    Vec3 scale = {
        1,1,1 
    };

};


class Component {

    public:
        virtual ~Component() = default;
        virtual void Update(float deltaTime) {}
};

class MeshComponent : public Component {
    public:
        std::string meshId;
        std::string materialId;
};

class Entity {
    
public:
    int id;
    Transform transform;
    std::vector<std::unique_ptr<Component>> components;

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) 
    {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = comp.get();
        components.push_back(std::move(comp));
        return ptr;
    }

    template<typename T>
    T* GetComponent() 
    {

        for (auto& c : components) 
        {
            if (auto* cast = dynamic_cast<T*>(c.get())) return cast;
        }
        return nullptr;
    }
};

class Renderer 
{
public:
    void BeginFrame() { 
        /* Clear buffers, bind framebuffer */ 
    }
    
    void DrawEntity(const Entity& entity) 
    {
        //send transform matrix to GPU
        glUniformMatrix4fv(...);
        glDrawElements(...);
    }
    
    void EndFrame() 
    { 
        /* Swap buffers */ 
    }
};

class Engine {
private:
    bool isRunning = true;
    std::vector<std::unique_ptr<Entity>> entities;
    Renderer renderer;
    
    std::chrono::high_resolution_clock::time_point lastTime;

public:
    void Init() 
    {
        lastTime = std::chrono::high_resolution_clock::now();
        // Theoretically init Window (GLFW/SDL), Graphics API (Vulkan/OpenGL)
    }

    Entity* CreateEntity() 
    {
        auto e = std::make_unique<Entity>();
        e->id = entities.size();
        Entity* ptr = e.get();
        entities.push_back(std::move(e));
        return ptr;
    }

    void Run() 
    {
        while (isRunning) 
        {
            auto now = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(now - lastTime).count();
            lastTime = now;

            //Input ---
            ProcessInput();

            // Update 
            for (auto& entity : entities) 
            {
                for (auto& comp : entity->components) 
                {
                    comp->Update(deltaTime);
                }
            }

            // --- 3. Render ---
            renderer.BeginFrame();
            for (auto& entity : entities) 
            {
                renderer.DrawEntity(*entity);
            }
            renderer.EndFrame();
        }
    }

    void ProcessInput() {

    }
};

int main() 
{
    Engine engine;
    engine.Init();

    Entity* player = engine.CreateEntity();

    player->transform.position = {
        0, 0, 5
    };
    
    player->AddComponent<MeshComponent>()->meshId = "main_player.obj";

    engine.Run();
    return 0;
}
