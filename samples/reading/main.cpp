#include <NBLib/NbtStream.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

BMLib::Buffer *get_file_data(const char *filename)
{
	std::fstream file(filename, std::ios::in | std::ios::binary);
	file.seekg(0, std::ios::end);
	std::streampos bin_sz = file.tellg();
	file.seekg(0, std::ios::beg);
	if (bin_sz < 1) {
		file.close();
		return nullptr;
	}
	char *bin_buf = (char *)std::malloc(bin_sz);
	file.read(bin_buf, bin_sz);
	file.close();
	return new BMLib::Buffer((std::uint8_t *)bin_buf, bin_sz);
}

std::string ws_padding(int padding)
{
	std::string result;
	for (int i = 0; i < padding; i++)
		result += " ";
	return result;
}

std::string nbt_stringify(NBLib::Tag::nbt_tag_t nbt_tag, int padding = 0, bool nameless = false)
{
	std::ostringstream to_write;

	if (std::holds_alternative<NBLib::Tag::compound_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::compound_tag_t compound_tag = std::get<NBLib::Tag::compound_tag_t>(nbt_tag.tag);
		std::size_t cpt_value_size = compound_tag.value.size();

		to_write << ws_padding(padding);
		to_write << "TAG_Compound";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": " << std::to_string(cpt_value_size) << " entries\n";
		if (cpt_value_size < 1) {
			to_write << ws_padding(padding);
			to_write << "{}";
		} else {
			to_write << ws_padding(padding);
			to_write << "{\n";
			for (std::size_t i = 0; i < cpt_value_size; i++) {
				to_write << nbt_stringify(compound_tag.value.at(i), padding == 0 ? 3 : padding * 2);
				to_write << "\n";
				if (i == cpt_value_size - 1) {
					to_write << ws_padding(padding);
					to_write << "}";
				}
			}
		}
	} else if (std::holds_alternative<NBLib::Tag::list_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::list_tag_t lst_tag = std::get<NBLib::Tag::list_tag_t>(nbt_tag.tag);
		std::size_t lt_value_size = lst_tag.value.size();

		to_write << ws_padding(padding);
		to_write << "TAG_List";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": " << std::to_string(lt_value_size) << " entries\n";
		if (lt_value_size < 1) {
			to_write << ws_padding(padding);
			to_write << "{}";
		} else {
			to_write << ws_padding(padding);
			to_write << "{\n";
			for (std::size_t i = 0; i < lt_value_size; i++) {
				to_write << nbt_stringify(lst_tag.value.at(i), padding == 0 ? 3 : padding * 2, true);
				to_write << "\n";
				if (i == lt_value_size - 1) {
					to_write << ws_padding(padding);
					to_write << "}";
				}
			}
		}
	}  else if (std::holds_alternative<NBLib::Tag::byte_array_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::byte_array_tag_t bt_tag = std::get<NBLib::Tag::byte_array_tag_t>(nbt_tag.tag);
		std::size_t bt_tag_size = bt_tag.value.size();

		to_write << ws_padding(padding);
		to_write << "TAG_Byte_Array";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": [" << std::to_string(bt_tag_size) << " bytes" << "]";
	} else if (std::holds_alternative<NBLib::Tag::string_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::string_tag_t str_tag = std::get<NBLib::Tag::string_tag_t>(nbt_tag.tag);

		to_write << ws_padding(padding);
		to_write << "TAG_String";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": " << str_tag.value;
	} else if (std::holds_alternative<NBLib::Tag::byte_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::byte_tag_t bt_tag = std::get<NBLib::Tag::byte_tag_t>(nbt_tag.tag);

		to_write << ws_padding(padding);
		to_write << "TAG_Byte";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": " << std::to_string(bt_tag.value);
	} else if (std::holds_alternative<NBLib::Tag::short_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::short_tag_t sht_tag = std::get<NBLib::Tag::short_tag_t>(nbt_tag.tag);

		to_write << ws_padding(padding);
		to_write << "TAG_Short";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": " << sht_tag.value;
	} else if (std::holds_alternative<NBLib::Tag::int_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::int_tag_t int_tag = std::get<NBLib::Tag::int_tag_t>(nbt_tag.tag);

		to_write << ws_padding(padding);
		to_write << "TAG_Int";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": " << int_tag.value;
	} else if (std::holds_alternative<NBLib::Tag::long_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::long_tag_t lng_tag = std::get<NBLib::Tag::long_tag_t>(nbt_tag.tag);

		to_write << ws_padding(padding);
		to_write << "TAG_Long";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": " << lng_tag.value;
	} else if (std::holds_alternative<NBLib::Tag::float_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::float_tag_t flt_tag = std::get<NBLib::Tag::float_tag_t>(nbt_tag.tag);

		to_write << ws_padding(padding);
		to_write << "TAG_Float";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": " << flt_tag.value;
	} else if (std::holds_alternative<NBLib::Tag::double_tag_t>(nbt_tag.tag)) {
		NBLib::Tag::double_tag_t dbl_tag = std::get<NBLib::Tag::double_tag_t>(nbt_tag.tag);

		to_write << ws_padding(padding);
		to_write << "TAG_Double";
		if (!nameless)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": " << dbl_tag.value;
	}

	return to_write.str();
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " <filename> <endianness: [big],little,network>" << std::endl;
		return 1;
	}
	BMLib::Buffer *fdat_buf = get_file_data(argv[1]);
	if (fdat_buf == nullptr) {
		std::cout << "Invalid filename given." << std::endl;
		return 1;
	}
	NBLib::NbtStreamType nbt_strm_type = NBLib::NbtStreamType::BIG;
	if (argc == 3) {
		if (!strcmp(argv[2], "little"))
			nbt_strm_type = NBLib::NbtStreamType::LITTLE;
		else if (!strcmp(argv[2], "network"))
			nbt_strm_type = NBLib::NbtStreamType::NETWORK;
	}
	NBLib::NbtStream nbt(nbt_strm_type, fdat_buf);
	auto nbt_tag = nbt.parse();
	std::cout << nbt_stringify(nbt_tag) << std::endl;
	return 0;
}
