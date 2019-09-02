#ifndef EVENTS_MANAGER_H
#define EVENTS_MANAGER_H

#include "Event.h"

#include <string>
#include <map>
#include <vector>
#include <functional>

namespace Events {

	class EventsManager {
		
		static EventsManager* instance;

		std::map<std::string, std::vector<std::function<void()>>> emptyCallbacks;
		std::map<std::string, std::vector<std::function<void(Base*)>>> eventCallbacks;

	public:

		static EventsManager* GetInstance();
		static void Destroy();

		// subscribe empty callbacks
		template<typename Context>
		void Subscribe(const std::string& name, void(Context::*callback)(), Context* context);
		template<typename Context>
		void Subscribe(const std::string& name, void(Context::*callback)() const, Context* context);

		// subscribe event callbacks
		template<typename Context>
		void Subscribe(const std::string& name, void(Context::*callback)(Base*), Context* context);
		template<typename Context>
		void Subscribe(const std::string& name, void(Context::*callback)(Base*) const, Context* context);

		void Trigger(const std::string& name);
		void Trigger(const std::string& name, Base* event);

	};

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(), Context* context) {
		emptyCallbacks[name].push_back(std::bind(callback, context));
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)() const, Context* context) {
		emptyCallbacks[name].push_back(std::bind(callback, context));
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(Base*), Context* context) {
		eventCallbacks[name].push_back(std::bind(callback, context, std::placeholders::_1));
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(Base*) const, Context* context) {
		eventCallbacks[name].push_back(std::bind(callback, context, std::placeholders::_1));
	}
}


#endif
