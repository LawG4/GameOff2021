
#include "Pipelines.h"
#include "Vertex.h"

// Define our forward declared templates

VkPipelineVertexInputStateCreateInfo PipelineInternals::Templates::vertexInput;
VkVertexInputBindingDescription PipelineInternals::Templates::bindingDesc;
std::vector<VkVertexInputAttributeDescription> PipelineInternals::Templates::attributes;

VkPipelineInputAssemblyStateCreateInfo PipelineInternals::Templates::inputAssembly;

VkPipelineViewportStateCreateInfo PipelineInternals::Templates::viewportState;
VkViewport PipelineInternals::Templates::viewport;
VkRect2D PipelineInternals::Templates::scissor;

VkPipelineRasterizationStateCreateInfo PipelineInternals::Templates::raster;
VkPipelineMultisampleStateCreateInfo PipelineInternals::Templates::multisample;

VkPipelineColorBlendStateCreateInfo PipelineInternals::Templates::blend;
VkPipelineColorBlendAttachmentState PipelineInternals::Templates::blendAttachment;

void PipelineInternals::Templates::initialiseTemplates()
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
    raster.cullMode = VK_CULL_MODE_NONE;
    raster.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

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
