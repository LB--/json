#include "json.hpp"

#include <cctype>
#include <iomanip>
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
				string const &s = w->v; //TODO: lowercase
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
				string const &s = w->v; //TODO: lowercase
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
				string const &s = w->v; //TODO: lowercase
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
				return w->v? 1 : 0;
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
				return w->v? "true" : "false";
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
		value const &value::operator[](string const &key) const noexcept
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
		value &value::operator[](string const &key)
		{
			if(auto w = dynamic_cast<wrap<object> *>(p.get()))
			{
				return w->v.equal_range(key).first->second;
			}
			throw std::domain_error{"json value is not an object"};
		}
		range value::operator()(string const &key) const noexcept
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
		range value::operator()(string const &key, ensure_type const &) const
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

		string escape(string const &s) noexcept
		{
			string ret;
			for(auto it = std::cbegin(s); it != std::cend(s); ++it)
			{
				if(*it == '"')
				{
					ret += "\\\"";
				}
				else if(*it == '\\')
				{
					ret += "\\\\";
				}
				else if(*it == '\b')
				{
					ret += "\\b";
				}
				else if(*it == '\f')
				{
					ret += "\\f";
				}
				else if(*it == '\n')
				{
					ret += "\\n";
				}
				else if(*it == '\r')
				{
					ret += "\\r";
				}
				else if(*it == '\t')
				{
					ret += "\\t";
				}
				else if(*it & 0b10000000 || !std::isprint(*it))
				{
					/*TODO
					std::uint16_t n = static_cast<unsigned char>(it->str()[0]);
					if(it->length() > 1)
					{
						n |= static_cast<std::uint16_t>(static_cast<unsigned char>(it->str()[1])) << 8;
					}
					ret += "\\u";
					std::ostringstream os;
					os << std::hex << std::setw(4) << std::setfill('0') << n;
					ret += os.str();
					*/
				}
				else
				{
					ret += *it;
				}
			}
			return ret;
		}
		string unescape(string const &s, bool exc = true)
		{
			string ret;
			for(auto it = std::cbegin(s); it != std::cend(s); ++it)
			{
				if(*it == '\\')
				{
					if(++it == std::cend(s))
					{
						if(exc)
						{
							throw std::range_error{"invalid escape at end of string"};
						}
						break;
					}
					if(*it == '\"'
					|| *it == '\\'
					|| *it == '/')
					{
						ret += *it;
					}
					else if(*it == 'b')
					{
						ret += '\b';
					}
					else if(*it == 'f')
					{
						ret += '\f';
					}
					else if(*it == 'n')
					{
						ret += '\n';
					}
					else if(*it == 'r')
					{
						ret += '\r';
					}
					else if(*it == 't')
					{
						ret += '\t';
					}
					else if(*it == 'u')
					{
						/*TODO
						string hex;
						for(std::size_t i = 0; i < 4; ++i)
						{
							if(++it == std::end(map) || it->length() > 1)
							{
								if(exc)
								{
									throw std::range_error{"invalid Unicode escape at end of string"};
								}
								return ret;
							}
							hex += *it;
						}
						std::uint16_t n;
						std::istringstream is {hex};
						if(!(is >> std::hex >> n) && exc)
						{
							throw std::range_error{"invalid Unicode escape"};
						}
						ret += static_cast<char>(static_cast<unsigned char>(n >> 8));
						ret += static_cast<char>(static_cast<unsigned char>(n));
						*/
					}
					else if(exc)
					{
						throw std::range_error{"invalid escape"};
					}
				}
				else
				{
					ret += *it;
				}
			}
			return ret;
		}

		std::istream &operator>>(std::istream &is, value &v) noexcept
		{
			//TODO
			return is;
		}
		std::ostream &operator<<(std::ostream &os, value const &v) noexcept
		{
			if(v.is_null())
			{
				return os << "null";
			}
			else if(auto w = dynamic_cast<value::wrap<bool> const *>(v.p.get()))
			{
				if(w->v)
				{
					return os << "true";
				}
				return os << "false";
			}
			else if(auto w = dynamic_cast<value::wrap<integer> const *>(v.p.get()))
			{
				return os << w->v;
			}
			else if(auto w = dynamic_cast<value::wrap<real> const *>(v.p.get()))
			{
				return os << w->v;
			}
			else if(auto w = dynamic_cast<value::wrap<string> const *>(v.p.get()))
			{
				os << '"' << escape(w->v) << '"';
			}
			else if(auto w = dynamic_cast<value::wrap<array> const *>(v.p.get()))
			{
				os << '[';
				bool first = true;
				for(auto const &e : w->v)
				{
					if(!e)
					{
						continue;
					}
					if(!first)
					{
						os << ',';
					}
					first = false;
					os << e;
				}
				return os << ']';
			}
			else if(auto w = dynamic_cast<value::wrap<object> const *>(v.p.get()))
			{
				os << '{';
				bool first = true;
				for(auto const &e : w->v)
				{
					if(!e.second)
					{
						continue;
					}
					if(!first)
					{
						os << ',';
					}
					first = false;
					os << '"' << escape(e.first) << "\":" << e.second;
				}
				return os << '}';
			}
			return os;
		}
		value deserialize(string const &s, value &v)
		{
			//TODO
			return v;
		}
		void serialize(value const &v, string &s, std::size_t depth)
		{
			if(!v)
			{
				throw std::domain_error{"cannot serialize invalid json value"};
			}
			string const indent {depth? string(depth - std::size_t{1u}, '\t') : ""};
			if(v.is_null())
			{
				s += "null";
			}
			else if(auto w = dynamic_cast<value::wrap<bool> const *>(v.p.get()))
			{
				if(w->v)
				{
					s += indent + "true";
				}
				else
				{
					s += indent + "false";
				}
			}
			else if(auto w = dynamic_cast<value::wrap<integer> const *>(v.p.get()))
			{
				s += std::to_string(w->v);
			}
			else if(auto w = dynamic_cast<value::wrap<real> const *>(v.p.get()))
			{
				s += std::to_string(w->v);
			}
			else if(auto w = dynamic_cast<value::wrap<string> const *>(v.p.get()))
			{
				s += '"' + escape(w->v) + '"';
			}
			else if(auto w = dynamic_cast<value::wrap<array> const *>(v.p.get()))
			{
				s += (depth? "[\n" : "[");
				bool first = true;
				for(auto const &e : w->v)
				{
					if(!first)
					{
						s += (depth? ",\n" : ",");
					}
					first = false;
					if(depth)
					{
						s += indent + '\t';
					}
					serialize(e, s, (depth? depth + 1 : depth));
				}
				s += indent + ']';
			}
			else if(auto w = dynamic_cast<value::wrap<object> const *>(v.p.get()))
			{
				s += (depth? "{\n" : "{");
				bool first = true;
				for(auto const &e : w->v)
				{
					if(!first)
					{
						s += (depth? ",\n" : ",");
					}
					first = false;
					if(depth)
					{
						s += indent + '\t';
					}
					s += '"' + escape(e.first) + "\":";
					if(depth)
					{
						if(e.second.is_array() || e.second.is_object())
						{
							s += '\n' + indent;
						}
						else
						{
							s += ' ';
						}
					}
					serialize(e.second, s, (depth? depth + 1 : depth));
				}
				s += indent + '}';
			}
		}
		string serialize(value const &v, bool pretty)
		{
			string s;
			serialize(v, s, (pretty? 1 : 0));
			return s;
		}
	}
}
