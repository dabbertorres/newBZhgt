#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <SFML/Graphics/Image.hpp>

typedef unsigned char byte;
typedef unsigned short word;

bool saveHGT(const std::string& file, const sf::Image& img, unsigned hm, unsigned sizeX, unsigned sizeY);

int main(int argc, char** argv)
{
	if(!(argc == 2 || argc == 3))
	{
		std::cout << "Incorrect # of args, must be 1 (image filename)\n";
		return 1;
	}
	
	std::string file = argv[1];
	std::string fileName = file.substr(0, file.find_last_of('.'));
	std::string fileExt = file.substr(file.find_last_of('.') + 1);
	
	// if second arg is given, it is the heightMod, otherwise, make it 0
	unsigned heightMod = argv[2] ? std::stoi(argv[2]) : 0;
	
	// check for supported file types via extension
	if(!(fileExt == "bmp" || fileExt == "png" || fileExt == "tga" || fileExt == "jpg" || fileExt == "gif" || fileExt == "psd" || fileExt == "hdr" || fileExt == "pic"))
	{
		std::cout << "Unsupported image type: " << fileExt << '\n';
		return 2;
	}
	
	sf::Image image;
	image.loadFromFile(file);
	
	std::cout << "This image will create a " << image.getSize().x * 10 << 'x' << image.getSize().y * 10 << " height map.\n"; 
	
	bool result = saveHGT(fileName + ".hgt", image, heightMod, image.getSize().x, image.getSize().y);
	
	if(result)
		std::cout << "HGT successfully created as " << fileName << ".hgt!\n";
	else
	{
		std::cout << "Failed creating HGT.\n";
		return 3;
	}
	
	return 0;
}

bool saveHGT(const std::string& file, const sf::Image& img, unsigned hm, unsigned sizeX, unsigned sizeY)
{
	std::ofstream fout;
	
	fout.open(file, std::ofstream::binary);
	
	if(!fout)
		return false;
	
	char xChunks = sizeX / 128;			// total chunks in map x
	char yChunks = sizeY / 128;			// total chunks in map y
	word h;								// two byte height value, valid range: 0..4094
	byte loH, hiH;						// height bytes
	
	for(int curYChunk = yChunks - 1; curYChunk > -1; curYChunk--)
	{
		for(int curXChunk = 0; curXChunk < xChunks; curXChunk++)
		{
			int basePos = (curXChunk * 128 * 4) + (curYChunk * 128 * 128 * xChunks * 4);
			
			for(int y = 127; y > -1; y--)
			{
				for(int x = 0; x < 128; x++)
				{
					int pos = ((y * sizeX) + x) * 4 + basePos;
					
					if(hm == 0)
					{
						// bitshifted madness mode
						hiH = (img.getPixelsPtr()[pos + 1] & 240) >> 4;
						loH = ((img.getPixelsPtr()[pos + 1] & 15) << 4) + (img.getPixelsPtr()[pos + 2] & 15);
					}
					else
					{
						// normal greyscale mode
						h = hm * (img.getPixelsPtr()[pos] + img.getPixelsPtr()[pos + 1] + img.getPixelsPtr()[pos + 2]);
						loH = h & 255;
						hiH = h >> 8;
					}
					
					fout.put(loH);
					fout.put(hiH);
				}
			}
		}
	}
	
	std::cout << '\n';
	
	return true;
}