#include <NBLib/NbtStream.hpp>
#include <NBLib/NbtHelper.hpp>
#include <iostream>
#include <fstream>

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

int main(int argc, char *argv[])
{
	BMLib::Buffer *fdat_buf = get_file_data("bigtest_big.nbt");
	if (fdat_buf == nullptr) {
		std::cout << "bigtest_big.nbt was not found. (https://github.com/vp817/nbt_test_files)" << std::endl;
		return 1;
	}
	NBLib::NbtStream nbt(NBLib::NbtStreamType::BIG, fdat_buf);
	NBLib::Tag::nbt_tag_t nbt_tag = nbt.parseSingle();
	if (!std::holds_alternative<NBLib::Tag::compound_tag_t>(nbt_tag.tag)) {
		std::cout << "Invalid nbt first tag" << std::endl;
		return 1;
	}
	NBLib::NbtHelper::CompoundTagWrapper cpd_wrapper(std::get<NBLib::Tag::compound_tag_t>(nbt_tag.tag));
	auto long_test_tag = std::get_if<NBLib::Tag::long_tag_t>(NBLib::NbtHelper::makeTagPtr(cpd_wrapper.getTag("longTest", NBLib::Tag::NbtTypeId::LONG)));
	if (long_test_tag == nullptr) {
		std::cout << "LongTag with name longTest has not been found." << std::endl;
		return 1;
	}
	std::cout << "longTest: " << long_test_tag->value << std::endl;
	auto short_test_tag = std::get_if<NBLib::Tag::short_tag_t>(NBLib::NbtHelper::makeTagPtr(cpd_wrapper.getTag("shortTest", NBLib::Tag::NbtTypeId::SHORT)));
	if (short_test_tag == nullptr) {
		std::cout << "ShortTag with name shortTest has not been found." << std::endl;
		return 1;
	}
	std::cout << "shortTest: " << short_test_tag->value << std::endl;
	return 0;
}
