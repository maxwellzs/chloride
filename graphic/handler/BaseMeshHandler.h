#ifndef BasicMeshHandler_h
#define BasicMeshHandler_h

#include "../RenderHandler.h"
#include "graphic/ShaderFactory.h"
#include <glad/glad.h>

namespace chloride {

	

	class MeshData {
	private:
		std::vector<Vertex> vertexList;
		std::vector<Face> faces;
		std::map<std::string, std::unique_ptr<Texture>> textureMap;

	public:
		std::vector<Vertex>& getVertex();
		std::vector<Face>& getFace();
		void addTexture(const std::string& key, std::unique_ptr<Texture> texture);
		const std::unique_ptr<Texture>& getTexture(const std::string& name) {
			if (!textureMap.count(name)) throw std::runtime_error("unknown map : " + name);
			return textureMap.at(name);
		}
	};

	/**
	 * @brief basic mesh handler handles the mesh without alteration
	 */
	class BaseMeshHandler : public RenderHandler {
	protected:

		std::unique_ptr<MeshData> data = std::make_unique<MeshData>();
		
		GLuint VAO, VBO, EBO, textureDiffuse;
		size_t vertexDrawCount;
		GLuint program;

		void checkGLError();

		/**
		 * @brief find the uniform variable location, if not found, raise an
		 * exception
		 * @param key the name of the variable 
		 * @param instance instance id
		 * @return position
		 */
		GLint locateUniform(const std::string& key, size_t instance);
		GLint locateUniform(const std::string& key);

		void setUniform(const std::string& key, const mat4& m) { glUniformMatrix4fv(locateUniform(key), 1, GL_TRUE, m.data); }
		void setUniform(const std::string& key, const vec4& v) { glUniform4f(locateUniform(key), v.x, v.y, v.z, v.w); }
		void setUniform(const std::string& key, const vec3& v) { glUniform3f(locateUniform(key), v.x, v.y, v.z); }
		void setUniform(const std::string& key, const vec2& v) { glUniform2f(locateUniform(key), v.x, v.y); }
		void setUniform(const std::string& key, uint32_t v) { glUniform1ui(locateUniform(key), v); }
		void setUniform(const std::string& key, float v) { glUniform1f(locateUniform(key), v); }
		void setUniform(const std::string& key, size_t instance, const mat4& m) { glUniformMatrix4fv(locateUniform(key, instance), 1, GL_TRUE, m.data); }
		void setUniform(const std::string& key, size_t instance, const vec4& v) { glUniform4f(locateUniform(key, instance), v.x, v.y, v.z, v.w); }
		void setUniform(const std::string& key, size_t instance, const vec3& v) { glUniform3f(locateUniform(key, instance), v.x, v.y, v.z); }
		void setUniform(const std::string& key, size_t instance, const vec2& v) { glUniform2f(locateUniform(key, instance), v.x, v.y); }
		void setUniform(const std::string& key, size_t instance, uint32_t v) { glUniform1ui(locateUniform(key, instance), v); }
		void setUniform(const std::string& key, size_t instance, float v) { glUniform1f(locateUniform(key, instance), v); }


	public:
		BaseMeshHandler(size_t instancePerCall);
		const std::unique_ptr<MeshData>& getMeshData();

		virtual void execute(const mat4& perspective, const mat4& cameraPos, const mat4& cameraRotation) override;
		virtual void initialize() override;

	};

};

#endif