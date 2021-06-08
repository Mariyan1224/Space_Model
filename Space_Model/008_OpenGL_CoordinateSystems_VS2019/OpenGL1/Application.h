#pragma once

#include <memory>
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Application
{
public:
	void Run();
	
private:
	void Init();
	void Update(float dt);
	void Render();

	void ProcessInput(GLFWwindow* window, float dt);

	friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


private:
	std::unique_ptr<VertexArray> _earthVa;
	std::unique_ptr<VertexArray> _spaceVa;
	std::unique_ptr<VertexArray> _sunVa;


	std::unique_ptr<Shader> _earthShader;
	std::unique_ptr<Shader> _spaceShader;
	std::unique_ptr<Shader> _sunShader;

	
	std::unique_ptr<Texture> _earthTexture_diffuse;
	std::unique_ptr<Texture> _earthTexture_specular;
	std::unique_ptr<Texture> _earthTexture_clouds;
	std::unique_ptr<Texture> _spaceTexture;
	std::unique_ptr<Texture> _sunTexture;
	
	glm::vec3 _lightPos = glm::vec3(7.0f, 0.0f, -4.0f); 
	glm::vec3 _lightColor = glm::vec3(1.f, 1.f, 1.f);


	glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 _cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float _yaw = -90.0f;
	float _pitch = 0.0f;

	float _fov = 45.f;
	
	
};
