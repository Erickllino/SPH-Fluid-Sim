#version 450 core

in vec3 vEyeCenter;

uniform mat4 uProj;
uniform float uRadius;

out vec4 FragColor;

void main()
{
    // gl_PointCoord in [0,1]^2, center = (0.5, 0.5)
    vec2 uv = gl_PointCoord * 2.0 - 1.0;   // [-1, 1]

    float r2 = dot(uv, uv);
    if (r2 > 1.0) discard;                  // outside sphere silhouette

    // View-space normal from the sphere surface
    float z = sqrt(1.0 - r2);
    vec3 N  = vec3(uv, z);                  // already normalised

    // ── Depth correction ─────────────────────────────────────────────────────
    // True eye-space hit point on the sphere
    vec3 hitEye = vEyeCenter + N * uRadius;
    vec4 clip   = uProj * vec4(hitEye, 1.0);
    gl_FragDepth = (clip.z / clip.w) * 0.5 + 0.5;

    // ── Lighting (view-space, directional) ───────────────────────────────────
    vec3 lightDir = normalize(vec3(1.0, 2.0, 1.5));
    float diff    = max(dot(N, lightDir), 0.0);

    vec3 viewDir  = vec3(0.0, 0.0, 1.0);   // toward camera in view space
    vec3 halfDir  = normalize(lightDir + viewDir);
    float spec    = pow(max(dot(N, halfDir), 0.0), 64.0);

    vec3 baseColor = vec3(1, 0, 0);  // Red
    vec3 col = 0.15 * baseColor             // ambient
             + diff * baseColor             // diffuse
             + spec * vec3(1.0);            // specular highlight

    FragColor = vec4(col, 1.0);
}
