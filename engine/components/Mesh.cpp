#include "Mesh.h"

chloride::Mesh::Mesh(std::unique_ptr<RenderInstanceRef> ref)
	: ref(std::move(ref))
{
}

void chloride::Mesh::updateGeometry(const mat4& actorRotation, const mat4& actorTranslation)
{
	ref->setUniform("actorRotation", std::make_unique<uniform>(actorRotation));
	ref->setUniform("actorTranslation", std::make_unique<uniform>(actorTranslation));
}
