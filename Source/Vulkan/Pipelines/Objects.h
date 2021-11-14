
#pragma once
#include "Memory.h"
#include "Vulkan.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

/// <summary>
/// So here's a quick overview of how sprites work. I spoke to some people in the very friendly Vulkan discord
/// server. It seems that most game engines will track pools and dynamically create a new one when it runs out
/// of space. However we do not have time to write something like this. Instead what I shall do is use a
/// descriptor pool per sprite sheet. This I think is the way to minimise descriptor set creation as I think
/// everything else can go into the vertex buffers
///
/// Texture coordinates : Vertex Buffer
/// MVP Matrix : Push constant
/// Sprite sheets : passed via descriptor set.
///
/// Since we want to be able to put multiple copies of the same sprite multiple times on the screen, we will
/// provide a structure called a render object. These are the things that the user can actually render
///
/// </summary>

class SpriteSheet
{
   public:
    SpriteSheet(const char* TextureFileName);
    ~SpriteSheet();

   private:
    /// <summary>We need one pool to allocate everything out of since it's easier to predict than managing a
    /// global one</summary>
    VkDescriptorPool pool;

    /// <summary>The layout that we use to stamp together a set</summary>
    VkDescriptorSetLayout layout;

    /// <summary>The sets that contain the image</summary>
    std::vector<VkDescriptorSet> sets;

    /// <summary>The sampler for reading the sprite sheet. You never know it could change per sprite
    /// sheet</summary>
    VkSampler sampler;
};

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
    /// <summary>Pointer to the sheet being used to draw this sprite</summary>
    SpriteSheet* sheet;

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

class RenderObject
{
   public:
    RenderObject(Sprite* sprite);
    ~RenderObject();
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);

   private:
    Sprite* _sprite;
    glm::mat4 _mvp;
    bool _mvpOutdated;
    glm::vec3 _pos;
    glm::vec3 _rot;
    glm::vec3 _scale;
};
