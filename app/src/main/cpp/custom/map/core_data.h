#include "../../utilities/math/structs/vxd.h"

const float hex_base_h = 0.086602540378443864676372317075294f;
const float hex_base_len = 0.1f;

const V3D hex_base_vertices[7] = {{0.0f,0.0f,0.0f},{0.0f, hex_base_len, 0.0f}, {hex_base_h, hex_base_len * 0.5f, 0.0f},{hex_base_h, -hex_base_len * 0.5f, 0.0f},{0.0f, -hex_base_len, 0.0f}, {-hex_base_h, -hex_base_len * 0.5f, 0.0f},{-hex_base_h, hex_base_len * 0.5f, 0.0f}};

const float hex_tex_base_w2 = 0.8515625f;
const float hex_tex_base_w = 0.4296875f;
const float hex_tex_base_h2 = 0.7421875f;
const float hex_tex_base_h = 0.2578125f;

const V2D hex_base_uv[] = {{hex_tex_base_w,(hex_tex_base_h2 + hex_tex_base_h)* 0.5f},{hex_tex_base_w, 0.0f}, {hex_tex_base_w2, hex_tex_base_h},{hex_tex_base_w2, hex_tex_base_h2},{hex_tex_base_w, 1.0f}, {0.0f, hex_tex_base_h2},{0.0f, hex_tex_base_h}};

const V3Du8 hex_base_indices[] = {{0,1,2}, {0,2,3},{0,3,4},{0,4,5},{0,5,6},{0,6,1}};

V2D hex_base_locations(int x, int y){
   V2D loc;
    loc.x =  2.0f * hex_base_h * (float)x /*+ ((y % 2) ? hex_base_h : 0)*/;
    // LOG("SOME" + std::to_string(loc.x));
    // if((unsigned int)y % 2){
    //     loc.x += hex_base_h;
    // }
    // LOG("SOME" + std::to_string(loc.x));
    loc.y = 0.65f * hex_base_len * (float)y;

    if(y & (0b1)){
        loc.x += hex_base_h;
    }
    return loc;
}