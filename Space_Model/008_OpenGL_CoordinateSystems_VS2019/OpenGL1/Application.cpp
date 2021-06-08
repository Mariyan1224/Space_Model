#include "Application.h"


#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>


#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../../icosphere/src/Icosphere.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::ProcessInput(GLFWwindow* window, float dt)
{
    const float cameraSpeed = 2.5f * dt; // adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        _cameraPos += cameraSpeed * _cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        _cameraPos -= cameraSpeed * _cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        _cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        _cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto* app = (Application*)glfwGetWindowUserPointer(window);

    app->_fov -= (float)yoffset;

    if (app->_fov < 1.0f)
        app->_fov = 1.0f;

    if (app->_fov > 45.0f)
        app->_fov = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static double lastX;
    static double lastY;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if (state != GLFW_PRESS)
        return;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    auto* app = (Application*)glfwGetWindowUserPointer(window);

    app->_yaw -= xoffset;
    app->_pitch -= yoffset;

    if (app->_pitch > 89.0f)
        app->_pitch = 89.0f;

    if (app->_pitch < -89.0f)
        app->_pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(app->_yaw)) * cos(glm::radians(app->_pitch));
    direction.y = sin(glm::radians(app->_pitch));
    direction.z = sin(glm::radians(app->_yaw)) * cos(glm::radians(app->_pitch));

    app->_cameraFront = glm::normalize(direction);
}
void Application::Run()
{
    GLFWwindow* window;

    /* Initialize the glfw */
    if (!glfwInit())
        return;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Space_View", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, 640, 480);

    glfwSetWindowUserPointer(window, this);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Init();

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

	while (!glfwWindowShouldClose(window))
	{
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window, deltaTime);
        Update(deltaTime);
        Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
	}

    glfwTerminate();
}

Icosphere earth_sphere(0.2f, 5, true);
Icosphere solar_sphere(0.5f, 5, true); // Sun diameter is 109 times bigger than Earth`s => 21.8f

void Application::Init()
{  
    // Earth data
   std::unique_ptr<VertexBuffer> vbo(new VertexBuffer(earth_sphere.getInterleavedVertices(), earth_sphere.getInterleavedVertexSize()));

    vbo->Bind();

    vbo->SetLayout(
        {
            {"aEarthPos", Float3},
            {"aEarthNormalCoords", Float3},
        	{"aEarthTexCoords", Float2}
        }
    );
    
    auto ibo = std::make_unique<IndexBuffer>(earth_sphere.getIndices(), earth_sphere.getIndexSize());

    _earthVa = std::make_unique<VertexArray>();

    _earthVa->SetVertexBuffer(std::move(vbo));
    _earthVa->SetIndexBuffer(std::move(ibo));
    _earthVa->Bind();

    _earthShader = std::make_unique<Shader>("res\\vertex.glsl", "res\\fragment.glsl");
    
    
    _earthTexture_diffuse = std::make_unique<Texture>("res\\pics\\2k_earth_nightmap.jpg"); //earth2048.bmp  icosa_earth.bmp dymaxionMap.png 2k_earth_nightmap.jpg    8k_earth_daymap.jpg
    _earthTexture_specular = std::make_unique<Texture>("res\\pics\\8k_earth_daymap.jpg"); //
    _earthTexture_clouds = std::make_unique<Texture>("res\\pics\\2k_earth_clouds.jpg"); //2k_earth_clouds.jpg


    // Skybox data

    
    
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    _spaceShader = std::make_unique<Shader>("res\\skyBoxVertex.glsl", "res\\skyBoxFragment.glsl");

    std::unique_ptr<VertexBuffer> spaceVbo(new VertexBuffer(&skyboxVertices, sizeof(skyboxVertices)));

    spaceVbo->Bind();
    spaceVbo->SetLayout(
        {
           {"aSpaceCoord", Float3}
        }
    );

    _spaceVa = std::make_unique<VertexArray>();
    _spaceVa->SetVertexBuffer(std::move(spaceVbo));
    _spaceVa->Bind();

    std::vector<std::string> faces
    {
        "res\\pics\\R_skybox.jpg",
        "res\\pics\\L_skybox.jpg",
        "res\\pics\\T_skybox.jpg",
        "res\\pics\\B_skybox.jpg",
        "res\\pics\\F_skybox.jpg",
        "res\\pics\\Ba_skybox.jpg"

        //"res\\right_skybox.jpg",
        //"res\\left_skybox.jpg",
        //"res\\top_skybox.jpg",
        //"res\\bottom_skybox.jpg",
        //"res\\front_skybox.jpg",
        //"res\\back_skybox.jpg"

        //"res\\right.jpg",
        //"res\\left.jpg",
        //"res\\top.jpg",
        //"res\\bottom.jpg",
        //"res\\front.jpg",
        //"res\\back.jpg"
    };
    _spaceTexture = std::make_unique<Texture>(faces);

    //Sun data

    std::unique_ptr<VertexBuffer> sun_vbo(new VertexBuffer(solar_sphere.getInterleavedVertices(), solar_sphere.getInterleavedVertexSize()));

    sun_vbo->Bind();

    sun_vbo->SetLayout(
        {
            {"aSunPos", Float3},
            {"aSunNormalCoords", Float3},
            {"aSunTexCoords", Float2}
        }
    );
    auto sun_ibo = std::make_unique<IndexBuffer>(solar_sphere.getIndices(), solar_sphere.getIndexSize());

    _sunVa = std::make_unique<VertexArray>();

    _sunVa->SetVertexBuffer(std::move(sun_vbo));
    _sunVa->SetIndexBuffer(std::move(sun_ibo));
    _sunVa->Bind();

    _sunShader = std::make_unique<Shader>("res\\sun_vertex.glsl", "res\\sun_fragment.glsl");

    _sunTexture = std::make_unique<Texture>("res\\pics\\8k_sun.jpg"); //8k_sun.jpg

    glEnable(GL_DEPTH_TEST);
}

