#ifndef Camera_h
#define Camera_h

#include "Object.h"
#include "Algebra.h"

using namespace algebra;

namespace chloride {

	/**
	 * @brief a camera hold user status of where camera is, camera rotation around
	 * x axis and y axis
	 * 
	 */
	class Camera : public Object {
	private:
		vec3 cameraPos = vec3(0,0,0);
		// rotation around x, y and z
		vec3 cameraRotation = vec3(0, 0, 0);

	public:
		/**
		 * @brief move the camera by delta value
		 * @param delta value
		 */
		void move(const vec3& delta);
		/**
		 * @brief move the camera to new location
		 * @param locaiton 
		 */
		void moveTo(const vec3& location);
		/**
		 * @brief look at a specific angle
		 * @param pos rotation around x, y and z axis, the rotation sequence is
		 * z -> y -> x
		 */
		void lookAt(const vec3& pos);
		/**
		 * @brief generate current camera translation matrix
		 * @return 
		 */
		mat4 getCameraTranslate();
		/**
		 * @brief generate current camera rotation matrix
		 * @return 
		 */
		mat4 getCameraRotation();
	};

};

#endif
