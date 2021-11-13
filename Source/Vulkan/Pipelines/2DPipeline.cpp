#include "Objects.h"
#include "Pipelines.h"

std::vector<RenderObject2D*> renderObjects;

VkDescriptorPool descpool;
VkDescriptorSetLayout setLayout;

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
        info.pSetLayouts = &setLayout;
        info.setLayoutCount = 1;

        vkCreatePipelineLayout(vk::logialDevice, &info, nullptr, &layout);
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

        vkCreateGraphicsPipelines(vk::logialDevice, nullptr, 1, &info, nullptr, &pipeline);
    }
}

void PipelineInternals::destroy2DPipeline()
{
    if (pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(vk::logialDevice, pipeline, nullptr);
    }

    if (layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(vk::logialDevice, layout, nullptr);
    }

    if (vertModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(vk::logialDevice, vertModule, nullptr);
    }
    if (fragModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(vk::logialDevice, fragModule, nullptr);
    }
}

void PipelineInternals::prepare2DCmdBuffer(VkCommandBuffer& cmd, uint32_t swapIndex)
{
    VkClearValue clear = {0.0094117f, 0.00078431f, 0.00392156f, 1.0f};
    VkRenderPassBeginInfo renderpass;
    memset(&renderpass, 0, sizeof(VkRenderPassBeginInfo));
    renderpass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderpass.clearValueCount = 1;
    renderpass.pClearValues = &clear;

    // Attach to onscreen renderpass
    renderpass.renderPass = vk::onscreenRenderPass;
    renderpass.renderArea.extent = vk::swapchainExtent;
    renderpass.renderArea.offset = {0, 0};

    // Begin the renderpass
    renderpass.framebuffer = vk::swapchainFb.at(swapIndex);
    vkCmdBeginRenderPass(cmd, &renderpass, VK_SUBPASS_CONTENTS_INLINE);

    // Loop through each of the render objects
    for (uint32_t i = 0; i < renderObjects.size(); i++) {
        RenderObject2D* obj = renderObjects.at(i);

        // If the pointer is null lets remove it from the list
        if (!obj) {
            renderObjects.erase(renderObjects.begin() + i);
            i--;
            continue;
        }

        // Now check if the object is active, if not skip it
        if (!obj->isActive) {
            continue;
        }

        // Perform a ubo update
        if (obj->requiresUBOUpdate(swapIndex)) {
            obj->recordCmd(cmd, swapIndex);
        }

        // Record the commands for this 2D object into the command buffer
        obj->recordCmd(cmd, swapIndex);
    }

    // End the renderpass
    vkCmdEndRenderPass(cmd);
}

void RenderObject2D::recordCmd(VkCommandBuffer& cmd, uint32_t swapIndex) {}

void RenderObject2D::updateUbo(uint32_t swapIndex) {}

RenderObject2D::RenderObject2D(const std::vector<glm::vec3>& pos, const std::vector<glm::vec3>& col)
{
    // We need to ensure the moduel is of course active
    isActive = true;

    // Assume the user has ensured that they are the right size
    std::vector<glm::vec3> vertexData(pos.size() * 2);

    // Push the inputs into the order of the vertex data
    for (uint32_t i = 0; i < pos.size(); i++) {
        vertexData.push_back(pos[i]);
        vertexData.push_back(col[i]);
    }

    vertexGroup = vk::createVertexBufferGroup(vertexData.size(), vertexData.data());
    indexGroup.buffer = VK_NULL_HANDLE;
    indexGroup.mem = VK_NULL_HANDLE;

    // Add this object to the list of things to render
    renderObjects.push_back(this);
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

        vkCreateDescriptorPool(vk::logialDevice, &info, nullptr, &descpool);
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

        vkCreateDescriptorSetLayout(vk::logialDevice, &info, nullptr, &setLayout);
    }
}

void PipelineInternals::destroyDescriptorSetLayouts2D()
{
    vkDestroyDescriptorSetLayout(vk::logialDevice, setLayout, nullptr);
    vkDestroyDescriptorPool(vk::logialDevice, descpool, nullptr);
}
