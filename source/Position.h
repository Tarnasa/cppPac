/// This represents an integer point in space

#pragma once

class Position
{
public:
	int x, y;
	Position(int x, int y) : x(x), y(y) {};
	Position(const Position& original) : x(original.x), y(original.y) {};

	Position& operator+=(const Position& other);
	Position& operator-=(const Position& other);
	Position& operator*=(int scalar);
	Position& operator/=(int scalar);
	Position& operator%=(const Position& other);
};

int abs(const Position& position);
int distance(const Position& a, const Position& b);

Position operator+(const Position& lhs, const Position& rhs);
Position operator-(const Position& lhs, const Position& rhs);
Position operator*(const Position& lhs, int rhs);
Position operator/(const Position& lhs, int rhs);
Position operator%(const Position& lhs, const Position& rhs);

bool operator==(const Position& lhs, const Position& rhs);
