#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "learnOpengl/shader_m.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static uint8_t filterIndex = 0;
static bool anisotropicEnable = 0;
uint32_t filterParam[] = {
    GL_NEAREST_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST ,
    GL_NEAREST_MIPMAP_LINEAR ,
    GL_LINEAR_MIPMAP_LINEAR ,
};

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    char path_vs[]      = "D:\\lr\\code\\own\\CG\\resource\\glsls\\texture.vs";
    char path_fs[]      = "D:\\lr\\code\\own\\CG\\resource\\glsls\\texture.fs";
    char path_vs_coordAxis[] = "D:\\lr\\code\\own\\CG\\resource\\glsls\\coordAxis.vs";
    char path_fs_coordAxis[] = "D:\\lr\\code\\own\\CG\\resource\\glsls\\coordAxis.fs";
    
    char path_texture[] = "D:\\lr\\code\\resource\\fangge.jpg";

    // texture data
    float vertexs[] = {
        -40.0,  -1.0, -1.0, 0.0, 0.0,
        -40.0,   40.0, -80.0, 0.0, 1.0,
         40.0,  40.0, -80.0, 1.0, 1.0,
         40.0,  -1.0, -1.0, 1.0, 0.0,
    };

    uint8_t elements[] = {
        0, 2, 1,
        0, 3, 2
    };

    //vao vbo ebo
    uint32_t vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void * )(3*sizeof(float)) );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //
    uint32_t textrueId;
    glGenTextures(1, &textrueId);
    glBindTexture(GL_TEXTURE_2D, textrueId);


    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
     //GL_NEAREST_MIPMAP_NEAREST 
     //GL_LINEAR_MIPMAP_NEAREST 
     //GL_NEAREST_MIPMAP_LINEAR 
     //GL_LINEAR_MIPMAP_LINEAR 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    if (GLAD_GL_ARB_texture_filter_anisotropic && anisotropicEnable) {
        // 创建纹理
        glBindTexture(GL_TEXTURE_2D, textrueId);

        // 设置各向异性过滤级别
        GLfloat maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }


    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(path_texture, &width, &height, &nrChannels, 0);
    if (data)
    {
        if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // coord axis data
    float vertexs_coordAxis[] = {
        -0.5,  0.0, 0.0,
         0.5,  0.0, 0.0,

         0.0,  0.5, 0.0,
         0.0, -0.5, 0.0,

         0.0,  0.0, 0.5,
         0.0,  0.0, -0.5,
    };

    uint32_t vao_coordAxis, vbo_coordAxis;
    glGenVertexArrays(1, &vao_coordAxis);
    glBindVertexArray(vao_coordAxis);

    glGenBuffers(1, &vbo_coordAxis);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coordAxis);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs_coordAxis), vertexs_coordAxis, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Shader ourShader(path_vs, path_fs);
    Shader coordAxisShader(path_vs_coordAxis, path_fs_coordAxis);

    auto drawCoordinateAxis = [&]() {
        glBindVertexArray(vao_coordAxis);
        coordAxisShader.use();

        coordAxisShader.setVec4("customColor", 1.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINES, 0, 2);

        coordAxisShader.setVec4("customColor", 0.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINES, 2, 2);

        coordAxisShader.setVec4("customColor", 0.0f, 0.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINES, 4, 2);

        glBindVertexArray(0);
    };

    //任意点自转
    auto drawImage_RotateAtAnyPoint = [&](const std::vector<glm::vec3> & points) {

        static float angle = 45.0f;
        glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = glm::mat4(1.0f);
#if 01
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.3f, 0.3f, 0.0f));
        glm::mat4 baseModel = model;
        // baseModel = rotate * translate;  先将旋转中心点平移到 （0，0）点，然后旋转
        //之后再通过 translate * baseModel的方式，将图形移动
#else
        glm::mat4 rotate = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translate = glm::translate(model, glm::vec3(0.3f, 0.3f, 0.0f));
        glm::mat4 baseModel = rotate * translate;

        glm::mat4 translate0 = glm::translate(model, glm::vec3(0.3f, 0.6f, 0.0f));
        glm::mat4 translate1 = glm::translate(model, glm::vec3(0.3f, -0.6f, 0.0f));
#endif
        angle += 0.7;

        glBindVertexArray(vao);
        glBindTexture(GL_SAMPLER_2D, textrueId);

        ourShader.use();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        for (int i = 0; i < points.size(); i++) {
            glm::mat4 translate = glm::translate(glm::mat4(1.0f), points.at(i));
            ourShader.setMat4("model", translate * baseModel);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
        }

        glBindVertexArray(0);
        glBindTexture(GL_SAMPLER_2D, 0);
    };

    auto drawImage_RotateAroundAnyPoint = [&](const std::vector<glm::vec3>& points) {

        static float angle = 45.0f;
        glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = glm::mat4(1.0f);
#if 01
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.1f, -0.1f, 0.0f));
        glm::mat4 baseModel = model;
        // baseModel = translate * rotate * translate;  先平移离 0 点远一点，再旋转
        //之后再通过 translate * baseModel的方式，将图形移动
#else
#endif
        angle += 0.7;

        glBindVertexArray(vao);
        glBindTexture(GL_SAMPLER_2D, textrueId);

        ourShader.use();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        for (int i = 0; i < points.size(); i++) {
            glm::mat4 translate = glm::translate(glm::mat4(1.0f), points.at(i));
            ourShader.setMat4("model", translate * baseModel);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
        }

        glBindVertexArray(0);
        glBindTexture(GL_SAMPLER_2D, 0);
    };

    auto testMipmap = [&](){
        static float angle = -55.0f;
        glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 scale = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 rotate = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 translate = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

        glm::mat4 baseModel = model;
   

        glBindVertexArray(vao);
        glBindTexture(GL_SAMPLER_2D, textrueId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterParam[filterIndex]);
        if (GLAD_GL_ARB_texture_filter_anisotropic) {
            if (anisotropicEnable) {
                // 设置各向异性过滤级别
                GLfloat maxAnisotropy;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
            }
            else {
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
            }
        }

        ourShader.use();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("model",  baseModel);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

        glBindVertexArray(0);
        glBindTexture(GL_SAMPLER_2D, 0);
    };

    std::vector<glm::vec3> points = {
        {0.5f, 0.5f, -1.0f},
        {-0.5f, 0.5f, -1.0f},
        {-0.5f, -0.5f, -1.0f},
        {0.5f, -0.5f, -1.0f},
        {1.7f, 1.7f, -1.0f},
        {-1.7f, 1.7f, -1.0f},
        {-1.7f, -1.7f, -1.0f},
        {1.7f, -1.7f, -1.0f},
    };
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        testMipmap();
        //drawImage_RotateAroundAnyPoint(points);
        //drawImage_RotateAtAnyPoint();
        drawCoordinateAxis();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void changeTextureParam(int index, bool enable) {
    filterIndex = index;
    anisotropicEnable = enable;
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        changeTextureParam(0, anisotropicEnable);

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        changeTextureParam(1, anisotropicEnable);

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        changeTextureParam(2, anisotropicEnable);

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        changeTextureParam(3, anisotropicEnable);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        changeTextureParam(filterIndex, !anisotropicEnable);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
