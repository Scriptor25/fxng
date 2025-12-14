#pragma once

#include <memory>
#include <vector>
#include <fxng/component.hxx>
#include <fxng/fxng.hxx>

namespace fxng
{
    class Entity final
    {
    public:
        explicit Entity(Scene &scene, Entity *parent, std::string id);

        [[nodiscard]] Scene &GetScene() const;
        [[nodiscard]] Entity *GetParent() const;
        [[nodiscard]] const std::string &GetId() const;

        void Insert(std::unique_ptr<Component> component);

        template<ComponentType C, typename... A>
        C &Create(A &&... a)
        {
            return *m_Components.emplace_back(std::make_unique<C>(m_Scene, *this, std::forward<A>(a)...));
        }

        template<ComponentType C>
        C *Get(const unsigned index = 0u)
        {
            auto x = 0u;
            for (auto &component : m_Components)
                if (auto p = dynamic_cast<C *>(component.get()))
                    if (x++ >= index)
                        return p;
            return nullptr;
        }

        void OnInit() const;
        void PreFrame() const;
        void OnFrame() const;
        void PostFrame() const;
        void OnExit() const;

        [[nodiscard]] std::vector<std::unique_ptr<Component>>::const_iterator begin() const;
        [[nodiscard]] std::vector<std::unique_ptr<Component>>::const_iterator end() const;

    private:
        Scene &m_Scene;
        Entity *m_Parent;
        std::string m_Id;

        std::vector<std::unique_ptr<Component>> m_Components;
    };
}
