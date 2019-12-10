#ifndef QUAD_HPP
#define QUAD_HPP

#include "Math/Vectors.hpp"

#include <iostream>
#include <vector>

template<typename T>
class Quad
{
public:
	//Quad() {};
	//~Quad() {};

	Quad *parent;
	Quad *topLeft;
	Quad *topRight;
	Quad *bottomLeft;
	Quad *bottomRight;

	vec3f size;
	vec3f position;

	std::vector<T> list;

	int depth;

	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const Quad<T>& quad);
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Quad<T>& quad) {
	for (int i = 0; i < quad.depth; ++i)
		os << '\t';
	os << "Size: (" << quad.size << ")\n";
	for (int i = 0; i < quad.depth; ++i)
		os << '\t';
	os << "Position: (" << quad.position << ")\n";
	for (int i = 0; i < quad.depth; ++i)
		os << '\t';
	os << "Count: " << quad.list.size() << '\n';

	if (quad.topLeft)
		os << *quad.topLeft;

	if (quad.topRight)
		os << *quad.topRight;

	if (quad.bottomLeft)
		os << *quad.bottomLeft;

	if (quad.bottomRight)
		os << *quad.bottomRight;

	return os;
}

template<typename T>
struct Comparator
{
	virtual void Partition(Quad<T>* _root) = 0;
	//{
		//_root->topLeft = new Quad;
		//_root->topLeft->position.Set(_root->position);
		//_root->topLeft->size.Set(_root->size / 2.0f);
		//
		//for (int i = 0; i < _root->list.size(); i++)
		//{
		//	if (_root->list[i].GetComponent<Transform>()->position)
		//}
	//}
};

template<typename ContentType, typename ComparatorType>
class QuadTree
{
public:
	const unsigned int MAX_GAMEOBJECTS;
	const unsigned int MAX_DEPTH;

	Comparator<ContentType>* algo;
	Quad<ContentType>* root;

	QuadTree(int _maxGameObjects) 
		: MAX_GAMEOBJECTS(_maxGameObjects)
		, MAX_DEPTH(1) {
		algo = new ComparatorType;
		root = new Quad<ContentType>;
	}

	void Sort() {
		if (root->list.size() > MAX_GAMEOBJECTS)
			algo->Partition(root);
	}

	template<typename ContentType, typename ComparatorType>
	friend std::ostream& operator<<(std::ostream& os, const QuadTree<ContentType, ComparatorType>& tree);
};

template<typename ContentType, typename ComparatorType>
std::ostream& operator<<(std::ostream& os, const QuadTree<ContentType, ComparatorType>& tree) {
	os << "MAX GO: " << tree.MAX_GAMEOBJECTS << '\n';
	os << *tree.root << '\n';
	return os;
}

#endif // !QUAD_HPP

