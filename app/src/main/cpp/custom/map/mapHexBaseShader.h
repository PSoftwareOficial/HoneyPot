#pragma once

const char* mapHexBasevSrc = R"(#version 320 es
precision mediump float;

layout(location = 0) in vec3 a_position;           // Hexagon vertex position
layout(location = 1) in vec2 a_uv;
layout(location = 2) in mat4 a_model;              // Instance model matrix
layout(location = 6) in uint a_texIndex;          // Texture index per instance

uniform mat4 u_viewProj;

out vec2 v_uv;
flat out uint v_texIndex;

void main() {
    gl_Position = u_viewProj * a_model * vec4(a_position, 1.0);
    v_uv = a_uv;
    v_texIndex = a_texIndex;
}
)";


const char* mapHexBasefSrc = R"(#version 320 es
precision mediump float;
precision mediump sampler2DArray;

in vec2 v_uv;
flat in uint v_texIndex;

uniform sampler2DArray u_texArray;

out vec4 fragColor;

void main() {
    fragColor = texture(u_texArray, vec3(v_uv, float(v_texIndex)));
}
)";
