#include <common/log.hxx>
#include <glal/vulkan.hxx>

glal::vulkan::PipelineT::PipelineT(DeviceT *device, const PipelineDesc &desc)
    : m_Device(device),
      m_Type(desc.Type),
      m_Topology(desc.Topology),
      m_Handle()
{
    const auto layout_impl = dynamic_cast<PipelineLayoutT *>(desc.Layout);

    switch (m_Type)
    {
    case PipelineType_Graphics:
    {
        std::vector<VkPipelineShaderStageCreateInfo> pipeline_shader_stage_create_infos(desc.StageCount);
        for (std::uint32_t i = 0; i < desc.StageCount; ++i)
        {
            const auto stage = desc.Stages + i;
            const auto shader_module_impl = dynamic_cast<ShaderModuleT *>(stage->Module);

            pipeline_shader_stage_create_infos[i] = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = ToVkShaderStage(stage->Stage),
                .module = shader_module_impl->GetHandle(),
                .pName = "main",
            };
        }

        std::vector<VkVertexInputBindingDescription> vertex_input_binding_descriptions(desc.VertexBindingCount);
        for (std::uint32_t i = 0; i < desc.VertexBindingCount; ++i)
        {
            const auto binding = desc.VertexBindings + i;

            vertex_input_binding_descriptions[i] = {
                .binding = binding->Binding,
                .stride = binding->Stride,
                .inputRate = binding->Instance ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX,
            };
        }

        std::vector<VkVertexInputAttributeDescription> vertex_input_attribute_descriptions(desc.VertexAttributeCount);
        for (std::uint32_t i = 0; i < desc.VertexAttributeCount; ++i)
        {
            const auto attribute = desc.VertexAttributes + i;

            vertex_input_attribute_descriptions[i] = {
                .location = attribute->Location,
                .binding = attribute->Binding,
                .format = ToVkFormat(attribute->Type, attribute->Count),
                .offset = attribute->Offset,
            };
        }

        const VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = static_cast<std::uint32_t>(vertex_input_binding_descriptions.size()),
            .pVertexBindingDescriptions = vertex_input_binding_descriptions.data(),
            .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(vertex_input_attribute_descriptions.size()),
            .pVertexAttributeDescriptions = vertex_input_attribute_descriptions.data(),
        };

        const VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = ToVkPrimitiveTopology(desc.Topology),
            .primitiveRestartEnable = desc.PrimitiveRestartEnable,
        };

        // TODO: make customizable
        const VkPipelineTessellationStateCreateInfo pipeline_tessellation_state_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
            .patchControlPoints = 0,
        };

        // TODO: make customizable
        const VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 0,
            .pViewports = nullptr,
            .scissorCount = 0,
            .pScissors = nullptr,
        };

        // TODO: make customizable
        const VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = true,
            .rasterizerDiscardEnable = true,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable = false,
            .depthBiasConstantFactor = 0.f,
            .depthBiasClamp = 0.f,
            .depthBiasSlopeFactor = 0.f,
            .lineWidth = 1.f,
        };

        // TODO: make customizable
        const VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = false,
            .minSampleShading = 0.f,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = false,
            .alphaToOneEnable = false,
        };

        // TODO: make customizable
        const VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_state_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable = desc.DepthTest,
            .depthWriteEnable = desc.DepthWrite,
            .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
            .depthBoundsTestEnable = false,
            .stencilTestEnable = false,
            .front = {},
            .back = {},
            .minDepthBounds = 0.f,
            .maxDepthBounds = 1.f,
        };

        // TODO: make customizable
        const VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = false,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 0,
            .pAttachments = nullptr,
            .blendConstants = { 0.f, 0.f, 0.f, 0.f },
        };

        const std::array dynamic_states
        {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };

        const VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = dynamic_states.size(),
            .pDynamicStates = dynamic_states.data(),
        };

        auto render_pass_impl = dynamic_cast<RenderPassT *>(desc.Pass);

        // TODO: render pass
        const VkGraphicsPipelineCreateInfo graphics_pipeline_create_info
        {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = static_cast<std::uint32_t>(pipeline_shader_stage_create_infos.size()),
            .pStages = pipeline_shader_stage_create_infos.data(),
            .pVertexInputState = &pipeline_vertex_input_state_create_info,
            .pInputAssemblyState = &pipeline_input_assembly_state_create_info,
            .pTessellationState = &pipeline_tessellation_state_create_info,
            .pViewportState = &pipeline_viewport_state_create_info,
            .pRasterizationState = &pipeline_rasterization_state_create_info,
            .pMultisampleState = &pipeline_multisample_state_create_info,
            .pDepthStencilState = &pipeline_depth_stencil_state_create_info,
            .pColorBlendState = &pipeline_color_blend_state_create_info,
            .pDynamicState = &pipeline_dynamic_state_create_info,
            .layout = layout_impl->GetHandle(),
            .renderPass = render_pass_impl->GetHandle(),
            .subpass = 0,
        };
        vkCreateGraphicsPipelines(
            m_Device->GetHandle(),
            nullptr,
            1,
            &graphics_pipeline_create_info,
            nullptr,
            &m_Handle);
        break;
    }
    case PipelineType_Compute:
    {
        const auto shader_module_impl = dynamic_cast<ShaderModuleT *>(desc.Stages->Module);

        const VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = ToVkShaderStage(desc.Stages->Stage),
            .module = shader_module_impl->GetHandle(),
            .pName = "main",
        };

        const VkComputePipelineCreateInfo compute_pipeline_create_info
        {
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage = pipeline_shader_stage_create_info,
            .layout = layout_impl->GetHandle(),
        };
        vkCreateComputePipelines(
            m_Device->GetHandle(),
            nullptr,
            1,
            &compute_pipeline_create_info,
            nullptr,
            &m_Handle);
        break;
    }
    default:
        common::Fatal("pipeline type not supported");
        break;
    }
}

glal::vulkan::PipelineT::~PipelineT()
{
    vkDestroyPipeline(m_Device->GetHandle(), m_Handle, nullptr);
}

glal::PipelineType glal::vulkan::PipelineT::GetType() const
{
    return m_Type;
}

glal::PrimitiveTopology glal::vulkan::PipelineT::GetTopology() const
{
    return m_Topology;
}

VkPipeline glal::vulkan::PipelineT::GetHandle() const
{
    return m_Handle;
}
