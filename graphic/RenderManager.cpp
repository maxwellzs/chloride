#include "RenderManager.h"
#include "engine/Engine.h"

chloride::RenderManager::~RenderManager()
{
	Engine::get().stop();
}

chloride::RenderManager& chloride::RenderManager::get()
{
	// TODO: insert return statement here
	static RenderManager instance;
	return instance;
}

bool chloride::RenderManager::handlerExist(const std::string& id)
{
	Spinlock lock(flag);
	return handlerMap.count(id);
}

const std::unique_ptr<chloride::RenderHandler>& chloride::RenderManager::getHandlerById(const std::string& id)
{
	// TODO: insert return statement here
	Spinlock lock(flag);
	if (!handlerMap.count(id)) throw std::runtime_error("invalid handler id");

	return handlerMap.at(id);
}

void chloride::RenderManager::addHandler(const std::string& id, std::unique_ptr<RenderHandler> handler)
{
	Spinlock lock(flag);
	// do not add duplicate handler
	if (handlerMap.count(id)) return;

	handler->initialize();

	handlerMap.insert({id, std::move(handler)});
	LOG(INFO) << "added render handler : " << id;
}

void chloride::RenderManager::setPerspective(const mat4& perspective)
{
	currentPerspective = perspective;
}

void chloride::RenderManager::setCameraLocation(const mat4& location)
{
	currentCameraPos = location;
}

void chloride::RenderManager::setCameraRotation(const mat4& rotation)
{
	currentCameraRotation = rotation;
}

void chloride::RenderManager::executeAll()
{
	Spinlock lock(flag);
	for (auto i = handlerMap.begin(); i != handlerMap.end(); ++i) {
		auto& handler = i->second;
		handler->execute(currentPerspective, currentCameraPos, currentCameraRotation);
	}
}
