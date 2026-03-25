

/*
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include "renderer/Renderer.h"
// #include "simulation/SPHSystem.cuh"
// #include "ui/DebugUI.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "SPH Fluid", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    SPHSystem sph(n_particles=10000);
    Renderer renderer(sph.getVBO()); // passa o VBO pra já registrar no CUDA
    DebugUI ui(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        sph.step();
        renderer.draw(sph.particleCount());
        ui.render(sph.params());
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "renderer/Renderer.h"
// #include "renderer/Shader.h"

#include <cmath>

int main()
{
    if (!glfwInit())
    {
        printf("GLFW init falhou\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "SPH Test", nullptr, nullptr);
    if (!window)
    {
        printf("Janela falhou\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("GLAD falhou\n");
        return -1;
    }

    printf("OpenGL: %s\n", glGetString(GL_VERSION));
    printf("GPU: %s\n", glGetString(GL_RENDERER));

    Renderer renderer(1000);
    float positions[9] = {
        -0.5f, -0.5f, -2.0f,
        0.0f, 0.5f, -2.0f,
        0.5f, -0.5f, -2.0f};

    float fov = 45.0f * 3.14159f / 180.0f; // 45 graus em radianos
    float aspect = 800.0f / 600.0f;
    float near = 0.1f, far = 100.0f;
    float f = 1.0f / tan(fov / 2.0f);

    float proj[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near) / (near - far), -1,
        0, 0, (2 * far * near) / (near - far), 0};

    float i = 0.001f;
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        float dt = (float)(currentTime - lastTime);
        lastTime = currentTime;
        glEnable(GL_PROGRAM_POINT_SIZE);
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.setMVP(proj);
        renderer.update(positions, 3);
        renderer.draw_triangle(3);

        for (int j = 0; j < 3; j++)
        {
            printf("%d\n", j);
            positions[3 * j + 3] -= i;
        };

        // Swap front and back buffers
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}