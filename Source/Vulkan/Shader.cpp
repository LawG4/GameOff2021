/*!********************************************************************************************************
\File          : Shader.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides Shader module creation
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Pipelines.h"
#include "Vulkan.h"

#include <fstream>

/// <summary> Reads in a spirv shader at the specified file location </summary>
/// <param name="filePath">File path of shader local to working directory</param>
/// <param name="shaderSource">A reference to a vector that is having the shder source placed into</param>
/// <returns>True if successful</returns>
bool readSpirv(const char* filePath, std::vector<uint32_t>& shaderSource)
{
    // Start by reading in the shader
    // Tell the os we're reading in binary data and open the file cursor at the end so we can get the length
    // of the file
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        Log.error("Could not open spirv shader {}", filePath);
        return false;
    }

    // Get the file size from the length of the file
    uint32_t byteCount = file.tellg();

    // Resize the shader source vector
    shaderSource.clear();
    shaderSource.resize(byteCount / 4);  // 4 bytes in 32 bits

    // Put the file header to the begining and read the data into the vector
    file.seekg(0);
    file.read(reinterpret_cast<char*>(shaderSource.data()), byteCount);
    return true;
}

VkShaderModule vk::createShaderModule(const char* filePath)
{
    VkShaderModule shaderModule;

    // Read in shader source
    std::vector<uint32_t> shaderSource;
    if (!readSpirv(filePath, shaderSource)) {
        return false;
    }

    VkShaderModuleCreateInfo module;
    memset(&module, 0, sizeof(VkShaderModuleCreateInfo));
    module.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    module.pCode = shaderSource.data();
    module.codeSize = shaderSource.size() * sizeof(uint32_t);

    if (vkCreateShaderModule(vk::logialDevice, &module, nullptr, &shaderModule) != VK_SUCCESS) {
        Log.error("Failed to create shader module {}", filePath);
    }

    return shaderModule;
}
