#include "PipelineModule.h"



PipelineModule::PipelineModule(VkDevice* device, VkExtent2D viewPort)
{
	//initialize pipeline object
	pipelineLayout.New(*device, vkDestroyPipelineLayout);

	//initialize vertshader module
	vertShaderModule.New(*device, vkDestroyShaderModule);

	//create the bytecode from file
	CreateShaderModule(device, ReadFile("shaders/vert.spv"), vertShaderModule);

	//initialize creation object
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	//vertShaderStageInfo.pNext;
	//vertShaderStageInfo.flags;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	//this is useful to set variables in the shader
	//vertShaderStageInfo.pSpecializationInfo;

	//create the bytecode from file
	fragShaderModule.New(*device, vkDestroyShaderModule);

	//initialize creation object

	CreateShaderModule(device, ReadFile("shaders/frag.spv"), fragShaderModule);
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	//save shaders inside struct
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	cout << "finish to load shaders" << endl;

	

	if (vkCreatePipelineLayout(*device, &BuildPipelineLayoutInfo(), nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

}

VkPipelineRasterizationStateCreateInfo PipelineModule::BuildRasterizerInfo()
{
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	return rasterizer;
}
VkPipelineMultisampleStateCreateInfo PipelineModule::BuildMultisamplingInfo()
{
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	return multisampling;
}
VkPipelineColorBlendAttachmentState PipelineModule::BuildColorBlendAttachmentInfo()
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	return colorBlendAttachment;
}
VkPipelineColorBlendStateCreateInfo PipelineModule::BuildColorBlendingInfo(VkPipelineColorBlendAttachmentState colorBlendAttachment)
{
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;
	return colorBlending;
}
VkPipelineViewportStateCreateInfo PipelineModule::BuildViewPortInfo(VkExtent2D viewPortExtent)
{
	//initialize viewport
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)viewPortExtent.width;
	viewport.height = (float)viewPortExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	//??
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = viewPortExtent;

	//set the viewport info
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	return viewportState;
}
VkPipelineLayoutCreateInfo PipelineModule::BuildPipelineLayoutInfo()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	return pipelineLayoutInfo;
}

PipelineModule::~PipelineModule()
{

}
vector<char> PipelineModule::ReadFile(const string & fileName)
{
	//open the file through the end
	ifstream file(fileName, ios::ate | ios::binary);

	if (!file.is_open())
		throw runtime_error("failed to open file!");

	//use the position of the reader to save howmuch bites i have stored
	size_t fileSize = (size_t)file.tellg();

	//the shader info
	vector<char> buffer(fileSize);
	
	//go back to the begining of the file
	file.seekg(0);

	//read the entire file
	file.read(buffer.data(), fileSize);

	//close the file
	file.close();

	//return the info gathered
	return buffer;
}
void PipelineModule::CreateShaderModule(VkDevice* deviceModule, const std::vector<char>& code, VkDeleter<VkShaderModule>& shaderModule)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//	createInfo.pNext;
//	createInfo.flags;
	createInfo.codeSize = code.size();
	createInfo.pCode = (uint32_t*)code.data();

	if (vkCreateShaderModule(*deviceModule, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
}

void PipelineModule::CreateRenderPass(SwapChainModule* swapChain, VkDevice* device)
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChain->format.format;
	//the format color attachment should match with the swapchain color format
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//	colorAttachment.flags;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subPass = {};
	subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//	subPass.inputAttachmentCount;
//	subPass.pInputAttachments;
	subPass.colorAttachmentCount = 1;
	subPass.pColorAttachments = &colorAttachmentRef;
//	subPass.pResolveAttachments;
//	subPass.pDepthStencilAttachment;
//	subPass.preserveAttachmentCount;
//	subPass.pPreserveAttachments;

	renderPass.New(*device, vkDestroyRenderPass);

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//	renderPassInfo.pNext;
//	renderPassInfo.flags;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subPass;
//	renderPassInfo.dependencyCount;
//	renderPassInfo.pDependencies;

	if (vkCreateRenderPass(*device, &renderPassInfo, nullptr, renderPass.Reset()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}

