#pragma once
#include "raylib.h"
#include "raygui.h"

enum Mode {
    Edit,
    Create,
    Delete,
    Link,
    Selection,
    Move,
    Rotation,
    Scale,
    Mirror,
    Perspective,
    Save,
    Load
};
enum Projection {
    XY,
    YZ
};
enum RotationAxis {
    Y,
    X,
    Z
};
struct GeometryObject {
    bool focused;
    bool deleted;
    bool selected;
};
struct Point : GeometryObject {
    Vector2 pos;
    Point() {
        focused  = false;
        deleted  = false;
        selected = false;
    }
};
struct Line : GeometryObject {
    Point* startPoint = nullptr;
    Point* endPoint   = nullptr;
    Line() {
        focused  = false;
        deleted  = false;
        selected = false;
    }
};
struct Point3D : GeometryObject {
    Vector3 pos;
    Point3D() {
        focused  = false;
        deleted  = false;
        selected = false;
    }
};
struct Line3D : GeometryObject {
    Point3D* startPoint = nullptr;
    Point3D* endPoint   = nullptr;
    Line3D() {
        focused  = false;
        deleted  = false;
        selected = false;
    }
};