//
// Created by GreenMuffin on 2026/1/8.
//

#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H
#include <string>


class Texture {
public:
    explicit Texture(const std::string& file_path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    [[nodiscard]] int get_width() const { return width_; }
    [[nodiscard]] int get_height() const { return height_; }

private:
    unsigned int texture_id_{0};
    std::string file_name_;
    unsigned char* local_buffer_;
    int width_, height_, bpp_;
};


#endif //LEARNOPENGL_TEXTURE_H