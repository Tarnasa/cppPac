/// This represents an integer point in space

#include <cstdlib>

#include "Position.h"

Position& Position::operator+=(const Position& other)
{
	x += other.x;
	y += other.y;
	return *this;
}
Position& Position::operator-=(const Position& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}
Position& Position::operator*=(int scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}
Position& Position::operator/=(int scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}
Position& Position::operator%=(const Position& other)
{
	x %= other.x;
	y %= other.y;
	return *this;
}

int abs(const Position& position) { return std::abs(position.x) + std::abs(position.y); }
int distance(const Position& a, const Position& b) { return abs(b - a); }

Position operator+(const Position& lhs, const Position& rhs)
{
	return Position(lhs) += rhs;
}
Position operator-(const Position& lhs, const Position& rhs)
{
	return Position(lhs) -= rhs;
}
Position operator*(const Position& lhs, int rhs)
{
	return Position(lhs) *= rhs;
}
Position operator/(const Position& lhs, int rhs)
{
	return Position(lhs) /= rhs;
}
Position operator%(const Position& lhs, const Position& rhs)
{
	return Position(lhs) %= rhs;
}

bool operator==(const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}
