#define GLFW_INCLUDE_NONE

#include <fstream>
#include <iostream>
#include <fxng/engine.hxx>
#include <fxng/glal.hxx>
#include <fxng/log.hxx>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

static void test_packaged()
{
#ifdef FXNG_PACKAGE
    std::cerr << "packaged" << std::endl;
#endif
}

struct Vertex
{
    float position[2];
    float color[3];
};

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

static constexpr Vertex vertices[] = {
    { { 0.0f, 0.57735027f }, { 1, 0, 0 } },
    { { -0.5f, -0.28867513f }, { 0, 1, 0 } },
    { { 0.5f, -0.28867513f }, { 0, 0, 1 } },
};

static fxng::glal::ShaderModule *load_shader_module(
    fxng::glal::Device *device,
    fxng::glal::ShaderStage stage,
    std::filesystem::path path)
{
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    fxng::Assert(stream.is_open(), "failed to open file {}", path);

    std::vector<char> code(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(code.data(), static_cast<long>(code.size()));

    return device->CreateShaderModule(
        {
            .Stage = stage,
            .Code = code.data(),
            .Size = code.size(),
        });
}

struct
{
    fxng::glal::Device *device;
    fxng::glal::Swapchain *swapchain;
} static application_state = {
    .device = nullptr,
    .swapchain = nullptr,
};

static void glfw_framebuffer_size_callback(GLFWwindow *window, const int width, const int height)
{
    application_state.device->DestroySwapchain(application_state.swapchain);
    application_state.swapchain = application_state.device->CreateSwapchain(
        {
            .NativeWindowHandle = window,
            .Extent = { width, height },
            .Format = fxng::glal::ImageFormat_BGRA8_UNorm,
            .ImageCount = 2,
        });
}

int main()
{
    test_packaged();

    glfwInit();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    const auto window = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    const auto instance = fxng::glal::CreateInstanceOpenGL(
        {
            .EnableValidation = true,
            .ApplicationName = "Hello World",
        });

    const auto physical_device_count = instance->EnumeratePhysicalDevices(nullptr);

    std::vector<fxng::glal::PhysicalDevice *> physical_devices(physical_device_count);
    instance->EnumeratePhysicalDevices(physical_devices.data());

    const auto physical_device = physical_devices.front();
    const auto device = physical_device->CreateDevice();

    const auto graphics_queue = device->GetQueue(fxng::glal::QueueType_Graphics);
    const auto present_queue = device->GetQueue(fxng::glal::QueueType_Present);

    application_state.device = device;
    application_state.swapchain = device->CreateSwapchain(
        {
            .NativeWindowHandle = window,
            .Extent = { width, height },
            .Format = fxng::glal::ImageFormat_BGRA8_UNorm,
            .ImageCount = 2,
        });

    const auto vertex_buffer = device->CreateBuffer(
        {
            .Size = sizeof(vertices),
            .Usage = fxng::glal::BufferUsage_VertexBuffer,
            .Memory = fxng::glal::MemoryUsage_HostToDevice,
        });

    {
        const auto mapped = vertex_buffer->Map();
        std::memcpy(mapped, vertices, sizeof(vertices));
        vertex_buffer->Unmap();
    }

    const auto vertex_shader = load_shader_module(device, fxng::glal::ShaderStage_Vertex, "vertex.spv");
    const auto fragment_shader = load_shader_module(device, fxng::glal::ShaderStage_Fragment, "fragment.spv");

    const std::array pipeline_stages
    {
        fxng::glal::PipelineStageDesc
        {
            .Stage = fxng::glal::ShaderStage_Vertex,
            .Module = vertex_shader,
        },
        fxng::glal::PipelineStageDesc
        {
            .Stage = fxng::glal::ShaderStage_Fragment,
            .Module = fragment_shader,
        },
    };

    constexpr std::array vertex_attributes
    {
        fxng::glal::VertexAttributeDesc
        {
            .Binding = 0,
            .Location = 0,
            .Type = fxng::glal::DataType_Float,
            .Count = 2,
            .Offset = offsetof(Vertex, position),
        },
        fxng::glal::VertexAttributeDesc
        {
            .Binding = 0,
            .Location = 1,
            .Type = fxng::glal::DataType_Float,
            .Count = 3,
            .Offset = offsetof(Vertex, color),
        },
    };

    const auto pipeline = device->CreatePipeline(
        {
            .Type = fxng::glal::PipelineType_Graphics,
            .Stages = pipeline_stages.data(),
            .StageCount = pipeline_stages.size(),
            .VertexAttributes = vertex_attributes.data(),
            .VertexAttributeCount = vertex_attributes.size(),
            .DepthTest = false,
            .DepthWrite = false,
            .BlendEnable = false,
        });

    const auto uniform_buffer = device->CreateBuffer(
        {
            .Size = sizeof(UniformBufferObject),
            .Usage = fxng::glal::BufferUsage_StorageBuffer,
            .Memory = fxng::glal::MemoryUsage_HostToDevice,
        });

    const auto command_buffer = device->CreateCommandBuffer(fxng::glal::CommandBufferUsage_Reusable);

    const auto fence = device->CreateFence();

    const auto start_time = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        const auto current_time = std::chrono::high_resolution_clock::now();
        const auto time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

        const auto swapchain = application_state.swapchain;

        const UniformBufferObject uniform_buffer_object
        {
            .model = glm::rotate(glm::mat4(1.f), time * glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f)),
            .view = glm::lookAt(glm::vec3(0.f, 0.f, -2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
            .proj = glm::perspective(
                glm::radians(45.0f),
                static_cast<float>(swapchain->GetExtent().Width) / static_cast<float>(swapchain->GetExtent().Height),
                0.1f,
                10.f),
        };

        {
            const auto mapped = uniform_buffer->Map();
            std::memcpy(mapped, &uniform_buffer_object, sizeof(uniform_buffer_object));
            uniform_buffer->Unmap();
        }

        const auto image_index = swapchain->AcquireNextImage(fence);
        const auto image_view = swapchain->GetImageView(image_index);

        const fxng::glal::RenderTargetDesc color_target
        {
            .View = image_view,
            .Clear = true,
            .Value = { .Color = { 0.05f, 0.05f, 0.10f, 1.00f } },
        };

        const fxng::glal::RenderPassDesc render_pass
        {
            .Color = &color_target,
            .ColorCount = 1,
            .Depth = nullptr,
            .DepthCount = 0,
        };

        command_buffer->Begin();

        command_buffer->Transition(image_view->GetImage(), fxng::glal::ResourceState_RenderTarget);

        command_buffer->BeginRenderPass(render_pass);

        command_buffer->SetPipeline(pipeline);
        command_buffer->SetViewport(
            0.0f,
            0.0f,
            static_cast<float>(swapchain->GetExtent().Width),
            static_cast<float>(swapchain->GetExtent().Height));
        command_buffer->SetScissor(
            0,
            0,
            swapchain->GetExtent().Width,
            swapchain->GetExtent().Height);

        command_buffer->BindBuffer(0, fxng::glal::ShaderStage_Vertex, uniform_buffer);

        command_buffer->BindVertexBuffer(vertex_buffer, 0);
        command_buffer->Draw(3, 0);

        command_buffer->EndRenderPass();

        command_buffer->Transition(image_view->GetImage(), fxng::glal::ResourceState_Present);

        command_buffer->End();

        graphics_queue->Submit(command_buffer, 1, fence);
        present_queue->Present(swapchain);
    }

    fxng::glal::DestroyInstance(instance);

    glfwDestroyWindow(window);
    glfwTerminate();

    /*fxng::Engine engine(
        {
            .Application = {
                .Name = "Game",
                .Vendor = "Default",
                .Version = 0,
            },
            .Windows = {
                { .Main = true },
            },
            .InitialScene = "entry",
        });*/
}
