#include "ShaderFactory.h"

void chloride::ShaderFactory::checkShaderState(GLuint shader)
{
	int  success;
	char infoLog[512];
	memset(infoLog, 0x0, sizeof(infoLog));
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		throw std::runtime_error(std::string(infoLog));
	}
}

void chloride::ShaderFactory::compileShader(GLuint shader, const std::string& source)
{
	glCompileShader(shader);
	try
	{
		checkShaderState(shader);
	}
	catch (const std::exception& e)
	{
		LOG(WARNING) << "error compiling vertex shader \"" << source << " : ";
		LOG(WARNING) << e.what();
		throw e;
	}
}

void chloride::ShaderFactory::linkProgram(GLuint program, const std::string& vertexSource, const std::string& fragmentSource)
{
	int  success;
	char infoLog[512];
	memset(infoLog, 0x0, sizeof(infoLog));
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		LOG(WARNING) << "error linking program {vertex shader=" << vertexSource << ", fragment shader=" << fragmentSource << "} : ";
		LOG(WARNING) << infoLog;
		throw std::runtime_error("link error");
	}
}

chloride::ShaderFactory& chloride::ShaderFactory::get()
{
	// TODO: insert return statement here
	static ShaderFactory instance;
	return instance;
}

GLuint chloride::ShaderFactory::loadShader(const std::string& vs, const std::string& fs)
{
	

	std::ifstream vsIn(vs, std::ios::beg), fsIn(fs, std::ios::beg);
	if (!vsIn.is_open() || !fsIn.is_open()) {
		// no shader file present or cannot read
		if (vsIn.is_open()) vsIn.close();
		if (fsIn.is_open()) fsIn.close();
		throw std::runtime_error("cannot open shader file : vertex shader=" + vs + " fragment shader=" + fs);
	}
	// read from file 
	std::ostringstream vsData, fsData;
	vsData << vsIn.rdbuf();
	fsData << fsIn.rdbuf();
	std::string vsSource = vsData.str();
	std::string fsSource = fsData.str();
	const char* vsSourcePtr = vsSource.c_str();
	const char* fsSourcePtr = fsSource.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// compile shader source
	glShaderSource(vertexShader, 1, &vsSourcePtr, nullptr);
	glShaderSource(fragmentShader, 1, &fsSourcePtr, nullptr);

	compileShader(vertexShader, vs);
	compileShader(fragmentShader, fs);

	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	linkProgram(program, vs, fs);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}
