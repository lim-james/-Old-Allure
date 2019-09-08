#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <iostream>

template<typename T>
class Optional {
	
	bool initialised;
	T value;

public:

	Optional() 
		: initialised(false) 
		, value(static_cast<T>(0))
	{}
	
	Optional(const T& value) 
		: initialised(true)
		, value(value) 
	{}

	const bool& HasValue() const {
		return initialised;
	}

	const T& Value() const {
		return value;
	}

	Optional<T>& operator=(const T& rhs) {
		initialised = true;
		value = rhs;
		return *this;
	}

	Optional<T>& operator=(const Optional<T>& rhs) {
		initialised = true;
		value = rhs.value;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const Optional<T>& rhs) {
		if (rhs.initialised)
			os << "Optional(" << rhs.value << ')';
		else
			os << "Optional(No Value)";

		return os;
	}

};

#endif
