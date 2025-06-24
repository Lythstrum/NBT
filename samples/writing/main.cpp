#include <NBLib/NbtStream.hpp>
#include <NBLib/NbtHelper.hpp>
#include <iostream>
#include <fstream>

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
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " <nbt_file_type: test,bigtest> <endianness: [big],little,network>" << std::endl;
		return 1;
	}
	NBLib::NbtStreamType nbt_strm_type = NBLib::NbtStreamType::BIG;
	if (argc == 3) {
		if (!strcmp(argv[2], "little"))
			nbt_strm_type = NBLib::NbtStreamType::LITTLE;
		else if (!strcmp(argv[2], "network"))
			nbt_strm_type = NBLib::NbtStreamType::NETWORK;
		else if (strcmp(argv[2], "big")) {
			std::cout << "Invalid nbt stream type" << std::endl;
			return 1;
		}
	}
	NBLib::NbtStream nbt(nbt_strm_type, BMLib::Buffer::allocate());
	std::vector<NBLib::Tag::nbt_tag_t> nbt_tags;
	if (!strcmp(argv[1], "test")) {
		nbt_tags = {
			NBLib::NbtHelper::createTag("hello world", NBLib::Tag::compound_tag_t {
				{NBLib::NbtHelper::createTag("name", NBLib::Tag::string_tag_t {"Bananrama"})}
			})
		};
	} else if (!strcmp(argv[1], "bigtest")) {
		std::vector<std::uint8_t> byte_array_value;
		for (int i = 0; i < 1000; i++)
			byte_array_value.push_back((i * i * 255 + i * 7) % 100);
		nbt_tags = {
			NBLib::NbtHelper::createTag("Level", NBLib::Tag::compound_tag_t {{
				NBLib::NbtHelper::createTag("shortTest", NBLib::Tag::short_tag_t {32767}),
				NBLib::NbtHelper::createTag("longTest", NBLib::Tag::long_tag_t {nbt_strm_type == NBLib::NbtStreamType::NETWORK ? 0x7ffffffffffffff : 0x7fffffffffffffff}),
				NBLib::NbtHelper::createTag("floatTest", NBLib::Tag::float_tag_t {0.49823147}),
				NBLib::NbtHelper::createTag("stringTest", NBLib::Tag::string_tag_t {"HELLO WORLD THIS IS A TEST STRING ÅÄÖ!"}),
				NBLib::NbtHelper::createTag("intTest", NBLib::Tag::int_tag_t {nbt_strm_type == NBLib::NbtStreamType::NETWORK ? 0x7ffffff : 0x7fffffff}),
				NBLib::NbtHelper::createTag("nested compound test", NBLib::Tag::compound_tag_t {{
					NBLib::NbtHelper::createTag("ham", NBLib::Tag::compound_tag_t {{
						NBLib::NbtHelper::createTag("name", NBLib::Tag::string_tag_t {"Hampus"}),
						NBLib::NbtHelper::createTag("value", NBLib::Tag::float_tag_t {0.75})
					}}),
					NBLib::NbtHelper::createTag("egg", NBLib::Tag::compound_tag_t {{
						NBLib::NbtHelper::createTag("name", NBLib::Tag::string_tag_t {"Eggbert"}),
						NBLib::NbtHelper::createTag("value", NBLib::Tag::float_tag_t {0.75})
					}})
				}}),
				NBLib::NbtHelper::createTag("listTest (long)", NBLib::Tag::list_tag_t {{
					NBLib::NbtHelper::createTag("", NBLib::Tag::long_tag_t {11}),
					NBLib::NbtHelper::createTag("", NBLib::Tag::long_tag_t {12}),
					NBLib::NbtHelper::createTag("", NBLib::Tag::long_tag_t {13}),
					NBLib::NbtHelper::createTag("", NBLib::Tag::long_tag_t {14}),
					NBLib::NbtHelper::createTag("", NBLib::Tag::long_tag_t {15})
				}}),
				NBLib::NbtHelper::createTag("byteTest", NBLib::Tag::byte_tag_t {0x7f}),
				NBLib::NbtHelper::createTag("listTest (compound)", NBLib::Tag::list_tag_t {{
					NBLib::NbtHelper::createTag("", NBLib::Tag::compound_tag_t {{
						NBLib::NbtHelper::createTag("name", NBLib::Tag::string_tag_t {"Compound tag #0"}),
						NBLib::NbtHelper::createTag("created-on", NBLib::Tag::long_tag_t {1264099775885}),
					}}),
					NBLib::NbtHelper::createTag("", NBLib::Tag::compound_tag_t {{
						NBLib::NbtHelper::createTag("name", NBLib::Tag::string_tag_t {"Compound tag #1"}),
						NBLib::NbtHelper::createTag("created-on", NBLib::Tag::long_tag_t {1264099775885}),
					}}),
				}}),
				NBLib::NbtHelper::createTag("byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))", NBLib::Tag::byte_array_tag_t {byte_array_value}),
				NBLib::NbtHelper::createTag("doubleTest", NBLib::Tag::double_tag_t {0.4931287132182315})
			}})
		};
	} else {
		std::cout << "Invalid nbt type" << std::endl;
		return 1;
	}
	nbt.load(nbt_tags);
	std::string filename(argv[1]);
	filename += "_";
	filename += argv[2];
	filename += ".nbt";
	if (put_file_data(filename.c_str(), nbt.getBuffer()))
		std::cout << "Successfully written nbt data to file: " << filename.c_str() << "." << std::endl;
	else
		std::cout << "Unable to write nbt data to file: " << filename.c_str() << "." << std::endl;
	return 0;
}
