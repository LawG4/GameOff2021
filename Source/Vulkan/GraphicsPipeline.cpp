/*!********************************************************************************************************
\File          : GraphicsPipeline.cpp
\Copyright     : MIT License
\Brief         : Handles creating the graphics pipeline
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vertex.h"
#include "Vulkan.h"
VkPipeline vk::graphicsPipeline;
VkPipelineLayout vk::graphicsLayout;

/// <summary> Creates all the state variables for fixed function processing </summary>
void createFixedFunctionStateVariables();

VkPipelineVertexInputStateCreateInfo vertexInput;
VkVertexInputBindingDescription bindingDesc;
std::vector<VkVertexInputAttributeDescription> attributes;

VkPipelineInputAssemblyStateCreateInfo inputAssembly;

VkPipelineViewportStateCreateInfo viewportState;
VkViewport viewport;
VkRect2D scissor;

VkPipelineRasterizationStateCreateInfo raster;
VkPipelineMultisampleStateCreateInfo multisample;

VkPipelineColorBlendStateCreateInfo blend;
VkPipelineColorBlendAttachmentState blendAttachment;

bool vk::createGraphicsPipeline()
{
    // Start by creating the graphics pipeline layout
    VkPipelineLayoutCreateInfo layout;
    memset(&layout, 0, sizeof(VkPipelineLayoutCreateInfo));
    layout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    if (vkCreatePipelineLayout(vk::logialDevice, &layout, nullptr, &vk::graphicsLayout) != VK_SUCCESS) {
        Log.error("Could not create graphics pipeline layout");
        return false;
    }

    // Start creating the graphics pipeline
    VkGraphicsPipelineCreateInfo pipeline;
    memset(&pipeline, 0, sizeof(VkGraphicsPipelineCreateInfo));
    pipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    // Attach the pipeline layout
    pipeline.layout = vk::graphicsLayout;

    // Attach the renderpass
    pipeline.renderPass = vk::onscreenRenderPass;
    pipeline.subpass = 0;

    // Create a structure to hold all the information about our shaders
    std::vector<VkPipelineShaderStageCreateInfo> shaders;
    VkPipelineShaderStageCreateInfo baseShader;
    memset(&baseShader, 0, sizeof(VkPipelineShaderStageCreateInfo));
    baseShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    baseShader.pName = "main";

    // Set the vertex shader
    baseShader.module = vk::vertModule;
    baseShader.stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaders.push_back(baseShader);

    // Set the fragment shader
    baseShader.module = vk::fragModule;
    baseShader.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaders.push_back(baseShader);

    // Attach the shaders to the the graphics pipeline
    pipeline.pStages = shaders.data();
    pipeline.stageCount = shaders.size();

    // Attach the boring fixed function state settings
    createFixedFunctionStateVariables();
    pipeline.pVertexInputState = &vertexInput;
    pipeline.pInputAssemblyState = &inputAssembly;
    pipeline.pViewportState = &viewportState;
    pipeline.pRasterizationState = &raster;
    pipeline.pMultisampleState = &multisample;
    pipeline.pColorBlendState = &blend;

    if (vkCreateGraphicsPipelines(vk::logialDevice, VK_NULL_HANDLE, 1, &pipeline, nullptr,
                                  &vk::graphicsPipeline) != VK_SUCCESS) {
        Log.error("Failed to create the graphics pipeline");
        return false;
    }

    return true;
}

void createFixedFunctionStateVariables()
{
    // Tell the pipeline how it can expect to recieve vertices
    memset(&vertexInput, 0, sizeof(VkPipelineVertexInputStateCreateInfo));
    vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    bindingDesc = Vertex::getBindingDescription();
    vertexInput.pVertexBindingDescriptions = &bindingDesc;
    vertexInput.vertexBindingDescriptionCount = 1;

    attributes = Vertex::getAttributeDescriptions();
    vertexInput.pVertexAttributeDescriptions = attributes.data();
    vertexInput.vertexAttributeDescriptionCount = attributes.size();

    // Tell the pipeline how to reconstruct the recieved vertex data
    memset(&inputAssembly, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Tell the pipeline about the viewport
    memset(&viewportState, 0, sizeof(VkPipelineViewportStateCreateInfo));
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

    viewport.x = 0.0;
    viewport.y = 0.0;
    viewport.width = static_cast<float>(vk::swapchainExtent.width);
    viewport.height = static_cast<float>(vk::swapchainExtent.height);
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;
    viewportState.pViewports = &viewport;
    viewportState.viewportCount = 1;

    scissor.offset = {0, 0};
    scissor.extent = vk::swapchainExtent;
    viewportState.pScissors = &scissor;
    viewportState.scissorCount = 1;

    // Tell the pipeline about the raster state
    memset(&raster, 0, sizeof(VkPipelineRasterizationStateCreateInfo));
    raster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

    raster.depthBiasEnable = VK_FALSE;
    raster.rasterizerDiscardEnable = VK_FALSE;
    raster.polygonMode = VK_POLYGON_MODE_FILL;
    raster.lineWidth = 1.0;
    raster.cullMode = VK_CULL_MODE_BACK_BIT;
    raster.frontFace = VK_FRONT_FACE_CLOCKWISE;

    // Tell the pipeline about multisampling
    memset(&multisample, 0, sizeof(VkPipelineMultisampleStateCreateInfo));
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.sampleShadingEnable = VK_FALSE;
    multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Tell the pipeline about how to blend for each attachment
    memset(&blendAttachment, 0, sizeof(VkPipelineColorBlendAttachmentState));
    blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                     VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    blendAttachment.blendEnable = VK_FALSE;

    memset(&blend, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
    blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blend.logicOpEnable = VK_FALSE;
    blend.pAttachments = &blendAttachment;
    blend.attachmentCount = 1;
}
