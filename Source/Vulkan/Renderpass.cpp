/*!********************************************************************************************************
\File          : Renderpass.cpp
\Copyright     : GPL-3.0 License
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
    VkAttachmentReference attachRef{};
    attachRef.attachment = 0;
    attachRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    // The depth attachment is at index 1
    VkAttachmentReference depthRef{};
    depthRef.attachment = 1;
    depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Attach the references to the subpass description
    subpass.pColorAttachments = &attachRef;
    subpass.colorAttachmentCount = 1;
    subpass.pDepthStencilAttachment = &depthRef;

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

    // Make an attachment description for the depthbuffer
    VkAttachmentDescription depthDesc{};
    depthDesc.format = VK_FORMAT_D32_SFLOAT;
    depthDesc.samples = VK_SAMPLE_COUNT_1_BIT;
    depthDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthDesc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthDesc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Attach the attachment descriptions over to the renderpass
    VkRenderPassCreateInfo renderpass{};
    renderpass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    VkAttachmentDescription attachments[2] = {attachmentDesc, depthDesc};
    renderpass.attachmentCount = 2;
    renderpass.pAttachments = attachments;

    renderpass.subpassCount = 1;
    renderpass.pSubpasses = &subpass;

    // Tell the render pass that the colour attachments have an external dependency
    // This is a dependency that comes from outside the subpasses : ie turning from presentation back into
    // renderable.
    // We're waiting on colour attachment and the depth attachment
    VkSubpassDependency dependency{};

    // Dependency is coming from outside the renderpass
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;

    // We're transitioing the colour attachment from present to writable. The depth buffer from depth testing
    // to depth writing.
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask =
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    // We want this transition to happen after the attachment has been displayed and before it gets written to
    dependency.srcStageMask =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

    // Attach to render pass
    renderpass.dependencyCount = 1;
    renderpass.pDependencies = &dependency;

    if (vkCreateRenderPass(vk::logicalDevice, &renderpass, nullptr, &vk::onscreenRenderPass) != VK_SUCCESS) {
        Log.error("Could not create the onscreen renderpass");
        return false;
    }

    return true;
}
