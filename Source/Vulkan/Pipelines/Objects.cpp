
#include "Objects.h"
#include "Vulkan.h"

RenderObject::RenderObject()
{
    // Require a UBO update
    requiresUBOUpdateVector.resize(vk::swapLength, true);

    // Set is active to trie
    isActive = true;

    // Memset our internal vulkan structs
    memset(&vertexGroup, 0, sizeof(vk::BufferGroup));
    memset(&indexGroup, 0, sizeof(vk::BufferGroup));
}

RenderObject::~RenderObject() {}

bool RenderObject::requiresUBOUpdate(uint32_t swapIndex) { return requiresUBOUpdateVector[swapIndex]; }

void RenderObject::scheduleUBOUpdate() { requiresUBOUpdateVector.resize(vk::swapLength, true); }
