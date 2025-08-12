#pragma once

#include "../../utilities/log/API.h"
#include <EGL/egl.h>
#include <GLES3/gl32.h>


struct shader_prog {
    public:
    void Init(const char* vSrc, const char* fSrc){
        program = createProgram(vSrc, fSrc);
    }
    ~shader_prog() {
        glDeleteProgram(program);
    }

    GLint program = 0;

    private:
    GLuint createProgram(const char* vSrc, const char* fSrc) {
        LOGI("Starting GL Program Compilation");
        GLuint vs = compileShader(GL_VERTEX_SHADER, vSrc);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fSrc);

        GLuint prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);

        GLint linked;
        glGetProgramiv(prog, GL_LINK_STATUS, &linked);
        if (!linked) {
            char log[512];
            glGetProgramInfoLog(prog, sizeof(log), NULL, log);
            glDeleteProgram(prog);
            LOGE("Error: Failed to compile Program. GL informs:");
            LOGE(log);
            return 0;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
        return prog;
    }
    static GLuint compileShader(GLenum type, const char* src) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            char log[512];
            glGetShaderInfoLog(shader, sizeof(log), NULL, log);
            glDeleteShader(shader);
        LOGE("Error: Failed to compile shader. GL informs:");
        LOGE(log);
            return 0;
        }
        return shader;
    }
};
