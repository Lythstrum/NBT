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

#include "NbtType.hpp"

namespace NBLib::NbtHelper
{
	class CompoundTagWrapper
	{
	private:
		Tag::compound_tag_t tag;

	public:
		CompoundTagWrapper(Tag::compound_tag_t &tag);

		Tag::nbt_any_tag_t getTag(std::string name, Tag::NbtTypeId type_id);
	};

	NBLib::Tag::nbt_any_tag_t *makeTagPtr(NBLib::Tag::nbt_any_tag_t value);

	Tag::nbt_tag_t createTag(std::string name, Tag::nbt_any_tag_t tag);
};
