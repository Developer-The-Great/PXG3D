#pragma once
#include <string>
#include <glad/glad.h>


namespace PXG
{
	struct Vector3;
	struct Mat4;


	class Shader
	{
	public:
		Shader(const char* vertexPath, const char* fragmentPath,std::string name = "shader");
		Shader(const char* vertexSource, const char* fragmentSource, std::string name, bool);

		~Shader();

		unsigned int GetShaderProgram() const;

		void Use() const;

		void Release();

		void SetFloat(const std::string &name, float value) const;
		void SetFloat(const GLint location, float value) const;

		void SetInt( const std::string &name, int value) const;
		void SetInt(const GLint location, int value) const;

		void SetVec3( const std::string & name,Vector3 vec) const;
		void SetVec3(const GLint location, Vector3 vec) const;

		//void SetVec4(const std::string &name, float v0, float v1, float v2, float v3);

		void SetMat4( const std::string &name, Mat4 matrixValue) const;

		unsigned int shaderID;

		std::string shaderName = "";
	private:


		static constexpr int MAX_LIGHTS = 10;
	};
}

