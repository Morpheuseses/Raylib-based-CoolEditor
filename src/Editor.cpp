#include "Editor.hpp"

 Editor::Editor(int height, int width, int configFlags) {
    mode = Create;
    points = new Point3D[MAX_POINTS_SIZE];
    projectedPoints = new Point3D[MAX_POINTS_SIZE];
    lines = new Line3D[MAX_LINES_SIZE];
    windowHeight = height;
    windowWidth = width;
    sidebarwidth = width * 0.25;
    isVertical = false;
    isGridDraw = true;
    isPointInfo = true;
    isLinesInfo = true;
    projection = XY;
    for (int i = 0; i < MAX_POINTS_SIZE; i++) {
        points[i].deleted = true;
    }
    for (int i = 0; i < MAX_LINES_SIZE; i++) {
        lines[i].deleted = true;
    }
    for (int i = 0; i < MAX_LINES_SIZE; i++) {
        projectedPoints[i].deleted = true;
    }
    editPoint = -1;
    Setup(configFlags);
} 
Editor::~Editor() {
    delete[] points;
    delete[] lines;
    delete painter;
    delete selector;
    delete transformer;
    CloseWindow();
}
void Editor::Setup(int configFlags) {
    SetConfigFlags(configFlags);
    painter = new Painter(windowHeight,windowWidth);
    selector = new Selector();
    transformer = new TransformOperation(windowWidth,windowHeight);
}
void Editor::Init(const char* windowName, int targetFPS) {
    InitWindow(windowWidth, windowHeight, windowName);
    SetTargetFPS(targetFPS);
    while (!WindowShouldClose()) {
        UpdateFrame();
    }
}
void Editor::UpdateFrame() {
    BeginDrawing();
    UpdateSelection();
    UpdateProjection();
    UpdatePoints();
    UpdateLines();
    UpdateMode();
    ClearBackground(BLACK);
    DrawFPS(10,10);
    UpdateMirror();
    DrawFrame();
    UpdateButtons();
    EndDrawing();
}

