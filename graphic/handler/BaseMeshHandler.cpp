#include "BaseMeshHandler.h"

std::vector<chloride::Vertex>& chloride::MeshData::getVertex()
{
	// TODO: insert return statement here
	return vertexList;
}

std::vector<chloride::Face>& chloride::MeshData::getFace()
{
	// TODO: insert return statement here
	return faces;
}

void chloride::MeshData::addTexture(const std::string& key, std::unique_ptr<Texture> texture)
{
	if (textureMap.count(key)) {
		throw std::runtime_error("duplicated texture id : " + key);
	}
	textureMap.insert({key, std::move(texture)});
}

void chloride::BaseMeshHandler::checkGLError()
{
	GLenum errorCode;
	if ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		LOG(FATAL) << "OpenGL error : " << error;
	}
}

GLint chloride::BaseMeshHandler::locateUniform(const std::string& key, size_t instance)
{
	std::string name = key + "[" + std::to_string(instance) + "]";
	int location = glGetUniformLocation(program, name.c_str());
	if (location == -1) throw std::runtime_error("cannot locate the uniform variable : " + key);

	return location;
}

GLint chloride::BaseMeshHandler::locateUniform(const std::string& key)
{
	int location = glGetUniformLocation(program, key.c_str());
	if (location == -1) throw std::runtime_error("cannot locate the uniform variable : " + key);

	return location;
}

chloride::BaseMeshHandler::BaseMeshHandler(size_t instancePerCall)
	: RenderHandler(instancePerCall), program(ShaderFactory::get().loadShader("shader/generic.vert", "shader/generic.frag"))
{
}

const std::unique_ptr<chloride::MeshData>& chloride::BaseMeshHandler::getMeshData()
{
	// TODO: insert return statement here
	return data;
}

void chloride::BaseMeshHandler::execute(const mat4& perspective, const mat4& cameraPos, const mat4& cameraRotation)
{
	Spinlock lock(flag);
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, textureDiffuse);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	checkGLError();

	// visit every instance and update uniform
	// first set all global uniform variable
	setUniform("perspective", perspective);
	setUniform("cameraPosition", cameraPos);
	setUniform("cameraRotation", cameraRotation);

	size_t instanceCount = 0;
	for (auto i = instanceMap.begin(); i != instanceMap.end(); ++i, ++instanceCount) {
		if (instanceCount % INSTANCE_PER_DRAW == 0 &&
			i != instanceMap.begin()) {
			// submit all instance
			
			glDrawElementsInstanced(GL_TRIANGLES, vertexDrawCount, GL_UNSIGNED_INT, 0, INSTANCE_PER_DRAW);
		}


		// bind uniform
		i->second->forEach([&](const std::string& key, const std::unique_ptr<uniform>& val) {
			auto& v = *val.get();
			if (std::holds_alternative<mat4>(v)) {
				setUniform(key, instanceCount % INSTANCE_PER_DRAW, std::get<mat4>(v));
			} else if (std::holds_alternative<vec4>(v)) {
				setUniform(key, instanceCount % INSTANCE_PER_DRAW, std::get<vec4>(v));
			}
			else if (std::holds_alternative<vec3>(v)) {
				setUniform(key, instanceCount % INSTANCE_PER_DRAW, std::get<vec4>(v));
			}
			else if (std::holds_alternative<vec2>(v)) {
				setUniform(key, instanceCount % INSTANCE_PER_DRAW, std::get<vec2>(v));
			}
			else if (std::holds_alternative<uint32_t>(v)) {
				setUniform(key, instanceCount % INSTANCE_PER_DRAW, std::get<uint32_t>(v));
			}
			else if (std::holds_alternative<float>(v)) {
				setUniform(key, instanceCount % INSTANCE_PER_DRAW, std::get<float>(v));
			}
		});
	}

	size_t remain = (instanceCount % INSTANCE_PER_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, vertexDrawCount, GL_UNSIGNED_INT, 0, remain);

}

void chloride::BaseMeshHandler::initialize()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	auto& vertex = data->getVertex();
	auto& faces = data->getFace();
	// submit vertices
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), data->getVertex().data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// submit index
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(Face), faces.data(), GL_STATIC_DRAW);
	vertexDrawCount = data->getFace().size() * 3; // 3 vertex per face

	/*
		|<-	vertex x,y,z (12 bytes) ->|<- normal x,y,z (12 bytes) ->|<- uv x,y (8 bytes) ->|
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*> (sizeof(vec3)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*> (sizeof(vec3)*2));
	glEnableVertexAttribArray(2);

	// textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &textureDiffuse);
	glBindTexture(GL_TEXTURE_2D, textureDiffuse);
	const auto& t = data->getTexture("diffuse");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->data);
	glGenerateMipmap(GL_TEXTURE_2D);
	checkGLError();

	// finally release mesh data
	data = nullptr;
}
