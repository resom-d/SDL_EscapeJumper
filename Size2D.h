#pragma once
#include <string>
using namespace std;

class Size2D
{
public:
	Size2D();
	Size2D(int w, int h);

	int w, h;

	virtual string toString();
};

