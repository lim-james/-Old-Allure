#include "EntityComparator.h"

#include "../Components/Transform/Transform.h"


EntityComparator::EntityComparator()
{
}


EntityComparator::~EntityComparator()
{
}

void EntityComparator::Partition(Quad<Entity*>* _root)
{
	_root->topLeft = new Quad<Entity*>;
	_root->topLeft->size.Set(_root->size.x / 2.0f, _root->size.y, _root->size.z / 2.0f);
	_root->topLeft->position.Set(_root->position.x - _root->topLeft->size.x / 2.0f, _root->position.y, _root->position.z + _root->topLeft->size.z / 2.0f);

	for (int i = 0; i < _root->list.size(); i++)
	{
		if (_root->list[i]->GetComponent<Transform>()->translation.x < _root->position.x 
			&& _root->list[i]->GetComponent<Transform>()->translation.z > _root->position.z
			&& _root->list[i]->GetComponent<Transform>()->translation.x > _root->position.x - _root->size.x / 2
			&& _root->list[i]->GetComponent<Transform>()->translation.z < _root->position.z + _root->size.z / 2)
		{
			_root->topLeft->list.push_back(_root->list[i]);
		}
	}

	_root->topRight = new Quad<Entity*>;
	_root->topRight->size.Set(_root->size.x / 2.0f, _root->size.y, _root->size.z / 2.0f);
	_root->topRight->position.Set(_root->position.x + _root->topRight->size.x / 2.0f, _root->position.y, _root->position.z + _root->topRight->size.z / 2.0f);

	for (int i = 0; i < _root->list.size(); i++)
	{
		if (_root->list[i]->GetComponent<Transform>()->translation.x > _root->position.x
			&& _root->list[i]->GetComponent<Transform>()->translation.z > _root->position.z
			&& _root->list[i]->GetComponent<Transform>()->translation.x < _root->position.x + _root->size.x / 2
			&& _root->list[i]->GetComponent<Transform>()->translation.z < _root->position.z + _root->size.z / 2)
		{
			_root->topRight->list.push_back(_root->list[i]);
		}
	}

	_root->bottomRight = new Quad<Entity*>;
	_root->bottomRight->size.Set(_root->size.x / 2.0f, _root->size.y, _root->size.z / 2.0f);
	_root->bottomRight->position.Set(_root->position.x + _root->bottomRight->size.x / 2.0f, _root->position.y, _root->position.z - _root->bottomRight->size.z / 2.0f);

	for (int i = 0; i < _root->list.size(); i++)
	{
		if (_root->list[i]->GetComponent<Transform>()->translation.x > _root->position.x
			&& _root->list[i]->GetComponent<Transform>()->translation.z < _root->position.z
			&& _root->list[i]->GetComponent<Transform>()->translation.x < _root->position.x + _root->size.x / 2
			&& _root->list[i]->GetComponent<Transform>()->translation.z > _root->position.z - _root->size.z / 2)
		{
			_root->bottomRight->list.push_back(_root->list[i]);
		}
	}

	_root->bottomLeft = new Quad<Entity*>;
	_root->bottomLeft->size.Set(_root->size.x / 2.0f, _root->size.y, _root->size.z / 2.0f);
	_root->bottomLeft->position.Set(_root->position.x - _root->bottomLeft->size.x / 2.0f, _root->position.y, _root->position.z - _root->bottomLeft->size.z / 2.0f);

	for (int i = 0; i < _root->list.size(); i++)
	{
		if (_root->list[i]->GetComponent<Transform>()->translation.x < _root->position.x
			&& _root->list[i]->GetComponent<Transform>()->translation.z < _root->position.z
			&& _root->list[i]->GetComponent<Transform>()->translation.x > _root->position.x - _root->size.x / 2
			&& _root->list[i]->GetComponent<Transform>()->translation.z > _root->position.z - _root->size.z / 2)
		{
			_root->bottomLeft->list.push_back(_root->list[i]);
		}
	}

	if (_root->topLeft->list.size() > 5)
		Partition(_root->topLeft);
	if (_root->topRight->list.size() > 5)
		Partition(_root->topRight);
	if (_root->bottomLeft->list.size() > 5)
		Partition(_root->bottomLeft);
	if (_root->bottomRight->list.size() > 5)
		Partition(_root->bottomRight);
}
