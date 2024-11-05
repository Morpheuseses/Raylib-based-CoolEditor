#pragma once
#include "raylib.h"
#include "BaseStructs.hpp"
#include <vector>

class Selector {
    std::vector<Point*> selected;
public:
    Selector();
    Selector(std::vector<Point*> selected);
    ~Selector();
    std::vector<Point*> RectangleSelect(Point* points,int size, Vector2 begin, Vector2 end);
    void Link(Line* lines,Point*& points, int points_size, int lines_size);
    void ClearPointsSelection(Point* points,int size);
};