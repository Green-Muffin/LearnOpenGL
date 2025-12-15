//
// Created by GreenMuffin on 2025/10/26.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr auto kScreenWidth = 800;
constexpr auto kScreenHeight = 600;
auto kWindowTitle = "OpenGL Window";

struct ShaderProgramSource {
    std::string vertex_source;
    std::string fragment_source;
};

// The opengl documentation can be found at https://www.docs.gl

static unsigned int compile_shader(const unsigned int type, const std::string& source) {
    const unsigned int id = glCreateShader(type );
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        const auto message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader) {
    const unsigned int program = glCreateProgram();
    const unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    const unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static ShaderProgramSource prase_shader(const std::string& file_path) {
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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(kScreenWidth, kScreenHeight, kWindowTitle, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    std::cout << glGetError() << std::endl;

    int fb_width = 0;
    int fb_height = 0;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    // Create and bind VAO
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    constexpr float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    // create and bind buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // size: the size in bytes of the buffer object's new data store
    // usage: GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW;
    // the GL_STATIC_DRAW means the data will most likely not change at all and used many times,
    // the GL_DYNAMIC_DRAW means the data will change occasionally and used many times, the GL_STREAM_DRAW means the data will change almost every time and used a few times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);

    // don't forget to enable the vertex attribute array
    glEnableVertexAttribArray(0);

    //index: the index of the generic vertex attribute to be modified
    // size: the number of attributes a vertex possesses ( is float not vec )
    // type: the data type of each component in the array
    // normalized: whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed
    // stride: the byte offset between consecutive vertex attributes. If the attributes are tightly packed, this can be set to 0
    // pointer: (const GLvoid*) offset of the first component of the first attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, static_cast<void *>(nullptr));

    // Parse shader once and reuse
    auto [vertex_source, fragment_source] = prase_shader("../res/basic.shader");
    const unsigned int shader = create_shader(vertex_source, fragment_source);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}