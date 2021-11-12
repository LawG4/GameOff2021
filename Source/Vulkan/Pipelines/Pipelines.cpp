
#include "Pipelines.h"

void vk::createPipelines()
{
    PipelineInternals::Templates::initialiseTemplates();
    PipelineInternals::create2DPipeline();
}

void vk::destroyPipelines() { PipelineInternals::destroy2DPipeline(); }

void prepareCommandBuffer(uint32_t swapIndex)
{
    // Reset the graphics pool at the current swap index as it's cheap
    vkResetCommandPool(vk::logialDevice, vk::graphicsPools.at(swapIndex),
                       VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);

    // Start recording the primary command buffer
    VkCommandBufferBeginInfo begin{};
    begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(vk::cmdBuffers.at(swapIndex), &begin);

    // Record the 2D command buffer
    PipelineInternals::prepare2DCmdBuffer(vk::cmdBuffers.at(swapIndex), swapIndex);

    // We're done recording so reset the buffer
    vkEndCommandBuffer(vk::cmdBuffers.at(swapIndex));
}
