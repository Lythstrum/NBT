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

#include <NBLib/NbtStream.hpp>
#include <iostream> // -------------TMP

NBLib::NbtStream::NbtStream(NbtStreamType nbt_type, BMLib::Buffer *buffer, std::size_t position)
	: nbt_type(nbt_type), BMLib::BinaryStream(buffer, position)
{
}

NBLib::Tag::NbtTypeId NBLib::NbtStream::readTagTypeId()
{
	std::uint8_t type_id = this->read<std::uint8_t>();
	if (type_id > (std::uint8_t)NBLib::Tag::NbtTypeId::COMPOUND) {
		return NBLib::Tag::NbtTypeId::UNKNOWN;
	}
	return (NBLib::Tag::NbtTypeId)type_id;
}

std::int8_t NBLib::NbtStream::readTagBytePL()
{
	return this->read<std::int8_t>();
}

std::int16_t NBLib::NbtStream::readTagShortPL()
{
	return this->read<std::int16_t>(this->nbt_type == NbtStreamType::BIG);
}

std::int32_t NBLib::NbtStream::readTagIntPL()
{
	switch (this->nbt_type) {
		case NbtStreamType::BIG:
		case NbtStreamType::LITTLE:
			return this->read<std::int32_t>(this->nbt_type == NbtStreamType::BIG);
		case NbtStreamType::NETWORK:
			return this->readZigZag<std::int32_t>();
	}
}

std::int64_t NBLib::NbtStream::readTagLongPL()
{
	switch (this->nbt_type) {
		case NbtStreamType::BIG:
		case NbtStreamType::LITTLE:
			return this->read<std::int64_t>(this->nbt_type == NbtStreamType::BIG);
		case NbtStreamType::NETWORK:
			return this->readZigZag<std::int64_t>();
	}
}

float NBLib::NbtStream::readTagFloatPL()
{
	return this->readFloat<float>(this->nbt_type == NbtStreamType::BIG);
}

double NBLib::NbtStream::readTagDoublePL()
{
	return this->readFloat<double>(this->nbt_type == NbtStreamType::BIG);
}

std::vector<std::int8_t> NBLib::NbtStream::readTagByteArrayPL()
{
	std::vector<std::int8_t> result;
	std::int32_t length = this->readTagIntPL();
	for (std::int32_t i = 0; i < length; i++) {
		result.push_back(this->readTagBytePL());
	}
	return result;
}

std::string NBLib::NbtStream::readTagStringPL()
{
	switch (this->nbt_type) {
		case NbtStreamType::BIG:
		case NbtStreamType::LITTLE:
			return this->readString<std::uint16_t>(this->nbt_type == NbtStreamType::BIG);
		case NbtStreamType::NETWORK:
			return this->readStringVarInt<std::uint32_t>();
	}
}

std::vector<struct NBLib::Tag::nbt_tag_t> NBLib::NbtStream::readTagListPL()
{
	std::vector<struct NBLib::Tag::nbt_tag_t> result;
	struct NBLib::Tag::nbt_tag_t nbt_tag = {};
	nbt_tag.type_id = this->readTagTypeId();
	std::int32_t length = this->readTagIntPL();
	for (std::int32_t i = 0; i < length; i++) {
		this->readTagPayload(&nbt_tag);
		result.push_back(nbt_tag);
	}
	return result;
}

std::vector<struct NBLib::Tag::nbt_tag_t> NBLib::NbtStream::readTagCompoundPL()
{
	std::vector<struct NBLib::Tag::nbt_tag_t> result;
	while (!this->eos()) {
		struct NBLib::Tag::nbt_tag_t nbt_tag = this->readTag();
		if (nbt_tag.type_id == NBLib::Tag::NbtTypeId::END)
			break;
		if (nbt_tag.type_id == NBLib::Tag::NbtTypeId::UNKNOWN)
			std::cout << "UNKNOWN TAG" << std::endl;
		result.push_back(nbt_tag);
	}
	return result;
}

void NBLib::NbtStream::readTagPayload(Tag::nbt_tag_t *value)
{
	switch (value->type_id) {
		case NBLib::Tag::NbtTypeId::END: {
			NBLib::Tag::end_tag_t tag = {};
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::BYTE: {
			NBLib::Tag::byte_tag_t tag = {};
			tag.value = this->readTagBytePL();
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::SHORT: {
			NBLib::Tag::short_tag_t tag = {};
			tag.value = this->readTagShortPL();
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::INT: {
			NBLib::Tag::int_tag_t tag = {};
			tag.value = this->readTagIntPL();
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::LONG: {
			NBLib::Tag::long_tag_t tag = {};
			tag.value = this->readTagLongPL();
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::FLOAT: {
			NBLib::Tag::float_tag_t tag = {};
			tag.value = this->readTagFloatPL();
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::DOUBLE: {
			NBLib::Tag::double_tag_t tag = {};
			tag.value = this->readTagDoublePL();
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::BYTE_ARRAY: {
			NBLib::Tag::byte_array_tag_t tag = {};
			tag.value = this->readTagByteArrayPL();
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::STRING: {
			NBLib::Tag::string_tag_t tag = {};
			tag.value = this->readTagStringPL();
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::LIST: {
			NBLib::Tag::list_tag_t tag = {};
			tag.value = this->readTagListPL();
			value->tag = tag;
		}
		break;
		case NBLib::Tag::NbtTypeId::COMPOUND: {
			NBLib::Tag::compound_tag_t tag = {};
			tag.value = this->readTagCompoundPL();
			value->tag = tag;
		}
		break;
	}
}

struct NBLib::Tag::nbt_tag_t NBLib::NbtStream::readTag()
{
	struct NBLib::Tag::nbt_tag_t result = {};
	result.type_id = this->readTagTypeId();
	if (result.type_id == NBLib::Tag::NbtTypeId::END) {
		result.name = "";
	} else if (result.type_id == NBLib::Tag::NbtTypeId::UNKNOWN) {
		return result;
	} else {
		result.name = this->readTagStringPL();
	}
	this->readTagPayload(&result);
	return result;
}

struct NBLib::Tag::nbt_tag_t NBLib::NbtStream::parse()
{
	NBLib::Tag::NbtTypeId type_id = this->readTagTypeId();
	if (type_id == NBLib::Tag::NbtTypeId::END || type_id == NBLib::Tag::NbtTypeId::UNKNOWN) {
		std::cout << "TMP ERR: INVALID TYPEID" << std::endl;
		throw "";
		// return nullptr;
	}
	this->rewind();
	return this->readTag();
}
