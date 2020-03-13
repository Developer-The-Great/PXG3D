#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Vector3.h"
#include "Debug.h"
#include "Mat4.h"

//#include <gl/GLU.h>

namespace PXG
{
	Shader::Shader(const char * vertexPath, const char * fragmentPath, std::string name)
	{
		shaderName = name;
		//initialize ifstream
		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vertexFile, fragmentFile;

		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			//open file
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);

			std::stringstream vertexStream, fragmentStream;

			//read file contents and stream them
			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			//close file
			vertexFile.close();
			fragmentFile.close();

			//convert stream into string
			vertexCode = vertexStream.str();
			fragmentCode = fragmentStream.str();
		}
		catch (std::ifstream::failure e)
		{
			Debug::Log(Verbosity::Error, "ERROR::SHADER_FILE::FILE_NOT_SUCESSFULLY_READ");
		}

		//std::cout << "vertex code :" << vertexCode << std::endl;

		const char* vertexShaderSource = vertexCode.c_str();
		const char* fragmentShaderSource = fragmentCode.c_str();

		//generate vertex shader
		unsigned int vertexShader, fragmentShader;
		int success;
		char infoLog[512];

		vertexShader = glCreateShader(GL_VERTEX_SHADER);


		//compile shader source
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			Debug::Log("ERROR::VERTEX_SHADER[{0}]::SHADER_COMPILATION_FAILED::{1}", vertexPath, infoLog);

		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			Debug::Log("ERROR::VERTEX_SHADER[{0}]::SHADER_COMPILATION_FAILED::{1}", fragmentPath, infoLog);

		}

		shaderID = glCreateProgram();

		glAttachShader(shaderID, vertexShader);
		glAttachShader(shaderID, fragmentShader);

		glLinkProgram(shaderID);

		glGetProgramiv(shaderID, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER_PROGRAM::LINKING_FAILED \n";
			std::cerr << infoLog << "\n";
		}

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		//store contents of files


		Debug::Log("Shader program for {0} and {1} has finished", vertexPath, fragmentPath);

	}

	Shader::Shader(const char* vertexSource, const char* fragmentSource, std::string name, bool)
	{

		const char* vertexShaderSource = vertexSource;
		const char* fragmentShaderSource = fragmentSource;
		//generate vertex shader
		unsigned int vertexShader, fragmentShader;
		int success;
		char infoLog[512];

		vertexShader = glCreateShader(GL_VERTEX_SHADER);


		//compile shader source
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			Debug::Log("ERROR::VERTEX_SHADER[{0}]::SHADER_COMPILATION_FAILED::{1}", vertexSource, infoLog);

		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			Debug::Log("ERROR::VERTEX_SHADER[{0}]::SHADER_COMPILATION_FAILED::{1}", fragmentSource, infoLog);

		}

		shaderID = glCreateProgram();

		glAttachShader(shaderID, vertexShader);
		glAttachShader(shaderID, fragmentShader);

		glLinkProgram(shaderID);

		glGetProgramiv(shaderID, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER_PROGRAM::LINKING_FAILED \n";
			std::cerr << infoLog << "\n";
		}

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		//store contents of files


		Debug::Log("Shader program for {0} and {1} has finished", vertexSource, fragmentSource);
	}


	Shader::~Shader()
	{
	}

	unsigned int Shader::GetShaderProgram() const
	{
		return shaderID;
	}

	void Shader::Use() const
	{
		glUseProgram(shaderID);
	}

	void Shader::Release()
	{
		glUseProgram(0);
	}

	void Shader::SetFloat( const std::string & name, float value) const
	{
		glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
	}

	void Shader::SetFloat(const GLint location, float value) const
	{
		glUniform1f(location, value);
	}

	void Shader::SetInt(const std::string & name, int value) const
	{
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);

	}

	void Shader::SetInt(const GLint location,int value) const
	{
		glUniform1i(location, value);
	}

	void Shader::SetVec3(const std::string & name, Vector3 vec) const
	{
		glUniform3f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y, vec.z);

	}

	void Shader::SetVec3(const GLint location, Vector3 vec) const
	{
		glUniform3f(location, vec.x,vec.y,vec.z);
	}

	void Shader::SetMat4(const std::string & name, Mat4 matrixValue) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1,
			GL_FALSE, glm::value_ptr(matrixValue.ToGLM()));
	}
}