void Application::Update(float dt)
{
    auto view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);

    glm::mat4 projection;
    // projection = glm::ortho(0.0f, 640.f, 0.0f, 480.0f, 0.1f, 100.0f);
    projection = glm::perspective(glm::radians(_fov), 800.0f / 600.0f, 0.1f, 100.0f);

    _earthShader->use();
    _earthShader->setFloatMat4("view", view);
    _earthShader->setFloatMat4("projection", projection);
    
    _sunShader->use();
    _sunShader->setFloatMat4("view", view);
    _sunShader->setFloatMat4("projection", projection);

    _spaceShader->use();
     view = glm::mat4(glm::mat3(glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp)));
    _spaceShader->setFloatMat4("view", view);
    _spaceShader->setFloatMat4("projection", projection);



}

void Application::Render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw space
    // glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

    glDepthMask(false);
    
    _spaceVa->Bind();

    _spaceShader->use();
    _spaceShader->setInt("spaceTex", 0);

    _spaceTexture->SpaceBind(0);

    glm::mat4 spaceModel = glm::mat4(1.0f);
    float spaceAngle = 60.0f + glfwGetTime() * 0.1;
    spaceModel = glm::rotate(spaceModel, glm::radians(spaceAngle), glm::vec3(12.0f, -3.0f, 20.0f));
    _spaceShader->setFloatMat4("model", spaceModel);

    glDrawArrays(GL_TRIANGLES, 0, 36);
   
    // glDepthFunc(GL_LESS); // set depth function back to default
    glDepthMask(true);

    //draw objects

    // Sun draw

    _sunTexture->Bind(0);
    _sunVa->Bind();
    _sunShader->use(); 
    _sunShader->setInt("sunTexture", 0);
      

    glm::mat4 sun_model = glm::mat4(1.0f);
    sun_model = glm::translate(sun_model, _lightPos);
    sun_model = glm::rotate(sun_model, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
    
   _sunShader->setFloatMat4("model", sun_model);

    glDrawElements(GL_TRIANGLES, solar_sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

    //  Earth draw
    
    _earthTexture_diffuse->Bind(0);
    _earthTexture_specular->Bind(1);
    _earthTexture_clouds->Bind(2);

    _earthShader->use();
    _earthShader->setInt("material.diffuse", 0);
    _earthShader->setInt("material.specular", 1);
    _earthShader->setInt("material.clouds", 2);
    _earthShader->setFloat("material.shininess", 0.5f);
    _earthShader->setFloat3("viewPos", _cameraPos);
    _earthShader->setFloat3("light.position", _lightPos);
    _earthShader->setFloat3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
   _earthShader->setFloat3("light.nightTexture", glm::vec3(0.0f, 0.0f, 0.0f));


    _earthVa->Bind();

   

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f)); // distance from Sun

    float angle =glfwGetTime() * 50.f; // orbit speed
    //model = glm::rotate(model, glm::radians(80.f), glm::vec3(-42.0f, 75.0f, 45.0f)); //-82.0f, 35.0f, 150.0f  -72.0f, 60.0f, 80.0f
  //  model = glm::rotate(model, glm::radians(angle), glm::vec3(12.0f, 3.0f, 20.0f)); //12.0f, 3.0f, 20.0f
    model = glm::rotate(model, glm::radians( 160.f), glm::vec3(-1.5f, 0.0f, 0.0f)); 
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 orbit_model = glm::mat4(1.0f);
    orbit_model = glm::rotate(sun_model, glm::radians(float(glfwGetTime() * 10.f)), glm::vec3(0.0f, 0.0f, 1.0f));

    _earthShader->setFloatMat4("model", model);
    _earthShader->setFloatMat4("orbit_model", orbit_model);


    glDrawElements(GL_TRIANGLES, earth_sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
}






