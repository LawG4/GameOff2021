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

    // Bind to the 2D graphics pipeline
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    // Loop through each of the render objects
    for (uint32_t i = 0; i < renderObjects.size(); i++) {
        Sprite* obj = renderObjects.at(i);

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
            obj->updateUbo(swapIndex);
        }

        // Record the commands for this 2D object into the command buffer
        obj->recordCmd(cmd, swapIndex);
    }

    // End the renderpass
    vkCmdEndRenderPass(cmd);
}

void Sprite::recordCmd(VkCommandBuffer& cmd, uint32_t swapIndex)
{
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &sets[swapIndex], 0, nullptr);
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(cmd, 0, 1, &vertexGroup.buffer, &offset);
    vkCmdDraw(cmd, 3, 1, 0, 0);
}

void Sprite::updateUbo(uint32_t swapIndex)
{
    // Transform the information we have into a model view projection matrix
    glm::mat4 mvp = glm::identity<glm::mat4>();

    mvp = glm::mat4(glm::quat(rot));
    mvp = glm::scale(mvp, scale);
    mvp = glm::translate(mvp, pos);
    mvp = ProjectionMatrices::orthogonal * mvp;
    VkDeviceSize size = sizeof(glm::mat4);
    // Copy over to the ubo
    void* data;
    vkMapMemory(vk::logicalDevice, ubos[swapIndex].mem, 0, size, 0, &data);
    memcpy(data, &mvp, size);
    vkUnmapMemory(vk::logicalDevice, ubos[swapIndex].mem);

    // Set this frame as not needing another update until it changes again
    requiresUBOUpdateVector[swapIndex] = false;
}

Sprite::Sprite(const std::vector<glm::vec3>& pos, const std::vector<glm::vec3>& col)
{
    // We need to ensure the moduel is of course active
    isActive = true;

    // Assume the user has ensured that they are the right size
    std::vector<glm::vec3> vertexData(pos.size() * 2);

    // Push the inputs into the order of the vertex data
    for (uint32_t i = 0; i < pos.size(); i++) {
        vertexData[2 * i] = (pos[i]);
        vertexData[2 * i + 1] = (col[i]);
    }

    vertexGroup = vk::createVertexBufferGroup(sizeof(glm::vec3) * vertexData.size(), vertexData.data());
    indexGroup.buffer = VK_NULL_HANDLE;
    indexGroup.mem = VK_NULL_HANDLE;

    // We're absolutley going to need a ubo update when creating an object
    scheduleUBOUpdate();

    // Pobably not the most efficient, but I don't seen anything stopping me from giving each object it's own
    // descriptor pool *dabs*
    {
        VkDescriptorPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        info.maxSets = 1 * vk::swapLength;
        VkDescriptorPoolSize size{};
        size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        size.descriptorCount = 1 * vk::swapLength;
        info.pPoolSizes = &size;
        info.poolSizeCount = 1;

        vkCreateDescriptorPool(vk::logicalDevice, &info, nullptr, &pool);
    }

    // Allocate a descriptor set for the
    {
        VkDescriptorSetAllocateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        info.descriptorPool = pool;
        info.descriptorSetCount = vk::swapLength;

        std::vector<VkDescriptorSetLayout> layouts(vk::swapLength, objectSetLayout);
        info.descriptorSetCount = vk::swapLength;
        info.pSetLayouts = layouts.data();

        sets.resize(vk::swapLength);
        vkAllocateDescriptorSets(vk::logicalDevice, &info, sets.data());
    }

    ubos.resize(vk::swapLength);
    for (uint32_t i = 0; i < vk::swapLength; i++) {
        ubos[i] =
          vk::createBufferGroup(sizeof(glm::mat4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    // Associate the sets with the buffers
    {
        for (uint32_t i = 0; i < vk::swapLength; i++) {
            VkWriteDescriptorSet writer{};
            writer.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writer.descriptorCount = 1;
            writer.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writer.dstBinding = 0;
            writer.dstSet = sets[i];

            VkDescriptorBufferInfo buff{};
            buff.buffer = ubos[i].buffer;
            buff.offset = 0;
            buff.range = VK_WHOLE_SIZE;
            writer.pBufferInfo = &buff;

            vkUpdateDescriptorSets(vk::logicalDevice, 1, &writer, 0, nullptr);
        }
    }

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
