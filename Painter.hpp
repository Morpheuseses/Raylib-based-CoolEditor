#pragma once

#include "raylib.h"
#include "raygui.h"
#include "BaseStructs.hpp"

class Painter {
    int windowHeight;
    int windowWidth;
public:
    Painter(int height, int width) {
        windowHeight = height;
        windowWidth = width;
    }
public:
    void DrawPoints(Point* points,int size) {
        for (int i = 0; i < size; i++) {
            if (!points[i].deleted && (points[i].pos.x != -1 && points[i].pos.y != -1))
                if (points[i].selected == true) {
                    DrawCircle(points[i].pos.x, points[i].pos.y,15,GREEN);
                }
                else if (points[i].focused == true)
                        DrawCircle(points[i].pos.x, points[i].pos.y,15,YELLOW);
                    else 
                        DrawCircle(points[i].pos.x, points[i].pos.y,10,LIGHTGRAY);   
        }
    }
    void DrawLines(Line* lines,int size) {
        for (int i = 0; i < size; i++) {
            if (!lines[i].deleted) {
                if (lines[i].selected == true) {
                   DrawLineEx(lines[i].startPoint->pos, lines[i].endPoint->pos, 3, GREEN);
                }
                else if (lines[i].focused == true)
                        DrawLineEx(lines[i].startPoint->pos, lines[i].endPoint->pos, 3, YELLOW);
                    else 
                        DrawLineEx(lines[i].startPoint->pos, lines[i].endPoint->pos, 3, LIGHTGRAY);
            }
        }
    }
    void DrawText(Mode mode,Point* points,Line* lines,int points_size,int lines_size) {
        const char* text; 
        switch (mode)
        {
            case Create:
                text = "Create mode";         
                break;
            case Edit:
                text = "Edit mode";
                break;
            case Delete:
                text = "Delete mode";
                break;
            case Link:
                text = "Link mode";
                break;
            case Selection:
                text = "Selection mode";
                break;
            case Rotation:
                text = "Rotation mode";
                break;
            case Scale:
                text = "Scale mode";
                break;
            case Mirror:
                text = "Mirror mode";
                break;
        }
        RAYLIB_H::DrawText(text, 10, windowHeight-50, 20, GRAY);
    }
    void DrawPointsInfo(Point* points, int points_size) {
        int number = 1;
        for (int i = 0; i < points_size; i++) {
            if (!points[i].deleted && (points[i].pos.x != -1 && points[i].pos.y != -1)) {
                RAYLIB_H::DrawText(TextFormat("[ %.0f, %.0f]", points[i].pos.x, points[i].pos.y), points[i].pos.x+5, points[i].pos.y+5, 12, GRAY);
                RAYLIB_H::DrawText(TextFormat("[%i]", number), points[i].pos.x-6, points[i].pos.y-6, 15, BLACK);
                number++;
            }
        }
    }
    void DrawLinesInfo(Line* lines, int lines_size) {
        for (int i = 0; i < lines_size; i++) {
            if (!lines[i].deleted) {
                int labelpointX = (lines[i].endPoint->pos.x + lines[i].startPoint->pos.x) / 2;
                int labelpointY = (lines[i].endPoint->pos.y + lines[i].startPoint->pos.y) / 2;
                float A = lines[i].startPoint->pos.y - lines[i].endPoint->pos.y;  
                float B = lines[i].endPoint->pos.x - lines[i].startPoint->pos.x;
                float C = lines[i].startPoint->pos.x * lines[i].endPoint->pos.y - lines[i].endPoint->pos.x * lines[i].startPoint->pos.y;
                RAYLIB_H::DrawText(TextFormat("(%.0f, %.0f, %.0f)", A, B, C), labelpointX+5, labelpointY+5, 12, GRAY);
            }
        }
    }
    void DrawRectangleSelection(Vector2 begin, Vector2 end) {
        Rectangle rec = {begin.x, begin.y,end.x-begin.x,end.y-begin.y};
        DrawRectangleLinesEx(rec,3,WHITE);
        DrawRectangle(begin.x, begin.y,end.x-begin.x,end.y-begin.y,CLITERAL(Color){50,50,80,70});
    }
    void DrawCursorPos() {
        Vector2 pos = GetMousePosition();
        RAYLIB_H::DrawText(TextFormat("X: %.0f, Y: %.0f", pos.x, pos.y), 
                                windowWidth-windowWidth*0.1, windowHeight-25, 20, WHITE);
    }
    void DrawSelectedInfo(float quantity) {
        RAYLIB_H::DrawText(TextFormat("%.0f objects were selected",quantity),
                                windowWidth-windowWidth*0.3, windowHeight-25,20,WHITE);
    }
    void DrawEquation(Line* lines, int size) {
        int labelpointX = 5;
        int labelpointY = windowHeight-25;
        int fontSize = 20;
        for (int i = 0; i < size; i++) {
            if (lines[i].focused) {
                float A = lines[i].startPoint->pos.y - lines[i].endPoint->pos.y;  
                float B = lines[i].endPoint->pos.x - lines[i].startPoint->pos.x;
                float C = lines[i].startPoint->pos.x * lines[i].endPoint->pos.y
                                -lines[i].endPoint->pos.x * lines[i].startPoint->pos.y;
                
                RAYLIB_H::DrawText(TextFormat("%.0fx+%.0fy+%.0f=0",A,B,C), labelpointX, labelpointY, fontSize, WHITE);
            }
        }        
    }
    void DrawSideInterface(int width) {
        //Rectangle rec = {windowWidth-width,windowHeight-30,width,windowHeight-30};
        DrawRectangle(windowWidth-width,0,width,windowHeight-30,DARKGRAY);
    }
    void DrawBottomInterface(Line* lines,int size) {
        DrawRectangle(0,windowHeight-30,windowWidth,30,CLITERAL(Color){40,40,40,255});
        DrawCursorPos();
        DrawEquation(lines,size);
    }
    void DrawMirrorLine(Vector2 vec, bool isX) {
        if (isX)
            DrawLine(vec.x,0,vec.x,windowHeight,WHITE);
        else
            DrawLine(0,vec.y,windowWidth,vec.y,WHITE);
    }
    void DrawGrid(int spacing) {
        for (int i = 0; i < windowWidth; i+=spacing) {
            DrawLine(i,0, i,windowHeight,DARKGRAY);
        }
        for (int i = 0; i < windowHeight; i+=spacing) {
            DrawLine(0,i, windowWidth,i,DARKGRAY);
        }
    }
};

