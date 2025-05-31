#ifndef BasicMeshHandler_h
#define BasicMeshHandler_h

#include "../RenderHandler.h"
#include <glad/glad.h>

namespace chloride {

	

	class MeshData {
	private:
		std::vector<Vertex> vertexList;
		std::vector<Face> faces;
		std::map<std::string, Texture> textureMap;

	public:
		std::vector<Vertex>& getVertex();
		std::vector<Face>& getFace();
		void addTexture(const std::string& key, Texture&& texture);
	};

	/**
	 * @brief basic mesh handler handles the mesh without alteration
	 */
	class BaseMeshHandler : public RenderHandler {
	protected:
		std::unique_ptr<MeshData> data = std::make_unique<MeshData>();
		
		GLuint VAO, VBO, EBO;

	public:
		BaseMeshHandler(size_t instancePerCall);
		const std::unique_ptr<MeshData>& getMeshData();

		virtual void execute() override;
		virtual void initialize() override;

	};

};

#endif