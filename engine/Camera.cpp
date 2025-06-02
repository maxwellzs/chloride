#include "Camera.h"

void chloride::Camera::move(const vec3& delta)
{
	cameraPos += delta;
}

void chloride::Camera::moveTo(const vec3& location)
{
	cameraPos = location;
}

void chloride::Camera::lookAt(const vec3& pos)
{
	cameraRotation = pos;
}

mat4 chloride::Camera::getCameraTranslate()
{
	return mat4(
		vec4(1, 0, 0, -cameraPos.x),
		vec4(0, 1, 0, -cameraPos.y),
		vec4(0, 0, 1, -cameraPos.z),
		vec4(0, 0, 0, 1)
	);
}

mat4 chloride::Camera::getCameraRotation()
{
	return mat4::rotate(vec3(-cameraRotation.x, -cameraRotation.y, -cameraRotation.z));
}
