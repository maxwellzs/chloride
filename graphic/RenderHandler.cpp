#include "RenderHandler.h"

chloride::RenderInstanceRef::~RenderInstanceRef()
{
	parent.freeInstance(slot);
}

chloride::RenderInstanceRef::RenderInstanceRef(RenderHandler& parentHandler, size_t slot)
	: parent(parentHandler), slot(slot)
{
}

chloride::RenderInstanceRef& chloride::RenderInstanceRef::setUniform(const std::string& name, std::unique_ptr<uniform> var)
{
	// TODO: insert return statement here
	parent.getInstanceBySlot(slot)->setUniform(name, std::move(var));
	return *this;
}

const std::unique_ptr<chloride::RenderInstance>& chloride::RenderInstanceRef::getInstance()
{
	// TODO: insert return statement here
	return parent.getInstanceBySlot(slot);
}

chloride::RenderInstance& chloride::RenderInstance::setUniform(const std::string& name, std::unique_ptr<uniform> var)
{
	// TODO: insert return statement here

	if (!uniformMap.count(name)) {
		uniformMap.insert({name, std::move(var)});
	}
	else {
		uniformMap.at(name) = std::move(var);
	}
	return *this;
}

void chloride::RenderInstance::forEach(std::function<void(const std::string& key, const std::unique_ptr<uniform>& uniform)> callback)
{
	for (auto& i : uniformMap) {
		callback(i.first, i.second);
	}
}

const std::unique_ptr<chloride::RenderInstance>& chloride::RenderHandler::getInstanceBySlot(size_t slot)
{
	// TODO: insert return statement here
	Spinlock lock(flag);

	if (!instanceMap.count(slot)) {
		throw std::runtime_error("unknown instance slot in render handler !");
	} 
	return instanceMap.at(slot);
}

chloride::RenderHandler::RenderHandler(size_t instancePerCall)
	: INSTANCE_PER_DRAW(instancePerCall)
{
}

std::unique_ptr<chloride::RenderInstanceRef> chloride::RenderHandler::allocateInstance()
{
	Spinlock lock(flag);

	size_t newId = generator.fetch_add(1);

	auto ref = std::make_unique<RenderInstanceRef>(*this, newId);
	// generate a stub instance
	instanceMap.insert({newId, std::make_unique<RenderInstance>() });
	
	return ref;
}

void chloride::RenderHandler::freeInstance(size_t slot)
{
	Spinlock lock(flag);
	// add lock later
	if (instanceMap.count(slot)) {
		// remove instance 
		instanceMap.erase(slot);
	}
}
