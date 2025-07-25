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

NBLib::NbtStream::NbtStream(NbtStreamType nbt_type, BMLib::Buffer *buffer, std::size_t position)
	: nbt_type(nbt_type), BMLib::BinaryStream(buffer, position)
{
	if (nbt_type >= NbtStreamType::_UNKNOWN)
		throw new std::invalid_argument("Invalid NbtType given.");
}

NBLib::Tag::NbtTypeId NBLib::NbtStream::readTagTypeId()
{
	std::uint8_t type_id = this->read<std::uint8_t>();
	if (type_id >= (std::uint8_t)Tag::NbtTypeId::_UNKNOWN) {
		return Tag::NbtTypeId::_UNKNOWN;
	}
	return (Tag::NbtTypeId)type_id;
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
	if (this->nbt_type == NbtStreamType::NETWORK)
		return this->readZigZag<std::int32_t>();
	return this->read<std::int32_t>(this->nbt_type == NbtStreamType::BIG);
}

std::int64_t NBLib::NbtStream::readTagLongPL()
{
	if (this->nbt_type == NbtStreamType::NETWORK)
		return this->readZigZag<std::int64_t>();
	return this->read<std::int64_t>(this->nbt_type == NbtStreamType::BIG);
}

float NBLib::NbtStream::readTagFloatPL()
{
	return this->readFloat<float>(this->nbt_type == NbtStreamType::BIG);
}

double NBLib::NbtStream::readTagDoublePL()
{
	return this->readFloat<double>(this->nbt_type == NbtStreamType::BIG);
}

std::vector<std::uint8_t> NBLib::NbtStream::readTagByteArrayPL()
{
	std::vector<std::uint8_t> result;
	std::int32_t length = this->readTagIntPL();
	for (std::int32_t i = 0; i < length; ++i)
		result.push_back(this->read<std::uint8_t>());
	return result;
}

std::string NBLib::NbtStream::readTagStringPL()
{
	if (this->nbt_type == NbtStreamType::NETWORK)
		return this->readStringVarInt<std::uint32_t>();
	return this->readString<std::uint16_t>(this->nbt_type == NbtStreamType::BIG);
}

std::vector<struct NBLib::Tag::nbt_tag_t> NBLib::NbtStream::readTagListPL()
{
	std::vector<struct Tag::nbt_tag_t> result;
	struct Tag::nbt_tag_t nbt_tag = {};
	nbt_tag.type_id = this->readTagTypeId();
	std::int32_t length = this->readTagIntPL();
	for (std::int32_t i = 0; i < length; ++i) {
		this->readTagPayload(&nbt_tag);
		result.push_back(nbt_tag);
	}
	return result;
}

std::vector<struct NBLib::Tag::nbt_tag_t> NBLib::NbtStream::readTagCompoundPL()
{
	std::vector<struct Tag::nbt_tag_t> result;
	while (!this->eos()) {
		struct Tag::nbt_tag_t nbt_tag = this->readTag();
		if (nbt_tag.type_id == Tag::NbtTypeId::END)
			break;
		if (nbt_tag.type_id == Tag::NbtTypeId::_UNKNOWN)
			throw exceptions::UnknownTagEncountered("Invalid tag encountered while reading compound tag.");
		result.push_back(nbt_tag);
	}
	return result;
}

std::vector<std::int32_t> NBLib::NbtStream::readTagIntArrayPL()
{
	std::vector<std::int32_t> result;
	std::int32_t length = this->readTagIntPL();
	for (std::int32_t i = 0; i < length; ++i)
		result.push_back(this->readTagIntPL());
	return result;
}

std::vector<std::int64_t> NBLib::NbtStream::readTagLongArrayPL()
{
	std::vector<std::int64_t> result;
	std::int32_t length = this->readTagIntPL();
	for (std::int32_t i = 0; i < length; ++i)
		result.push_back(this->readTagLongPL());
	return result;
}

