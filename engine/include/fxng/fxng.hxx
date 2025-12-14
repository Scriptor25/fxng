#pragma once

#include <optional>
#include <yaml-cpp/yaml.h>

class GLFWwindow;

namespace fxng
{
    class Engine;

    class Scene;
    class Entity;
    class Component;
}

template<typename T>
concept ComponentType = __is_base_of(fxng::Component, T);

namespace YAML
{
    template<typename T>
    struct convert<std::optional<T>>
    {
        static Node encode(const std::optional<T> &value)
        {
            if (value.has_value())
                return convert<T>::encode(value.value());
            return {};
        }

        static bool decode(const Node &node, std::optional<T> &value)
        {
            if (node.IsDefined())
                value = node.as<T>();
            else
                value = std::nullopt;
            return true;
        }
    };
}
