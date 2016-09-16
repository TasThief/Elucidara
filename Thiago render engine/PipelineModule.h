#pragma once
#include "RenderEngine.h"
class Hub;
class PipelineModule
{
public:
	VkDeleter<VkShaderModule> vertShaderModule;
	VkDeleter<VkShaderModule> fragShaderModule;
	VkDeleter<VkPipelineLayout> pipelineLayout;
	PipelineModule(VkDevice * device, VkExtent2D viewPort);
	VkPipelineRasterizationStateCreateInfo BuildRasterizerInfo();
	VkPipelineMultisampleStateCreateInfo BuildMultisamplingInfo();
	void CreateShaderModule(VkDevice * deviceModule, const std::vector<char>& code, VkDeleter<VkShaderModule>& shaderModule);
	VkPipelineColorBlendAttachmentState BuildColorBlendAttachmentInfo();
	VkPipelineViewportStateCreateInfo BuildViewPortInfo(VkExtent2D viewPortExtent);
	~PipelineModule();

	static vector<char> ReadFile(const string& fileName);
};

