#pragma once
#include "Vulkan.h"

void prepareCommandBuffer(uint32_t swapIndex);

// Commands for preparing each command buffer

void prepare2dCmdBuffer(VkCommandBuffer& cmd, uint32_t swapIndex);
