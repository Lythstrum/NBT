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

namespace NBLib
{
	enum class NbtStreamType
	{
		BIG,
		LITTLE,
		NETWORK
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
		std::vector<std::int8_t> readTagByteArrayPL();
		std::string readTagStringPL();
		std::vector<struct Tag::nbt_tag_t> readTagListPL();
		std::vector<struct Tag::nbt_tag_t> readTagCompoundPL();
		void readTagPayload(Tag::nbt_tag_t *value);

		struct Tag::nbt_tag_t readTag();

		struct Tag::nbt_tag_t parse();

		void add(struct Tag::nbt_tag_t tag);

		void load();
	};
};
