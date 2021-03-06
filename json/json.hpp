#ifndef LB_json_json_HeaderPlusPlus
#define LB_json_json_HeaderPlusPlus

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace LB
{
	namespace json
	{
		enum struct deserialize_forgiveness
		{
			unforgiving,
			forgiving
		};
		struct deserialize_settings final
		{
			deserialize_forgiveness forgiveness = deserialize_forgiveness::unforgiving;
		};

		enum struct serialize_formatting
		{
			compact,
			pretty
		};
		struct serialize_settings final
		{
			serialize_formatting formatting = serialize_formatting::compact;
		};

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
			using object = std::vector<std::pair<string, value>>;

			value() noexcept = delete;
			value(std::nullptr_t) noexcept;
			value(bool v);
			value(integer v);
			value(real v);
			value(string const &v);
			value(string &&v);
			template<std::size_t N>
			value(string::value_type const (&literal)[N])
			: value(string{literal, N-1})
			{
			}
			value(array const &v);
			value(array &&v);
			value(object const &v);
			value(object &&v);
			value(value const &v);
			value(value &&) noexcept;
			value &operator=(value const &v);
			value &operator=(value &&) noexcept;
			~value() noexcept;

			friend bool operator==(value const &a, value const &b) noexcept;
			friend bool operator<(value const &a, value const &b) noexcept;

			operator type() const noexcept
			{
				return t;
			}

			bool           boolean_value() const  ;
			bool    const &boolean_cref () const &;
			bool          &boolean_ref  ()       &;
			integer        integer_value() const  ;
			integer const &integer_cref () const &;
			integer       &integer_ref  ()       &;
			real           real_value   () const  ;
			real    const &real_cref    () const &;
			real          &real_ref     ()       &;
			string  const &string_cref  () const &;
			string        &string_ref   ()       &;
			array   const &array_cref   () const &;
			array         &array_ref    ()       &;
			object  const &object_cref  () const &;
			object        &object_ref   ()       &;

			friend value deserialize(string const &json_utf8, deserialize_settings settings = {});
			friend string serialize(value const &root, serialize_settings settings = {});

		private:
			type t;
			struct base;
			std::unique_ptr<base> p;
			template<typename T>
			struct wrap;

			friend void serialize(value const &v, string &s, std::size_t depth);
		};

		using array = value::array;
		using object = value::object;
	}
}

#endif
