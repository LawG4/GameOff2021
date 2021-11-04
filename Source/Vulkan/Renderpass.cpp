/*!********************************************************************************************************
\File          : Renderpass.cpp
\Copyright     : MIT License
\Brief         : Provides functionality for constructing renderpasses
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

VkRenderPass vk::onscreenRenderPass;

bool vk::createOnScreenRenderpass()
{
    // Vulkan renderpasses are made up as a series of subpasses, the onscreen renderpass currently only has
    // one subpass
    VkSubpassDescription subpass;
    memset(&subpass, 0, sizeof(VkSubpassDescription));

    // So that the subpass knows the layouts of its render targets we supply it with a reference, in this case
    // the onscreen attachment is colour attachment at index 0 and we want to draw to it
    VkAttachmentReference reference;
    memset(&reference, 0, sizeof(VkAttachmentReference));
    reference.attachment = 0;
    reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &reference;

    // Renderpasses are also responsible for transitioning images from layouts, and also what to do when the
    // image is loaded and stored. in this case we want to tell the renderpass how to transition the onscreen
    // attachment
    VkAttachmentDescription attachmentDesc;
    memset(&attachmentDesc, 0, sizeof(VkAttachmentDescription));
    attachmentDesc.format = vk::swapchainFormat;
    attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;

    // When we load the attachment we want to clear it so we have a fresh swapchain, and we want to save it so
    // it can be presented to the screen. We're not using stencil buffers so we dont care
    attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    // We don't want to use barriers to transition the render target, instead we want the begginging and
    // ending of the render pass to control the layout of the onscreen attachment, this is because the
    // onscreen image needs to transition to present mode
    attachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Finally hand this all over to the renderpass
    VkRenderPassCreateInfo renderpass;
    memset(&renderpass, 0, sizeof(VkRenderPassCreateInfo));
    renderpass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderpass.attachmentCount = 1;
    renderpass.pAttachments = &attachmentDesc;
    renderpass.subpassCount = 1;
    renderpass.pSubpasses = &subpass;

    // Tell the render pass that the colour attachment has an external dependency
    // ie turning from presentation back into renderable
    VkSubpassDependency dependency;
    memset(&dependency, 0, sizeof(VkSubpassDependency));
    // Dependency is coming from outside the renderpass
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    // We're transitioing the attachment from present to writable
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    // We want this transition to happen after the attachment has been displayed and before it gets written to
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    // Attach to render pass
    renderpass.dependencyCount = 1;
    renderpass.pDependencies = &dependency;

    if (vkCreateRenderPass(vk::logialDevice, &renderpass, nullptr, &vk::onscreenRenderPass) != VK_SUCCESS) {
        Log.error("Could not create the onscreen renderpass");
        return false;
    }

    return true;
}
