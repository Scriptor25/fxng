#include <fxng/scene.hxx>

void fxng::Scene::Clear()
{
    m_Entities.clear();
}

fxng::Entity &fxng::Scene::Create(std::string id)
{
    return m_Entities.emplace_back(*this, nullptr, std::move(id));
}

fxng::Entity *fxng::Scene::Get(const std::string &id)
{
    for (auto &entity : m_Entities)
        if (entity.GetId() == id)
            return &entity;
    return nullptr;
}

void fxng::Scene::OnInit() const
{
    for (auto &entity : m_Entities)
        entity.OnInit();
}

void fxng::Scene::PreFrame() const
{
    for (auto &entity : m_Entities)
        entity.PreFrame();
}

void fxng::Scene::OnFrame() const
{
    for (auto &entity : m_Entities)
        entity.OnFrame();
}

void fxng::Scene::PostFrame() const
{
    for (auto &entity : m_Entities)
        entity.PostFrame();
}

void fxng::Scene::OnExit() const
{
    for (auto &entity : m_Entities)
        entity.OnExit();
}

std::vector<fxng::Entity>::const_iterator fxng::Scene::begin() const
{
    return m_Entities.begin();
}

std::vector<fxng::Entity>::const_iterator fxng::Scene::end() const
{
    return m_Entities.end();
}
