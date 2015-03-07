#ifndef LB_json_HeaderPlusPlus
#define LB_json_HeaderPlusPlus

#include <algorithm>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iostream>
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
		using integer = std::intmax_t;
		using real = long double;
		using string = std::string;

		enum struct ensure_type { yes };
		enum struct by_ref { yes };

		enum struct type
		{
			invalid,
			null,
			boolean,
			integer,
			real,
			string,
			array,
			object
		};
		bool operator<(type a, type b) noexcept
		{
			using ut = typename std::underlying_type<type>::type;
			return static_cast<ut>(a) < static_cast<ut>(b);
		}

		struct value final
		{
			using array = std::vector<value>;
			using object = std::multimap<string, value>;
			using range = std::multiset<value>;

			value() noexcept
			: t{type::invalid}
			, p{nullptr}
			{
			}
			value(std::nullptr_t) noexcept
			: t{type::null}
			, p{nullptr}
			{
			}
			value(bool v) noexcept
			: t{type::boolean}
			, p{new wrap<bool>{v}}
			{
			}
			value(integer v) noexcept
			: t{type::integer}
			, p{new wrap<integer>{v}}
			{
			}
			value(real v) noexcept
			: t{type::real}
			, p{new wrap<real>{v}}
			{
			}
			value(string const &v) noexcept
			: t{type::string}
			, p{new wrap<string>{v}}
			{
			}
			value(string &&v) noexcept
			: t{type::string}
			, p{new wrap<string>{v}}
			{
			}
			value(array const &v) noexcept
			: t{type::array}
			, p{new wrap<array>{v}}
			{
			}
			value(array &&v) noexcept
			: t{type::array}
			, p{new wrap<array>{v}}
			{
			}
			template<typename ForwardIt>
			value(ForwardIt begin, ForwardIt end) noexcept
			: value{array{begin, end}}
			{
			}
			value(std::initializer_list<value> v) noexcept
			: value{array{v}}
			{
			}
			value(object const &v) noexcept
			: t{type::object}
			, p{new wrap<object>{v}}
			{
			}
			value(object &&v) noexcept
			: t{type::object}
			, p{new wrap<object>{v}}
			{
			}
			value(std::initiaizer_list<std::pair<string, value>> v) noexcept
			: value{object{v}}
			{
			}
			value(value const &v) noexcept
			: t{v.t}
			, p{v.p->clone()}
			{
			}
			value(value &&) noexcept = default;
			value &operator=(value const &v) noexcept
			{
				t = v.t;
				p.reset(v.p->clone());
			}
			value &operator=(value &&) noexcept = default;
			~value() noexcept = default;

			friend bool operator==(value const &a, value const &b) noexcept
			{
				return a.t == b.t && *a.p == *b.p;
			}
			friend bool operator<(value const &a, value const &b) noexcept
			{
				return a.t < b.t || (a.t == b.t && *a.p < *b.p);
			}

			operator type() const noexcept
			{
				return t;
			}

			operator bool() const noexcept
			{
				return is_valid();
			}
			bool is_valid() const noexcept
			{
				return (t == type::null) != p;
			}
			bool is_null() const noexcept
			{
				return t == type::null;
			}
			bool is_boolean() const noexcept
			{
				return t == type::boolean;
			}
			bool is_numeric() const noexcept
			{
				return is_integer() || is_real();
			}
			bool is_integer() const noexcept
			{
				return t == type::integer;
			}
			bool is_real() const noexcept
			{
				return t == type::real;
			}
			bool is_string() const noexcept
			{
				return t == type::string;
			}
			bool is_array() const noexcept
			{
				return t == type::array;
			}
			bool is_object() const noexcept
			{
				return t == type::object;
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
				//TODO: range check and throw std::out_of_bounds
				if(can_integer())
				{
					return static_cast<T>(as_integer());
				}
				return static_cast<T>(as_real(ensure_type::yes));
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

			value operator[](std::size_t index) const noexcept;
			value &operator[](std::size_t index);
			array as_array() const noexcept;
			array as_array(ensure_type const &) const;
			array const &as_array(by_ref const &) const;
			array &as_array(by_ref const &);
			value operator[](string const &key) const noexcept;
			value &operator[](string const &key) noexcept;
			range operator()(string const &key) const noexcept;
			range operator()(string const &key, ensure_type const &) const;
			object as_object() const noexcept;
			object as_object(ensure_type const &) const;
			object const &as_object(by_ref const &) const;
			object &as_object(by_ref const &);

		private:
			type t;
			struct base
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

				virtual bool equals(base const *) const noexcept = 0;
				virtual bool less(base const *) const noexcept = 0;
				virtual base *clone() const noexcept = 0;
			};
			std::unique_ptr<base> p;
			template<typename T>
			struct wrap final
			: base
			{
				T v;

				virtual bool equals(base const *p) const noexcept override
				{
					if(auto w = dynamic_cast<wrap const *>(p))
					{
						return v == w->v;
					}
					return false;
				}
				virtual bool less(base const &p) const noexcept override
				{
					if(auto w = dynamic_cast<wrap const *>(p))
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
		};

		using array = value::array;
		using object = value::object;
		using range = value::range;
	}
}

#endif
