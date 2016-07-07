#include "json.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>

namespace json = LB::json;

int main()
{
	json::value v
	(
		json::object
		{
			{"test", json::array{nullptr, true, json::integer{7}, json::real{3.141592653589793238462643383279L}, "test", json::array{}, json::object{}}},
			{"test", nullptr},
			{"integer zero", json::integer{0}},
			{"real zero", json::real{0.0}},
			{"null character in string", json::value{"a\0b"}},
			{"max integer", std::numeric_limits<json::integer>::max()},
			{"min integer", std::numeric_limits<json::integer>::min()},
			{"max real", std::numeric_limits<json::real>::max()},
			{"min real", std::numeric_limits<json::real>::min()},
		}
	);
	std::cout
		<< serialize(v) << std::endl
		<< serialize(v, {json::serialize_formatting::pretty}) << std::endl;

	return EXIT_SUCCESS;
}
