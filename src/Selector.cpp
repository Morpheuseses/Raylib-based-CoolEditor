#include "Selector.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

Selector::Selector() {
    selected = std::vector<Point3D*>();
}
Selector::Selector(std::vector<Point3D*> selected) {
    this->selected = selected;
}
Selector::~Selector() {
}
std::vector<Point3D*> Selector::RectangleSelect(Point3D* points,int size, Vector2 begin, Vector2 end, Projection projection) {
    Rectangle rec = {begin.x, begin.y,end.x-begin.x,end.y-begin.y};
    for (int i = 0; i < size; i++) {
        switch (projection) {
            case XY:
                if (CheckCollisionPointRec({points[i].pos.x,points[i].pos.y},rec) && std::find(selected.begin(), selected.end(),&points[i]) == selected.end() && !points[i].deleted) {
                    selected.push_back(&points[i]);
                    points[i].selected = true;
                }
                break;
            case YZ:
                if (CheckCollisionPointRec({points[i].pos.z,points[i].pos.y},rec) && std::find(selected.begin(), selected.end(),&points[i]) == selected.end() && !points[i].deleted) {
                    selected.push_back(&points[i]);
                    points[i].selected = true;
                }
                break;
        }
        
    }
    return selected;
}
void Selector::Link(Line3D* lines,Point3D*& points, int points_size, int lines_size, Projection projection) {
    Vector2 pos = GetMousePosition();
    Vector2 pointPos;
    for (int i = 0; i < points_size; i++) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            switch (projection) {
                case XY:
                    pointPos = {points[i].pos.x, points[i].pos.y};
                    if (CheckCollisionPointCircle(pos,pointPos,10) && selected.size() != 2) {
                        selected.push_back(&points[i]); 
                        std::cout << "Point have been selected"<< std::endl; 
                    }
                    break;
                case YZ:
                    pointPos = {points[i].pos.z, points[i].pos.y};
                    if (CheckCollisionPointCircle(pos,pointPos,10) && selected.size() != 2) {
                        selected.push_back(&points[i]); 
                        std::cout << "Point have been selected"<< std::endl; 
                    }
                    break;
            }
            
        if (selected.size() == 2) {
            break;
        }
    }
    for (int i = 0; i < lines_size; i++) {
        if (selected.size() == 2 && lines[i].deleted) {
            if (selected[0]->pos.x != selected[1]->pos.x || selected[0]->pos.y != selected[1]->pos.y) {
                 lines[i].deleted = false;
                lines[i].startPoint = selected[0];
                lines[i].endPoint = selected[1];
                std::cout << "Line have been created"<< std::endl; 
                selected.clear();
                return;   
            }
        }
    }
    if (selected.size() >= 2) {
        selected.clear();
    }
}
void Selector::ClearPointsSelection(Point3D* points,int size) {
    for (int i = 0; i < size; i++) {
        points[i].selected = false;
    }
    selected.clear();
}