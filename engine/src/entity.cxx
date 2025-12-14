#include <fxng/component.hxx>
#include <fxng/entity.hxx>

fxng::Entity::Entity(Scene &scene, Entity *parent, std::string id)
    : m_Scene(scene),
      m_Parent(parent),
      m_Id(std::move(id))
{
}

fxng::Scene &fxng::Entity::GetScene() const
{
    return m_Scene;
}

fxng::Entity *fxng::Entity::GetParent() const
{
    return m_Parent;
}

const std::string &fxng::Entity::GetId() const
{
    return m_Id;
}

void fxng::Entity::Insert(std::unique_ptr<Component> component)
{
    m_Components.emplace_back(std::move(component));
}

void fxng::Entity::OnInit() const
{
    for (auto &component : m_Components)
        component->OnInit();
}

void fxng::Entity::PreFrame() const
{
    for (auto &component : m_Components)
        component->PreFrame();
}

void fxng::Entity::OnFrame() const
{
    for (auto &component : m_Components)
        component->OnFrame();
}

void fxng::Entity::PostFrame() const
{
    for (auto &component : m_Components)
        component->PostFrame();
}

void fxng::Entity::OnExit() const
{
    for (auto &component : m_Components)
        component->OnExit();
}

std::vector<std::unique_ptr<fxng::Component>>::const_iterator fxng::Entity::begin() const
{
    return m_Components.begin();
}

std::vector<std::unique_ptr<fxng::Component>>::const_iterator fxng::Entity::end() const
{
    return m_Components.end();
}
