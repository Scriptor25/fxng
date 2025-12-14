#version 460 core

#define LIGHT_SOURCE_TYPE_DIRECTIONAL 0
#define LIGHT_SOURCE_TYPE_POINT 1

layout (location = 0) in vec4 POSITION;
layout (location = 1) in vec3 NORMAL;
layout (location = 2) in vec2 TEX;
layout (location = 3) in vec4 WORLD_POSITION;

layout (location = 0) out vec4 color;

uniform vec3 AMBIENT;
uniform vec3 DIFFUSE;
uniform vec3 SPECULAR;
uniform float SHININESS;

struct Camera {
    vec3 Position;
};

uniform Camera CAMERA;

struct LightSource {
    uint Type;
    vec3 Position;
    vec3 Direction;
    vec3 Diffuse;
    vec3 Specular;
    float LinearFalloff;
    float QuadraticFalloff;
};

layout (std430, binding = 1) buffer LIGHT_SOURCE_BUFFER {
    LightSource LIGHTS[];
};

void main() {

    vec3 P = WORLD_POSITION.xyz / WORLD_POSITION.w;
    vec3 N = normalize(NORMAL);
    vec3 V = normalize(CAMERA.Position - P);

    vec3 acc = AMBIENT;
    for (uint m = 0u; m < LIGHTS.length; ++m) {
        vec3 Lm, Hm, diffuse, specular;
        float attenuation, ndl;

        switch (LIGHTS[m].Type) {
            case LIGHT_SOURCE_TYPE_DIRECTIONAL:
                {
                    Lm = -normalize(LIGHTS[m].Direction);
                    attenuation = 1.0;
                    break;
                }
            case LIGHT_SOURCE_TYPE_POINT:
                {
                    vec3 forward;
                    float dist;

                    forward = LIGHTS[m].Position - P;
                    dist = max(length(forward), 0.001);
                    Lm = forward / dist;
                    attenuation = 1.0 / (1.0 + LIGHTS[m].LinearFalloff * dist + LIGHTS[m].QuadraticFalloff * dist * dist);
                    break;
                }
            default:
                continue;
        }

        Hm = normalize(Lm + V);

        ndl = max(dot(Lm, N), 0.0);

        diffuse = DIFFUSE * ndl * LIGHTS[m].Diffuse;
        specular = ndl > 0.0 ? SPECULAR * pow(max(dot(N, Hm), 0.0), SHININESS) * LIGHTS[m].Specular : vec3(0.0);

        acc += (diffuse + specular) * attenuation;
    }

    color = vec4(min(acc, vec3(1.0)), 1.0);
}
