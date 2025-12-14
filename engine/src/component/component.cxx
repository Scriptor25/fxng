#include <fxng/component.hxx>

fxng::Component::Component(Scene &scene, Entity &parent)
    : m_Scene(scene),
      m_Parent(parent)
{
}

fxng::Scene &fxng::Component::GetScene() const
{
    return m_Scene;
}

fxng::Entity &fxng::Component::GetParent() const
{
    return m_Parent;
}

void fxng::Component::OnInit()
{
}

void fxng::Component::PreFrame()
{
}

void fxng::Component::OnFrame()
{
}

void fxng::Component::PostFrame()
{
}

void fxng::Component::OnExit()
{
}
