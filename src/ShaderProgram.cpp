#include "ShaderProgram.h"
#include <fstream>
#include <string.h>


/// Local error handling code
namespace ShaderErrorHandling
{
    /// Holds a compilation or linking error line
    struct Error
    {
        Error(int line_, int code_, const std::string& msg) : file(0), line(line_), code(code_), text(msg) {}

        /* Expects input string like "0(45) : error C1008: undefined variable "rgb" " */
        Error(const std::string& msg) 
            : file(0), line(0), code(0), text()
        {
            //int file;
            char str[64];
            memset(str, 0, 64);
            sscanf(msg.c_str(), "%d(%d) : error C%d: %s",&file, &line, &code, str);

            size_t err_start = msg.find(str);
            this->text = msg.substr(err_start);	
        } 

        /// Prints the full debug info
        void print() const
        {
            std::cerr << "Error " << code << " at line " << line << " : " << text << std::endl;
        }

        /// Just prints the line number and error message
        void printLight() const
        {
            std::cerr << "line " << line << ": " << text << std::endl;
        }

        int file;
        int line;
        int code;
        std::string text;
    };

    /** Parses the error string from the compiler, builds a list with error 
      codes and prints them.
      */
    static std::vector<Error> handleErrors(char* errorblock)
    {
        std::vector<Error> errors;

        // parse the whole error block
        char* str = strtok(errorblock, "\n");
        while (str)
        {
            errors.push_back(Error(str));
            str = strtok(0, "\n");
        }

        // print all errors
        for (std::vector<Error>::iterator it = errors.begin(); it != errors.end(); ++it)
            it->printLight();

        return errors;
    }

} // end namespace

ShaderProgram::ShaderProgram()
 : myProgram(0), myVertexShader(0), myFragmentShader(0)
{
    glError();
}

ShaderProgram::~ShaderProgram() {
    assert(myProgram != 0);
    assert(myVertexShader != 0);
    assert(myFragmentShader != 0);
    glDeleteProgram(myProgram);
    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);
    glError();
}

bool ShaderProgram::LoadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    glError();

    std::vector<std::string> vertexSource = ReadSingleSource(vertexShaderPath);
    std::vector<std::string> fragmentSource = ReadSingleSource(fragmentShaderPath);
    bool compileResult0 = CompileShader(vertexSource, GL_VERTEX_SHADER);
    bool compileResult1 = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

    if (!compileResult0 || !compileResult1) {
        std::cerr << "Unable to link shader due to compilation errors." << std::endl;
        throw std::runtime_error("Unable to link shader due to compilation errors.");
    }

    CompileProgram();

    FindUniforms();
    FindVertexAttributes();
    assert(myProgram != 0);

    std::clog << "Created shader program (" << vertexShaderPath << ", " << fragmentShaderPath << ")" << std::endl;
    std::cout << "myProgram: " << myProgram << std::endl;

    glError();
    return true;
}

bool ShaderProgram::LoadFromFile(const std::string &combinedShaderPath) {
    glError();

    std::vector<std::string> vertexSource;
    std::vector<std::string> fragmentSource;
    ReadCombinedSource(combinedShaderPath, vertexSource, fragmentSource);
    glError();
    bool compileResult0 = CompileShader(vertexSource, GL_VERTEX_SHADER);
    glError();
    bool compileResult1 = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
    glError();

    if (!compileResult0 || !compileResult1) {
        std::cerr << "Unable to link shader due to compilation errors." << std::endl;
        throw std::runtime_error("Unable to link shader due to compilation errors.");
    }

    CompileProgram();
    glError();

    FindUniforms();
    glError();
    FindVertexAttributes();
    glError();
    assert(myProgram != 0);

    std::clog << "Created shader program (" << combinedShaderPath << ")" << std::endl;
    std::cout << "myProgram: " << myProgram << std::endl;

    glError();
    return true;
}

std::vector<std::string> ShaderProgram::ReadSingleSource(const std::string &shaderPath) {
    std::vector<std::string> contents;
    std::ifstream file;
    file.open(shaderPath.c_str());

    if (!file.is_open()) {
        std::cerr << "Error opening file " << shaderPath << "!\n";
        throw std::runtime_error("Error opening shader file '" + shaderPath +"'");
    }

    std::string line;
    while (!file.eof()) {
        std::getline(file, line);
        line.append("\n");
        contents.push_back(line);	
    }
    file.close();
    return contents;
}

void ShaderProgram::ReadCombinedSource(const std::string &combinedShaderPath, std::vector<std::string> &vertexSource, std::vector<std::string> &fragmentSource) {
    std::vector<std::string> contents = ReadSingleSource(combinedShaderPath);
    // Filter out vertex and fragment shader parts
    int state = 0;
    for (size_t i = 0; i < contents.size(); i++) {
        if (contents[i].find("@vertex:") != std::string::npos) {
            state = 1;
            continue;
        }
        if (contents[i].find("@fragment:") != std::string::npos) {
            state = 2;
            continue;
        }

        switch(state) {
            case 0:
                // Junk
                break;
            case 1:
                vertexSource.push_back(contents[i]);
                //std::cout <<"V " << contents[i];
                break;
            case 2:
                fragmentSource.push_back(contents[i]);
                //std::cout << "F " << contents[i];
                break;
        }
    }
}

