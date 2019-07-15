#include "Point.h"
#include "Util.h"
#include <cmath>

Point::Point()
{
	XOJ_INIT_TYPE(Point);
}

Point::Point(const Point& p)
{
	XOJ_INIT_TYPE(Point);

	this->x = Util::setPrecision(p.x, DECIMAL_PLACES);
	this->y = Util::setPrecision(p.y, DECIMAL_PLACES);
	this->z = Util::setPrecision(p.z, DECIMAL_PLACES);
}

Point::Point(double x, double y)
{
	XOJ_INIT_TYPE(Point);

	this->x = Util::setPrecision(x, DECIMAL_PLACES);
	this->y = Util::setPrecision(y, DECIMAL_PLACES);
}

Point::Point(double x, double y, double z)
{
	XOJ_INIT_TYPE(Point);

	this->x = Util::setPrecision(x, DECIMAL_PLACES);
	this->y = Util::setPrecision(y, DECIMAL_PLACES);
	this->z = Util::setPrecision(z, DECIMAL_PLACES);
}

Point::~Point()
{
	XOJ_RELEASE_TYPE(Point);
}

double Point::lineLengthTo(const Point& p)
{
	XOJ_CHECK_TYPE(Point);

	return std::hypot(this->x - p.x, this->y - p.y);
}

double Point::slopeTo(const Point& p)
{
	XOJ_CHECK_TYPE(Point);

	return std::atan2(this->x - p.x, this->y - p.y);
}

Point Point::lineTo(const Point& p, double length)
{
	XOJ_CHECK_TYPE(Point);

	double factor = lineLengthTo(p);
	factor = length / factor;

	double x = p.x - this->x;
	double y = p.y - this->y;
	x *= factor;
	y *= factor;
	x += this->x;
	y += this->y;

	return Point(x, y);
}

bool Point::equalsPos(const Point& p)
{
	XOJ_CHECK_TYPE(Point);

	return this->x == p.x && this->y == p.y;
}
