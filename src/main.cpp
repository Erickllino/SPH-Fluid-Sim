

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
#include "mat4.h"
#include "vec3.h"
#include "renderer/Camera.h"

// #include "renderer/Shader.h"

#include <cmath>
const float PI = 3.14159265358979323846f;

const float WIDTH = 800;
const float HEIGHT = 600;

vec3 RotY(float angle, vec3 v, vec3 out)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    mat4 rotY = {{{cosA, 0, sinA, 0},
                  {0, 1, 0, 0},
                  {-sinA, 0, cosA, 0},
                  {0, 0, 0, 1}}};

    out = transformPoint(rotY, v);

    return out;
}

vec3 RotX(float angle, vec3 v, vec3 out)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    mat4 rotX = {{{1, 0, 0, 0},
                  {0, cosA, -sinA, 0},
                  {0, sinA, cosA, 0},
                  {0, 0, 0, 1}}};

    out = transformPoint(rotX, v);

    return out;
}

vec3 RotZ(float angle, vec3 v, vec3 out)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    mat4 rotZ = {{{cosA, -sinA, 0, 0},
                  {sinA, cosA, 0, 0},
                  {0, 0, 1, 0},
                  {0, 0, 0, 1}}};

    out = transformPoint(rotZ, v);

    return out;
}

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

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "SPH Test", nullptr, nullptr);
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

    // Renderer renderer(1000, "triangle");

    ShapeDebugRenderer shapeRenderer("triangle");
    DebugRenderer debugRenderer("axis");
    ParticleRenderer particleRenderer(1, "particle");

    vec3 positions[3] = {
        {0.0f, 0.0f, 0.0f},
        {0.5f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}};

    vec3 part = {1.0f, 0.0f, 0.0f};
    // vec3 part = {-0.7f, 0.0f, -1.0f};
    float i = PI / 72.0f;
    double lastTime = glfwGetTime();

    Camera camera(vec3(2.0f, 2.0f, 3.0f), // posição da câmera
                  vec3(0.0f, 0.0f, 0.0f), // olhando para a origem
                  vec3(0.0f, 1.0f, 0.0f), // up = Y
                  45.0f * PI / 180.0f);   // fov em radianos

    // float fov = 45.0f * PI / 180.0f; // 45 graus em radianos
    // float aspect = WIDTH / HEIGHT;
    // float near = 0.1f, far = 100.0f;

    // mat4 proj = perspective(fov, aspect, near, far, proj);
    // mat4 view = lookAt(vec3(2.0f, 2.0f, 3.0f),  // posição da câmera
    //                    vec3(0.0f, 0.0f, 0.0f),  // olhando para a origem
    //                    vec3(0.0f, 1.0f, 0.0f)); // up = Y
    // mat4 vp = view * proj;

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {

        double currentTime = glfwGetTime();
        float dt = (float)(currentTime - lastTime);
        lastTime = currentTime;

        glEnable(GL_PROGRAM_POINT_SIZE);
        // glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shapeRenderer.setMVP(camera.vp);

        debugRenderer.drawAxes(camera.vp);

        printf("Camera position: (%.2f, %.2f, %.2f)\n", camera.cam_pos.x, camera.cam_pos.y, camera.cam_pos.z);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            camera.move_camera(vec3(0.1f, 0.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            camera.move_camera(vec3(-0.1f, 0.0f, 0.0f));
        }

        // if (int(currentTime) == 0)
        // {
        // }
        // else if (int(currentTime) % 3 == 1)
        // {
        //     part = RotY(i, part, part);
        // }
        // else if (int(currentTime) % 3 == 2)
        // {
        //     part = RotZ(i, part, part);
        // }
        // else if (int(currentTime) % 3 == 0)
        // {
        //     part = RotX(i, part, part);
        // }

        // if (currentTime >= 10.0f)
        // {
        //     part = RotX(PI / 72.0f, part, part);
        // };

        // if (currentTime >= 15.0f)
        // {
        //     part = RotZ(PI / 72.0f, part, part);
        // };

        particleRenderer.setViewProj(camera.view, camera.proj);
        particleRenderer.update((float *)&part, 1);
        particleRenderer.drawParticles(1, 0.1f, HEIGHT);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}