//
// Created by GreenMuffin on 2026/1/8.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/fwd.hpp"

struct ShaderProgramSource {
    std::string vertex_source;
    std::string fragment_source;
};

class Shader {
public:
    explicit Shader(const std::string& file_path);
    ~Shader();

    void bind() const;
    void unbind() const;

    void set_uniform_1i(const std::string& name, int value);
    void set_uniform_2f(const std::string& name, float v0, float v1);
    void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3);
    void set_uniform_mat4f(const std::string& name, const glm::mat4& matrix);

private:
    unsigned int shader_id_;
    std::string file_path_;
    std::unordered_map<std::string, unsigned int> uniform_location_cache_;

    unsigned int compile_shader_(unsigned int type, const std::string& source);
    unsigned int create_shader_(const std::string& vertex_shader, const std::string& fragment_shader);
    ShaderProgramSource prase_shader_(const std::string& file_path);
    unsigned int get_uniform_location(const std::string& name);
};


#endif //LEARNOPENGL_SHADER_H