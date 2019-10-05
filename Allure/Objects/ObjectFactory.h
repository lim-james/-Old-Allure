#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

// Objects
#include "Camera/CameraObject.h"
#include "GameObject/GameObject.h"

// Components
#include <Components/Transform/Transform.h>
#include <Components/Camera/Camera.h>
#include <Render/Render.h>
#include <Script/Script.h>

// Supporting
#include <MACROS.h>
#include <Helpers/String/StringHelpers.h>

#include <Entity/EntityManager.h>
#include <Components/ComponentsManager.h>
#include <Systems/SystemsManager.h>

#include <typeindex>

class ObjectFactory {

	EntityManager* entities;
	ComponentsManager* components;
	SystemsManager* systems;

public:

	ObjectFactory();
	~ObjectFactory();

	template<typename ObjectType>
	ObjectType* Create();

	void Update(const float& dt);

private:

	template<typename ObjectType>
	void AddComponents(ObjectType* object);

};

template<typename ObjectType>
ObjectType* ObjectFactory::Create() {
	ObjectType* object = entities->Fetch<ObjectType>();

	AddComponents(object);

	object->Initialize();
	object->Use();

	return object;
}

template<typename ObjectType>
void ObjectFactory::AddComponents(ObjectType* object) {
	std::string typeName = nameof(ObjectType);
	typeName = Helpers::Pair(typeName, ' ').second;

	object->AddComponent(components->Fetch<Transform>());

	if (typeName == "GameObject") {
		object->AddComponent(components->Fetch<Render>());
	} else if (typeName == "CameraObject") {
		object->AddComponent(components->Fetch<Camera>());
		object->AddComponent(components->Fetch<Script>());
	}
}

#endif