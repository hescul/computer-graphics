#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <cmath>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double offsetX, double offsetY);
void processInput(GLFWwindow* window);
void debugInfo();

// settings
constexpr auto SCR_WIDTH = 800;
constexpr auto SCR_HEIGHT = 600;

// camera
auto camera = Camera();

auto lastX = SCR_WIDTH  / 2.0f;
auto lastY = SCR_HEIGHT / 2.0f;

auto firstMouse = true;

// delta time
auto deltaTime = 0.0f;  // Time between current frame and last frame
auto lastFrame = 0.0f;  // Time of last frame

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    // Tell GLFW that it should hide the cursor and capture it. Capturing a cursor means that,
    // once the application has focus, the mouse cursor stays within the center of the window.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

#ifndef NDEBUG
    debugInfo();
#endif


    // build and compile our shader program
    // ------------------------------------
    const auto shader = Shader("shaders/shader.vert", "shaders/shader.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    constexpr float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    constexpr unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    // Texture coordinates range from 0 to 1 in the x and y axis (for 2D texture images).
    // Retrieving the texture color using texture coordinates is called sampling.
    // Texture sampling has a loose interpretation and can be done in many different ways.
    // It is thus our job to tell OpenGL how it should sample its textures.

    const glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 
        reinterpret_cast<void*>(0)
    );
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 
        reinterpret_cast<void*>(3 * sizeof(float)) // NOLINT(performance-no-int-to-ptr)
    );
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // texture
    // -------
    unsigned int texture1;
    glGenTextures(1, &texture1);
    // Just like other objects we need to bind it so any subsequent texture commands will configure the currently bound texture.
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    // OpenGL expects the 0.0 coordinate on the y - axis to be on the bottom side of the image,
    // but images usually have 0.0 at the top of the y - axis
    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_load("textures/container.jpg", &width, &height, &channels, 0);

    if (data) {
        // Start generating a texture using the previously loaded image data.
        // 1st param: this operation will generate a texture on the currently bound texture object at the same target.
        // 2nd param: specifies the mipmap level for which we want to create a texture for.
        // 3rd param: what kind of format we want to store the texture.
        // 4th, 5th param: the width and height of the resulting texture.
        // 6th param: should always be 0 (some legacy stuff).
        // 7th, 8th param: the format and data type of the source image
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // This will automatically generate all the required mipmaps for the currently bound texture.
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << '\n';
    }

    // It is good practice to free the image memory.
    stbi_image_free(data);

    // the second texture
    unsigned int texture2;
    glGenTextures(1, &texture2);
    // Just like other objects we need to bind it so any subsequent texture commands will configure the currently bound texture.
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // ReSharper disable once StringLiteralTypo
    data = stbi_load("textures/awesomeface.png", &width, &height, &channels, 0);

    if (data) {
        // We now load a .png image that includes an alpha (transparency) channel.
        // We need to specify that the image data contains an alpha channel as well by using GL_RGBA.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        // This will automatically generate all the required mipmaps for the currently bound texture.
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << '\n';
    }
    stbi_image_free(data);

    // Tell OpenGL to which texture unit each shader sampler belongs to.
    shader.use(); // don't forget to activate the shader before setting uniforms!  
    // glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // set it manually
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1); // or with shader class

    // transformation
    // --------------
    // model matrix
    // auto model = glm::mat4(1.0f);
    // model = rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // view matrix
    // auto view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    // view = translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // projection matrix
    // constexpr auto ratio = static_cast<float>(SCR_WIDTH) / SCR_HEIGHT;
    // const auto projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // update delta time
        // -----------------
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Now we update the uniform color. Uniforms are a useful tool for setting attributes that
        // may change every frame, or for interchanging data between application and shaders.
        // const auto currentTime = glfwGetTime();
        // const auto greenValue = std::sin(currentTime) / 2.0 + 0.5;
        // shader.setFloat("ourColor", static_cast<float>(greenValue));

        // Bind the texture before calling glDrawElements and it will then automatically assign the
        // texture to the fragment shader's sampler.
        // glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // rotate the cube over time
        // const auto model = rotate(
        //     glm::mat4(1.0f), static_cast<float>(glfwGetTime()) * glm::radians(50.0f), 
        //     glm::vec3(0.5f, 1.0f, 0.0f)
        // );

        // rotate the scene over time
        // constexpr auto radius = 10.0f;
        // const auto camX = static_cast<float>(std::sin(glfwGetTime())) * radius;
        // const auto camZ = static_cast<float>(std::cos(glfwGetTime())) * radius;

        // requires a position, target and up vector respectively
        const auto view = camera.viewMatrix();

        constexpr auto ratio = static_cast<float>(SCR_WIDTH) / SCR_HEIGHT;
        const auto projection = glm::perspective(glm::radians(camera.zoom), ratio, 0.1f, 100.0f);

        // Be sure to activate the shader.
        shader.use();

        // We send transformation matrices to the shader. This is usually done each frame since
        // transformation matrices tend to change a lot.
        // shader.setTransform("model", value_ptr(model));
        shader.setTransform("view", value_ptr(view));
        shader.setTransform("projection", value_ptr(projection));

        // As we only have a single VAO there's no need to bind it every time,
        // but we'll do so to keep things a bit more organized.
        glBindVertexArray(vao);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0); // no need to unbind it every time
        for (unsigned int i = 0; i < 10; i++) {
            auto model = glm::mat4(1.0f);
            model = translate(model, cubePositions[i]);
            const float angle = 20.0f * static_cast<float>(i + 1);
            model = rotate(
                model, static_cast<float>(glfwGetTime()) * glm::radians(angle), 
                glm::vec3(1.0f, 0.3f, 0.5f)
            );
            shader.setTransform("model", value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    shader.destroy();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    // const auto cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // move forward
        camera.processKeyboard(Camera::Movement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        // move backward
        camera.processKeyboard(Camera::Movement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // move left
        camera.processKeyboard(Camera::Movement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
	    // move right
        camera.processKeyboard(Camera::Movement::RIGHT, deltaTime);
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void mouseCallback(GLFWwindow* window, const double xPos, const double yPos) {
    if (firstMouse) { // initially set to true{
        lastX = static_cast<float>(xPos);
        lastY = static_cast<float>(yPos);
        firstMouse = false;
    }

    const auto offsetX = static_cast<float>(xPos) - lastX;
    // reversed since y-coordinates range from bottom to top
    const auto offsetY = static_cast<float>(yPos) - lastY;
    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);

    camera.processMouse(offsetX, offsetY);
}

void scrollCallback(GLFWwindow* window, const double offsetX, const double offsetY) {
    camera.processScroll(static_cast<float>(offsetY));
}

void debugInfo() {
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << '\n';

    int maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    std::cout << "Maximum nr of texture units supported: " << maxTextureUnits << '\n';
}