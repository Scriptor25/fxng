#include <fxng/internal/glal_opengl.hxx>
#include <GL/glew.h>

fxng::glal::opengl::Fence::Fence(Device *device)
    : m_Device(device),
      m_Sync()
{
}

void fxng::glal::opengl::Fence::Wait()
{
    m_Sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    glWaitSync(m_Sync, 0, GL_TIMEOUT_IGNORED);
}

void fxng::glal::opengl::Fence::Reset()
{
    if (m_Sync)
    {
        glDeleteSync(m_Sync);
        m_Sync = nullptr;
    }
}
