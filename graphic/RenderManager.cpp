#include "RenderManager.h"

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
}

void chloride::RenderManager::executeAll()
{
	for (auto i = handlerMap.begin(); i != handlerMap.end(); ++i) {
		auto& handler = i->second;
		handler->execute();
	}
}
