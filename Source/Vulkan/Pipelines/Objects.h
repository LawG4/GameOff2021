
#pragma once
#include "Memory.h"
#include "Vulkan.h"

class RenderObject
{
   public:
    RenderObject();
    ~RenderObject();

    /// <summary>is this object getting rendered and updated </summary>
    bool isActive;

    /// <summary>Returns a bool for if the current frames UBO is outdated</summary>
    /// <param name="swapIndex">Index of the buffer in the ubos</param>
    /// <returns>True if update is required</returns>
    bool requiresUBOUpdate(uint32_t swapIndex);

    /// <summary>Marks all the bools in requiresUBOUpdateVecetor to true so they get updated </summary>
    void scheduleUBOUpdate();
   private:
    std::vector<bool> requiresUBOUpdateVector;

    /// <summary>Vertex buffer group </summary>
    vk::BufferGroup vertexGroup;

    /// <summary>Index buffer group</summary>
    vk::BufferGroup indexGroup;

    /// <summary>List of all the uniform buffers </summary>
    std::vector<vk::BufferGroup> ubos;
};

class RenderObject2D : public RenderObject
{
   public:
    void recordCmd(VkCommandBuffer& cmd, uint32_t swapIndex);
    void updateUbo(uint32_t swapIndex);
};