void Editor::UpdateMode() {
    if (IsKeyPressed(KEY_C)) {
        mode = Create;
    }
    if (IsKeyPressed(KEY_E)) {
        mode = Edit;
    }
    if (IsKeyPressed(KEY_D)) {
        mode = Delete;
    }
    if (IsKeyPressed(KEY_L)) {
        mode = Link;
    }
    if (IsKeyPressed(KEY_S)) {
        mode = Selection;
    }
    if (IsKeyPressed(KEY_R)) {
        mode = Rotation;
    }
    if (IsKeyPressed(KEY_G)) {
        mode = Scale;
    }
    if (IsKeyPressed(KEY_M)) {
        mode = Mirror;
    }
}
void Editor::UpdateProjection() {
    if (IsKeyPressed(KEY_V)) {
        transformer->SetWorldCoords(points, MAX_POINTS_SIZE);
        Vector3 zero = {1600/2,900/2,0};
        Vector3 center = {0,0,0};
        for (int i = 0; i < selected.size(); i++) {
            center.x += selected[i]->pos.x;
            center.y += selected[i]->pos.y;
            center.z += selected[i]->pos.z;
        }
        center.x = center.x / selected.size();
        center.y = center.y / selected.size();
        center.z = center.z / selected.size();
        transformer->ProjectPoints3D(selected, zero, {1,1,(float)windowWidth}, false, projection);
        /*
        auto worldCoords = transformer->GetWorldCoords();
        for (int i = 0; i < MAX_POINTS_SIZE; i++) {
            points[i] = worldCoords[i];
        }
        */
    }
}
void Editor::ProjectionToggle() {
    projection = projection == XY ? YZ : XY;
}
void Editor::UpdateButtons() {
    float buttonSize = (windowHeight-30) / 30;
    Rectangle EditModeRect = {windowWidth-sidebarwidth+5,5,sidebarwidth,buttonSize};
    if (GuiButton(EditModeRect, "Edit Mode")) {
        mode = Edit; 
    }
    Rectangle CreateModeRect = {windowWidth-sidebarwidth+5,buttonSize+5*2,sidebarwidth,buttonSize};
    if (GuiButton(CreateModeRect, "Create Mode")) {
        mode = Create; 
    }
    Rectangle RotationModeRect = {windowWidth-sidebarwidth+5,buttonSize*2+5*3,sidebarwidth,buttonSize};
    if (GuiButton(RotationModeRect, "Rotate Mode")) {
        mode = Rotation; 
    }
    Rectangle ScaleModeRect = {windowWidth-sidebarwidth+5,buttonSize*3+5*4,sidebarwidth,buttonSize};
    if (GuiButton(ScaleModeRect, "Scale Mode")) {
        mode = Scale; 
    }
    Rectangle DeleteModeRect = {windowWidth-sidebarwidth+5,buttonSize*4+5*5,sidebarwidth,buttonSize};
    if (GuiButton(DeleteModeRect, "Delete Mode")) {
        mode = Delete; 
    }
    Rectangle SelectionModeRect = {windowWidth-sidebarwidth+5,buttonSize*5+5*6,sidebarwidth,buttonSize};
    if (GuiButton(SelectionModeRect, "Selection Mode")) {
        mode = Selection; 
    }
    Rectangle LinkModeRect = {windowWidth-sidebarwidth+5,buttonSize*6+5*7,sidebarwidth,buttonSize};
    if (GuiButton(LinkModeRect, "Link Mode")) {
        mode = Link; 
    }
    Rectangle MirrorModeRect = {windowWidth-sidebarwidth+5,buttonSize*7+5*8,sidebarwidth-100,buttonSize};
    if (GuiButton(MirrorModeRect, "Mirror Mode")) {
        mode = Mirror; 
    }
    Rectangle MirrorModeRectToggleXY = {windowWidth-90, buttonSize*7+5*8, 85, buttonSize};
    if (GuiButton(MirrorModeRectToggleXY, "XY toggle")) {
        isVertical = isVertical ? false : true; 
    }
    Rectangle GridToggleRect = {windowWidth-sidebarwidth+5,buttonSize*8+5*9,sidebarwidth,buttonSize};
    if (GuiButton(GridToggleRect, "Grid visibility")) {
        isGridDraw = isGridDraw ? false : true; 
    }
    Rectangle PointsInfoRect = {windowWidth-sidebarwidth+5,buttonSize*9+5*10,sidebarwidth,buttonSize};
    if (GuiButton(PointsInfoRect, "Points info visibility")) {
        isPointInfo = isPointInfo ? false : true; 
    }
    Rectangle LinesInfoRect = {windowWidth-sidebarwidth+5,buttonSize*10+5*11,sidebarwidth,buttonSize};
    if (GuiButton(LinesInfoRect, "Lines info visibility")) {
        isLinesInfo = isLinesInfo ? false : true; 
    }
    Rectangle ProjectionToggleRect = {windowWidth-sidebarwidth+5,buttonSize*11+5*12,sidebarwidth,buttonSize};
    if (GuiButton(ProjectionToggleRect, "Projection")) {
        ProjectionToggle(); 
    }
}
void Editor::UpdateMirror() {
    if (mode == Mirror) {
        Vector3 pos;
        if (projection == YZ) 
            pos = {0,GetMousePosition().y,GetMousePosition().x};
        else
            pos = {GetMousePosition().x,GetMousePosition().y,0};
        if (IsKeyPressed(KEY_X))
            if (!isVertical)
                isVertical = true;
            else 
                isVertical = false;
        painter->DrawMirrorLine(GetMousePosition(),isVertical);
        if (!selected.empty()) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
                Vector3 zero = {0,0,0};
                if (isVertical)
                    transformer->MirrorPoints3D(selected,zero,pos,true,projection);
                else
                    transformer->MirrorPoints3D(selected,zero,pos,false,projection);
            }
            
        }
    }
}
void Editor::UpdateSelection() {
    if (mode == Selection) {
        Vector2 start;
        Vector2 end;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            beginPosSelection = GetMousePosition();
            //std::cout << "pressed" << std::endl; 
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            Vector2 endPosSelection = GetMousePosition();
            if (endPosSelection.x>=beginPosSelection.x && endPosSelection.y>=beginPosSelection.y) {
                start = beginPosSelection;
                end   = endPosSelection;
            }
            else if (endPosSelection.x<=beginPosSelection.x && endPosSelection.y<=beginPosSelection.y) {
                start = endPosSelection;
                end   = beginPosSelection;
            }
            else if (endPosSelection.x>=beginPosSelection.x && endPosSelection.y<=beginPosSelection.y) {
                start = {beginPosSelection.x, endPosSelection.y};
                end   = {endPosSelection.x, beginPosSelection.y};
            }
            else if (endPosSelection.x<=beginPosSelection.x && endPosSelection.y>=beginPosSelection.y) {
                start = {endPosSelection.x,beginPosSelection.y};
                end   = {beginPosSelection.x, endPosSelection.y};
            }
            painter->DrawRectangleSelection(start,end);
            //std::cout << "down" << std::endl;
            selected = selector->RectangleSelect(points,MAX_POINTS_SIZE,start,end,projection);
            std::cout << "Were selected: " << selected.size() << std::endl;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            //std::cout << "released" << std::endl;
            selector->ClearPointsSelection(points,MAX_POINTS_SIZE);
        }
    }
}
void Editor::PrintArray() {
    for (int i = 0; i < MAX_POINTS_SIZE; i++) {
        if (!points[i].deleted)
            
            std::cout << "X " << points[i].pos.x << "Y " << points[i].pos.y << "Z " << points[i].pos.z << std::endl;
            
    }
}
void Editor::UpdatePoints() {
    if (mode == Create) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < MAX_POINTS_SIZE; i++) {
                if (!CheckCollisionPointCircle(GetMousePosition(), {points[i].pos.x,points[i].pos.x}, 10) && GetMousePosition().x<windowWidth-sidebarwidth)
                    if (points[i].deleted) {
                        auto mousePos = GetMousePosition();
                        points[i].pos = {mousePos.x,mousePos.y,1};
                        points[i].deleted = false;
                        break;
                        
                    }
            }
        }
    }
    
    if (mode == Edit) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            selected.clear();
        }
        if (selected.empty()) {
            for (int i = 0; i < MAX_POINTS_SIZE; i++) {
                if (!points[i].deleted)
                    switch (projection) {
                        case XY:
                            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) 
                            && (CheckCollisionPointCircle(GetMousePosition(),{points[i].pos.x, points[i].pos.y}, 10) 
                            || points[i].selected)) {
                                editPoint = i;
                                points[i].selected = true;
                            }
                            else if (CheckCollisionPointCircle(GetMousePosition(),{points[i].pos.x, points[i].pos.y}, 10)) {
                                points[i].focused = true;
                            }
                            else {
                                points[i].focused = false;
                            }
                            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                                editPoint = -1;
                                points[i].selected = false;
                            }
                            break;
                        case YZ:
                            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) 
                            && (CheckCollisionPointCircle(GetMousePosition(),{points[i].pos.z, points[i].pos.y}, 10) 
                            || points[i].selected)) {
                                editPoint = i;
                                points[i].selected = true;
                            }
                            else if (CheckCollisionPointCircle(GetMousePosition(),{points[i].pos.z, points[i].pos.y}, 10)) {
                                points[i].focused = true;
                            }
                            else {
                                points[i].focused = false;
                            }
                            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                                editPoint = -1;
                                points[i].selected = false;
                            }
                            break;
                    };
                    
            }
            if (editPoint >= 0)
                if (GetMousePosition().x < windowWidth-10 && GetMousePosition().x > 0+10 
                                && GetMousePosition().y < windowHeight-10 && GetMousePosition().y > 0+10) {
                    auto pos = GetMousePosition();
                    switch (projection) {
                        case XY:
                            points[editPoint].pos.x = pos.x;
                            points[editPoint].pos.y = pos.y;
                            break;
                        case YZ:
                            points[editPoint].pos.z = pos.x;
                            points[editPoint].pos.y = pos.y;
                            break;
                    }
                    
                }
                    
        
        } 
    else {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                firstPoint = GetMousePosition();
                transformer->setInitial(selected);
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (secondPoint.x != GetMousePosition().x || secondPoint.y != GetMousePosition().y) {
                    secondPoint = GetMousePosition();
                    Vector2 fp_z = {firstPoint.x,firstPoint.y};
                    Vector2 sp_z = {secondPoint.x,secondPoint.y}; 
                    MovePoints3D(selected,fp_z,sp_z,projection);
                    DrawLine(firstPoint.x,firstPoint.y,secondPoint.x,secondPoint.y,WHITE);
                }
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                firstPoint = {-1,-1};
                secondPoint ={-1,-1};
            }
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_P)){
                CopyPaste();
            }
        }
    }
    if (mode == Delete) {
        if (!selected.empty()) {
            for (int i = 0; i < selected.size(); i++) {
                selected[i]->deleted = true;
            } 
            selected.clear();
            return;
        }
        for (int i = 0; i < MAX_POINTS_SIZE; i++) {
            Vector2 pos;
            switch (projection) {
                case XY:
                    pos = {points[i].pos.x, points[i].pos.y};
                    if (CheckCollisionPointCircle(GetMousePosition(),pos, 10))
                        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                            points[i].deleted = true;
                    }
                    break;
                case YZ:
                    pos = {points[i].pos.z, points[i].pos.y};
                    if (CheckCollisionPointCircle(GetMousePosition(),pos, 10))
                        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                            points[i].deleted = true;
                    }
                    break;
            }
                
        }   
    }
    
    if (mode == Rotation && !selected.empty() && GetMousePosition().x < windowWidth-sidebarwidth) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            firstPoint = GetMousePosition();
            transformer->setInitial(selected);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            if (secondPoint.x != GetMousePosition().x || secondPoint.y != GetMousePosition().y) {
                secondPoint = GetMousePosition();
                Vector2 fp_z = {firstPoint.x,firstPoint.y};
                Vector2 sp_z = {secondPoint.x,secondPoint.y}; 
                if (projection == XY)
                    RotatePoints3D(selected,fp_z,sp_z,projection,Z);
                if (projection == YZ)
                    RotatePoints3D(selected,fp_z,sp_z,projection,X);
                DrawLine(firstPoint.x,firstPoint.y,secondPoint.x,secondPoint.y,WHITE);
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            firstPoint = {-1,-1};
            secondPoint ={-1,-1};
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            firstPoint = GetMousePosition();
            transformer->setInitial(selected);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            if (secondPoint.x != GetMousePosition().x || secondPoint.y != GetMousePosition().y) {
                secondPoint = GetMousePosition();
                Vector3 center = {0,0,0};
                for (int i = 0; i < selected.size(); i++) {
                    center.x += selected[i]->pos.x;
                    center.y += selected[i]->pos.y;
                    center.z += selected[i]->pos.z;
                }
                center.x = center.x / selected.size();
                center.y = center.y / selected.size();
                center.z = center.z / selected.size();
                RotatePoints3D(selected,firstPoint,secondPoint,center,projection,Y);    
                DrawLine(firstPoint.x,firstPoint.y,secondPoint.x,secondPoint.y,WHITE);
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            firstPoint = {-1,-1};
            secondPoint ={-1,-1};
        }
    }
    
    if (mode == Scale && !selected.empty()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            firstPoint = GetMousePosition();
            transformer->setInitial(selected);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            if (secondPoint.x != GetMousePosition().x || secondPoint.y != GetMousePosition().y) {
                secondPoint = GetMousePosition();
                Vector3 point = {0,0,0};
                for (int i = 0; i < selected.size(); i++) {
                    point.x += selected[i]->pos.x;
                    point.y += selected[i]->pos.y;
                    point.z += selected[i]->pos.z;
                }
                point.x = point.x / selected.size();
                point.y = point.y / selected.size();
                point.z = point.z / selected.size();
                ScalePoints(selected,point,secondPoint,true);
                DrawLine(firstPoint.x,firstPoint.y,secondPoint.x,secondPoint.y,WHITE);
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            firstPoint = {-1,-1};
            secondPoint ={-1,-1};
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            firstPoint = GetMousePosition();
            transformer->setInitial(selected);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            if (secondPoint.x != GetMousePosition().x || secondPoint.y != GetMousePosition().y) {
                secondPoint = GetMousePosition();
                Vector3 point = {0,0,0};
                for (int i = 0; i < selected.size(); i++) {
                    point.x += selected[i]->pos.x;
                    point.y += selected[i]->pos.y;
                    point.z += selected[i]->pos.z;
                }
                point.x = point.x / selected.size();
                point.y = point.y / selected.size();
                point.z = point.z / selected.size();
                ScalePoints(selected,point,secondPoint,false);
                DrawLine(firstPoint.x,firstPoint.y,secondPoint.x,secondPoint.y,WHITE);
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            firstPoint = {-1,-1};
            secondPoint ={-1,-1};
        }
    }
    
}
void Editor::UpdateLines() {

    for (int i = 0; i < MAX_LINES_SIZE; i++) {
        if (lines[i].startPoint == nullptr || lines[i].endPoint == nullptr)
            lines[i].deleted = true;
        else if (lines[i].startPoint->deleted || lines[i].endPoint->deleted) {
            lines[i].deleted = true;
        }
            
    }
    if (mode == Link) {
        Vector2 pos = GetMousePosition();
        selector->Link(lines,points,MAX_POINTS_SIZE,MAX_LINES_SIZE,projection);
    }
    if (mode == Edit) {
        for (int i = 0; i < MAX_LINES_SIZE; i++) {
            if (!lines[i].deleted)
                switch (projection) {
                    case XY:
                        if (CheckCollisionPointLine(GetMousePosition(),{lines[i].startPoint->pos.x, lines[i].startPoint->pos.y}, 
                                {lines[i].endPoint->pos.x, lines[i].endPoint->pos.y}, 2)) {
                            lines[i].focused = true;
                        }
                        else {
                            lines[i].focused = false;
                        }
                        break;
                    case YZ:
                        if (CheckCollisionPointLine(GetMousePosition(),{lines[i].startPoint->pos.z, lines[i].startPoint->pos.y}, 
                                {lines[i].endPoint->pos.z, lines[i].endPoint->pos.y}, 2)) {
                            lines[i].focused = true;
                        }
                        else {
                            lines[i].focused = false;
                        }
                        break;
                }
                
        }
    }
    if (mode == Delete) {
        Vector2 pos = GetMousePosition();
        for (int i = 0; i < MAX_LINES_SIZE; i++) 
            if (!lines[i].deleted)
                if (CheckCollisionPointLine(pos,{lines[i].startPoint->pos.x,lines[i].startPoint->pos.y}, 
                                {lines[i].endPoint->pos.x, lines[i].endPoint->pos.y},4)) {                        
                        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) 
                            lines[i].deleted = true;
                }
    }
}
void Editor::MovePoints(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint) {
    transformer->MovePoints(selected,firstPoint,secondPoint,true);
}
void Editor::MovePoints3D(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint,Projection projection) {
    transformer->MovePoints3D(selected,firstPoint,secondPoint,true,projection);
}
void Editor::RotatePoints(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint) {
    transformer->RotatePoints(selected,firstPoint,secondPoint,true);
}
void Editor::RotatePoints3D(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint, Projection projection, RotationAxis rotation) {
    transformer->RotatePoints3D(selected,firstPoint,secondPoint,true,projection,rotation);
}
void Editor::RotatePoints3D(std::vector<Point3D*> selected, Vector2 firstPoint, Vector2 secondPoint, Vector3 center, Projection projection, RotationAxis rotation) {
    transformer->RotatePoints3D(selected,firstPoint,secondPoint,center,true,projection,rotation);
}
void Editor::ScalePoints(std::vector<Point3D*> selected, Vector3 firstPoint, Vector2 secondPoint,bool isGeneral) {
    transformer->ScalePoints3D(selected,firstPoint,secondPoint,true,isGeneral,projection);
}
void Editor::DrawFrame() {
    if (isGridDraw)
        painter->DrawGrid(40);
    painter->DrawLines(lines,MAX_LINES_SIZE,projection);
    painter->DrawPoints(points,MAX_POINTS_SIZE,projection);
    painter->DrawText(mode);
    if (isPointInfo)
        painter->DrawPointsInfo(points,MAX_POINTS_SIZE,projection);
    if (isLinesInfo)
        painter->DrawLinesInfo(lines,MAX_LINES_SIZE,projection);
    painter->DrawSideInterface(windowWidth*0.25);
    painter->DrawBottomInterface(lines,MAX_LINES_SIZE,projection);
    painter->DrawSelectedInfo(selected.size());
}
Point3D* Editor::CreateNewPoint2D(Vector2 pos) {
    for (int i =0; i < MAX_POINTS_SIZE; i++) {
        if (points[i].deleted) {
            points[i].pos      = {pos.x, pos.y, 1};
            points[i].selected = false;
            points[i].focused  = false;
            points[i].deleted  = false;
            return &points[i];
        }
    }        
    return nullptr;
}
Point3D* Editor::CreateNewPoint3D(Vector3 pos) {
    for (int i = 0; i < MAX_POINTS_SIZE; i++) {
        if (points[i].deleted) {
            points[i].pos      = {pos.x, pos.y, pos.z};
            points[i].selected = false;
            points[i].focused  = false;
            points[i].deleted  = false;
            return &points[i];
        }
    }
    return points;
}
bool Editor::CheckIfLineExist(Point3D* p1, Point3D* p2) {
    for (int i = 0; i < MAX_LINES_SIZE; i++) {
        if (lines[i].startPoint == p1 && lines[i].endPoint == p2 || lines[i].startPoint == p2 && lines[i].endPoint == p1) {
            return true;
        }
    }
    return false;
}
std::vector<Point3D*> Editor::GetPointAllLines(Point3D*& point) {
    std::vector<Point3D*> res;
    for (int i = 0; i < MAX_POINTS_SIZE; i++) {
        if (CheckIfLineExist(point, &points[i])) {
            res.push_back(&points[i]);
        }
    }
    return res;
}
Line3D* Editor::CreateLine(Point3D* p1, Point3D* p2) {
    for (int i = 0; i < MAX_LINES_SIZE; i++) {
        if (lines[i].deleted) {
            lines[i].startPoint = p1;
            lines[i].endPoint   = p2;
            lines[i].selected   = false;
            lines[i].focused    = false;
            lines[i].deleted    = false;
            return &lines[i];
        }
    }
    return lines;
}
void Editor::CopyPaste() {
    std::vector<Point3D*> new_selected;
    if (!selected.empty()) {
        for (int i = 0; i < selected.size(); i++) {
            Vector3 pos = {selected[i]->pos.x+5,selected[i]->pos.y+5,selected[i]->pos.z+5};
            new_selected.push_back(CreateNewPoint3D(pos));
            
        }
        for (int i =0; i < selected.size(); i++) {
            auto linkedPoints = GetPointAllLines(selected[i]);
            std::cout << "Linked size:" << linkedPoints.size() << std::endl;
                for (int j = 0; j < selected.size(); j++) {
                        if (std::find(linkedPoints.begin(), linkedPoints.end(), selected[j]) != linkedPoints.end()) {
                            CreateLine(new_selected[j],new_selected[i]);
                            std::cout << "Line has been created"<< std::endl;
                        }
                }
            }
        }
        selected = new_selected;
    }
