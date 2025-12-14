#version 460 core

layout (location = 0) in vec4 POSITION;
layout (location = 1) in vec3 NORMAL;
layout (location = 2) in vec2 TEX;

layout (location = 0) out vec4 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec2 tex;
layout (location = 3) out vec4 world_position;

uniform mat4 MODEL;
uniform mat4 INVMODEL;
uniform mat4 VIEW;
uniform mat4 PROJECTION;

void main() {

    position = PROJECTION * VIEW * MODEL * POSITION;
    normal = mat3(transpose(INVMODEL)) * NORMAL;
    tex = TEX;
    world_position = MODEL * POSITION;
}
