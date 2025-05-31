#include <ng-log/logging.h>
#include "utils/MeshFactory.h"

int main(int argc, char** argv) {
	FLAGS_colorlogtostderr = 1;
	FLAGS_logtostderr = 1;

	using namespace chloride;

	nglog::InitializeLogging(argv[0]);
	LOG(INFO) << "program started";

	Singleton<MeshFactory>::get().loadMesh<BaseMeshHandler, size_t>("C:\\Users\\Alessia\\Documents\\model\\anarchy.fbx", 16);

	nglog::ShutdownLogging();
	return 0;
}