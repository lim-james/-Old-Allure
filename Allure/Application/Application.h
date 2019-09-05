#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Events/Event.h"
#include "../Utility/Optional.hpp"

#include "../Window/Window.h"
#include "../Input/Controller/InputController.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Application {

	const unsigned FPS = 60;
	const unsigned FRAMERATE = 1000 / FPS;

	Window* context;
	InputController inputController;

	// Vulkan
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

	bool Initialize(const int& width, const int& height, const char* title, const bool& fullscreen);
	void Run();
	void Exit();
	
private:

	void OnEvent(Events::Event* event);

	// Vulkan
	bool InitVulkan(const char* title);

	bool CreateInstance(const char* title);
	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();

	bool SetupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	bool CreateSurface();

	void PopulateDebugMessageCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	bool PickPhysicalDevice();

	bool IsDeviceSuitable(const VkPhysicalDevice& device);

	QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device);
	bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device);
	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device);

	bool CreateSwapChain();

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	bool CreateLogicalDevice();

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

};

#endif
