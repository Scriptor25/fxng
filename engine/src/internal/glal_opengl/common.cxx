#include <fxng/internal/glal_opengl.hxx>

fxng::glal::Instance *fxng::glal::CreateInstanceOpenGL(const InstanceDesc &desc)
{
    return new opengl::Instance(desc);
}

void fxng::glal::opengl::TranslateFormat(
    const ImageFormat format,
    std::uint32_t *internal_format,
    std::uint32_t *external_format,
    std::uint32_t *type)
{
    switch (format)
    {
    case ImageFormat_None:
        internal_format && ((*internal_format = 0));
        external_format && ((*external_format = 0));
        type && ((*type = 0));
        break;
    case ImageFormat_RGBA8_UNorm:
        internal_format && ((*internal_format = GL_RGBA8));
        external_format && ((*external_format = GL_RGBA));
        type && ((*type = GL_UNSIGNED_BYTE));
        break;
    case ImageFormat_RGBA8_SRGB:
        internal_format && ((*internal_format = GL_SRGB8_ALPHA8));
        external_format && ((*external_format = GL_RGBA));
        type && ((*type = GL_UNSIGNED_BYTE));
        break;
    case ImageFormat_BGRA8_UNorm:
        internal_format && ((*internal_format = GL_RGBA8));
        external_format && ((*external_format = GL_BGRA));
        type && ((*type = GL_UNSIGNED_BYTE));
        break;
    case ImageFormat_RG16F:
        internal_format && ((*internal_format = GL_RG16F));
        external_format && ((*external_format = GL_RG));
        type && ((*type = GL_FLOAT));
        break;
    case ImageFormat_RGBA16F:
        internal_format && ((*internal_format = GL_RGBA16F));
        external_format && ((*external_format = GL_RGBA));
        type && ((*type = GL_FLOAT));
        break;
    case ImageFormat_RGBA32F:
        internal_format && ((*internal_format = GL_RGBA32F));
        external_format && ((*external_format = GL_RGBA));
        type && ((*type = GL_FLOAT));
        break;
    case ImageFormat_D24S8:
        internal_format && ((*internal_format = GL_DEPTH24_STENCIL8));
        external_format && ((*external_format = GL_DEPTH_STENCIL));
        type && ((*type = GL_UNSIGNED_INT_24_8));
        break;
    case ImageFormat_D32F:
        internal_format && ((*internal_format = GL_DEPTH_COMPONENT32F));
        external_format && ((*external_format = GL_DEPTH_COMPONENT));
        type && ((*type = GL_FLOAT));
        break;
    }
}
