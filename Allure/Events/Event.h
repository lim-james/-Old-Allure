#ifndef EVENT_H
#define EVENT_H

#include <string>

namespace Events {

	struct Base { 
		std::string name;
	};

	template<typename Data>
	struct Event : public Base {
		Data data;

		Event(const Data& data)
			: data(data)
		{}
	};

}

#endif
