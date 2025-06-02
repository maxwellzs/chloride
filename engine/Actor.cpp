#include "Actor.h"
#include "Engine.h"

void chloride::Actor::updateGeometry()
{
	mat4 actorTranslation = mat4::move(location);
	mat4 actorRotation = mat4::rotate(rotation);

	actorMesh->updateGeometry(actorRotation, actorTranslation);
}

void chloride::Actor::update(Engine& ctx, clock_t delta)
{
	static double theta = 0;
	theta += delta * 1.0 / 1000;
	rotation.z = theta;
	location.x = 3.0 + sin(theta);
	updateGeometry();
}

chloride::Actor& chloride::Actor::bindMesh(std::unique_ptr<Mesh> mesh)
{
	actorMesh = std::move(mesh);
	return *this;
}
