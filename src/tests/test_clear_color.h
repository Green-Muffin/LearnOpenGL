//
// Created by GreenMuffin on 2026/1/10.
//

#ifndef LEARNOPENGL_TEST_CLEAR_COLOR_H
#define LEARNOPENGL_TEST_CLEAR_COLOR_H
#include "test.h"


namespace tests {
    class TestClearColor : public Test{
    public:
        TestClearColor();
        ~TestClearColor() override;

        void on_update(float delta_time) override;
        void on_render() override;
        void on_imgui_render() override;

    private:

        float clear_color_[4];
    };
}


#endif //LEARNOPENGL_TEST_CLEAR_COLOR_H