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

void chloride::MeshData::addTexture(const std::string& key, Texture&& texture)
{
	if (textureMap.count(key)) {
		throw std::runtime_error("duplicated texture id : " + key);
	}
	textureMap.insert({key, std::move(texture)});
}

chloride::BaseMeshHandler::BaseMeshHandler(size_t instancePerCall)
	: RenderHandler(instancePerCall)
{
}

const std::unique_ptr<chloride::MeshData>& chloride::BaseMeshHandler::getMeshData()
{
	// TODO: insert return statement here
	return data;
}

void chloride::BaseMeshHandler::execute()
{
	// visit every instance and update uniform
	size_t instanceCount = 0;
	for (auto i = instanceMap.begin(); i != instanceMap.end(); ++i, ++instanceCount) {
		if (instanceCount == 0) {
			// call draw
		}
	}
}

void chloride::BaseMeshHandler::initialize()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/*
		|<-	vertex x,y,z (12 bytes) ->|<- normal x,y,z (12 bytes) ->|<- uv x,y (8 bytes) ->|
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*> (sizeof(vec3)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*> (sizeof(vec3) * 2));
	glEnableVertexAttribArray(2);

	glBufferData(GL_ARRAY_BUFFER, data->getVertex().size() * sizeof(Vertex), data->getVertex().data(), GL_STATIC_DRAW);
}
