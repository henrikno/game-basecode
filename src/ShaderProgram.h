#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "common.h"

class ShaderProgram {
public:
    ShaderProgram();
    bool LoadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    bool LoadFromFile(const std::string& combinedShaderPath);
    virtual ~ShaderProgram ();
    void Enable();
    void Disable();

private:
    GLuint myProgram;
    GLuint myVertexShader, myFragmentShader;

private:
    std::vector<std::string> ReadSingleSource(const std::string &shaderPath);
    void ReadCombinedSource(const std::string &combinedShaderPath, std::vector<std::string> &vertexSource, std::vector<std::string> &fragmentSource);

    bool CompileShaderFromFile(const std::string& fileName, GLenum type);
    bool LoadShaderFromSource(const std::string& shaderSource, GLenum type);
    std::vector<std::string> ReadCombinedSource(const std::string &combinedShaderPath);
    void FindUniforms();
    void FindVertexAttributes();
    bool CompileShader(const std::vector<std::string> &source, GLenum type);
    void CompileProgram();
};
