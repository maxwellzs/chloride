#ifndef ShaderFactory_h
#define ShaderFactory_h

#include <string>
#include <sstream>
#include <fstream>
#include <glad/glad.h>
#include <ng-log/logging.h>

namespace chloride {

	class ShaderFactory {

	private:
		ShaderFactory() = default;
		void checkShaderState(GLuint shader);
		void compileShader(GLuint shader, const std::string& source);
		void linkProgram(GLuint program, const std::string& vertexSource, const std::string& fragmentSource);
	public:
		static ShaderFactory& get();
		/**
		 * @brief load a shader program
		 * @param vs vertex shader
		 * @param fs fragment shader
		 * @return program
		 */
		GLuint loadShader(const std::string& vs, const std::string& fs);

	};

};

#endif