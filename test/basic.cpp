#include "json.hpp"

#include <iostream>

namespace json = LB::json;

int main()
{
	std::cout << serialize(json::value(json::array{true, nullptr, "test", json::integer{7}})) << std::endl;
}
