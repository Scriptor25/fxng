#pragma once

#include <vector>
#include <fxng/entity.hxx>

namespace fxng
{
    class Scene final
    {
    public:
        void Clear();

        Entity &Create(std::string id);
        Entity *Get(const std::string &id);

        void OnInit() const;
        void PreFrame() const;
        void OnFrame() const;
        void PostFrame() const;
        void OnExit() const;

        [[nodiscard]] std::vector<Entity>::const_iterator begin() const;
        [[nodiscard]] std::vector<Entity>::const_iterator end() const;

    private:
        std::vector<Entity> m_Entities;
    };
}
