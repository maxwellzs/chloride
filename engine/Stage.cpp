#include "Stage.h"

void chloride::Stage::addActor(std::unique_ptr<Actor> actor)
{
	actorList.emplace_back(std::move(actor));
}

void chloride::Stage::forEach(std::function<void(const std::unique_ptr<Actor>&)> callback)
{
	for (auto& i : actorList) {
		callback(i);
	}
}
