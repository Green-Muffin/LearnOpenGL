//
// Created by GreenMuffin on 2026/1/8.
//

#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include "renderer.h"

Shader::Shader(const std::string& file_path) : file_path_(file_path) {
    auto [vertex_source, fragment_source] = prase_shader_(file_path);
    shader_id_ = create_shader_(vertex_source, fragment_source);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(shader_id_));
}

void Shader::bind() const {
    GLCall(glUseProgram(shader_id_));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::set_uniform_4f(const std::string &name, const float v0, const float v1, const float v2, const float v3) {
    GLCall(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
}

unsigned int Shader::compile_shader_(const unsigned int type, const std::string &source) {
    GLCall(const unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        const auto message = static_cast<char*>(alloca(length * sizeof(char)));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

unsigned int Shader::create_shader_(const std::string &vertex_shader, const std::string &fragment_shader) {
    GLCall(const unsigned int program = glCreateProgram());
    const unsigned int vs = compile_shader_(GL_VERTEX_SHADER, vertex_shader);
    const unsigned int fs = compile_shader_(GL_FRAGMENT_SHADER, fragment_shader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

ShaderProgramSource Shader::prase_shader_(const std::string &file_path) {
    std::ifstream stream(file_path);

    enum class ShaderType { none = -1, vertex = 0, fragment = 1 };

    std::string line;
    auto type = ShaderType::none;
    std::stringstream ss[2];
    while (getline(stream, line)) {
        if (line.find("@shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::vertex;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::fragment;
            }
        } else {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }
    return { ss[static_cast<int>(ShaderType::vertex)].str(), ss[static_cast<int>(ShaderType::fragment)].str() };
}

unsigned int Shader::get_uniform_location(const std::string &name) {
    if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) {
        return uniform_location_cache_[name];
    }
    GLCall(const int location = glGetUniformLocation(shader_id_, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    uniform_location_cache_[name] = location;
    return location;
}
