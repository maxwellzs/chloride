#ifndef Mesh_h
#define Mesh_h

#include "graphic/RenderHandler.h"
#include "Algebra.h"
#include "../Object.h"

using namespace algebra;

namespace chloride {

	class MeshFactory;

	/**
	 * @brief a mesh is a component belongs to an Actor
	 * when a mesh is loaded, the corresponding handler is created
	 */
	class Mesh : public Object {


	protected:
		std::unique_ptr<RenderInstanceRef> ref;

	public:
		// called from factory function
		Mesh(std::unique_ptr<RenderInstanceRef> ref);

		void updateGeometry(const mat4& actorRotation, const mat4& actorTranslation);
	};

};

#endif
