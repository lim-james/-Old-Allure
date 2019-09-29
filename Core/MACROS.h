#ifndef MACROS_H
#define MACROS_H

#define __FILENAME__ strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__
#define indexof(type) std::type_index(typeid(type))

#endif