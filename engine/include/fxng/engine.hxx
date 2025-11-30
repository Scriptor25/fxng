#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fxng/fxng.hxx>

namespace fxng
{
    struct ApplicationConfig final
    {
        std::string Name;
        std::string Icon;
        std::string Vendor;
        uint32_t Version = 0;
    };

    struct WindowConfig final
    {
        bool Main = false;
        bool Visible = true;
        bool Decorated = true;
        bool Resizable = true;
        bool Fullscreen = false;
        uint32_t Width = 1024;
        uint32_t Height = 768;
    };

    struct EngineConfig final
    {
        ApplicationConfig Application;
        std::vector<WindowConfig> Windows;
    };

    class Engine final
    {
    public:
        explicit Engine(const EngineConfig &config);
        ~Engine();

    protected:
        void Frame();

    private:
        GLFWwindow *m_PrimaryWindow = nullptr;
        std::vector<GLFWwindow *> m_Windows;
    };
}
