#pragma once

#include "EngineDefines.h"
#include "IApplication.h"

#include <vector>
#include <memory>

namespace engine
{

class RenderContext;
class Renderer;
class World;

class Engine
{
public:
	Engine() = delete;
	explicit Engine(std::unique_ptr<IApplication> pApplication);
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(Engine&&) = delete;
	virtual ~Engine();

	static ENGINE_API Engine* Create(std::unique_ptr<IApplication> pApplication);
	static ENGINE_API void Destroy(Engine* pEngine);

	//
	// Init all basic modules.
	//
	ENGINE_API void Init(EngineInitArgs args);

	//
	// Execute main loop.
	//
	ENGINE_API void Run();

	//
	// Shutdown all modules in order.
	//
	ENGINE_API void Shutdown();

private:
	std::unique_ptr<IApplication> m_pApplication;
};

}