void ShaderProgram::Enable() {
    glError();
    glUseProgram(myProgram);
    glError();
}

void ShaderProgram::Disable() {
    glError();
    glUseProgram(0);
    glError();
}

void ShaderProgram::FindUniforms() {
    glError();
    // Get all uniforms from shader
    GLint nUniforms = -1;
    GLint maxUniformNameLength;
    glGetProgramiv(myProgram, GL_ACTIVE_UNIFORMS, &nUniforms);
    glGetProgramiv(myProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
    GLchar *uniformName = new GLchar[maxUniformNameLength+1];

    for (int i = 0; i < nUniforms; i++) {
        GLsizei uniformNameLength = -1;
        GLint uniformSize = -1;
        GLenum uniformType = GL_ZERO;
        // TODO: glGetActiveUniform is deprecated, should use
        // glGetActiveUniformName or glGetActiveUniformsiv
        glGetActiveUniform(myProgram, i, maxUniformNameLength, &uniformNameLength, &uniformSize, &uniformType, uniformName);
        GLint uniformLocation = glGetUniformLocation(myProgram, uniformName);
        std::cout << "UNIFORM: " << uniformLocation << ", " << uniformName << " " << uniformType << std::endl;
        //if (uniformLocation != -1)
            //myUniforms[uniformName] = UniformFactory(uniformType, uniformLocation, this); //new Uniform(uniformLocation, this);
    }
    delete uniformName;
    glError();
}

void ShaderProgram::FindVertexAttributes() {
    glError();
    GLint nAttributes = -1;
    GLint maxAttributeNameLength = -1;
    glGetProgramiv(myProgram, GL_ACTIVE_ATTRIBUTES, &nAttributes);
    glGetProgramiv(myProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
    GLchar *attribName = new GLchar[maxAttributeNameLength+1];

    for (int i = 0; i < nAttributes; i++) {
        GLsizei nameLength = -1;
        GLint attribSize = -1;
        GLenum attribType = GL_ZERO;
        glGetActiveAttrib(myProgram, i, maxAttributeNameLength, &nameLength, &attribSize, &attribType, attribName);
        GLint attribLocation = glGetAttribLocation(myProgram, attribName);
        std::cout << "ATTRIB:" << attribLocation << ", " << attribName << " " << attribType << std::endl;
        // TODO: Ignore attribs starting with gl_
        // if (strcmp(attribName, "gl_", 3) == 0)
        //if (attribLocation != -1)
            //myAttribs[attribName] = new Attribute(attribLocation, this);
    }
    delete attribName;
    glError();
}

void ShaderProgram::CompileProgram() {
    glError();
    // Load vertex and fragment shader from file, check for errors

    myProgram = glCreateProgram();
    glError();
    assert(myProgram != 0);
    glAttachShader(myProgram, myVertexShader);
    glError();
    glAttachShader(myProgram, myFragmentShader);
    glError();
    glLinkProgram(myProgram);
    glError();

    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset( buffer, 0, BUFFER_SIZE );
    int length = 0;

    memset(buffer, 0, BUFFER_SIZE);
    glGetProgramInfoLog(myProgram, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
        std::cerr << "Error linking shader!" << std::endl;
        ShaderErrorHandling::handleErrors(buffer);
    }

    glValidateProgram( myProgram );
    int status;
    glGetProgramiv( myProgram, GL_VALIDATE_STATUS, &status );
    if (status == GL_FALSE) {
        std::cerr << "Error validating shader\n";
    }
}

bool ShaderProgram::CompileShaderFromFile(const std::string &fileName, GLenum type) {
    glError();
    std::vector<std::string> contents = ReadSingleSource(fileName);
    glError();
    return CompileShader(contents, type);
}

bool ShaderProgram::CompileShader(const std::vector<std::string> &source, GLenum type) {
    int current = 0;
    if (type == GL_VERTEX_SHADER)
        current = myVertexShader = glCreateShader(GL_VERTEX_SHADER);
    else
        current = myFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glError();

    const char **glLines = new const char*[source.size()];
    for (unsigned int i = 0; i < source.size(); ++i)
        glLines[i] = source[i].c_str();

    glShaderSource( current, source.size(), glLines , 0 );
    glError();
    glCompileShader( current );
    glError();
    delete[] glLines;

    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset( buffer, 0, BUFFER_SIZE );
    int length = 0;

    bool result = true;
    glGetShaderInfoLog( current, BUFFER_SIZE, &length, buffer );
    if (length > 0) {
        std::cerr << "Error compiling " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader \"" << "\"." << std::endl;
        ShaderErrorHandling::handleErrors(buffer);
        result = false;
    }
    glError();
    return result;
}
