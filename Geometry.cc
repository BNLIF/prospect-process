#include "Geometry.h"

Geometry::Geometry()
{
    WIDTH_X = 0.1461; // m
    WIDTH_Y = 0.1461; // m
    WIDTH_Z = 1.175; // m
}

int Geometry::SegRow(int segNo)
{
    return segNo/14;
}

int Geometry::SegCol(int segNo)
{
    return segNo - SegRow(segNo)*14;
}

float Geometry::SegX(int segNo)
{
    return (SegCol(segNo)-6.5) * WIDTH_X;
}

float Geometry::SegY(int segNo)
{
    return (SegRow(segNo)-5) * WIDTH_Y;
}


Geometry::~Geometry()
{

}