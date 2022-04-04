#include "CImg/CImg.h"
#include <vector>
using namespace cimg_library;
using namespace std;

template<class T>
struct Color3
{
	T R;
	T G;
	T B;
};

vector<Color3<unsigned char>> cimg2rgb(CImg<unsigned char> img)
{
	vector<Color3<unsigned char>> ReadTextureBulkData;

	cimg_forXY(img, x, y)
	{
		Color3<unsigned char> NewColor;
		NewColor.R = img(x, y, 0);
		NewColor.G = img(x, y, 1);
		NewColor.B = img(x, y, 2);
		ReadTextureBulkData.push_back(std::move(NewColor));
	}

	return ReadTextureBulkData;
}