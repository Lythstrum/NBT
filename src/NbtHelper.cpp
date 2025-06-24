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

#include <NBLib/NbtHelper.hpp>
#include <stdexcept>

NBLib::NbtHelper::CompoundTagWrapper::CompoundTagWrapper(Tag::compound_tag_t &tag)
	: tag(tag)
{
}

NBLib::Tag::nbt_any_tag_t NBLib::NbtHelper::CompoundTagWrapper::getTag(std::string name, Tag::NbtTypeId type_id)
{
	Tag::nbt_any_tag_t result = std::monostate {};
	for (std::size_t i = 0; i < this->tag.value.size(); i++) {
		Tag::nbt_tag_t nbt_tag = this->tag.value.at(i);
		if (nbt_tag.name == name && nbt_tag.type_id == type_id)
			result = nbt_tag.tag;
	}
	return result;
}

NBLib::Tag::nbt_any_tag_t *NBLib::NbtHelper::makeTagPtr(NBLib::Tag::nbt_any_tag_t value)
{
	NBLib::Tag::nbt_any_tag_t *result = &value;
	return result;
}

NBLib::Tag::nbt_tag_t NBLib::NbtHelper::createTag(std::string name, Tag::nbt_any_tag_t tag)
{
	if (std::holds_alternative<Tag::end_tag_t>(tag))
		throw new std::invalid_argument("Unable to create tag due to invalid tag argument.");
	struct Tag::nbt_tag_t result = {};
	result.name = name;
	if (std::holds_alternative<Tag::byte_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::BYTE;
	else if (std::holds_alternative<Tag::short_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::SHORT;
	else if (std::holds_alternative<Tag::int_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::INT;
	else if (std::holds_alternative<Tag::long_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::LONG;
	else if (std::holds_alternative<Tag::float_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::FLOAT;
	else if (std::holds_alternative<Tag::double_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::DOUBLE;
	else if (std::holds_alternative<Tag::byte_array_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::BYTE_ARRAY;
	else if (std::holds_alternative<Tag::string_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::STRING;
	else if (std::holds_alternative<Tag::list_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::LIST;
	else if (std::holds_alternative<Tag::compound_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::COMPOUND;
	else if (std::holds_alternative<Tag::int_array_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::INT_ARRAY;
	else if (std::holds_alternative<Tag::long_array_tag_t>(tag))
		result.type_id = Tag::NbtTypeId::LONG_ARRAY;
	result.tag = tag;
	return result;
}
