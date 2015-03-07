#include <LB/json/.hpp>

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
			//TODO: range check the real
			if(is_boolean() || is_integer() || is_real())
			{
				return true;
			}
			else if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				//TODO: check for valid JSON number
			}
			return false;
		}
		bool value::can_real() const noexcept
		{
			//TODO: range check the integer
			if(is_integer() || is_real())
			{
				return true;
			}
			else if(auto w = dynamic_cast<wrap<string> const *>(p.get()))
			{
				//TODO: check for valid JSON number
			}
			return false;
		}
		bool value::is_true() const noexcept
		{
			//TODO
		}
		bool value::is_false() const noexcept
		{
			//TODO
		}
		integer value::as_integer() const noexcept
		{
			//TODO
		}
		real value::as_real() const noexcept
		{
			//TODO
		}
		string value::as_string() const noexcept
		{
			//TODO
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
	}
}
