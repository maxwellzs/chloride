#include <ng-log/logging.h>
#include "utils/MeshFactory.h"
#include "engine/Engine.h"

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

	auto staticMeshActor = std::make_unique<Actor>("static mesh");

	staticMeshActor->bindMesh(MeshFactory::get().loadMesh<BaseMeshHandler, size_t>("C:\\Users\\Alessia\\Documents\\model\\anarchy.fbx", 16));
	staticMeshActor->getLocation().x = 2.1;

	Engine::get().getCurrentStage()->addActor(std::move(staticMeshActor));

	Engine::get().run();

	nglog::ShutdownLogging();
	return 0;
}