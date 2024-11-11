#pragma once
#include "raylib.h"
#include "BaseStructs.hpp"
#include <vector>

class Selector {
    std::vector<Point3D*> selected;
public:
    Selector();
    Selector(std::vector<Point3D*> selected);
    ~Selector();
    std::vector<Point3D*> RectangleSelect(Point3D* points,int size, Vector2 begin, Vector2 end, Projection projection);
    void Link(Line3D* lines,Point3D*& points, int points_size, int lines_size,Projection projection);
    void ClearPointsSelection(Point3D* points,int size);
};