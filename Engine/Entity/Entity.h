#ifndef ENTITY_H
#define ENTITY_H

#include "../Components/ComponentsManager.h"

#include <Events/Event.h>
#include <Quad.hpp>
#include <MACROS.h>

#include <map>
#include <typeindex>
#include <string>

class Entity {

	Quad<Entity*>* quad;
	std::vector<Quad<Entity*>*> quadList;
	
	std::string tag;

	Entity* parent;
	std::vector<Entity*> children;

	bool used;
	bool staticEntity;

	ComponentsManager* componentsManager;
	std::map<std::type_index, Component*> components;

public:

	Entity();
	virtual ~Entity();

	virtual void Build() = 0;

	virtual void Initialize();
	void Destroy();

	Quad<Entity*>* GetQuad() const;
	void EmptyQuadList();
	std::vector<Quad<Entity*>*> const GetQuadList() const;
	bool CompareQuad(std::vector<Quad<Entity*>*> _quad) const;
	void SetQuadList(Quad<Entity*>* const _quad);
	void SetQuad(Quad<Entity*>* const _quad);

	const std::string& GetTag() const;
	void SetTag(const std::string& _tag);

	Entity* const GetParent() const;
	virtual void SetParent(Entity * const entity);

	void AddChild(Entity* const entity);
	void RemoveChild(Entity* const entity);

	void ClearChildren();
	std::vector<Entity*>& GetChildren();

	void SetActive(const bool& state);

	void Use();
	const bool& IsUsed() const;

	template<typename ComponentType>
	const bool HasComponent() const;

	template<typename ComponentType>
	void AddComponent();

	template<typename ComponentType>
	ComponentType* const GetComponent();

	friend class EntityManager;

};

template<typename ComponentType>
const bool Entity::HasComponent() const {
	return components.find(indexof(ComponentType)) != components.end();
}

template<typename ComponentType>
void Entity::AddComponent() {
	if (HasComponent<ComponentType>()) return;

	Component* component = componentsManager->Fetch<ComponentType>();
	component->SetParent(this);
	components[indexof(ComponentType)] = component;
}

template<typename ComponentType>
ComponentType* const Entity::GetComponent() {
	return static_cast<ComponentType * const>(components[indexof(ComponentType)]);
}

#endif
