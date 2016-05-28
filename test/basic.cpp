#include "json.hpp"

#include <iostream>

namespace json = LB::json;

int main()
{
	json::value v
	(
		json::object
		{
			{"test", json::array{nullptr, true, json::integer{7}, json::real{3.141592654}, "test", json::array{}, json::object{}}},
			{"test", nullptr}
		}
	);
	std::cout
		<< serialize(v) << std::endl
		<< serialize(v, {json::serialize_formatting::pretty}) << std::endl;
}
