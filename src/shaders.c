#include <stdlib.h>
#include <stdio.h>
#include "shaders.h"

const char* vertex_source =
    "#version 330 core\n"
    "layout (location = 0) in vec2 a_pos;\n"
    "uniform mat3 m;\n"
    "void main() {\n"
    "   vec3 pos = m * vec3(a_pos.xy, 1.0);\n"
    "   gl_Position = vec4(pos.xy, 0.0, 1.0);\n"
    "}";

const char* fragment_source =
    "#version 330 core\n"
    "out vec4 out_color;\n"
    "uniform vec4 color;\n"
    "void main() {\n"
    "   out_color = color;\n"
    "}";

char* readSource(const char* filepath);
GLuint createShader(const char* source, GLenum type);
GLuint createProgram(GLuint vertex_shader, GLuint fragment_shader);

GLuint createBasicProgram(const char* vs_source, const char* fs_source) {
    GLuint vertex_shader = createShader(vs_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = createShader(fs_source, GL_FRAGMENT_SHADER);

    return createProgram(vertex_shader, fragment_shader);
}

GLuint createBasicProgramFromFiles(const char* vs_filepath, const char* fs_filepath) {
    const char* vs_source = readSource(vs_filepath);
    const char* fs_source = readSource(fs_filepath);

    return createBasicProgram(vs_source, fs_source);
}

char* readSource(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    long length;
    char* str = NULL;

    if (file) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        str = malloc(length);
        if (str)
            fread(str, 1, length, file);
        fclose(file);
    }
    else
        printf("Unable to open the file %s", filepath);

    return str;
}

GLuint createShader(const char* source, GLenum type) {
    GLint success;
    GLint info_length;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    // Error handling
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
    if (!success) {
        GLchar* info_log = malloc(info_length * sizeof(*info_log));
        glGetShaderInfoLog(shader, info_length, NULL, info_log);
        printf("%s", info_log);
        free(info_log);
    }

    return shader;
}

// Vertex and fragment shaders only for now (and only one of each)
GLuint createProgram(GLuint vertex_shader, GLuint fragment_shader) {
    GLint success;
    GLint info_length;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    // Error handling
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
    if (!success) {
        GLchar* info_log = malloc(info_length * sizeof(*info_log));
        glGetProgramInfoLog(program, info_length, NULL, info_log);
        printf("%s", info_log);
        free(info_log);
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    if (!success)
        return 0;
    return program;
}
