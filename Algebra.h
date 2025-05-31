#ifndef Algebra_h
#define Algebra_h

#include <cstring>
#include <iostream>

namespace algebra {

	struct vec2 {
	public:
		float x, y;

		vec2(float x, float y) : x(x), y(y) {}
		vec2(const vec2& v) : x(v.x), y(v.y) {}
		void operator=(const vec2& v) { x = v.x; y = v.y; }
		vec2 operator-(const vec2& v) const { return  { x - v.x, y - v.y }; }
		vec2 operator+(const vec2& v) const { return  { x + v.x, y + v.y }; }
		float dot(const vec2& v) const {
			return x * v.x + y * v.y;
		}

		float length() const {
			return dot(*this);
		}

		friend std::ostream& operator<<(std::ostream& out, const vec2& v) {
			out << "[" << v.x << "," << v.y << "]" << std::endl;
			return out;
		}
	};

	struct vec3 {
	public:
		float x, y, z;

		vec3(float x, float y, float z) : x(x), y(y), z(z) {}
		vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}
		void operator=(const vec3& v) { x = v.x; y = v.y; z = v.z; }
		vec3 operator-(const vec3& v) const { return  { x - v.x, y - v.y, z - v.z }; }
		vec3 operator+(const vec3& v) const { return  { x + v.x, y + v.y, z + v.z }; }
		vec3 cross(const vec3& v) const {
			return vec3(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
			);
		}
		float dot(const vec3& v) const {
			return x * v.x + y * v.y + z * v.z;
		}

		float length() const {
			return dot(*this);
		}

		friend std::ostream& operator<<(std::ostream& out, const vec3& v) {
			out << "[" << v.x << "," << v.y << "," << v.z << "]" << std::endl;
			return out;
		}
	};

	struct vec4 {
	public:
		float x, y, z, w;

		vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		vec4(const vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		vec4(const vec3& v) : x(v.x), y(v.y), z(v.z), w(1) {}
		void operator=(const vec4& v) { x = v.x; y = v.y; z = v.z; w = v.w; }
		vec4 operator-(const vec4& v) { return  { x - v.x, y - v.y, z - v.z, w - v.w }; }
		vec4 operator+(const vec4& v) { return  { x + v.x, y + v.y, z + v.z, w + v.w }; }

		friend std::ostream& operator<<(std::ostream& out, const vec4& v) {
			out << "[" << v.x << " " << v.y << " " << v.z << " " << v.w << "]" << std::endl;
			return out;
		}
	};

	struct mat4 {
	public:
		float data[16];

		mat4() { 
			memset(data, 0x0, sizeof(float) * 16);
		}
		mat4(const vec4 r1, const vec4 r2, const vec4 r3, const vec4 r4) {
			memcpy(data, &r1, sizeof(r1));
			memcpy(data + 4, &r2,  sizeof(vec4));
			memcpy(data + 8, &r3,  sizeof(vec4));
			memcpy(data + 12, &r4,  sizeof(vec4));
		}
		mat4(const mat4& m) {
			memcpy(data, m.data, sizeof(float) * 16);
		}
		void operator=(const mat4& m) {
			memcpy(data, m.data, sizeof(float) * 16);
		}
		inline mat4 operator*(const mat4& m) {
#define MUL(r, c) (data[r*4]*m.data[c] + data[r*4+1]*m.data[4+c] + data[r*4+2]*m.data[8+c] + data[r*4+3]*m.data[12+c])
#define ROW(r) vec4(MUL(r, 0), MUL(r, 1), MUL(r, 2), MUL(r, 3))
			return mat4(
				ROW(0), ROW(1), ROW(2), ROW(3)
			);
#undef ROW
#undef MUL
		}

		static mat4 move(const vec3& v) {
			return mat4(
				vec4(1, 0, 0, v.x),
				vec4(0, 1, 0, v.y),
				vec4(0, 0, 1, v.z),
				vec4(0, 0, 0, 1)
			);
		}

		static mat4 rotate(const vec3& v) {
			float cx = cos(v.x);
			float sx = sin(v.x);
			float cy = cos(v.y);
			float sy = sin(v.y);
			float cz = cos(v.z);
			float sz = sin(v.z);
			return mat4( // row
				vec4(1, 0, 0, 0),
				vec4(0, cx, sx, 0),
				vec4(0, -sx, cx, 0),
				vec4(0, 0, 0, 1)
			) * mat4( // pitch
				vec4(cy, 0,  sy, 0),
				vec4(0,  1,  0,  0),
				vec4(-sy,0,  cy, 0),
				vec4(0,  0,  0,  1)
			) * mat4( // yaw
				vec4(cz, sz, 0,  0),
				vec4(-sz,cz, 0,  0),
				vec4(0,  0,  1,  0),
				vec4(0,  0,  0,  1)
			);
		}

		static mat4 eyes() {
			return mat4(
				vec4(1,0,0,0),
				vec4(0,1,0,0),
				vec4(0,0,1,0),
				vec4(0,0,0,1)
			);
		}

	/*	static mat4 perspective(float near, float far, float fov, float ratio) {
			
		}*/


		friend std::ostream& operator<<(std::ostream& out, const mat4& v) {
			for (size_t i = 0; i < 4; i++)
			{
				out << "[" << v.data[i*4] << " " << v.data[i * 4 + 1] << " " << v.data[i * 4 + 2] << " " << v.data[i * 4 + 3] << "]" << std::endl;
			}
			return out;
		}
	};

	

};

#endif