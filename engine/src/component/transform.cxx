#include <fxng/component.hxx>
#include <fxng/entity.hxx>

fxng::Transform::Transform(Scene &scene, Entity &parent)
    : Component(scene, parent)
{
}

fxng::Transform *fxng::Transform::SetTranslation(const glm::vec3 translation)
{
    m_Translation = translation;
    m_Dirty = true;
    return this;
}

fxng::Transform *fxng::Transform::SetRotation(const glm::quat rotation)
{
    m_Rotation = rotation;
    m_Dirty = true;
    return this;
}

fxng::Transform *fxng::Transform::SetScale(const glm::vec3 scale)
{
    m_Scale = scale;
    m_Dirty = true;
    return this;
}

fxng::Transform *fxng::Transform::Translate(const glm::vec3 translation)
{
    m_Translation += translation;
    m_Dirty = true;
    return this;
}

fxng::Transform *fxng::Transform::Rotate(const float angle, const glm::vec3 axis)
{
    m_Rotation = rotate(m_Rotation, angle, axis);
    m_Dirty = true;
    return this;
}

fxng::Transform *fxng::Transform::Scale(const glm::vec3 scale)
{
    m_Scale *= scale;
    m_Dirty = true;
    return this;
}

fxng::Transform *fxng::Transform::LookAt(const glm::vec3 eye, const glm::vec3 center, const glm::vec3 up)
{
    const auto direction = normalize(center - eye);
    m_Rotation = glm::quatLookAt(direction, up);
    m_Translation = eye;
    m_Dirty = true;
    return this;
}

void fxng::Transform::OnInit()
{
    m_Dirty = true;
}

void fxng::Transform::PreFrame()
{
    if (!m_Dirty)
        return;

    m_Dirty = false;
    m_Matrix = glm::translate({ 1.f }, m_Translation)
               * glm::mat4_cast(m_Rotation)
               * glm::scale({ 1.f }, m_Scale);
    m_Inverse = glm::scale({ 1.f }, 1.f / m_Scale)
                * glm::mat4_cast(glm::conjugate(m_Rotation))
                * glm::translate({ 1.f }, -m_Translation);

    const auto parent = m_Parent.GetParent();
    if (!parent)
        return;

    parent->PreFrame();

    const auto transform = parent->Get<Transform>();
    m_Matrix = transform->m_Matrix * m_Matrix;
    m_Inverse = m_Inverse * transform->m_Inverse;
}
