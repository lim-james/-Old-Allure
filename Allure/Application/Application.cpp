#include "Application.h"

#include "../Timer/Timer.h"
#include "../Events/Manager/EventsManager.h"	

#include "../Input/Events/InputEvents.h"

#include <iostream>
#include <set>
#include <algorithm>

bool Application::Initialize(const int& width, const int& height, const char* title, const bool& fullscreen) {
	context = nullptr;
	physicalDevice = VK_NULL_HANDLE;

	// initialize GLFW
	if (!glfwInit()) {
		std::cout << "ERROR : GLFW INITIALISATION FAILED.";
		return false;
	}

	context = new Window(width, height, title, fullscreen);
	inputController.Initialize(context->Get());

	if (!InitVulkan(title)) {
		return false;
	}

	Events::EventsManager::GetInstance()->Subscribe("EXIT", &Window::Close, context);
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("SCROLL_INPUT", &Application::OnEvent, this);

	return true;
}

void Application::Run() {
	Timer timer;
	timer.Start();

	while (!context->ShouldClose()) {
		glfwPollEvents();

		const float et = static_cast<float>(timer.GetElapsedTime());
		const float dt = static_cast<float>(timer.GetDeltaTime());
		const int FPS = static_cast<int>(1.f / dt);

		std::string title = "FPS : ";
		title += std::to_string(FPS);
		context->SetTitle(title.c_str());

		context->SwapBuffers();
		timer.Update();
	}
}

void Application::Exit() {
	vkDestroyDevice(device, nullptr);

	if (enableValidationLayers)
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
	
	if (context)
		delete context;

	glfwTerminate();
}

void Application::OnEvent(Events::Event* event) {

	if (event->name == "KEY_INPUT") {
		Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
		// quit program if escaped
		if (input->key == GLFW_KEY_ESCAPE && input->action == GLFW_RELEASE) {
			Events::EventsManager::GetInstance()->Trigger("EXIT");
			return;
		}
	} else if (event->name == "CURSOR_POSITION_INPUT") {
		Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	} else if (event->name == "MOUSE_BUTTON_INPUT") {
		Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
	} else if (event->name == "SCROLL_INPUT") {
		Events::ScrollInput* input = static_cast<Events::ScrollInput*>(event);
	}

}

bool Application::InitVulkan(const char* title) {
	return CreateInstance(title) 
		&& SetupDebugMessenger() 
		&& CreateSurface()
		&& PickPhysicalDevice() 
		&& CreateLogicalDevice()
		&& CreateSwapChain();
}

bool Application::CreateInstance(const char* title) {
	if (enableValidationLayers && !CheckValidationLayerSupport()) {
		std::cout << "Validation Layers requested, but not available.\n";
		return false;
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = title;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Allure";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	const auto extensions = GetRequiredExtensions();
	// enabled extensions
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		PopulateDebugMessageCreateInfo(debugCreateInfo);
		createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		std::cout << "Failed to create instance.\n";
		return false;
	}

	return true;
}

bool Application::CheckValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (auto& layerName : validationLayers) {
		bool layerFound = false;

		for (auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerCount) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> Application::GetRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);	

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool Application::SetupDebugMessenger() {
	if (!enableValidationLayers) return true;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	PopulateDebugMessageCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		std::cout << "Failed to set up debug messenger.\n";
		return false;
	}

	return true;
}

VkResult Application::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void Application::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

bool Application::CreateSurface() {
	if (glfwCreateWindowSurface(instance, context->Get(), nullptr, &surface) != VK_SUCCESS) {
		std::cout << "Failed to create window surface!\n";
		return false;
	}

	return true;
}

void Application::PopulateDebugMessageCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = Application::DebugCallback;
}

bool Application::PickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		std::cout << "Failed to find GPUs with Vulkan support.\n";
		return false;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		std::cout << "Failed to find a stuitable GPU.";
		return false;
	}

	return true;
}

bool Application::IsDeviceSuitable(const VkPhysicalDevice& device) {
	const QueueFamilyIndices indices = FindQueueFamilies(device);

	const bool extensionSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionSupported) {
		const SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.IsComplete() && extensionSupported && swapChainAdequate;
}

Application::QueueFamilyIndices Application::FindQueueFamilies(const VkPhysicalDevice& device) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.graphicsFamily = i;
		}

		if (indices.IsComplete()) {
			break;
		}

		++i;
	}

	return indices;
}

bool Application::CheckDeviceExtensionSupport(const VkPhysicalDevice& device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	
	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

Application::SwapChainSupportDetails Application::QuerySwapChainSupport(const VkPhysicalDevice& device) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool Application::CreateSwapChain() {
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);
	return true;
}

VkSurfaceFormatKHR Application::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR Application::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Application::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	} else {
		const auto windowSize = context->GetSize<uint32_t>();
		VkExtent2D actualExtent = { windowSize.x, windowSize.y };

		actualExtent.width = max(capabilities.minImageExtent.width, min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = max(capabilities.minImageExtent.height, min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

bool Application::CreateLogicalDevice() {
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.Value(), indices.presentFamily.Value() };

	float queuePriority = 1.f;
	for (auto queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		std::cout << "Failed to create logical device.";
		return false;
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.Value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.Value(), 0, &presentQueue);

	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Application::DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cout << "validation layer: " << pCallbackData->pMessage << std::endl;

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {

	}

	return VK_FALSE;
}


