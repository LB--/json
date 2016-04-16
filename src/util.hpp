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
		enum struct by_ref { yes };

		bool is_numeric() const noexcept
		{
			return is_integer() || is_real();
		}

		bool can_boolean() const noexcept;
		bool can_integer() const noexcept;
		bool can_real() const noexcept;
		bool can_string() const noexcept
		{
			return is_null() || is_boolean() || is_numeric() || is_string();
		}
		bool can_size() const noexcept
		{
			return is_array() || is_object();
		}

		bool is_true() const noexcept;
		bool is_true(ensure_type const &) const
		{
			if(can_boolean())
			{
				return is_true();
			}
			throw std::domain_error{"json value is not a boolean"};
		}
		bool is_false() const noexcept;
		bool is_false(ensure_type const &) const
		{
			if(can_boolean())
			{
				return is_false();
			}
			throw std::domain_error{"json value is not a boolean"};
		}

		template<typename T>
		T as_numeric() const noexcept
		{
			if(can_integer())
			{
				return static_cast<T>(as_integer());
			}
			else if(can_real())
			{
				return static_cast<T>(as_real());
			}
			return T{};
		}
		template<typename T>
		T as_numeric(ensure_type const &) const
		{
			if(can_integer())
			{
				integer i = as_integer();
				if(i < std::numeric_limits<T>::min()
				|| i > std::numeric_limits<T>::max())
				{
					throw std::out_of_range{"json value cannot be cast to requested range"};
				}
				return static_cast<T>(i);
			}
			real r = as_real(ensure_type::yes);
			if(r < std::numeric_limits<T>::min()
			|| r > std::numeric_limits<T>::max())
			{
				throw std::out_of_range{"json value cannot be cast to requested range"};
			}
			return static_cast<T>(r);
		}
		integer as_integer() const noexcept;
		integer as_integer(ensure_type const &) const
		{
			if(can_integer())
			{
				return as_integer();
			}
			throw std::domain_error{"json value is not an integer"};
		}
		real as_real() const noexcept;
		real as_real(ensure_type const &) const
		{
			if(can_real())
			{
				return as_real();
			}
			throw std::domain_error{"json value is not a real"};
		}

		string as_string() const noexcept;
		string as_string(ensure_type const &) const
		{
			if(can_string())
			{
				return as_string();
			}
			throw std::domain_error{"json value is not a string"};
		}
		string const &as_string(by_ref const &) const;
		string &as_string(by_ref const &);

		std::size_t size() const noexcept;
		std::size_t size(ensure_type const &) const
		{
			if(can_size())
			{
				return size();
			}
			throw std::domain_error{"json value does not have a size"};
		}

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
	}
}

#endif
