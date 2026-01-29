//
// Created by GreenMuffin on 2026/1/10.
//

#ifndef LEARNOPENGL_TEST_H
#define LEARNOPENGL_TEST_H


namespace tests {
    class Test {
    public:
        Test();
        virtual ~Test();

        virtual void on_start() { }
        virtual void on_update(float delta_time) { }
        virtual void on_render() { }
        virtual void on_imgui_render() { }
        virtual void on_end() { }

    private:


    };
}


#endif //LEARNOPENGL_TEST_H