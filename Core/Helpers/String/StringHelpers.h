#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <vector>
#include <string>
#include <sstream>

namespace Helpers {

	template<typename T>
	T Parse(const std::string& content) {
		std::istringstream iss(content);
		T result;
		iss >> result;
		return result;
	}

	std::vector<std::string> Split(const std::string& content, const char& delimiter);

	template<typename T>
	std::vector<T> Split(const std::string& content, const char& delimiter) {
		std::vector<T> parts;

		std::string buffer;
		std::istringstream iss(content);
		
		while (std::getline(iss, buffer, delimiter))
			parts.push_back(Parse<T>(buffer));

		return parts;
	}

	std::string Join(const std::vector<std::string>& list);
	std::string Join(const std::vector<std::string>& list, const char& separator);
	std::string Join(const std::vector<std::string>& list, const std::string& separator);

	template<typename Separator>
	std::pair<std::string, std::string> Pair(const std::string& content, const Separator& separator);
	template<typename Key, typename Value, typename Separator>
	std::pair<Key, Value> Pair(const std::string& content, const Separator& separator);
	
	std::string Trim(std::string content);

}

template<typename Separator>
std::pair<std::string, std::string> Helpers::Pair(const std::string& content, const Separator& separator) {
	const auto position = content.find(separator);
	return std::make_pair(content.substr(0, position), content.substr(position + 1));
}

template<typename Key, typename Value, typename Separator>
std::pair<Key, Value> Helpers::Pair(const std::string& content, const Separator& separator) {
	const auto position = content.find(separator);
	return std::make_pair(
		Parse(content.substr(0, position)),
		Parse(content.substr(position + 1))
	);
}

#endif