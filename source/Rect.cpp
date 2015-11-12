/// Represents an integer rectangle

#include "Rect.h"

int Rect::GetWidth() const
{
	return (bottom - top) + 1;
}
int Rect::GetHeight() const
{
	return (right - left) + 1;
}

bool Rect::Inside(const Position& position) const
{
	return (position.x >= left && position.y >= top && position.x <= right && position.y <= bottom);
}

bool Rect::Inside(int x, int y) const
{
	return (x >= left && y >= top && x <= right && y <= bottom);
}
