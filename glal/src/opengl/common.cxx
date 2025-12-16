#include <common/log.hxx>
#include <glal/opengl.hxx>

glal::Instance glal::CreateInstanceOpenGL(const InstanceDesc &desc)
{
    return new opengl::InstanceT(desc);
}

void glal::opengl::TranslateImageFormat(
    const ImageFormat image_format,
    GLenum *internal_format,
    GLenum *external_format,
    GLenum *type)
{
    switch (image_format)
    {
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

void glal::opengl::TranslateDataType(
    const DataType data_type,
    std::uint32_t *size,
    GLenum *type,
    GLboolean *normalized)
{
    switch (data_type)
    {
    case DataType_None:
        common::Fatal("data type is not set");

    case DataType_UInt8:
        size && ((*size = sizeof(GLubyte)));
        type && ((*type = GL_UNSIGNED_BYTE));
        normalized && ((*normalized = GL_TRUE));
        break;
    case DataType_UInt16:
        size && ((*size = sizeof(GLushort)));
        type && ((*type = GL_UNSIGNED_SHORT));
        normalized && ((*normalized = GL_FALSE));
        break;
    case DataType_UInt32:
        size && ((*size = sizeof(GLuint)));
        type && ((*type = GL_UNSIGNED_INT));
        normalized && ((*normalized = GL_FALSE));
        break;
    case DataType_Int8:
        size && ((*size = sizeof(GLbyte)));
        type && ((*type = GL_BYTE));
        normalized && ((*normalized = GL_TRUE));
        break;
    case DataType_Int16:
        size && ((*size = sizeof(GLshort)));
        type && ((*type = GL_SHORT));
        normalized && ((*normalized = GL_FALSE));
        break;
    case DataType_Int32:
        size && ((*size = sizeof(GLint)));
        type && ((*type = GL_INT));
        normalized && ((*normalized = GL_FALSE));
        break;
    case DataType_Half:
        size && ((*size = sizeof(GLhalf)));
        type && ((*type = GL_HALF_FLOAT));
        normalized && ((*normalized = GL_FALSE));
        break;
    case DataType_Float:
        size && ((*size = sizeof(GLfloat)));
        type && ((*type = GL_FLOAT));
        normalized && ((*normalized = GL_FALSE));
        break;
    case DataType_Fixed:
        size && ((*size = sizeof(GLfixed)));
        type && ((*type = GL_FIXED));
        normalized && ((*normalized = GL_FALSE));
        break;
    case DataType_Double:
        size && ((*size = sizeof(GLdouble)));
        type && ((*type = GL_DOUBLE));
        normalized && ((*normalized = GL_FALSE));
        break;
    }
}
