#ifndef VULKAN_H
#define VULKAN_H

#include "../Utility/Optional.hpp"

#include "../Window/Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Vulkan {
	
	Window* context;

#if _DEBUG
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkInstance instance;

	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImageView> swapChainImageViews;

	struct QueueFamilyIndices {
		Optional<uint32_t> graphicsFamily;	
		Optional<uint32_t> presentFamily;	

		const bool IsComplete() const {
			return graphicsFamily.HasValue() && presentFamily.HasValue();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

public:

	bool Initialize(const char* title, Window* context);
	void Destroy();

private:

	/*
		Basic set up	
	*/

	bool CreateInstance(const char* title);
	


	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();



	bool SetupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);



	/*
		Presentation setup	
	*/

	bool CreateSurface();

	void PopulateDebugMessageCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);



	bool PickPhysicalDevice();

	bool IsDeviceSuitable(const VkPhysicalDevice& device);

	QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device);
	bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device);
	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device);



	bool CreateLogicalDevice();



	bool CreateSwapChain();

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);



	bool CreateImageViews();



	/*
		Graphics pipeline basics
	*/



	bool CreateGraphicsPipeline();



	/*
		static methods	
	*/

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);




};

#endif
