#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>

#include "stb_image.h"

struct StbImageFree
{
	void operator()(std::uint8_t* ptr)
	{
		stbi_image_free(ptr);
	}
};

using StbSmartPtr = std::unique_ptr<std::uint8_t[], StbImageFree>;

bool saveHGT(const std::string& file, const std::uint8_t* img, std::size_t heightModifier, std::size_t width, std::size_t height);

int main(int argc, char** argv) try
{
	if(!(argc == 2 || argc == 3))
		throw std::runtime_error("Incorrect # of args, must be 1 (filename of image)");

	std::string file = argv[1];
	std::string fileName = file.substr(0, file.find_last_of('.'));
	std::string fileExt = file.substr(file.find_last_of('.') + 1);

	// if second arg is given, it is the heightMod, otherwise, make it 0
	auto heightMod = argv[2] ? std::stoi(argv[2]) : 0;

	// check for supported file types via extension
	if(!(fileExt == "bmp" || fileExt == "png" || fileExt == "tga" || fileExt == "jpg" || fileExt == "gif" || fileExt == "psd" || fileExt == "hdr" || fileExt == "pic"))
		throw std::runtime_error("Unsupported image format");

	int width = 0;
	int height = 0;
	StbSmartPtr image(stbi_load(file.c_str(), &width, &height, nullptr, 4));

	std::cout << "This image will create a " << width * 10 << " x " << height * 10 << " height map.\n";

	bool result = saveHGT(fileName + ".hgt", image.get(), heightMod, width, height);

	if(result)
		std::cout << "HGT successfully created as " << fileName << ".hgt!\n";
	else
		throw std::runtime_error("Failed creating HGT");

	return 0;
}
catch(const std::runtime_error& re)
{
	std::cerr << re.what() << '\n';
	return 1;
}

bool saveHGT(const std::string& file, const std::uint8_t* img, std::size_t heightModifier, std::size_t width, std::size_t height)
{
	// write to memory...
	std::stringstream ss(std::ios::binary);

	// total chunks in map
	auto xChunks = width / 128;
	auto yChunks = height / 128;

	// decide how we're writing bytes before we loop
	std::function<std::pair<std::uint8_t, std::uint8_t>(int)> byteWrite;

	if(heightModifier == 0)
	{
		// math mode
		byteWrite = [&](int pos)
		{
			// store top 4 bits in the high byte
			std::uint8_t high = (img[pos + 1] & 240) >> 4;

			// and store the next 8 bits in the low byte
			std::uint8_t low = ((img[pos + 1] & 15) << 4) + (img[pos + 2] & 15);

			return std::make_pair(low, high);
		};
	}
	else
	{
		// normal greyscale mode
		byteWrite = [&](int pos)
		{
			std::uint16_t val = static_cast<std::uint16_t>(heightModifier * (img[pos] + img[pos + 1] + img[pos + 2]));
			std::uint8_t low = val & 255;
			std::uint8_t high = val >> 8;

			return std::make_pair(low, high);
		};
	}

	for(auto y = yChunks - 1; y >= 0; --y)
	{
		for(auto x = 0u; x < xChunks; ++x)
		{
			auto basePos = 128 * 4 * (x + y * 128 * xChunks);

			for(auto yi = 127u; yi >= 0; --yi)
			{
				for(auto xi = 0u; xi < 128; ++xi)
				{
					auto pos = (yi * width + xi) * 4 + basePos;

					auto pair = byteWrite(pos);

					ss.put(pair.first);
					ss.put(pair.second);
				}
			}
		}
	}

	// ...then to disk, once
	std::ofstream fout(file, std::ios::binary);

	if(!fout)
		return false;

	std::string out = ss.str();
	fout.write(out.c_str(), out.length());

	return true;
}