#include "Objects.h"
#include "Pipelines.h"

std::vector<RenderObject2D*> renderObjects;

void PipelineInternals::create2DPipeline() {}

void PipelineInternals::destroy2DPipeline() {}

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
