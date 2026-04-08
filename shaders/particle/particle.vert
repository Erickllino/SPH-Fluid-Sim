#version 450 core

layout(location = 0) in vec3 aPos;

uniform mat4 uView;
uniform mat4 uProj;
uniform float uRadius;
uniform float uViewportH;

out vec3 vEyeCenter;

void main()
{
    vec4 eyePos = uView * vec4(aPos, 1.0);
    vEyeCenter   = eyePos.xyz;

    gl_Position  = uProj * eyePos;

    // Screen-space diameter: f * diameter / -z  (in NDC), scaled to pixels
    // uProj[1][1] == f (focal length, y-axis scale)
    float diam = 2.0 * uRadius;
    gl_PointSize = uViewportH * uProj[1][1] * uRadius / (-eyePos.z);

    // small bias so the silhouette isn't clipped
    gl_PointSize *= 1.1;
}
