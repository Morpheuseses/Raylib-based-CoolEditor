#pragma once

#include "raylib.h"
#include "raygui.h"
#include "BaseStructs.hpp"
#include <vector>

class TransformOperation {
    std::vector<Point> initialState;
    int width;
    int height;
private:
    std::vector<std::vector<float>> MakeMoveMatrix2D(Vector2 vec);
    std::vector<std::vector<float>> MakeMirrorMatrix2D(int isX);
    std::vector<std::vector<float>> MakeRotationMatrix2D(Vector2 vec);
    std::vector<std::vector<float>> MakeGeneralScaleMatrix2D(Vector2 vec);
    std::vector<std::vector<float>> MakeScaleMatrix2D(Vector2 vec);
    std::vector<std::vector<float>> ConvertPointsToVector2D(std::vector<Point*> points);
    std::vector<std::vector<float>> ConvertPointsToVector2D_WP(std::vector<Point> points);
    std::vector<std::vector<float>> MatrixMultiplyPoints(std::vector<std::vector<float>> points, std::vector<std::vector<float>> operation);
public:
    TransformOperation(int width, int height);
    void MirrorPoints(std::vector<Point*> points, Vector2 firstPoint, Vector2 secondPoint, bool isX);
    void MovePoints(std::vector<Point*> points, Vector2 firstPoint, Vector2 secondPoint, bool initial);
    void RotatePoints(std::vector<Point*> points, Vector2 firstPoint, Vector2 secondPoint, bool initial);
    void ScalePoints(std::vector<Point*> points, Vector2 firstPoint, Vector2 secondPoint, bool initial, bool isGeneral);
    void setInitial(std::vector<Point*> initial);
};