#include <iostream>
#include <string>
#include <vector>
#include <fstream>

typedef unsigned char byte;
typedef unsigned short word;

struct bitmapHeader
{
	word type;
	word lof;
	word lof2;
	word x_hot;
	word y_hot;
	word first_pixel;
	word first_pixel2;
	word header_size;
	word header_size2;
	word x_size;             
	word x_size2;
	word y_size;
	word y_size2;
	word target;
	word bits_per_pixel;      
	word compression_method;
	word compression_method2;
	word compressed_size;
	word compressed_size2;
	word x_res;              
	word x_res2;
	word y_res;
	word y_res2;
	word used_clrs;
	word used_clrs2;         
	word important_clrs;
	word important_clrs2; // 54 bytes
} bitmap;

struct tga_header
{
   int shite;
   int shit1;
   int shit2;
   word xs;
   word ys;
   byte bpp;
   byte magic;
} tga;

bool loadTGA24(const std::string& file)
{
	std::ifstream fin;
	fin.open(file, std::ifstream::binary);
	
	
}
