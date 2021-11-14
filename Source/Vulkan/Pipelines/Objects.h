
#pragma once
#include "Memory.h"
#include "Vulkan.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

class Sprite
{
   public:
    Sprite();
    ~Sprite();
    Sprite(const std::vector<glm::vec3>& pos, const std::vector<glm::vec3>& col);

    /// <summary>is this object getting rendered and updated </summary>
    bool isActive;

    /// <summary>Returns a bool for if the current frames UBO is outdated</summary>
    /// <param name="swapIndex">Index of the buffer in the ubos</param>
    /// <returns>True if update is required</returns>
    bool requiresUBOUpdate(uint32_t swapIndex);

    /// <summary>Records the command for this current frame</summary>
    /// <param name="cmd">Reference to the command buffer being used</param>
    /// <param name="swapIndex">The frame index of the current frame being recorded</param>
    void recordCmd(VkCommandBuffer& cmd, uint32_t swapIndex);

    /// <summary>Marks all the bools in requiresUBOUpdateVecetor to true so they get updated </summary>
    void scheduleUBOUpdate();

    /// <summary> Updates any UBOs that the user might have changed</summary>
    /// <param name="swapIndex">The swapIndex of the buffer to update</param>
    void updateUbo(uint32_t swapIndex);

    glm::vec3 pos = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::vec3 rot = glm::vec3(0, 0, 0);

   protected:
    std::vector<bool> requiresUBOUpdateVector;

    /// <summary>Vertex buffer group </summary>
    vk::BufferGroup vertexGroup;

    /// <summary>Index buffer group</summary>
    vk::BufferGroup indexGroup;

    /// <summary>List of all the uniform buffers </summary>
    std::vector<vk::BufferGroup> ubos;

    VkDescriptorPool pool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> sets;
};
