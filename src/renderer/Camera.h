
#include "mat4.h"
#include "vec3.h"

class Camera
{
public:
    Camera(vec3 pos, vec3 cam_target, vec3 cam_up, float fov);
    ~Camera();

    float fov;
    float aspect;
    float near;
    float far;
    vec3 cam_pos;
    vec3 cam_target;
    vec3 cam_up;

    mat4 view;
    mat4 proj;
    mat4 vp = view * proj;

    vec3 getPosition() const { return cam_pos; };
    void move_camera(const vec3 &delta);

    void update();
};
