//
// Created by GreenMuffin on 2026/1/10.
//

#ifndef LEARNOPENGL_TEST_MENU_H
#define LEARNOPENGL_TEST_MENU_H
#include <functional>
#include <iostream>
#include <utility>
#include <vector>

#include "test.h"


namespace tests {
    class TestMenu : public Test{
    public:
        TestMenu(Test*& current_test);
        ~TestMenu() override;

        void on_imgui_render() override;

        template <typename T>
        void register_test(const std::string& name) {
            std::cout << "Registering test \"" << name << "\"" << std::endl;
            tests_.push_back({name, []() { return new T(); }});
        }

    private:
        Test* & current_test_;
        std::vector<std::pair<std::string, std::function<Test*()>>> tests_;
    };
}


#endif //LEARNOPENGL_TEST_MENU_H