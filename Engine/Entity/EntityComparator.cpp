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
	const auto halfSize = _root->size * 0.5f;

	_root->topLeft = new Quad<Entity*>;
	_root->topLeft->parent = _root;
	_root->topLeft->depth = _root->depth + 1;
	_root->topLeft->size.Set(halfSize.x, _root->size.y, halfSize.z);
	_root->topLeft->position.Set(_root->position.x - halfSize.x, _root->position.y, _root->position.z + halfSize.z);

	_root->topRight = new Quad<Entity*>;
	_root->topRight->parent = _root;
	_root->topRight->depth = _root->depth + 1;
	_root->topRight->size.Set(halfSize.x, _root->size.y, halfSize.z);
	_root->topRight->position.Set(_root->position.x + halfSize.x, _root->position.y, _root->position.z + halfSize.z);

	_root->bottomRight = new Quad<Entity*>;
	_root->bottomRight->parent = _root;
	_root->bottomRight->depth = _root->depth + 1;
	_root->bottomRight->size.Set(halfSize.x, _root->size.y, halfSize.z);
	_root->bottomRight->position.Set(_root->position.x + halfSize.x, _root->position.y, _root->position.z - halfSize.z);

	_root->bottomLeft = new Quad<Entity*>;
	_root->bottomLeft->parent = _root;
	_root->bottomLeft->depth = _root->depth + 1;
	_root->bottomLeft->size.Set(halfSize.x, _root->size.y, halfSize.z);
	_root->bottomLeft->position.Set(_root->position.x - halfSize.x, _root->position.y, _root->position.z - halfSize.z);

	for (int i = 0; i < _root->list.size(); i++)
	{
		auto transform = _root->list[i]->GetComponent<Transform>();
		const auto& translation = transform->GetWorldTranslation();
		const auto& scale = transform->scale;
		
		_root->list[i]->GetQuadList().empty();
		auto dest = _root->topLeft;

		if (translation.x < _root->position.x) {
			if (translation.z < _root->position.z) {
				dest = _root->bottomLeft;
			}
		} else {
			if (translation.z < _root->position.z) {
				dest = _root->bottomRight;
			} else {
				dest = _root->topRight;
			}
		}

		if (translation.x + scale.x < _root->position.x) {
			if (translation.z + scale.z < _root->position.z) {
				_root->list[i]->SetQuadList(_root->bottomLeft);
			}
			if (translation.z + scale.z > _root->position.z) {
				_root->list[i]->SetQuadList(_root->topLeft);
			}
		}
		if (translation.x + scale.x > _root->position.x) {
			if (translation.z + scale.z < _root->position.z) {
				_root->list[i]->SetQuadList(_root->bottomRight);
			}
			if (translation.z + scale.z > _root->position.z) {
				_root->list[i]->SetQuadList(_root->topRight);
			}
		}



		_root->list[i]->SetQuad(dest);
		dest->list.push_back(_root->list[i]);
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