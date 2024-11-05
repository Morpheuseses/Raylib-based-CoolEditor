#include "TransformOperation.hpp"
#include <vector>
#include <iostream>
#include <cmath>

std::vector<std::vector<float>> TransformOperation::MakeMoveMatrix2D(Vector2 vec) {
    return {
        {1,     0,     0},
        {0,     1,     0},
        {vec.x, vec.y, 1}
    };
}
std::vector<std::vector<float>> TransformOperation::MakeMirrorMatrix2D(int isX) {
    if (isX) 
        return {
            {-1,     0,    0},
            {0,     1,     0},
            {0,     0,     1}
        };
    else 
        return {
            {1,     0,     0},
            {0,    -1,     0},
            {0,     0,     1}
        };
}
std::vector<std::vector<float>> TransformOperation::MakeRotationMatrix2D(Vector2 vec) {
    float pi = 3.1415;
    float cos = std::cosf(vec.x / (width*0.25) * pi);
    float sin = std::sinf(vec.x / (width*0.25) * pi);
    std::cout << sin << " " << cos << std::endl; 
    return {
        {cos,  -sin,     0},
        {sin,   cos,     0},
        {0,     0,       1}
    };
}
std::vector<std::vector<float>> TransformOperation::MakeGeneralScaleMatrix2D(Vector2 vec) {
    float s = 1 / (vec.x / (width*0.1)+1);
    return {
        {1,   0,    0},
        {0,   1,    0},
        {0,   0,    s}
    };
}
std::vector<std::vector<float>> TransformOperation::MakeScaleMatrix2D(Vector2 vec) {
    float sx = vec.x / (width*0.1)+1;
    float sy = vec.y / (width*0.1)+1;
    return {
        {sx,   0,    0},
        {0,   sy,    0},
        {0,   0,     1}
    };
}   
std::vector<std::vector<float>> TransformOperation::ConvertPointsToVector2D(std::vector<Point*> points) {
    auto res = std::vector<std::vector<float>>(points.size(),std::vector<float>(3,0));
    for (int i = 0; i < points.size(); i++) {
        res[i][0] = points[i]->pos.x;
        res[i][1] = points[i]->pos.y;
        res[i][2] = 1; 
    }
    return res;
}
std::vector<std::vector<float>> TransformOperation::ConvertPointsToVector2D_WP(std::vector<Point> points) {
    auto res = std::vector<std::vector<float>>(points.size(),std::vector<float>(3,0));
    for (int i = 0; i < points.size(); i++) {
        res[i][0] = points[i].pos.x;
        res[i][1] = points[i].pos.y;
        res[i][2] = 1; 
    }
    return res;
}
std::vector<std::vector<float>> TransformOperation::MatrixMultiplyPoints(std::vector<std::vector<float>> points, std::vector<std::vector<float>> operation) {
    auto res = std::vector<std::vector<float>>(points.size(), std::vector<float>(operation[0].size(),0));
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < operation[0].size(); j++) {
            for (int k = 0; k < operation.size(); k++) {
                res[i][j] += points[i][k]*operation[k][j];
            }
        }
    }
    return res;
}
TransformOperation::TransformOperation(int width, int height) {
    this->width  = width;
    this->height = height;
}
void TransformOperation::MirrorPoints(std::vector<Point*> points, Vector2 firstPoint, Vector2 secondPoint, bool isX) {
    Vector2 vec = {secondPoint.x-firstPoint.x, secondPoint.y-firstPoint.y};
    Vector2 vecn = {-secondPoint.x+firstPoint.x, -secondPoint.y+firstPoint.y};
    auto operation = MakeMirrorMatrix2D(isX);
    auto move_n    = MakeMoveMatrix2D(vecn);
    auto move_p    = MakeMoveMatrix2D(vec);
    std::vector<std::vector<float>> newPoints;
    newPoints = MatrixMultiplyPoints(ConvertPointsToVector2D(points),move_n);
    newPoints = MatrixMultiplyPoints(newPoints,operation);
    newPoints = MatrixMultiplyPoints(newPoints,move_p);
    for (int i = 0; i < points.size(); i++) {
        points[i]->pos = {newPoints[i][0], newPoints[i][1]};
        std::cout << newPoints[i][0] << " "<< newPoints[i][1] << std::endl;
    }
}
void TransformOperation::MovePoints(std::vector<Point*> points, Vector2 firstPoint, Vector2 secondPoint, bool initial) {
    Vector2 vec = {secondPoint.x-firstPoint.x, secondPoint.y-firstPoint.y};
    auto operation = MakeMoveMatrix2D(vec);
    std::vector<std::vector<float>> newPoints;
    if (!initial)
        newPoints = MatrixMultiplyPoints(ConvertPointsToVector2D(points),operation);
    else
        newPoints = MatrixMultiplyPoints(ConvertPointsToVector2D_WP(initialState),operation);
    for (int i = 0; i < points.size(); i++) {
        points[i]->pos = {newPoints[i][0], newPoints[i][1]};
        std::cout << newPoints[i][0] << " "<< newPoints[i][1] << std::endl;
    }
}
void TransformOperation::RotatePoints(std::vector<Point*> points, Vector2 firstPoint, Vector2 secondPoint, bool initial) {
    Vector2 vec = {secondPoint.x-firstPoint.x, secondPoint.y-firstPoint.y};
    Vector2 vecn = {-firstPoint.x,-firstPoint.y};
    auto rotateOP = MakeRotationMatrix2D(vec);
    auto moveOP_n = MakeMoveMatrix2D(vecn);
    auto moveOP_p = MakeMoveMatrix2D(firstPoint);

    std::vector<std::vector<float>> newPoints;

    //auto complexMatrix = MatrixMultiplyPoints(ConvertPointsToVector2D(points),rotateOP);
    //complexMatrix      = MatrixMultiplyPoints(complexMatrix,rotateOP);
    //complexMatrix      = MatrixMultiplyPoints(complexMatrix,moveOP_p);

    if (!initial) {
        newPoints = MatrixMultiplyPoints(ConvertPointsToVector2D(points),moveOP_n);
        newPoints = MatrixMultiplyPoints(newPoints,rotateOP);
        newPoints = MatrixMultiplyPoints(newPoints,moveOP_p);
    }
    else {
        newPoints = MatrixMultiplyPoints(ConvertPointsToVector2D_WP(initialState),moveOP_n);
        newPoints = MatrixMultiplyPoints(newPoints,rotateOP);
        newPoints = MatrixMultiplyPoints(newPoints,moveOP_p);
    }
    for (int i = 0; i < points.size(); i++) {
        points[i]->pos = {newPoints[i][0], newPoints[i][1]};
        std::cout << newPoints[i][0] << " "<< newPoints[i][1] << " " << newPoints[i][2] <<  std::endl;
    }
}
void TransformOperation::ScalePoints(std::vector<Point*> points, Vector2 firstPoint, Vector2 secondPoint, bool initial, bool isGeneral) {
    Vector2 vec = {secondPoint.x-firstPoint.x, secondPoint.y-firstPoint.y};
    Vector2 vecn = {-firstPoint.x,-firstPoint.y};

    std::vector<std::vector<float>> newPoints;
    std::vector<std::vector<float>> scale; 
    if (isGeneral)
        scale = MakeGeneralScaleMatrix2D(vec);
    else 
        scale = MakeScaleMatrix2D(vec);
    auto moveOP_n = MakeMoveMatrix2D(vecn);
    auto moveOP_p = MakeMoveMatrix2D(firstPoint);

    if (!initial) {
        newPoints = MatrixMultiplyPoints(ConvertPointsToVector2D(points),moveOP_n);
        newPoints = MatrixMultiplyPoints(newPoints,scale);
        newPoints = MatrixMultiplyPoints(newPoints,moveOP_p);
    }
    else {
        newPoints = MatrixMultiplyPoints(ConvertPointsToVector2D_WP(initialState),moveOP_n);
        newPoints = MatrixMultiplyPoints(newPoints,scale);
        newPoints = MatrixMultiplyPoints(newPoints,moveOP_p);
    }
    for (int i = 0; i < points.size(); i++) {
        points[i]->pos = {newPoints[i][0] / newPoints[i][2], newPoints[i][1] / newPoints[i][2]};
        std::cout << newPoints[i][0] << " "<< newPoints[i][1] << " " << newPoints[i][2] <<  std::endl;
    }
}
void TransformOperation::setInitial(std::vector<Point*> initial) {
    this->initialState = std::vector<Point>(initial.size());
    for (int i = 0; i < initial.size(); i++) {
        this->initialState[i] = *initial[i];
    }
}