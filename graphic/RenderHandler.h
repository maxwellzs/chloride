#ifndef RenderHandler_h
#define RenderHandler_h

#include <vector>
#include <variant>
#include <map>
#include <queue>
#include <functional>

#include "utils/Spinlock.h"
#include "Algebra.h"
using namespace algebra;

namespace chloride {

	/**
	 * @brief a texture represent a part of memory with the texture data
	 */
	struct Texture {
		unsigned char* data = nullptr;
		//enum TextureType {
		//	RGB,
		//	RGBA
		//} type;
		size_t depth, width, height;
		Texture(size_t size, size_t width, size_t height, size_t depth) 
		: depth(depth) ,width(width), height(height) {
			// so dangerous...
			data = new unsigned char[size];
		}
		~Texture() {
			delete[]data;
		}

		unsigned char* getData() const { return data; }
	};

	struct Vertex {
		vec3 coordinate;
		vec3 normal;
		vec2 texture;
		Vertex(const vec3& coord, const vec3& normal, const vec2& texture) :
			coordinate(coord), normal(normal), texture(texture) {
		}
		Vertex(const Vertex& v) : coordinate(v.coordinate), normal(v.normal), texture(v.texture) {};
		void operator=(const Vertex& v) {
			coordinate = v.coordinate; normal = v.normal; texture = v.texture;
		}
	};

	struct Face {
		// index of vertex in this face
		uint32_t p1, p2, p3;
		Face(uint32_t p1, uint32_t p2, uint32_t p3) : p1(p1), p2(p2), p3(p3) {};
		Face(const Face& f) : p1(f.p1), p2(f.p2), p3(f.p3) {}
		void operator=(const Face& f) {
			p1 = f.p1; p2 = f.p2; p3 = f.p3;
		}
	};

	class RenderHandler;
	class RenderInstance;
	class RenderIntanceRef;

	using uniform = std::variant< uint32_t, float , vec2, vec3, vec4, mat4 > ;

	/**
	 * @brief a render instance is an object referencing to a handler, requiring a
	 * slot from the uniform buffer. When an instance deconstruct, it release the
	 * occupied slot in the render handler
	 */
	class RenderInstanceRef {
	protected:

		RenderHandler& parent;
		size_t slot;

	public:
		~RenderInstanceRef();
		RenderInstanceRef(RenderHandler& parentHandler, size_t slot);

		RenderInstanceRef& setUniform(const std::string& name, std::unique_ptr<uniform> var);
		const std::unique_ptr<RenderInstance>& getInstance();
	};

	/**
	 * @brief a render instance store the uniform variables from @class RenderInstanceRef
	 */
	class RenderInstance {
	protected:
		
		std::map<std::string, std::unique_ptr<uniform>> uniformMap;

	public:
		virtual ~RenderInstance() = default;
		/**
		 * @brief set the uniform variable
		 * @param name variable name
		 * @param var value
		 * @return this reference
		 */
		RenderInstance& setUniform(const std::string& name, std::unique_ptr<uniform> var);

		/**
		 * @brief iterate throw this instance to retrieve all uniforms
		 * @param foreach callback
		 */
		void forEach(std::function<void (const std::string& key, const std::unique_ptr<uniform>& uniform)> callback);
	};

	/**
	 * @brief abstract class for rendering, holds multiple render instance
	 * and uniform buffers. A handler implementation must bind a shader program
	 * in order to function
	 */
	class RenderHandler {
		friend RenderInstance;
		friend RenderInstanceRef;

	protected:
		std::atomic_flag flag;

		std::atomic_size_t generator;
		std::map<size_t, std::unique_ptr<RenderInstance>> instanceMap;
		const size_t INSTANCE_PER_DRAW;

		/**
		 * @brief get the instance in the given slot
		 * @param slot slot
		 * @return instance pointer
		 */
		virtual const std::unique_ptr<RenderInstance>& getInstanceBySlot(size_t slot);

	public:
		virtual ~RenderHandler() = default;
		/**
		 * @brief construct handler
		 * @param instancePerCall how many uniform slot are there in shader
		 */
		RenderHandler(size_t instancePerCall);

		virtual std::unique_ptr<RenderInstanceRef> allocateInstance();
		virtual void freeInstance(size_t slot);

		/**
		 * @brief actual function to draw every instance
		 * @param perspective the perspective matrix
		 * @param camera the camera transformation matrix
		 */
		virtual void execute(const mat4& perspective, const mat4& cameraPos, const mat4& cameraRotation) = 0;

		/**
		 * @brief call once when added to RenderManager
		 */
		virtual void initialize() {};
	};



};

#endif