void NBLib::NbtStream::readTagPayload(Tag::nbt_tag_t *value)
{
	switch (value->type_id) {
		case Tag::NbtTypeId::END: {
			value->type_name = "End";
			Tag::end_tag_t tag = {};
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::BYTE: {
			value->type_name = "Byte";
			Tag::byte_tag_t tag = {};
			tag.value = this->readTagBytePL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::SHORT: {
			value->type_name = "Short";
			Tag::short_tag_t tag = {};
			tag.value = this->readTagShortPL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::INT: {
			value->type_name = "Int";
			Tag::int_tag_t tag = {};
			tag.value = this->readTagIntPL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::LONG: {
			value->type_name = "Long";
			Tag::long_tag_t tag = {};
			tag.value = this->readTagLongPL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::FLOAT: {
			value->type_name = "Float";
			Tag::float_tag_t tag = {};
			tag.value = this->readTagFloatPL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::DOUBLE: {
			value->type_name = "Double";
			Tag::double_tag_t tag = {};
			tag.value = this->readTagDoublePL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::BYTE_ARRAY: {
			value->type_name = "Byte_Array";
			Tag::byte_array_tag_t tag = {};
			tag.value = this->readTagByteArrayPL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::STRING: {
			value->type_name = "String";
			Tag::string_tag_t tag = {};
			tag.value = this->readTagStringPL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::LIST: {
			value->type_name = "List";
			Tag::list_tag_t tag = {};
			tag.value = this->readTagListPL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::COMPOUND: {
			value->type_name = "Compound";
			Tag::compound_tag_t tag = {};
			tag.value = this->readTagCompoundPL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::INT_ARRAY: {
			value->type_name = "Int_Array";
			Tag::int_array_tag_t tag = {};
			tag.value = this->readTagIntArrayPL();
			value->tag = tag;
		}
		break;
		case Tag::NbtTypeId::LONG_ARRAY: {
			value->type_name = "Long_Array";
			Tag::long_array_tag_t tag = {};
			tag.value = this->readTagLongArrayPL();
			value->tag = tag;
		}
		break;
	}
}

struct NBLib::Tag::nbt_tag_t NBLib::NbtStream::readTag()
{
	struct Tag::nbt_tag_t result = {};
	result.type_id = this->readTagTypeId();
	if (result.type_id == Tag::NbtTypeId::END)
		result.name = "";
	else if (result.type_id == Tag::NbtTypeId::_UNKNOWN)
		return result;
	else
		result.name = this->readTagStringPL();
	this->readTagPayload(&result);
	return result;
}

void NBLib::NbtStream::writeTagTypeId(NBLib::Tag::NbtTypeId value)
{
	this->write<std::uint8_t>((std::uint8_t)value);
}

void NBLib::NbtStream::writeTagBytePL(std::int8_t value)
{
	this->write<std::int8_t>(value);
}

void NBLib::NbtStream::writeTagShortPL(std::int16_t value)
{
	this->write<std::int16_t>(value, this->nbt_type == NbtStreamType::BIG);
}

void NBLib::NbtStream::writeTagIntPL(std::int32_t value)
{
	if (this->nbt_type == NbtStreamType::NETWORK) {
		this->writeZigZag<std::int32_t>(value);
		return;
	}
	this->write<std::int32_t>(value, this->nbt_type == NbtStreamType::BIG);
}

void NBLib::NbtStream::writeTagLongPL(std::int64_t value)
{
	if (this->nbt_type == NbtStreamType::NETWORK) {
		this->writeZigZag<std::int64_t>(value);
		return;
	}
	this->write<std::int64_t>(value, this->nbt_type == NbtStreamType::BIG);
}

void NBLib::NbtStream::writeTagFloatPL(float value)
{
	this->writeFloat<float>(value, this->nbt_type == NbtStreamType::BIG);
}

void NBLib::NbtStream::writeTagDoublePL(double value)
{
	this->writeFloat<double>(value, this->nbt_type == NbtStreamType::BIG);
}

void NBLib::NbtStream::writeTagByteArrayPL(std::vector<std::uint8_t> value)
{
	std::int32_t length = static_cast<std::int32_t>(value.size());
	this->writeTagIntPL(length);
	for (std::int32_t i = 0; i < length; ++i)
		this->write<std::uint8_t>(value[i]);
}

void NBLib::NbtStream::writeTagStringPL(std::string value)
{
	if (this->nbt_type == NbtStreamType::NETWORK) {
		this->writeStringVarInt<std::uint32_t>(value);
		return;
	}
	this->writeString<std::uint16_t>(value, this->nbt_type == NbtStreamType::BIG);
}

void NBLib::NbtStream::writeTagListPL(std::vector<struct NBLib::Tag::nbt_tag_t> value)
{
	if (value.empty())
		this->writeTagTypeId(Tag::NbtTypeId::END);
	else
		this->writeTagTypeId(value.begin()->type_id);
	std::int32_t length = static_cast<std::int32_t>(value.size());
	this->writeTagIntPL(length);
	for (std::int32_t i = 0; i < length; ++i)
		this->writeTagPayload(value[i]);
}

void NBLib::NbtStream::writeTagCompoundPL(std::vector<struct NBLib::Tag::nbt_tag_t> value)
{
	for (std::size_t i = 0; i < value.size(); ++i)
		this->writeTag(value[i]);
	this->writeTagTypeId(Tag::NbtTypeId::END);
}

void NBLib::NbtStream::writeTagIntArrayPL(std::vector<std::int32_t> value)
{
	std::int32_t length = static_cast<std::int32_t>(value.size());
	for (std::int32_t i = 0; i < length; ++i)
		this->writeTagIntPL(value[i]);
}

void NBLib::NbtStream::writeTagLongArrayPL(std::vector<std::int64_t> value)
{
	std::int32_t length = static_cast<std::int32_t>(value.size());
	for (std::int32_t i = 0; i < length; ++i)
		this->writeTagLongPL(value[i]);
}

void NBLib::NbtStream::writeTagPayload(Tag::nbt_tag_t &value)
{
	switch (value.type_id) {
		case Tag::NbtTypeId::BYTE:
			this->writeTagBytePL(std::get<Tag::byte_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::SHORT:
			this->writeTagShortPL(std::get<Tag::short_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::INT:
			this->writeTagIntPL(std::get<Tag::int_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::LONG:
			this->writeTagLongPL(std::get<Tag::long_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::FLOAT:
			this->writeTagFloatPL(std::get<Tag::float_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::DOUBLE:
			this->writeTagDoublePL(std::get<Tag::double_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::BYTE_ARRAY:
			this->writeTagByteArrayPL(std::get<Tag::byte_array_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::STRING:
			this->writeTagStringPL(std::get<Tag::string_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::LIST:
			this->writeTagListPL(std::get<Tag::list_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::COMPOUND:
			this->writeTagCompoundPL(std::get<Tag::compound_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::INT_ARRAY:
			this->writeTagIntArrayPL(std::get<Tag::int_array_tag_t>(value.tag).value);
		break;
		case Tag::NbtTypeId::LONG_ARRAY:
			this->writeTagLongArrayPL(std::get<Tag::long_array_tag_t>(value.tag).value);
		break;
	}
}

void NBLib::NbtStream::writeTag(struct NBLib::Tag::nbt_tag_t value)
{
	if (value.type_id == Tag::NbtTypeId::END || value.type_id >= Tag::NbtTypeId::_UNKNOWN)
		throw std::invalid_argument("Invalid tag id.");
	this->writeTagTypeId(value.type_id);
	this->writeTagStringPL(value.name);
	this->writeTagPayload(value);
}

struct NBLib::Tag::nbt_tag_t NBLib::NbtStream::parseSingle()
{
	Tag::NbtTypeId type_id = this->readTagTypeId();
	if (type_id == Tag::NbtTypeId::END || type_id == Tag::NbtTypeId::_UNKNOWN) {
		throw exceptions::InvalidNbtFile("Unable to parse nbt data for it not being an actual nbt data. ");
	}
	this->position -= 1;
	return this->readTag();
}

std::vector<struct NBLib::Tag::nbt_tag_t> NBLib::NbtStream::parse()
{
	std::vector<struct Tag::nbt_tag_t> result;
	while (!this->eos())
		result.push_back(this->parseSingle());
	return result;
}

void NBLib::NbtStream::load(std::vector<struct NBLib::Tag::nbt_tag_t> value)
{
	for (std::size_t i = 0; i < value.size(); ++i)
		this->writeTag(value[i]);
}
