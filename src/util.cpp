#include "util.hpp"

#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>

namespace LB
{
	namespace json
	{
		bool can_boolean(value const &v) noexcept
		{
			if(v == type::boolean)
			{
				return true;
			}
			else if(v == type::string)
			{
				auto const &s = v.string_cref(); //TODO: lowercase
				return s == "true"
				||     s == "false"
				||     s == "1"
				||     s == "0";
			}
			else if(v == type::integer)
			{
				auto i = v.integer_value();
				return i == 1
				||     i == 0;
			}
			return false;
		}
		bool can_integer(value const &v) noexcept
		{
			if(v == type::boolean || v == type::integer)
			{
				return true;
			}
			else if(v == type::real)
			{
				auto r = v.real_value();
				return r > std::numeric_limits<integer>::min()
				&&     r < std::numeric_limits<integer>::max();
			}
			else if(v == type::string)
			{
				std::istringstream s {v.string_cref()};
				integer i;
				string t;
				return (s >> i) && !(s >> t);
			}
			return false;
		}
		bool can_real(value const &v) noexcept
		{
			if(v == type::real)
			{
				return true;
			}
			else if(v == type::integer)
			{
				auto i = v.integer_value();
				return i > std::numeric_limits<real>::min()
				&&     i < std::numeric_limits<real>::max();
			}
			else if(v == type::string)
			{
				std::istringstream s {v.string_cref()};
				real r;
				string t;
				return (s >> r) && !(s >> t);
			}
			return false;
		}
		bool is_true(value const &v) noexcept
		{
			if(v == type::boolean)
			{
				return v.boolean_value();
			}
			else if(v == type::string)
			{
				auto const &s = v.string_cref(); //TODO: lowercase
				return s == "true" || s == "1";
			}
			else if(v == type::integer)
			{
				return v.integer_value() == 1;
			}
			return false;
		}
		bool is_false(value const &v) noexcept
		{
			if(v == type::boolean)
			{
				return v.boolean_value();
			}
			else if(v == type::string)
			{
				auto const &s = v.string_cref(); //TODO: lowercase
				return s == "false" || s == "0";
			}
			else if(v == type::integer)
			{
				return v.integer_value() == 0;
			}
			return false;
		}
		integer as_integer(value const &v) noexcept
		{
			if(v == type::integer)
			{
				return v.integer_value();
			}
			else if(v == type::boolean)
			{
				return v.boolean_value()? 1 : 0;
			}
			else if(v == type::real)
			{
				return static_cast<integer>(v.real_value());
			}
			else if(v == type::string)
			{
				std::istringstream s {v.string_cref()};
				integer i;
				string t;
				if((s >> i) && !(s >> t))
				{
					return i;
				}
			}
			return {};
		}
		real as_real(value const &v) noexcept
		{
			if(v == type::real)
			{
				return v.real_value();
			}
			else if(v == type::integer)
			{
				return static_cast<real>(v.integer_value());
			}
			else if(v == type::string)
			{
				std::istringstream s {v.string_cref()};
				real r;
				string t;
				if((s >> r) && !(s >> t))
				{
					return r;
				}
			}
			return {};
		}
		string as_string(value const &v) noexcept
		{
			if(v == type::string)
			{
				return v.string_cref();
			}
			else if(v == type::integer)
			{
				return std::to_string(v.integer_value());
			}
			else if(v == type::real)
			{
				return std::to_string(v.real_value());
			}
			else if(v == type::boolean)
			{
				return v.boolean_value()? "true" : "false";
			}
			else if(v == type::null)
			{
				return "null";
			}
			return {};
		}
		std::size_t size(value const &v) noexcept
		{
			if(v == type::array)
			{
				return v.array_cref().size();
			}
			else if(v == type::object)
			{
				return v.object_cref().size();
			}
			return {};
		}
		/*
		value const &operator[](std::size_t index) const noexcept
		{
			if(auto w = dynamic_cast<wrap<array> const *>(p.get()))
			{
				if(index < w->v.size())
				{
					return w->v[index];
				}
			}
			return invalid_value();
		}
		value &operator[](std::size_t index)
		{
			if(auto w = dynamic_cast<wrap<array> *>(p.get()))
			{
				return w->v.at(index);
			}
			throw std::domain_error{"json value is not an array"};
		}
		array as_array() const noexcept
		{
			if(auto w = dynamic_cast<wrap<array> const *>(p.get()))
			{
				return w->v;
			}
			return {};
		}
		array as_array(ensure_type const &) const
		{
			if(auto w = dynamic_cast<wrap<array> const *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an array"};
		}
		array const &as_array(by_ref const &) const
		{
			if(auto w = dynamic_cast<wrap<array> const *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an array"};
		}
		array &as_array(by_ref const &)
		{
			if(auto w = dynamic_cast<wrap<array> *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an array"};
		}
		value const &operator[](string const &key) const noexcept
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				if(w->v.find(key) != std::end(w->v))
				{
					return w->v.equal_range(key).first->second;
				}
			}
			return invalid_value();
		}
		value &operator[](string const &key)
		{
			if(auto w = dynamic_cast<wrap<object> *>(p.get()))
			{
				return w->v.equal_range(key).first->second;
			}
			throw std::domain_error{"json value is not an object"};
		}
		range operator()(string const &key) const noexcept
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				auto r = w->v.equal_range(key);
				range s;
				for(auto it = r.first; it != r.second; ++it)
				{
					s.emplace(it->second);
				}
				return s;
			}
			return {};
		}
		range operator()(string const &key, ensure_type const &) const
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				auto r = w->v.equal_range(key);
				range s;
				for(auto it = r.first; it != r.second; ++it)
				{
					s.emplace(it->second);
				}
				return s;
			}
			throw std::domain_error{"json value is not an object"};
		}
		object as_object() const noexcept
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				return w->v;
			}
			return {};
		}
		object as_object(ensure_type const &) const
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an object"};
		}
		object const &as_object(by_ref const &) const
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an object"};
		}
		object &as_object(by_ref const &)
		{
			if(auto w = dynamic_cast<wrap<object> *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an object"};
		}
		*/
	}
}
