#include <NBLib/NbtStream.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

BMLib::Buffer *get_file_data(const char *filename)
{
	std::fstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open())
		return nullptr;
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

std::string nbt_stringify(NBLib::Tag::nbt_tag_t nbt_tag, int base_pad_value = 3, int padding = 0)
{
	std::ostringstream to_write;

	std::visit([&](auto &&arg) {
		using T = std::decay_t<decltype(arg)>;
		to_write << ws_padding(padding);
		to_write << "TAG_" << nbt_tag.type_name;
		if (nbt_tag.name.size() != 0)
			to_write << "(\"" << nbt_tag.name << "\")";
		to_write << ": ";
		if constexpr (std::is_same_v<T, NBLib::Tag::compound_tag_t> || std::is_same_v<T, NBLib::Tag::list_tag_t>) {
			std::size_t atp_size = arg.value.size();

			to_write << std::to_string(atp_size) << " entries";
			if constexpr (std::is_same_v<T, NBLib::Tag::list_tag_t>) {
				if (atp_size > 0)
					to_write << " of type TAG_" << arg.value.begin()->type_name;
			}
			to_write << "\n";
			if (atp_size < 1) {
				to_write << ws_padding(padding);
				to_write << "{}";
			} else {
				to_write << ws_padding(padding);
				to_write << "{\n";
				for (std::size_t i = 0; i < atp_size; i++) {
					to_write << nbt_stringify(arg.value.at(i), base_pad_value, padding == 0 ? base_pad_value : padding + base_pad_value);
					to_write << "\n";
				}
				to_write << ws_padding(padding);
				to_write << "}";
			}
		} else if constexpr (std::is_same_v<T, NBLib::Tag::string_tag_t>)
			to_write << arg.value;
		else if constexpr (
			std::is_same_v<T, NBLib::Tag::short_tag_t> || std::is_same_v<T, NBLib::Tag::int_tag_t> || std::is_same_v<T, NBLib::Tag::long_tag_t> ||
			std::is_same_v<T, NBLib::Tag::byte_tag_t> || std::is_same_v<T, NBLib::Tag::float_tag_t> || std::is_same_v<T, NBLib::Tag::double_tag_t>)
				to_write << std::to_string(arg.value);
		else if constexpr (
			std::is_same_v<T, NBLib::Tag::byte_array_tag_t> || std::is_same_v<T, NBLib::Tag::int_array_tag_t> ||
			std::is_same_v<T, NBLib::Tag::long_array_tag_t>) {
				to_write << ": [" << std::to_string(arg.value.size()) << " ";
				if constexpr (std::is_same_v<T, NBLib::Tag::byte_array_tag_t>)
					to_write << "bytes";
				else if constexpr (std::is_same_v<T, NBLib::Tag::int_array_tag_t>)
					to_write << "integers";
				else if constexpr (std::is_same_v<T, NBLib::Tag::long_array_tag_t>)
					to_write << "long integers";
				to_write << "]";
		}
	}, nbt_tag.tag);

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
	for (auto &nbt_tag : nbt.parse())
		std::cout << nbt_stringify(nbt_tag) << std::endl;
	return 0;
}
