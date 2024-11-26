#pragma once

#include "raylib.h"
#include "raygui.h"
#include "BaseStructs.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include "TransformOperation.hpp"
#include "Painter.hpp"
#include "Selector.hpp"
#include <fstream>

class Editor {
    Mode mode;
    Point3D* projectedPoints;
    Point3D* points;
    Line3D* lines;
    Line3D* projectedLines;
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
    std::vector<Point3D*> selected;
    Vector2 firstPoint;
    Vector2 secondPoint;
    float sidebarwidth;
    bool isVertical;
    bool isGridDraw;
    bool isPointInfo;
    bool isLinesInfo;
    Projection projection;
    bool isPerspective;
    int bufferSize;
    char** Text;
public: 
    Editor(int height, int width, int configFlags);
    ~Editor();
    void Setup(int configFlags);
    void Init(const char* windowName, int targetFPS);
    void UpdateFrame();
private:
    void UpdateMode();
    void UpdateProjection();
    void UpdateMirror();
    void UpdateSelection();
    void UpdatePoints();
    void UpdateLines();
    void UpdateButtons();
    void ProjectionToggle();
    void MovePoints(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint);
    void MovePoints3D(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint, Projection projection);
    void RotatePoints(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint);
    void RotatePoints3D(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint, Projection projection, RotationAxis rotation);
    void RotatePoints3D(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint, Vector3 center, Projection projection, RotationAxis rotation);
    void ScalePoints(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint,Vector3 center, bool isGeneral);
    void DrawFrame();
    Point3D* CreateNewPoint2D(Vector2 pos);
    Point3D* CreateNewPoint3D(Vector3 pos);
    bool CheckIfLineExist(Point3D* p1, Point3D* p2);
    std::vector<Point3D*> GetPointAllLines(Point3D*& point);
    Line3D* CreateLine(Point3D* p1, Point3D* p2);
    void CopyPaste();
    Line3D CopyLinesProjectedPoints(Line3D*,int);
    void PrintArray();
    void SaveProject(std::string);
    void LoadProject(std::string);
    void OpenSaveWindow();
    void OpenLoadWindow();
};