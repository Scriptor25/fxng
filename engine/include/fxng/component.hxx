#pragma once

#include <fxng/fxng.hxx>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace fxng
{
    class Component
    {
    public:
        explicit Component(Scene &scene, Entity &parent);
        virtual ~Component() = default;

        [[nodiscard]] Scene &GetScene() const;
        [[nodiscard]] Entity &GetParent() const;

        virtual void OnInit();
        virtual void PreFrame();
        virtual void OnFrame();
        virtual void PostFrame();
        virtual void OnExit();

    protected:
        Scene &m_Scene;
        Entity &m_Parent;
    };

    class Transform final : public Component
    {
    public:
        explicit Transform(Scene &scene, Entity &parent);

        Transform *SetTranslation(glm::vec3 translation);
        Transform *SetRotation(glm::quat rotation);
        Transform *SetScale(glm::vec3 scale);

        Transform *Translate(glm::vec3 translation);
        Transform *Rotate(float angle, glm::vec3 axis);
        Transform *Scale(glm::vec3 scale);

        Transform *LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

        void OnInit() override;
        void PreFrame() override;

    private:
        bool m_Dirty = true;

        glm::mat4 m_Matrix{ 1.f }, m_Inverse{ 1.f };
        glm::vec3 m_Translation{ 0.f };
        glm::quat m_Rotation{};
        glm::vec3 m_Scale{ 1.f };
    };

    class Camera final : public Component
    {
    public:
        explicit Camera(Scene &scene, Entity &parent);

    private:
    };

    class Model final : public Component
    {
    public:
        explicit Model(Scene &scene, Entity &parent);

    private:
    };
}
