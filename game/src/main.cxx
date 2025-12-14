#define GLFW_INCLUDE_NONE

#include <fstream>
#include <iostream>
#include <fxng/engine.hxx>
#include <fxng/glal.hxx>
#include <fxng/log.hxx>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void test_packaged()
{
#ifdef FXNG_PACKAGE
    std::cerr << "packaged" << std::endl;
#endif
}

struct Vertex
{
    float pos[2];
    float color[3];
};

static constexpr Vertex vertices[] = {
    { { 0.0f, -0.5f }, { 1, 0, 0 } },
    { { 0.5f, 0.5f }, { 0, 1, 0 } },
    { { -0.5f, 0.5f }, { 0, 0, 1 } },
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
    stream.read(code.data(), code.size());

    return device->CreateShaderModule(
        {
            .Stage = stage,
            .Code = code.data(),
            .Size = code.size(),
        });
}

int main()
{
    test_packaged();

    glfwInit();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    const auto native_window_handle = glfwCreateWindow(1280, 720, "Hello World", nullptr, nullptr);
    glfwMakeContextCurrent(native_window_handle);

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

    const auto swapchain = device->CreateSwapchain(
        {
            .NativeWindowHandle = native_window_handle,
            .Extent = { 1280, 720 },
            .Format = fxng::glal::ImageFormat_BGRA8_UNorm,
            .ImageCount = 1,
        });

    const auto vertex_buffer = device->CreateBuffer(
        {
            .Size = sizeof(vertices),
            .Usage = fxng::glal::BufferUsage_VertexBuffer,
            .Memory = fxng::glal::MemoryUsage_HostToDevice,
        });

    const auto mapped = vertex_buffer->Map();
    std::memcpy(mapped, vertices, sizeof(vertices));
    vertex_buffer->Unmap();

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

    const auto pipeline = device->CreatePipeline(
        {
            .Type = fxng::glal::PipelineType_Graphics,
            .Stages = pipeline_stages.data(),
            .StageCount = pipeline_stages.size(),
            .DepthTest = false,
            .DepthWrite = false,
            .BlendEnable = false,
        });

    const auto command_buffer = device->CreateCommandBuffer(fxng::glal::CommandBufferUsage_Reusable);

    const auto fence = device->CreateFence();

    while (!glfwWindowShouldClose(native_window_handle))
    {
        glfwPollEvents();

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
            0,
            0,
            static_cast<float>(swapchain->GetExtent().Width),
            static_cast<float>(swapchain->GetExtent().Height));
        command_buffer->SetScissor(
            0,
            0,
            swapchain->GetExtent().Width,
            swapchain->GetExtent().Height);

        command_buffer->BindVertexBuffer(vertex_buffer, 0);
        command_buffer->Draw(3, 0);

        command_buffer->EndRenderPass();

        command_buffer->Transition(image_view->GetImage(), fxng::glal::ResourceState_Present);

        command_buffer->End();

        graphics_queue->Submit(command_buffer, 1, fence);
        present_queue->Present(swapchain);

        fence->Wait();
        fence->Reset();
    }

    fxng::glal::DestroyInstance(instance);

    glfwDestroyWindow(native_window_handle);
    glfwTerminate();

    fxng::Engine engine(
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
        });
}
