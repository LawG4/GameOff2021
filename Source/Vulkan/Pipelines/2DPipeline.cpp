#include "Pipelines.h"

void prepare2dCmdBuffer(VkCommandBuffer& cmd, uint32_t swapIndex)
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

    vkCmdEndRenderPass(cmd);
}
