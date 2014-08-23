#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <SFML/Graphics/Image.hpp>

typedef unsigned char byte;
typedef unsigned short word;

bool saveHGT(const std::string& file, unsigned hm);

int main(int argc, char** argv)
{
	if(argc != 2 || argc == 3)
	{
		std::cout << "Incorrect # of args, must be 1 (image filename)\n";
		return 1;
	}
	
	std::string file = argv[1];
	std::string fileName = file.substr(0, file.find_last_of('.'));
	std::string fileExt = file.substr(file.find_last_of('.') + 1);
	
	// if second arg is given, it is the heightMod, otherwise, make it 0
	unsigned heightMod = argv[2] ? std::stoi(argv[2]) : 0;
	
	if(!(fileExt == "bmp" || fileExt == "png" || fileExt == "tga" || fileExt == "jpg" || fileExt == "gif" || fileExt == "psd" || fileExt == "hdr" || fileExt == "pic"))
	{
		std::cout << "Unsupported image type: " << fileExt << '\n';
		return 2;
	}
	
	sf::Image image;
	image.loadFromFile(file);
	
	bool result = saveHGT(fileName + ".hgt", heightMod);
	
	if(result)
		std::cout << "HGT successfully created as " << fileName << ".hgt!\n";
	else
	{
		std::cout << "Failed creating HGT.\n";
		return 3;
	}
	
	return 0;
}

bool saveHGT(const std::string& file, unsigned hm)
{
	std::ofstream fout;
	
	fout.open(file, std::ofstream::binary);
	
	return true;
}