#include "json.hpp"

namespace LB
{
	namespace json
	{
		string escape(string const &s)
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

		value deserialize(string const &s)
		{
			//TODO
			return nullptr;
		}
		void serialize(value const &v, string &s, std::size_t depth)
		{
			string const indent {depth? string(depth - std::size_t{1u}, '\t') : ""};
			if(v == type::null)
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
						if(e.second == type::array || e.second == type::object)
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
