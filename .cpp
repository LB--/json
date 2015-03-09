#include <LB/json/.hpp>

#include <limits>
#include <sstream>

namespace LB
{
	namespace json
	{
		bool value::can_boolean() const noexcept
		{
			if(is_boolean())
			{
				return true;
			}
			else if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				string const &s = w->v;
				//TODO: convert to lowercase
				//http://www.boost.org/doc/libs/1_57_0/libs/locale/doc/html/conversions.html
				return s == "true" || s == "false" || s == "1" || s == "0";
			}
			else if(auto w = dynamic_cast<wrap<integer> const *>(p.get()))
			{
				return w->v == 1 || w->v == 0;
			}
			return false;
		}
		bool value::can_integer() const noexcept
		{
			if(is_boolean() || is_integer())
			{
				return true;
			}
			else if(auto w = dynamic_cast<wrap<real> const *>(p.get()))
			{
				return w->v > std::numeric_limits<integer>::min()
				&&     w->v < std::numeric_limits<integer>::max();
			}
			else if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				std::istringstream s {w->v};
				integer i;
				string t;
				return (s >> i) && !(s >> t);
			}
			return false;
		}
		bool value::can_real() const noexcept
		{
			if(is_real())
			{
				return true;
			}
			else if(auto w = dynamic_cast<wrap<integer> const *>(p.get()))
			{
				return w->v > std::numeric_limits<real>::min()
				&&     w->v < std::numeric_limits<real>::max();
			}
			else if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				std::istringstream s {w->v};
				real r;
				string t;
				return (s >> r) && !(s >> t);
			}
			return false;
		}
		bool value::is_true() const noexcept
		{
			if(auto w = dynamic_cast<wrap<bool> const *>(p.get()))
			{
				return w->v;
			}
			else if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				string const &s = w->v;
				//TODO: convert to lowercase
				//http://www.boost.org/doc/libs/1_57_0/libs/locale/doc/html/conversions.html
				return s == "true" || s == "1";
			}
			else if(auto w = dynamic_cast<wrap<integer> const *>(p.get()))
			{
				return w->v == 1;
			}
			return false;
		}
		bool value::is_false() const noexcept
		{
			if(auto w = dynamic_cast<wrap<bool> const *>(p.get()))
			{
				return w->v;
			}
			else if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				string const &s = w->v;
				//TODO: convert to lowercase
				//http://www.boost.org/doc/libs/1_57_0/libs/locale/doc/html/conversions.html
				return s == "false" || s == "0";
			}
			else if(auto w = dynamic_cast<wrap<integer> const *>(p.get()))
			{
				return w->v == 0;
			}
			return false;
		}
		integer value::as_integer() const noexcept
		{
			if(auto w = dynamic_cast<wrap<integer> const *>(p.get()))
			{
				return w->v;
			}
			else if(auto w = dynamic_cast<wrap<bool> const *>(p.get()))
			{
				return w->v ? 1 : 0;
			}
			else if(auto w = dynamic_cast<wrap<real> const *>(p.get()))
			{
				return static_cast<integer>(w->v);
			}
			else if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				std::istringstream s {w->v};
				integer i;
				string t;
				if((s >> i) && !(s >> t))
				{
					return i;
				}
			}
			return {};
		}
		real value::as_real() const noexcept
		{
			if(auto w = dynamic_cast<wrap<real> const *>(p.get()))
			{
				return w->v;
			}
			else if(auto w = dynamic_cast<wrap<integer> const *>(p.get()))
			{
				return static_cast<real>(w->v);
			}
			else if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				std::istringstream s {w->v};
				real r;
				string t;
				if((s >> r) && !(s >> t))
				{
					return r;
				}
			}
			return {};
		}
		string value::as_string() const noexcept
		{
			if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				return w->v;
			}
			else if(auto w = dynamic_cast<wrap<integer> const *>(p.get()))
			{
				return std::to_string(w->v);
			}
			else if(auto w = dynamic_cast<wrap<real> const *>(p.get()))
			{
				return std::to_string(w->v);
			}
			else if(auto w = dynamic_cast<wrap<bool> const *>(p.get()))
			{
				return w->v ? "true" : "false";
			}
			else if(is_null())
			{
				return "null";
			}
			return {};
		}
		string const &value::as_string(by_ref const &) const
		{
			if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not a string"};
		}
		string &value::as_string(by_ref const &)
		{
			if(auto w = dynamic_cast<wrap<string> *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not a string"};
		}
		std::size_t value::size() const noexcept
		{
			if(auto w = dynamic_cast<wrap<array> const *>(p.get()))
			{
				return w->v.size();
			}
			else if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				return w->v.size();
			}
			return {};
		}
		static value const &invalid_value() noexcept
		{
			static value v;
			return v;
		}
		value const &value::operator[](std::size_t index) const noexcept
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
		value &value::operator[](std::size_t index)
		{
			if(auto w = dynamic_cast<wrap<array> *>(p.get()))
			{
				return w->v.at(index);
			}
			throw std::domain_error{"json value is not an array"};
		}
		array value::as_array() const noexcept
		{
			if(auto w = dynamic_cast<wrap<array> const *>(p.get()))
			{
				return w->v;
			}
			return {};
		}
		array value::as_array(ensure_type const &) const
		{
			if(auto w = dynamic_cast<wrap<array> const *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an array"};
		}
		array const &value::as_array(by_ref const &) const
		{
			if(auto w = dynamic_cast<wrap<array> const *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an array"};
		}
		array &value::as_array(by_ref const &)
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
					return w->v.at(key);
				}
			}
			return invalid_value();
		}
		value &value::operator[](string const &key)
		{
			if(auto w = dynamic_cast<wrap<object> *>(p.get()))
			{
				return w->v.at(key);
			}
			throw std::domain_error{"json value is not an object"};
		}
		range value::operator()(string const &key) const noexcept
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				auto r = w->v.equal_range(key);
				return {r.first, r.second};
			}
			return {};
		}
		range value::operator()(string const &key, ensure_type const &) const
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				auto r = w->v.equal_range(key);
				return {r.first, r.second};
			}
			throw std::domain_error{"json value is not an object"};
		}
		object value::as_object() const noexcept
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				return w->v;
			}
			return {};
		}
		object value::as_object(ensure_type const &) const
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an object"};
		}
		object const &value::as_object(by_ref const &) const
		{
			if(auto w = dynamic_cast<wrap<object> const *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an object"};
		}
		object &value::as_object(by_ref const &)
		{
			if(auto w = dynamic_cast<wrap<object> *>(p.get()))
			{
				return w->v;
			}
			throw std::domain_error{"json value is not an object"};
		}
	}
}
