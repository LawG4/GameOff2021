/*!********************************************************************************************************
\File          : Pipelines.h
\Copyright     : GPL-3.0 License
\Brief         : Provides utility for recording all the different pipelines
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#pragma once
#include "Vulkan.h"

namespace vk
{

void prepareCommandBuffer(uint32_t swapIndex);
VkShaderModule createShaderModule(const char* filePath);

void createPipelines();
void destroyPipelines(bool destroyAllResources);

void createDescriptorSetLayouts();
void destroyDescriptorSetLayouts();
}  // namespace vk

namespace PipelineInternals
{

// Commands for creating the pipelines

void create2DPipeline();
void createDescriptorSetLayouts2D();

// Commands for preparing each command buffer

/// <summary> </summary>
/// <param name="cmd"></param>
/// <param name="swapIndex"></param>
void prepare2DCmdBuffer(VkCommandBuffer& cmd, uint32_t swapIndex);

// Commands for destroying each of the pipelines

void destroy2DPipeline(bool destroyAllResources);
void destroyDescriptorSetLayouts2D();

namespace Templates
{
void initialiseTemplates();
extern VkPipelineVertexInputStateCreateInfo vertexInput;
extern VkVertexInputBindingDescription bindingDesc;
extern std::vector<VkVertexInputAttributeDescription> attributes;

extern VkPipelineInputAssemblyStateCreateInfo inputAssembly;

extern VkPipelineViewportStateCreateInfo viewportState;
extern VkViewport viewport;
extern VkRect2D scissor;

extern VkPipelineRasterizationStateCreateInfo raster;
extern VkPipelineMultisampleStateCreateInfo multisample;

extern VkPipelineColorBlendStateCreateInfo blend;
extern VkPipelineColorBlendAttachmentState blendAttachment;
}  // namespace Templates

}  // namespace PipelineInternals
