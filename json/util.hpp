#ifndef LB_json_util_HeaderPlusPlus
#define LB_json_util_HeaderPlusPlus

#include "json.hpp"

#include <algorithm>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace LB
{
	namespace json
	{
		using range = std::multiset<value>;

		enum struct ensure_type { yes };

		bool is_numeric(value const &v) noexcept
		{
			return v == type::integer || v == type::real;
		}

		bool can_boolean(value const &v) noexcept;
		bool can_integer(value const &v) noexcept;
		bool can_real(value const &v) noexcept;
		bool can_string(value const &v) noexcept
		{
			return v == type::null
			||     v == type::boolean
			||     is_numeric(v)
			||     v == type::string;
		}
		bool can_size(value const &v) noexcept
		{
			return v == type::array || v == type::object;
		}

		bool is_true(value const &v) noexcept;
		bool is_true(value const &v, ensure_type const &)
		{
			if(can_boolean(v))
			{
				return is_true(v);
			}
			throw std::domain_error{"json value is not a boolean"};
		}
		bool is_false(value const &v) noexcept;
		bool is_false(value const &v, ensure_type const &)
		{
			if(can_boolean(v))
			{
				return is_false(v);
			}
			throw std::domain_error{"json value is not a boolean"};
		}

		integer as_integer(value const &v) noexcept;
		integer as_integer(value const &v, ensure_type const &)
		{
			if(can_integer(v))
			{
				return as_integer(v);
			}
			throw std::domain_error{"json value is not an integer"};
		}
		real as_real(value const &v) noexcept;
		real as_real(value const &v, ensure_type const &)
		{
			if(can_real(v))
			{
				return as_real(v);
			}
			throw std::domain_error{"json value is not a real"};
		}
		template<typename T>
		T as_numeric(value const &v) noexcept
		{
			if(can_integer(v))
			{
				return static_cast<T>(as_integer(v));
			}
			else if(can_real(v))
			{
				return static_cast<T>(as_real(v));
			}
			return T{};
		}
		template<typename T>
		T as_numeric(value const &v, ensure_type const &)
		{
			if(can_integer(v))
			{
				integer i = as_integer(v);
				if(i < std::numeric_limits<T>::min()
				|| i > std::numeric_limits<T>::max())
				{
					throw std::out_of_range{"json value cannot be cast to requested range"};
				}
				return static_cast<T>(i);
			}
			real r = as_real(v, ensure_type::yes);
			if(r < std::numeric_limits<T>::min()
			|| r > std::numeric_limits<T>::max())
			{
				throw std::out_of_range{"json value cannot be cast to requested range"};
			}
			return static_cast<T>(r);
		}

		string as_string(value const &v) noexcept;
		string as_string(value const &v, ensure_type const &)
		{
			if(can_string(v))
			{
				return as_string(v);
			}
			throw std::domain_error{"json value is not a string"};
		}

		std::size_t size(value const &v) noexcept;
		std::size_t size(value const &v, ensure_type const &)
		{
			if(can_size(v))
			{
				return size(v);
			}
			throw std::domain_error{"json value does not have a size"};
		}

		/*
		value const &operator[](std::size_t index) const noexcept;
		value &operator[](std::size_t index);
		array as_array() const noexcept;
		array as_array(ensure_type const &) const;
		array const &as_array(by_ref const &) const;
		array &as_array(by_ref const &);
		value const &operator[](string const &key) const noexcept;
		value &operator[](string const &key);
		range operator()(string const &key) const noexcept;
		range operator()(string const &key, ensure_type const &) const;
		object as_object() const noexcept;
		object as_object(ensure_type const &) const;
		object const &as_object(by_ref const &) const;
		object &as_object(by_ref const &);
		*/
	}
}

#endif
