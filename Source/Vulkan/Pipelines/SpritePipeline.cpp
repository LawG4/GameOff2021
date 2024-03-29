/*!********************************************************************************************************
\File          : SpritesPipeline.cpp
\Copyright     : GPL-3.0 License
\Brief         : The graphics pipeline utility for sprite rendering
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Pipelines.h"
#include "Sprites.h"

std::vector<SpriteSheet*> SpriteInternals::activeSheets;

VkDescriptorPool descpool;
VkDescriptorSetLayout SpriteInternals::textureSetLayout;

VkShaderModule vertModule;
VkShaderModule fragModule;

VkPipelineLayout SpriteInternals::layout;
VkPipeline SpriteInternals::pipeline;

void PipelineInternals::create2DPipeline()
{
    Log.info("Creating 2D graphics Pipeline");

    // Create the graphics pipeline layout
    {
        VkPipelineLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        // Tell the pipeline layout to expect 1 push constant in the vertex stage
        VkPushConstantRange range{};
        range.offset = 0;
        range.size = sizeof(glm::mat4);
        range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        info.pPushConstantRanges = &range;
        info.pushConstantRangeCount = 1;

        // Tell the pipeline layout to expect 1 sampler in the fragment stage
        info.pSetLayouts = &SpriteInternals::textureSetLayout;
        info.setLayoutCount = 1;

        vkCreatePipelineLayout(vk::logicalDevice, &info, nullptr, &SpriteInternals::layout);
    }

    // Create the graphics pipeline using the internal templates, they were set for the 2d
    {
        VkGraphicsPipelineCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        info.layout = SpriteInternals::layout;

        // attach the shaders
        vertModule = vk::createShaderModule("Shaders/TestShader.vert.spv");
        fragModule = vk::createShaderModule("Shaders/TestShader.frag.spv");

        VkPipelineShaderStageCreateInfo shader{};
        shader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader.stage = VK_SHADER_STAGE_VERTEX_BIT;
        shader.module = vertModule;
        shader.pName = "main";

        std::vector<VkPipelineShaderStageCreateInfo> stages(2, shader);
        stages[1].module = fragModule;
        stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        info.pStages = stages.data();
        info.stageCount = stages.size();

        // 2D objects use the onscreen renderpass
        info.renderPass = vk::onscreenRenderPass;

        // Attach the fixed function state
        info.pVertexInputState = &PipelineInternals::Templates::vertexInput;
        info.pInputAssemblyState = &PipelineInternals::Templates::inputAssembly;
        info.pViewportState = &PipelineInternals::Templates::viewportState;
        info.pRasterizationState = &PipelineInternals::Templates::raster;
        info.pMultisampleState = &PipelineInternals::Templates::multisample;

        // Change colour blend state to allow alpha blending
        VkPipelineColorBlendStateCreateInfo blend = PipelineInternals::Templates::blend;
        VkPipelineColorBlendAttachmentState blendAttachment{};
        blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                         VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        blendAttachment.blendEnable = VK_TRUE;
        blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        blendAttachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;
        blend.pAttachments = &blendAttachment;
        info.pColorBlendState = &blend;

        VkPipelineDepthStencilStateCreateInfo depthState{};
        depthState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthState.stencilTestEnable = VK_FALSE;
        depthState.depthTestEnable = VK_TRUE;
        depthState.depthWriteEnable = VK_TRUE;
        depthState.depthCompareOp = VK_COMPARE_OP_LESS;

        info.pDepthStencilState = &depthState;

        vkCreateGraphicsPipelines(vk::logicalDevice, nullptr, 1, &info, nullptr, &SpriteInternals::pipeline);
    }
}

void PipelineInternals::destroy2DPipeline(bool destroyAllResources)
{
    if (destroyAllResources) {
        // Destroy the index buffer used for 2D buffers
        if (SpriteInternals::quadIndexGroup.buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(vk::logicalDevice, SpriteInternals::quadIndexGroup.buffer, nullptr);
        }
        if (SpriteInternals::quadIndexGroup.mem != VK_NULL_HANDLE) {
            vkFreeMemory(vk::logicalDevice, SpriteInternals::quadIndexGroup.mem, nullptr);
        }
    }
    if (SpriteInternals::pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(vk::logicalDevice, SpriteInternals::pipeline, nullptr);
    }

    if (SpriteInternals::layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(vk::logicalDevice, SpriteInternals::layout, nullptr);
    }

    if (vertModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(vk::logicalDevice, vertModule, nullptr);
    }
    if (fragModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(vk::logicalDevice, fragModule, nullptr);
    }
}

void SpriteInternals::recordSpritePipeline(VkCommandBuffer& cmd, const uint32_t index)
{
    // Don't render the sprite sheets if there are none active
    if (SpriteInternals::activeSheets.size() == 0) {
        return;
    }

    // Bind to the pipeline
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, SpriteInternals::pipeline);

    // Bind to the index buffer since they're all the same
    vkCmdBindIndexBuffer(cmd, SpriteInternals::quadIndexGroup.buffer, 0, VK_INDEX_TYPE_UINT16);

    // Go through each active sprite sheet
    for (SpriteSheet* sheet : SpriteInternals::activeSheets) {
        sheet->appendCommands(cmd, SpriteInternals::layout);
    }
}

void PipelineInternals::createDescriptorSetLayouts2D()
{
    // Create the pool
    {
        VkDescriptorPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        info.maxSets = 1 * vk::swapLength;

        VkDescriptorPoolSize size{};
        size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        size.descriptorCount = 1 * vk::swapLength;
        info.pPoolSizes = &size;
        info.poolSizeCount = 1;

        vkCreateDescriptorPool(vk::logicalDevice, &info, nullptr, &descpool);
    }

    // create the layout for the per buffer bad boi
    {
        VkDescriptorSetLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

        VkDescriptorSetLayoutBinding binding{};
        binding.binding = 0;
        binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        binding.descriptorCount = 1;

        info.bindingCount = 1;
        info.pBindings = &binding;

        vkCreateDescriptorSetLayout(vk::logicalDevice, &info, nullptr, &SpriteInternals::textureSetLayout);
    }
}

void PipelineInternals::destroyDescriptorSetLayouts2D()
{
    vkDestroyDescriptorSetLayout(vk::logicalDevice, SpriteInternals::textureSetLayout, nullptr);
    vkDestroyDescriptorPool(vk::logicalDevice, descpool, nullptr);
}
