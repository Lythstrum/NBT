#include <NBLib/NbtStream.hpp>
#include <NBLib/NbtHelper.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>

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

bool put_file_data(const char *filename, BMLib::Buffer *buffer)
{
	std::fstream file(filename, std::ios::out | std::ios::binary);
	if (!file.is_open())
		return false;
	file.write((char *)buffer->getBinary(), buffer->getPosition());
	return true;
}

int main(int argc, char *argv[])
{
	if (argc < 4) {
		std::cout << "Usage: " << argv[0] << " <filename> <default_endianness: big,little,network> <to_endianness: big,little,network>" << std::endl;
		return 1;
	}
	BMLib::Buffer *fdat_buf = get_file_data(argv[1]);
	if (fdat_buf == nullptr) {
		std::cout << "Invalid filename given." << std::endl;
		return 1;
	}
	NBLib::NbtStreamType def_nbt_strm_type = NBLib::NbtStreamType::BIG, to_nbt_strm_type = NBLib::NbtStreamType::BIG;
	if (!strcmp(argv[2], "little"))
		def_nbt_strm_type = NBLib::NbtStreamType::LITTLE;
	else if (!strcmp(argv[2], "network"))
		def_nbt_strm_type = NBLib::NbtStreamType::NETWORK;
	else if (strcmp(argv[2], "big")) {
		invalid_nbt_strm_type:
		std::cout << "Invalid nbt stream type" << std::endl;
		return 1;
	}
	if (!strcmp(argv[3], "little"))
		to_nbt_strm_type = NBLib::NbtStreamType::LITTLE;
	else if (!strcmp(argv[3], "network"))
		to_nbt_strm_type = NBLib::NbtStreamType::NETWORK;
	else if (strcmp(argv[3], "big"))
		goto invalid_nbt_strm_type;
	NBLib::NbtStream nbt_default_endian(def_nbt_strm_type, fdat_buf);
	NBLib::NbtStream nbt_to_endian(to_nbt_strm_type, BMLib::Buffer::allocate());
	nbt_to_endian.load(nbt_default_endian.parse());

	std::filesystem::path path_filename = argv[1];
	std::string filename((char *)path_filename.replace_extension().c_str());
	filename += "_";
	filename += argv[3];
	filename += ".nbt";
	if (put_file_data(filename.c_str(), nbt_to_endian.getBuffer()))
		std::cout << "Successfully switched nbt endian and written the data to the file: " << filename.c_str() << "." << std::endl;
	else
		std::cout << "Unable to write nbt data to file: " << filename.c_str() << "." << std::endl;
	return 0;
}
