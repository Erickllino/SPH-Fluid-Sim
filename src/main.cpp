

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
/*
#include <glad/glad.h> // sempre antes do glfw
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    // ── GLFW init ────────────────────────────────────────────────
    if (!glfwInit())
    {
        std::cerr << "GLFW: falhou ao inicializar\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "SPH - sanity check", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "GLFW: falhou ao criar janela\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // ── GLAD init ─────────────────────────────────────────────────
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD: falhou ao carregar ponteiros OpenGL\n";
        return -1;
    }

    // ── Info da GPU ───────────────────────────────────────────────
    std::cout << "Vendor:   " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Version:  " << glGetString(GL_VERSION) << "\n";

    // ── Loop ──────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // azul escuro — cor clássica de "funcionou"
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

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

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}