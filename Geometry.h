#ifndef GEOMETRY_H
#define GEOMETRY_H

class Geometry {
public:
    Geometry();
    virtual ~Geometry();

    int SegRow(int segNo);
    int SegCol(int segNo);
    float SegX(int segNo);
    float SegY(int segNo);


    float WIDTH_X; // m
    float WIDTH_Y; // m
    float WIDTH_Z; // m

};

#endif