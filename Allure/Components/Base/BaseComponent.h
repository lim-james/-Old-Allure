#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

class Entity;

struct BaseComponent {

	Entity* parent;

	BaseComponent();
	virtual ~BaseComponent();

	void SetActive(const bool& state);
	const bool& IsActive() const;

private:

	bool active;

};

#endif