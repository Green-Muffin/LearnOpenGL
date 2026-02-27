//
// Created by GreenMuffin on 2026/1/8.
//

#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include "renderer.h"

constexpr auto ShaderPath = "C:\\Users\\dodoe\\repos\\CppProjects\\LearnOpenGL\\res\\shader\\";

Shader::Shader(const std::string& file_path) : file_path_(file_path) {
    auto [vertex_source, fragment_source] = prase_shader_(file_path);
    shader_id_ = create_shader_(vertex_source, fragment_source);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(shader_id_));
}

void Shader::bind() const {
    if (shader_id_ == 0) {
        return;
    }
    GLCall(glUseProgram(shader_id_));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::set_uniform_1i(const std::string &name, const int value) {
    bind();
    GLCall(glUniform1i(get_uniform_location(name), value));
}

void Shader::set_uniform_2f(const std::string &name, const float v0, const float v1) {
    bind();
    GLCall(glUniform2f(get_uniform_location(name), v0, v1));
}

void Shader::set_uniform_3f(const std::string &name, const float v0, const float v1, const float v2) {
    bind();
    GLCall(glUniform3f(get_uniform_location(name), v0, v1, v2));
}

void Shader::set_uniform_4f(const std::string &name, const float v0, const float v1, const float v2, const float v3) {
    bind();
    GLCall(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
}

void Shader::set_uniform_mat4f(const std::string &name, const glm::mat4 &matrix) {
    bind();
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]);
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

	if (vs == 0 || fs == 0) {
		GLCall(glDeleteProgram(program));
		return 0;
	}

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));

	int link_status = GL_FALSE;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &link_status));
	if (link_status == GL_FALSE) {
		int length = 0;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		const auto message = static_cast<char*>(alloca(length * sizeof(char)));
		GLCall(glGetProgramInfoLog(program, length, &length, message));
		std::cout << "Failed to link shader program!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteProgram(program));
		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));
		return 0;
	}

	GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

ShaderProgramSource Shader::prase_shader_(const std::string &file_name) {
    const auto full_path = ShaderPath + file_name;
    std::ifstream stream(full_path);

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
