#pragma once
#include "RenderEngine.h"
class Hub;
class PipelineModule
{
public:
	VkDeleter<VkShaderModule> vertShaderModule;
	VkDeleter<VkShaderModule> fragShaderModule;
	VkDeleter<VkPipelineLayout> pipelineLayout;
	VkDeleter<VkRenderPass> renderPass;
	PipelineModule(VkDevice * device, VkExtent2D viewPort);
	VkPipelineRasterizationStateCreateInfo BuildRasterizerInfo();
	VkPipelineMultisampleStateCreateInfo BuildMultisamplingInfo();
	void CreateShaderModule(VkDevice * deviceModule, const std::vector<char>& code, VkDeleter<VkShaderModule>& shaderModule);
	void CreateRenderPass(SwapChainModule* swapChain, VkDevice* device);
	VkPipelineColorBlendAttachmentState BuildColorBlendAttachmentInfo();
	VkPipelineLayoutCreateInfo BuildPipelineLayoutInfo();
	VkPipelineColorBlendStateCreateInfo BuildColorBlendingInfo(VkPipelineColorBlendAttachmentState colorBlendAttachment);
	VkPipelineViewportStateCreateInfo BuildViewPortInfo(VkExtent2D viewPortExtent);
	~PipelineModule();

	static vector<char> ReadFile(const string& fileName);
};


