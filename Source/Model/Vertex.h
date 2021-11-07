/*!********************************************************************************************************
\File          : Vertex.h
\Copyright     : MIT License
\Brief         : Models are constructed as a list of vertices
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once
#include "Vulkan.h"
#include "glm/glm.hpp"

// Define a struct that holds the vertex input description for elements in this program
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;

    /// <summary> Vertex binding describes how to load the data from our vertices struct </summary>
    /// <returns> A vertex binding description for vertices of this type</returns>
    static VkVertexInputBindingDescription getBindingDescription();

    /// <summary>Tell Vulkan how to load each of the members that make up one vertex </summary>
    /// <returns>A vector of vertex attribute descriptions for vertices of this type</returns>
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
};
