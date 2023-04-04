#include <glad/glad.h>

extern const char* vertex_source;
extern const char* fragment_source;

GLuint createBasicProgram(const char* vs_source, const char* fs_source);
GLuint createBasicProgramFromFiles(const char* vs_filepath, const char* fs_filepath);
