#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
#include <fxng/fxng.hxx>
#include <fxng/scene.hxx>

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

        std::string InitialScene;
    };

    struct ShaderAttribute final
    {
        std::string Name;
        std::string Type;
        std::optional<std::string> Reference;
    };

    struct ShaderIndex final
    {
        std::string Id, Name, Source;
        std::vector<ShaderAttribute> Input, Output, Uniform;
    };

    enum MaterialShaderStage
    {
        MaterialShaderStage_Vertex,
        MaterialShaderStage_TessellationControl,
        MaterialShaderStage_TessellationEvaluation,
        MaterialShaderStage_Geometry,
        MaterialShaderStage_Fragment,
    };

    struct MaterialIndex final
    {
        std::string Id, Name;
        std::unordered_map<MaterialShaderStage, std::string> Stages;
    };

    struct MeshIndex final
    {
        std::string Id, Name, Source;
    };

    struct ComponentIndex final
    {
        std::string Type;
        std::map<std::string, YAML::Node> Properties;
    };

    struct EntityIndex final
    {
        std::string Id, Name;
        std::vector<ComponentIndex> Components;
    };

    struct SceneIndex final
    {
        std::string Id, Name;
        std::vector<EntityIndex> Entities;
    };

    class Engine final
    {
    public:
        explicit Engine(const EngineConfig &config);
        ~Engine();

        Scene &GetScene();

        void InitScene();
        void ExitScene();

        void ClearScene();

    protected:
        void IndexAssets();
        void IndexYaml(std::filesystem::path path);

        void Frame();

    private:
        GLFWwindow *m_PrimaryWindow = nullptr;
        std::vector<GLFWwindow *> m_Windows;

        Scene m_Scene;
    };
}
