#include "Size2D.h"

Size2D::Size2D()
{
	w = 0;
	h = 0;
}

Size2D::Size2D(int w, int h)
{
	this->w = w;
	this->h = h;
}

string Size2D::toString()
{
	return string(to_string(w) + ":" + to_string(h));
}
