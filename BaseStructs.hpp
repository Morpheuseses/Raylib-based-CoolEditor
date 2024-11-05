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
    Mirror
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