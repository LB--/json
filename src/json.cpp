#include "json.hpp"

namespace LB
{
	namespace json
	{
		struct value::base
		{
			virtual ~base() noexcept = default;

			friend bool operator==(base const &a, base const &b) noexcept
			{
				return a.equals(b);
			}
			friend bool operator<(base const &a, base const &b) noexcept
			{
				return a.less(b);
			}

			virtual bool equals(base const &) const noexcept = 0;
			virtual bool less(base const &) const noexcept = 0;
			virtual base *clone() const noexcept = 0;
		};
		template<typename T>
		struct value::wrap final
		: base
		{
			T v;
			wrap() noexcept
			: v{}
			{
			}
			wrap(T const &t) noexcept
			: v(t)
			{
			}
			wrap(T &&t) noexcept
			: v(t)
			{
			}
			wrap(wrap const &w) noexcept
			: v(w.v)
			{
			}
			wrap(wrap &&) noexcept = default;
			wrap &operator=(wrap const &) = delete;
			wrap &operator=(wrap &&) = delete;

			virtual bool equals(base const &p) const noexcept override
			{
				if(auto w = dynamic_cast<wrap const *>(&p))
				{
					return v == w->v;
				}
				return false;
			}
			virtual bool less(base const &p) const noexcept override
			{
				if(auto w = dynamic_cast<wrap const *>(&p))
				{
					return v < w->v;
				}
				return false;
			}
			virtual wrap *clone() const noexcept override
			{
				return new wrap{*this};
			}
		};

		value::value(std::nullptr_t) noexcept
		: t{type::null}
		, p{nullptr}
		{
		}
		value::value(bool v)
		: t{type::boolean}
		, p{new wrap<bool>{v}}
		{
		}
		value::value(integer v)
		: t{type::integer}
		, p{new wrap<integer>{v}}
		{
		}
		value::value(real v)
		: t{type::real}
		, p{new wrap<real>{v}}
		{
		}
		value::value(string const &v)
		: t{type::string}
		, p{new wrap<string>{v}}
		{
		}
		value::value(string &&v)
		: t{type::string}
		, p{new wrap<string>{v}}
		{
		}
		value::value(char const *v)
		: t{type::string}
		, p{new wrap<string>{v}}
		{
		}
		value::value(array const &v)
		: t{type::array}
		, p{new wrap<array>{v}}
		{
		}
		value::value(array &&v)
		: t{type::array}
		, p{new wrap<array>{v}}
		{
		}
		value::value(object const &v)
		: t{type::object}
		, p{new wrap<object>{v}}
		{
		}
		value::value(object &&v)
		: t{type::object}
		, p{new wrap<object>{v}}
		{
		}
		value::value(value const &v)
		: t{v.t}
		, p{v.p? v.p->clone() : nullptr}
		{
		}
		value &value::operator=(value const &v)
		{
			t = v.t;
			p.reset(v.p? v.p->clone() : nullptr);
			return *this;
		}
		value::~value() noexcept = default;

		bool operator==(value const &a, value const &b) noexcept
		{
			return a.t == b.t
			&&     a.t != type::null
			&&     *a.p == *b.p;
		}
		bool operator<(value const &a, value const &b) noexcept
		{
			return json::operator</*work around VC++ bug*/(a.t, b.t)
			||     (   a.t == b.t
			        && a.t != type::null
			        && *a.p < *b.p);
		}

		bool value::boolean_value() const
		{
			return dynamic_cast<wrap<bool> const &>(*p).v;
		}
		bool const &value::boolean_cref() const &
		{
			return dynamic_cast<wrap<bool> const &>(*p).v;
		}
		bool &value::boolean_ref() &
		{
			return dynamic_cast<wrap<bool> &>(*p).v;
		}
		integer value::integer_value() const
		{
			return dynamic_cast<wrap<integer> const &>(*p).v;
		}
		integer const &value::integer_cref() const &
		{
			return dynamic_cast<wrap<integer> const &>(*p).v;
		}
		integer &value::integer_ref() &
		{
			return dynamic_cast<wrap<integer> &>(*p).v;
		}
		real value::real_value() const
		{
			return dynamic_cast<wrap<real> const &>(*p).v;
		}
		real const &value::real_cref() const &
		{
			return dynamic_cast<wrap<real> const &>(*p).v;
		}
		real &value::real_ref() &
		{
			return dynamic_cast<wrap<real> &>(*p).v;
		}
		string const &value::string_cref() const &
		{
			return dynamic_cast<wrap<string> const &>(*p).v;
		}
		string &value::string_ref() &
		{
			return dynamic_cast<wrap<string> &>(*p).v;
		}
		array const &value::array_cref() const &
		{
			return dynamic_cast<wrap<array> const &>(*p).v;
		}
		array &value::array_ref() &
		{
			return dynamic_cast<wrap<array> &>(*p).v;
		}
		object const &value::object_cref() const &
		{
			return dynamic_cast<wrap<object> const &>(*p).v;
		}
		object &value::object_ref() &
		{
			return dynamic_cast<wrap<object> &>(*p).v;
		}

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
