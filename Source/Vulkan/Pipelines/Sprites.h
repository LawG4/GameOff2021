
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
    VkDescriptorPool pool = VK_NULL_HANDLE;

    /// <summary>The layout that we use to stamp together a set</summary>
    VkDescriptorSetLayout layout = VK_NULL_HANDLE;

    /// <summary>The sets that contain the image</summary>
    std::vector<VkDescriptorSet> sets;

    VkImage texture = VK_NULL_HANDLE;

    VkDeviceMemory textureMemory = VK_NULL_HANDLE;

    VkImageView textureView = VK_NULL_HANDLE;

    /// <summary>The sampler for reading the sprite sheet. You never know it could change per sprite
    /// sheet</summary>
    VkSampler sampler = VK_NULL_HANDLE;
};

class Sprite
{
   public:
    /// <summary>Creates a vertex buffer for the sprites to rendered with</summary>
    /// <param name="spriteSheet">Reference to the sprite sheet</param>
    /// <param name="texCoord">4 element vector pointing out the texture coordinates starts at top left and
    /// goes clockwise</param>
    Sprite(SpriteSheet* spriteSheet, const std::vector<glm::vec2>& texCoord);
    ~Sprite();

    void render(const glm::mat4& mvp);

   private:
    /// <summary>Pointer to the sheet being used to draw this sprite</summary>
    SpriteSheet* _sheet;

    /// <summary>Vertex buffer group </summary>
    vk::BufferGroup _vertexGroup;
};

class SpriteInstance
{
   public:
    SpriteInstance(Sprite* sprite);
    SpriteInstance(Sprite* sprite, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot);
    ~SpriteInstance();

    // Sets this sprite instance to be rendered this frame.
    void render();

    // Setters
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);

    // Getters
    glm::mat4 getMvp();
    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();

   private:
    Sprite* _sprite;
    glm::mat4 _mvp;
    bool _mvpOutdated;
    glm::vec3 _pos;
    glm::vec3 _rot;
    glm::vec3 _scale;

    glm::mat4 calculateMVP();
};

namespace SpriteInternals
{
extern vk::BufferGroup quadIndexGroup;
extern VkPipeline pipeline;
}
