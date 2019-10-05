#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

struct Component {

	Entity* parent;

	Component();
	virtual ~Component();

	virtual void Initialize();

	virtual void SetActive(const bool& state);
	const bool& IsActive() const;

protected:

	bool active;

};

#endif