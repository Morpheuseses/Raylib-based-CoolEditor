#pragma once

#include "raylib.h"
#include "raygui.h"
#include "BaseStructs.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "TransformOperation.hpp"
#include "Painter.hpp"
#include "Selector.hpp"

class Editor {
    Mode mode;
    Point* points;
    Line* lines;
    const int MAX_LINES_SIZE  = 600;
    const int MAX_POINTS_SIZE = 300;
    int windowHeight;
    int windowWidth;
    Painter* painter;
    Selector* selector;
    TransformOperation* transformer;
    int editPoint; 
    Vector2 beginPosSelection;
    Vector2 endPosSelection;
    std::vector<Point*> selected;
    Vector2 firstPoint;
    Vector2 secondPoint;
    float sidebarwidth;
    bool isX;
    bool isGridDraw;
    bool isPointInfo;
    bool isLinesInfo;
public: 
    Editor(int height, int width, int configFlags);
    ~Editor();
    void Setup(int configFlags);
    void Init(const char* windowName, int targetFPS);
    void UpdateFrame();
private:
    void UpdateMode();
    void UpdateMirror();
    void UpdateSelection();
    void UpdatePoints();
    void UpdateLines();
    void UpdateButtons();
    void MovePoints(std::vector<Point*> selected, Vector2 firstPoint, Vector2 secondPoint);
    void RotatePoints(std::vector<Point*> selected, Vector2 firstPoint, Vector2 secondPoint);
    void ScalePoints(std::vector<Point*> selected, Vector2 firstPoint, Vector2 secondPoint,bool isGeneral);
    void DrawFrame();
    Point* CreateNewPoint2D(Vector2 pos);
    Point* CreateNewPoint3D(Vector3 pos);
    bool CheckIfLineExist(Point* p1, Point* p2);
    std::vector<Point*> GetPointAllLines(Point*& point);
    Line* CreateLine(Point* p1, Point* p2);
    void CopyPaste();
};