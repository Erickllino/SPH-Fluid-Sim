#include "Camera.h"

const float PI = 3.14159265358979323846f;

const float WIDTH = 800;
const float HEIGHT = 600;

Camera::Camera(vec3 pos, vec3 cam_target, vec3 cam_up, float fov)
    : cam_pos(pos), fov(fov), aspect(WIDTH / HEIGHT), near(0.1f), far(100.0f)
{
    // cam_pos = vec3(2.0f, 2.0f, 3.0f);
    cam_target = vec3(0.0f, 0.0f, 0.0f);
    cam_up = vec3(0.0f, 1.0f, 0.0f);
    view = lookAt(cam_pos, cam_target, cam_up);
    proj = perspective(fov, aspect, near, far, proj);
    vp = view * proj;
}

Camera::~Camera() {}

void Camera::move_camera(const vec3 &delta)
{
    cam_pos += delta;
    view = lookAt(cam_pos, cam_target, cam_up);
    vp = view * proj;
}
