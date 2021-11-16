
#include "Sprites.h"
#include "Vulkan.h"

struct SpriteVertices {
    glm::vec3 pos;
    glm::vec2 tex;
};

const std::vector<glm::vec3> quadCoordinates = {
  {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f}};

std::vector<uint16_t> indexArray = {0, 1, 2, 2, 3, 0};

vk::BufferGroup SpriteInternals::quadIndexGroup = {VK_NULL_HANDLE, VK_NULL_HANDLE};

Sprite::Sprite(SpriteSheet* spriteSheet, const std::vector<glm::vec2>& texCoord)
{
    // Store the sprite sheet
    _sheet = spriteSheet;

    // Ensure that the tex coordinate has at least 4 entries
    if (texCoord.size() < 4) {
        Log.error("Texture coordinates not long enough");
        return;
    }

    // Fill the vertex buffer on the CPU side
    std::vector<SpriteVertices> vertices(4);
    for (uint32_t i = 0; i < 4; i++) {
        vertices[i] = {quadCoordinates[i], texCoord[i]};
    }

    // Create the vertex buffer for this sprite
    VkDeviceSize size = sizeof(SpriteVertices) * vertices.size();
    _vertexGroup = vk::createVertexBufferGroup(size, vertices.data());

    // If the index buffer has not been defined then create it
    if (SpriteInternals::quadIndexGroup.buffer == VK_NULL_HANDLE) {
        SpriteInternals::quadIndexGroup =
          vk::createVertexBufferGroup(sizeof(uint16_t) * indexArray.size(), indexArray.data());
    }
}

Sprite::~Sprite()
{
    // Destory the vertex buffer
    if (_vertexGroup.buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(vk::logicalDevice, _vertexGroup.buffer, nullptr);
    }
    if (_vertexGroup.mem != VK_NULL_HANDLE) {
        vkFreeMemory(vk::logicalDevice, _vertexGroup.mem, nullptr);
    }
}

void Sprite::render(const glm::mat4& mvp) {}
