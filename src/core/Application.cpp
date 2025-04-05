#include <glad/gl.h> // MUST be first!

#include "core/Application.hpp"

#include "core/Logger.hpp"
#include "core/Window.hpp"
#include "ecs/ECS.hpp"
#include "ecs/component/CameraComponent.hpp"
#include "ecs/component/MeshComponent.hpp"
#include "ecs/system/CameraSystem.hpp"
#include "ecs/system/RenderSystem.hpp"
#include "render/ChunkMesh.hpp"
#include "render/ChunkMeshBuilder.hpp"
#include "world/World.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "ecs/component/TransformComponent.hpp"
#include "ecs/system/ChunkLoadingSystem.hpp"

namespace mc::core
{
Application::Application()
{
    Logger::init();
    Logger::get()->info("Application created");
}

Application::~Application()
{
    shutdown();
    // TODO Fix GL ERROR 1282
}

bool Application::initialize()
{
    if (!initializeWindow()) { return false; }

    initializeECS();
    initializeCamera();
    initializeWorld();
    initializeRenderSystems();

    m_lastFrameTime = glfwGetTime();
    return true;
}

bool Application::initializeWindow()
{
    m_window = std::make_unique<Window>("VoxelGame", 1280, 720);
    return m_window->isOpen();
}

void Application::initializeECS()
{
    m_ecs = std::make_unique<ecs::ECS>();
}

void Application::initializeCamera()
{
    const auto cameraEntity = m_ecs->createEntity();
    m_ecs->addComponent<ecs::CameraComponent>(cameraEntity, ecs::CameraComponent{});
    m_ecs->addComponent<ecs::TransformComponent>(cameraEntity, ecs::TransformComponent{});

    m_cameraSystem = std::make_shared<ecs::CameraSystem>(*m_ecs, 1280.0f / 720.0f);
    m_ecs->addSystem(m_cameraSystem);
}

void Application::initializeWorld()
{
    constexpr uint8_t areaRadius{1};
    m_world = std::make_unique<world::World>();
    m_world->generateInitialArea(areaRadius);

    for (int x = -areaRadius; x <= areaRadius; ++x)
    {
        for (int z = -areaRadius; z <= areaRadius; ++z)
        {
            const glm::ivec3 pos{x, 0, z};
            if (!m_world->isChunkLoaded(pos))
            {
                m_world->loadChunk(pos);
            }
        }
    }
}

void Application::initializeRenderSystems()
{
    m_chunkLoadingSystem = std::make_shared<ecs::ChunkLoadingSystem>(*m_ecs, *m_world, 9);
    m_ecs->addSystem(m_chunkLoadingSystem);

    m_renderSystem = std::make_shared<ecs::RenderSystem>(*m_ecs, m_cameraSystem, *m_world, 8);
    m_ecs->addSystem(m_renderSystem);
}

void Application::run()
{
    if (!initialize()) return;

    while (m_window->isOpen())
    {
        const double currentTime = glfwGetTime();
        const float deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
        m_lastFrameTime = currentTime;

        m_window->pollEvents();
        update(deltaTime);
        render();
        m_window->swapBuffers();
    }
}

void Application::update(float deltaTime)
{
    m_ecs->update(deltaTime);
}

void Application::render()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ecs->update(0.0f); // TODO replace with render()
}

void Application::shutdown()
{
    Logger::get()->info("Shutting down application");
    m_renderSystem.reset();
    m_cameraSystem.reset();
    m_world.reset();
    m_ecs.reset();
    m_window.reset();
}
}
