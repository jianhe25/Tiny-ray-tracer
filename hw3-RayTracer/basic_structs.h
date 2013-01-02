#include <vector>
#include <algorithm>

using namespace std;

struct Color
{
	unsigned char r, g, b;
	Color() : r(0), g(0), b(0) {}
	Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {}
};

const Color BLACK(0, 0, 0);
const Color WHITE(255, 255, 255);

