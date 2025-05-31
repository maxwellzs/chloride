#ifndef MeshLoader_h
#define MeshLoader_h

#include <memory>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ng-log/logging.h>
#include <png.h>

#include "Singleton.h"
#include "graphic/RenderManager.h"
#include "graphic/handler/BaseMeshHandler.h"
#include "engine/components/Mesh.h"

namespace chloride {

	class MeshFactory {
		friend class Singleton<MeshFactory>;

	private:
		MeshFactory() = default;

		void readFromFile(const std::string& path, const std::unique_ptr<MeshData>& data);
		void loadTexture(const std::unique_ptr<MeshData>& data, const aiScene* scene, const aiMaterial* mat, aiTextureType type);
		void decodeImage(const std::unique_ptr<MeshData>& data, const std::string& key, const char* pngData, size_t dataSize);

	public:
		/**
		 * @brief create a mesh object from source
		 * when a mesh is loaded the first time, a handler with the id path will be created
		 * @tparam HandlerType the handler type to be create
		 * @tparam Args handler constructer argument
		 * @param path 
		 * @return 
		 */
		template<typename HandlerType, typename ... Args>
		std::unique_ptr<Mesh> loadMesh(const std::string& path, Args ... args) {
			static_assert(std::is_base_of<BaseMeshHandler, HandlerType>::value, "Derived must inherit from Base");


			// check if the handler exist
			if (Singleton<RenderManager>::get().handlerExist(path)) {
				const auto& handler = Singleton<RenderManager>::get().getHandlerById(path);
				return std::make_unique<Mesh>(handler->allocateInstance());
			} 

			// handler not exist
			std::unique_ptr<HandlerType> handler = std::make_unique<HandlerType>(args ...);
			// parse data
			readFromFile(path, handler->getMeshData());

			Singleton<RenderManager>::get().addHandler(path, std::move(handler));
			const auto& h = Singleton<RenderManager>::get().getHandlerById(path);

			return std::make_unique<Mesh>(h->allocateInstance());

		}
	
	};

};

#endif
