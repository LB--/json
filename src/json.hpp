#ifndef LB_json_json_HeaderPlusPlus
#define LB_json_json_HeaderPlusPlus

#include <cstdint>
#include <initializer_list>
#include <map>
#include <memory>
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

		enum struct type
		{
			null,
			boolean,
			integer,
			real,
			string,
			array,
			object
		};
		inline bool operator<(type a, type b) noexcept
		{
			using ut = std::underlying_type_t<type>;
			return static_cast<ut>(a) < static_cast<ut>(b);
		}

		struct value final
		{
			using array = std::vector<value>;
			using object = std::multimap<string, value>;

			value() noexcept = delete;
			value(std::nullptr_t) noexcept
			: t{type::null}
			, p{nullptr}
			{
			}
			value(bool v)
			: t{type::boolean}
			, p{new wrap<bool>{v}}
			{
			}
			value(integer v)
			: t{type::integer}
			, p{new wrap<integer>{v}}
			{
			}
			value(real v)
			: t{type::real}
			, p{new wrap<real>{v}}
			{
			}
			value(string const &v)
			: t{type::string}
			, p{new wrap<string>{v}}
			{
			}
			value(string &&v)
			: t{type::string}
			, p{new wrap<string>{v}}
			{
			}
			value(char const *v)
			: t{type::string}
			, p{new wrap<string>{v}}
			{
			}
			value(array const &v)
			: t{type::array}
			, p{new wrap<array>{v}}
			{
			}
			value(array &&v)
			: t{type::array}
			, p{new wrap<array>{v}}
			{
			}
			template<typename ForwardIt>
			value(ForwardIt begin, ForwardIt end)
			: value(array{begin, end})
			{
			}
			value(std::initializer_list<value> v)
			: value(array{v})
			{
			}
			value(object const &v)
			: t{type::object}
			, p{new wrap<object>{v}}
			{
			}
			value(object &&v)
			: t{type::object}
			, p{new wrap<object>{v}}
			{
			}
			value(std::initializer_list<std::pair<string const, value>> v)
			: value(object{v})
			{
			}
			value(value const &v)
			: t{v.t}
			, p{v.p? v.p->clone() : nullptr}
			{
			}
			value(value &&) noexcept = default;
			value &operator=(value const &v)
			{
				t = v.t;
				p.reset(v.p? v.p->clone() : nullptr);
				return *this;
			}
			value &operator=(value &&) noexcept = default;
			~value() noexcept = default;

			friend bool operator==(value const &a, value const &b) noexcept
			{
				return a.t == b.t && *a.p == *b.p;
			}
			friend bool operator<(value const &a, value const &b) noexcept
			{
				return json::operator</*work around VC++ bug*/(a.t, b.t) || (a.t == b.t && *a.p < *b.p);
			}

			operator type() const noexcept
			{
				return t;
			}

			bool           boolean_value() const;
			bool    const &boolean_cref () const &;
			bool          &boolean_ref  ()       &;
			integer        integer_value() const;
			integer const &integer_cref () const &;
			integer       &integer_ref  ()       &;
			real           real_value   () const;
			real    const &real_cref    () const &;
			real          &real_ref     ()       &;
			string  const &string_cref  () const &;
			string        &string_ref   ()       &;
			array   const &array_cref   () const &;
			array         &array_ref    ()       &;
			object  const &object_cref  () const &;
			object        &object_ref   ()       &;

			friend value deserialize(string const &json_utf8);
			friend string serialize(value const &root, bool pretty = false);

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

				virtual bool equals(base const &) const noexcept = 0;
				virtual bool less(base const &) const noexcept = 0;
				virtual base *clone() const noexcept = 0;
			};
			std::unique_ptr<base> p;
			template<typename T>
			struct wrap final
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

			friend void serialize(value const &v, string &s, std::size_t depth);
		};

		using array = value::array;
		using object = value::object;
	}
}

#endif
