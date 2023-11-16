#include <GL/glew.h>

const char *readGLSLFile(const char *filename);
unsigned int compileShader(GLenum shaderType, const char *shaderCode);
unsigned int linkShaders(unsigned int vertexShader, unsigned int fragmentShader);
