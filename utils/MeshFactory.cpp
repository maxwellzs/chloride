#include "MeshFactory.h"

void chloride::MeshFactory::readFromFile(const std::string& path, const std::unique_ptr<MeshData>& data)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
  
    if (!scene ||
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || 
        !scene->mRootNode)
    {
        throw std::runtime_error("error loading mesh : " + std::string(importer.GetErrorString()) + " " + path);
        return;
    }

    if (!scene->HasMeshes()) throw std::runtime_error("error missing meshes in imported file : " + path);

    const aiMesh* mesh = scene->mMeshes[0];
    if (!mesh->mNormals) throw std::runtime_error("error missing normal in imported file : " + path);


    // parse vertex
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        auto v = mesh->mVertices[i];
        auto n = mesh->mNormals[i];
        auto t = mesh->mTextureCoords[0][i];
        
        data->getVertex().emplace_back(Vertex(
            vec3(v.x, v.y, v.z), // coordinate
            vec3(n.x, n.y, n.z), // normal
            vec2(t.x, t.y) // texture
        ));
    }

    // parse faces
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        auto f = mesh->mFaces[i];
        if (f.mNumIndices != 3) throw std::runtime_error("non-triangle primitive dectected");
        data->getFace().emplace_back(Face(f.mIndices[0], f.mIndices[1], f.mIndices[2]));
    }
    LOG(INFO) << "loaded in " << data->getVertex().size() << " vertex and " << data->getFace().size() << " faces from mesh \"" << path << "\"";

    auto mat = scene->mMaterials[mesh->mMaterialIndex];
    loadTexture(data, scene, mat, aiTextureType_DIFFUSE);
    loadTexture(data, scene, mat, aiTextureType_SPECULAR);

}

void chloride::MeshFactory::loadTexture(const std::unique_ptr<MeshData>& data, const aiScene* scene, const aiMaterial* mat, aiTextureType type)
{
    for (size_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString path;
        mat->GetTexture(type, i, &path);
        auto t = scene->GetEmbeddedTexture(path.C_Str());
        if (!t) {
            LOG(WARNING) << "texture at path \"" << path.C_Str() << "\" is undefined, probably not properly embedded";
        }
        else {
            if (t->mHeight != 0) throw std::runtime_error("the texture \"" + std::string(path.C_Str()) + "\" is not in png format");
            /*LOG(INFO) << "load " << t->mWidth << " bytes for texture " << path.C_Str() << " type=" << type;
            std::ofstream out("texture.png", std::ios::beg | std::ios::binary);
            out.write(reinterpret_cast<const char*>(t->pcData), t->mWidth);
            out.close();*/
            try
            {
                switch (type)
                {
                case aiTextureType_DIFFUSE: decodeImage(data, "diffuse", reinterpret_cast<const char*>(t->pcData), t->mWidth);
                    break;
                case aiTextureType_SPECULAR: decodeImage(data, "specular", reinterpret_cast<const char*>(t->pcData), t->mWidth);
                    break;
                default:
                    break;
                }
            }
            catch (const std::exception& e)
            {
                throw std::runtime_error("exception happened when loading texture \"" + std::string(path.C_Str()) + "\" :" + std::string(e.what()));
            }
            
        }
    }
}

void chloride::MeshFactory::decodeImage(const std::unique_ptr<MeshData>& data, const std::string& key, const char* pngData, size_t dataSize)
{
    png_structp png_ctx = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ctx) throw std::runtime_error("cannot create png read structure");
    png_infop info = png_create_info_struct(png_ctx);
    if (!info) throw std::runtime_error("cannot create png info structure");

    if (setjmp(png_jmpbuf(png_ctx)) != 0)
    {
        png_destroy_read_struct(&png_ctx, &info, NULL);
        throw std::runtime_error("error reading png data");
    }

    struct ReadStruct {
        size_t dataSize;
        size_t readSize;
        const char* dataPtr;
    };

    ReadStruct read{dataSize, 0, pngData};
  
    png_set_read_fn(png_ctx, &read, [](png_structp png_ptr, png_bytep outBytes, png_size_t bytesToRead) {
        ReadStruct* read = (ReadStruct*)png_get_io_ptr(png_ptr);
        size_t actualRead = read->dataSize - read->readSize > bytesToRead ? bytesToRead : read->dataSize - read->readSize;
        memcpy(outBytes, read->dataPtr + read->readSize, actualRead);
        read->readSize += actualRead;
        });
    png_read_info(png_ctx, info);

    size_t imageWidth = png_get_image_width(png_ctx, info);
    size_t imageHeight = png_get_image_height(png_ctx, info);
    png_byte color = png_get_color_type(png_ctx, info);

    switch (color)
    {
    case PNG_COLOR_TYPE_GRAY:
        png_set_expand(png_ctx);
        png_set_gray_to_rgb(png_ctx);
        png_set_add_alpha(png_ctx, 0xffff, PNG_FILLER_AFTER);
        break;

    case PNG_COLOR_TYPE_RGBA: break;
    case PNG_COLOR_TYPE_GA:
        png_set_expand(png_ctx);
        png_set_gray_to_rgb(png_ctx);

    default:
        throw std::runtime_error("unsupported color type");
        png_destroy_read_struct(&png_ctx, &info, nullptr);
        break;
    }
    png_read_update_info(png_ctx, info);

    // load data 
    size_t rowSize = png_get_rowbytes(png_ctx, info);
    size_t imageSize = rowSize * imageHeight;
    size_t imageDepth = png_get_bit_depth(png_ctx, info);

    if (png_get_channels(png_ctx, info) != 4) throw std::runtime_error("missing channel");

    LOG(INFO) << "size after decode : "
        << imageWidth << " x " << imageHeight << " totle " << imageSize << " bytes, depth=" << imageDepth;

    Texture t(imageSize, imageWidth, imageHeight, imageDepth);

    for (size_t i = 0; i < imageHeight; i++)
    {
        png_read_row(png_ctx, t.data + rowSize * i, nullptr);
    }
    data->addTexture(key, std::move(t));

    png_destroy_read_struct(&png_ctx, &info, NULL);
}

