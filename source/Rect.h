/// Represents an integer rectangle

#pragma once

#include "Position.h"

class Rect
{
public:
	int left, top, right, bottom;

	Rect(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom) {};

	int GetWidth() const;
	int GetHeight() const;

	bool Inside(const Position& position) const;
	bool Inside(int x, int y) const;
};
