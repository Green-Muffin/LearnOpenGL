//
// Created by GreenMuffin on 2026/1/8.
//

#include "texture.h"

#include "renderer.h"
#include "stb_image.h"

constexpr auto TexturePath = "C:\\Users\\dodoe\\repos\\CppProjects\\LearnOpenGL\\res\\textures\\";

Texture::Texture(const std::string &file_name)
    : file_name_(file_name), local_buffer_(nullptr), width_(0), height_(0), bpp_(0) {
    stbi_set_flip_vertically_on_load(1);    // Flipped to match the OpenGL texture coordinate system
    const auto full_path = TexturePath + file_name;
    local_buffer_ = stbi_load(full_path.c_str(), &width_, &height_, &bpp_, 4);

    GLCall(glGenTextures(1, &texture_id_));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_id_));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer_));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (local_buffer_) {
        stbi_image_free(local_buffer_);
    }
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &texture_id_));
}

void Texture::bind(const unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_id_));
}

void Texture::unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
