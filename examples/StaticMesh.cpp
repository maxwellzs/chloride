#include <ng-log/logging.h>
#include "utils/MeshFactory.h"
#include "engine/Engine.h"

class ActorImpl : public chloride::Actor {
private:
	float a, b;
	float theta = 0;
public:
	ActorImpl(float a, float b) : a(a), b(b) {}

	virtual void update(chloride::Engine& ctx, clock_t delta) override {
		theta += delta * 1.0 / 1000;
		rotation.z = theta;
		location.y = 5.0 + a * sin(theta);
		location.x = b * cos(theta);
		Actor::update(ctx, delta);
	}
};

int main(int argc, char** argv) {
	FLAGS_colorlogtostderr = 1;
	FLAGS_logtostderr = 1;

	using namespace chloride;

	nglog::InitializeLogging(argv[0]);
	LOG(INFO) << "program started";

	//RenderManager* engine1 = &Singleton<RenderManager>::get();
	//RenderManager* engine2 = &Singleton<RenderManager>::get();

	//LOG(INFO) << "engine1 = "  << engine1 << " " << "engine2 = "  << engine2 << " equals=" << (engine1 == engine2);

	Engine::get().bindInterface(std::make_unique<Interface>(800, 800, "static mesh"));
	Engine::get().setCurrentStage(std::make_unique<Stage>());

	auto staticMeshActor1 = std::make_unique<ActorImpl>(-1.0, -1.5);
	auto staticMeshActor2 = std::make_unique<ActorImpl>(1.0, 1.5);

	staticMeshActor1->getLocation().z = 1.5;
	staticMeshActor2->getLocation().z = -1.5;


	staticMeshActor1->bindMesh(MeshFactory::get().loadMesh<BaseMeshHandler, size_t>("C:\\Users\\Alessia\\Documents\\model\\anarchy.fbx", 16));
	staticMeshActor2->bindMesh(MeshFactory::get().loadMesh<BaseMeshHandler, size_t>("C:\\Users\\Alessia\\Documents\\model\\anarchy.fbx", 16));

	//staticMeshActor->getLocation().x = 2.1;

	Engine::get().getCurrentStage()->addActor(std::move(staticMeshActor1));
	Engine::get().getCurrentStage()->addActor(std::move(staticMeshActor2));


	Engine::get().run();

	nglog::ShutdownLogging();
	return 0;
}