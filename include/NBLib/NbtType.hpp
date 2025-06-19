// NBT - nbt c++ library implemention.
//
// Copyright (C) 2025  vp817
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <cstdint>
#include <variant>
#include <vector>

namespace NBLib::Tag
{
	enum class NbtTypeId : std::uint8_t
	{
		END,
		BYTE,
		SHORT,
		INT,
		LONG,
		FLOAT,
		DOUBLE,
		BYTE_ARRAY,
		STRING,
		LIST,
		COMPOUND,
		UNKNOWN
	};

	struct end_tag_t
	{
	};

	struct byte_tag_t
	{
		std::int8_t value;
	};

	struct short_tag_t
	{
		std::int16_t value;
	};

	struct int_tag_t
	{
		std::int64_t value;
	};

	struct long_tag_t
	{
		std::int64_t value;
	};

	struct float_tag_t
	{
		float value;
	};

	struct double_tag_t
	{
		double value;
	};

	struct byte_array_tag_t
	{
		std::vector<std::int8_t> value;
	};

	struct string_tag_t
	{
		std::string value;
	};

	struct list_tag_t
	{
		std::vector<struct nbt_tag_t> value;
	};

	struct compound_tag_t
	{
		std::vector<struct nbt_tag_t> value;
	};

	using nbt_any_tag_t = std::variant<struct end_tag_t, struct byte_tag_t, struct short_tag_t, struct int_tag_t, struct long_tag_t, struct float_tag_t, struct double_tag_t, struct byte_array_tag_t, struct string_tag_t, struct list_tag_t, struct compound_tag_t>;

	struct nbt_tag_t
	{
		NbtTypeId type_id;
		std::string type_name;
		std::string name;
		nbt_any_tag_t tag;
	};
};
