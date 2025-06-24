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

#pragma once

#include <BMLib/BinaryStream.hpp>
#include "NbtType.hpp"
#include <NBLib/exceptions/UnknwonTagEncountered.hpp>
#include <NBLib/exceptions/InvalidNbtFile.hpp>

namespace NBLib
{
	enum class NbtStreamType
	{
		BIG,
		LITTLE,
		NETWORK,
		_UNKNOWN
	};

	class NbtStream : public BMLib::BinaryStream
	{
	private:
		NbtStreamType nbt_type;
		std::vector<struct Tag::nbt_tag_t> ws_tag_list;

	public:
		explicit NbtStream(NbtStreamType nbt_type, BMLib::Buffer *buffer, std::size_t position = 0);

		Tag::NbtTypeId readTagTypeId();

		std::int8_t readTagBytePL();
		std::int16_t readTagShortPL();
		std::int32_t readTagIntPL();
		std::int64_t readTagLongPL();
		float readTagFloatPL();
		double readTagDoublePL();
		std::vector<std::uint8_t> readTagByteArrayPL();
		std::string readTagStringPL();
		std::vector<struct Tag::nbt_tag_t> readTagListPL();
		std::vector<struct Tag::nbt_tag_t> readTagCompoundPL();
		std::vector<std::int32_t> readTagIntArrayPL();
		std::vector<std::int64_t> readTagLongArrayPL();
		void readTagPayload(Tag::nbt_tag_t *value);

		struct Tag::nbt_tag_t readTag();

		void writeTagTypeId(Tag::NbtTypeId value);

		void writeTagBytePL(std::int8_t value);
		void writeTagShortPL(std::int16_t value);
		void writeTagIntPL(std::int32_t value);
		void writeTagLongPL(std::int64_t value);
		void writeTagFloatPL(float value);
		void writeTagDoublePL(double value);
		void writeTagByteArrayPL(std::vector<std::uint8_t> value);
		void writeTagStringPL(std::string value);
		void writeTagListPL(std::vector<struct Tag::nbt_tag_t> value);
		void writeTagCompoundPL(std::vector<struct Tag::nbt_tag_t> value);
		void writeTagIntArrayPL(std::vector<std::int32_t> value);
		void writeTagLongArrayPL(std::vector<std::int64_t> value);
		void writeTagPayload(Tag::nbt_tag_t &value);

		void writeTag(struct Tag::nbt_tag_t value);

		struct Tag::nbt_tag_t parseSingle();
		std::vector<struct Tag::nbt_tag_t> parse();

		void load(std::vector<struct Tag::nbt_tag_t> value);
	};
};
