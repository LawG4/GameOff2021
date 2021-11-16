#include "Pipelines.h"
#include "Sprites.h"

std::vector<Sprite*> renderObjects;

VkDescriptorPool descpool;
VkDescriptorSetLayout cameraSetLayout;
VkDescriptorSetLayout objectSetLayout;

VkShaderModule vertModule;
VkShaderModule fragModule;

VkPipelineLayout layout;
VkPipeline pipeline;

void PipelineInternals::create2DPipeline()
{
    Log.info("Creating 2D graphics Pipeline");

    // Create the graphics pipeline layout
    {
        VkPipelineLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        // set 0 is layout per model transfor
        info.pSetLayouts = &cameraSetLayout;
        info.setLayoutCount = 1;

        vkCreatePipelineLayout(vk::logicalDevice, &info, nullptr, &layout);
    }

    // Create the graphics pipeline using the internal templates, they were set for the 2d
    {
        VkGraphicsPipelineCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        info.layout = layout;

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
        info.pColorBlendState = &PipelineInternals::Templates::blend;

        vkCreateGraphicsPipelines(vk::logicalDevice, nullptr, 1, &info, nullptr, &pipeline);
    }
}

void PipelineInternals::destroy2DPipeline()
{
    if (pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(vk::logicalDevice, pipeline, nullptr);
    }

    if (layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(vk::logicalDevice, layout, nullptr);
    }

    if (vertModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(vk::logicalDevice, vertModule, nullptr);
    }
    if (fragModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(vk::logicalDevice, fragModule, nullptr);
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
        binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        binding.descriptorCount = 1;

        info.bindingCount = 1;
        info.pBindings = &binding;

        vkCreateDescriptorSetLayout(vk::logicalDevice, &info, nullptr, &objectSetLayout);

        // Camera buffer has the same layout
        vkCreateDescriptorSetLayout(vk::logicalDevice, &info, nullptr, &cameraSetLayout);
    }
}

void PipelineInternals::destroyDescriptorSetLayouts2D()
{
    vkDestroyDescriptorSetLayout(vk::logicalDevice, cameraSetLayout, nullptr);
    vkDestroyDescriptorSetLayout(vk::logicalDevice, objectSetLayout, nullptr);
    vkDestroyDescriptorPool(vk::logicalDevice, descpool, nullptr);

    for (Sprite* obj : renderObjects) {
        delete (obj);
    }
}
