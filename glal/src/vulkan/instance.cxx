#include <cstring>
#include <common/log.hxx>
#include <glal/vulkan.hxx>
#include <GLFW/glfw3.h>

static constexpr std::array layers
{
    "VK_LAYER_KHRONOS_validation",
};

static constexpr std::array extensions
{
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
};

static void check_layers_present()
{
    uint32_t property_count;
    vkEnumerateInstanceLayerProperties(&property_count, nullptr);

    std::vector<VkLayerProperties> properties(property_count);
    vkEnumerateInstanceLayerProperties(&property_count, properties.data());

    for (const auto layer : layers)
    {
        auto found = false;
        for (const auto &property : properties)
            if (std::strcmp(layer, property.layerName) == 0)
            {
                found = true;
                break;
            }

        common::Assert(found, "layer {} not present", layer);
    }
}

static void noop()
{
    // do nothing
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_message_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data)
{
    common::LogLevel log_level;
    switch (message_severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        log_level = common::LogLevel_Verbose;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        log_level = common::LogLevel_Info;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        log_level = common::LogLevel_Warning;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        log_level = common::LogLevel_Error;
        break;
    default:
        log_level = common::LogLevel_Debug;
        break;
    }

    const char *message_type_string;
    switch (message_type)
    {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        message_type_string = "General";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        message_type_string = "Performance";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        message_type_string = "Validation";
        break;
    default:
        message_type_string = "Other";
        break;
    }

    common::Log(log_level, "[{}] {}", message_type_string, callback_data->pMessage);

    if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        noop();

    return VK_FALSE;
}

static VkResult vkXCreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pMessenger)
{
    if (const auto pfn = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")); pfn)
        return pfn(instance, pCreateInfo, pAllocator, pMessenger);
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

static void vkXDestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks *pAllocator)
{
    if (const auto pfn = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")); pfn)
        pfn(instance, messenger, pAllocator);
}

glal::vulkan::InstanceT::InstanceT(const InstanceDesc &desc)
    : m_Handle()
{
    check_layers_present();

    const VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info
    {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                       | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
                       | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
        .pfnUserCallback = debug_message_callback,
        .pUserData = nullptr,
    };

    std::uint32_t required_extension_count;
    const auto required_extensions = glfwGetRequiredInstanceExtensions(&required_extension_count);

    std::vector<const char *> enabled_extensions;
    enabled_extensions.insert(
        enabled_extensions.end(),
        required_extensions,
        required_extensions + required_extension_count);
    enabled_extensions.insert(
        enabled_extensions.end(),
        extensions.begin(),
        extensions.end());

    const VkApplicationInfo application_info
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = desc.ApplicationName,
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "GLAL",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_1,
    };

    const VkInstanceCreateInfo instance_create_info
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = &debug_utils_messenger_create_info,
        .pApplicationInfo = &application_info,
        .enabledLayerCount = layers.size(),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = static_cast<std::uint32_t>(enabled_extensions.size()),
        .ppEnabledExtensionNames = enabled_extensions.data(),
    };

    const auto result = vkCreateInstance(&instance_create_info, nullptr, &m_Handle);
    common::Assert(result == VK_SUCCESS, "failed to create vulkan instance");

    vkXCreateDebugUtilsMessengerEXT(m_Handle, &debug_utils_messenger_create_info, nullptr, &m_DebugUtilsMessenger);

    std::uint32_t count;
    vkEnumeratePhysicalDevices(m_Handle, &count, nullptr);

    std::vector<VkPhysicalDevice> physical_devices(count);
    vkEnumeratePhysicalDevices(m_Handle, &count, physical_devices.data());

    for (const auto physical_device : physical_devices)
        m_PhysicalDevices.emplace_back(this, physical_device);
}

glal::vulkan::InstanceT::~InstanceT()
{
    vkXDestroyDebugUtilsMessengerEXT(m_Handle, m_DebugUtilsMessenger, nullptr);
    vkDestroyInstance(m_Handle, nullptr);
}

std::uint32_t glal::vulkan::InstanceT::EnumeratePhysicalDevices(PhysicalDevice *devices)
{
    if (devices)
        *devices = m_PhysicalDevices.data();
    return m_PhysicalDevices.size();
}

VkInstance glal::vulkan::InstanceT::GetHandle() const
{
    return m_Handle;
}
