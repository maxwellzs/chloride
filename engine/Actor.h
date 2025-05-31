#ifndef Actor_h
#define Actor_h

#include "Algebra.h"
#include "Object.h"
#include "components/Mesh.h"

using namespace algebra;

namespace chloride {

	class Engine;

	/**
	 * @brief An actor is a object that can be add to a Stage
	 * an actor can reference multiple components, such as texture, meshes
	 */
	class Actor : public Object {
	protected:
		std::string name;
		
		// 3D right hand coordinate
		vec3 location{0,0,0};
		// pitch yaw roll
		vec3 rotation{0,0,0};

		std::unique_ptr<Mesh> actorMesh;
	
		/**
		 * @brief submit the location of this actor and its mesh, so that
		 * the uniform variable will be updated and make render correct
		 */
		void updateGeometry();

	public:
		virtual ~Actor() = default;
		Actor() : name("anonymous actor") {}
		Actor(const std::string& name) : name(name) {}

		const std::string& getName() {
			return name;
		}

		vec3& getLocation() {
			return location;
		}

		vec3& getRotation() {
			return rotation;
		}

		/**
		 * @brief game logic function, override this to implement
		 * functions, update 
		 * @param ctx current engine 
		 * @param delta how many game ticks before last game tick
		 */
		virtual void update(Engine& ctx, clock_t delta);

		/**
		 * @brief attach mesh to this actor
		 * @param mesh 
		 * @return this reference
		 */
		Actor& bindMesh(std::unique_ptr<Mesh> mesh);

	};

};

#endif
