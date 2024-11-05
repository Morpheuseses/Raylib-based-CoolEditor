#include "Editor.hpp"

 Editor::Editor(int height, int width, int configFlags) {
    mode = Create;
    points = new Point[MAX_POINTS_SIZE];
    lines = new Line[MAX_LINES_SIZE];
    windowHeight = height;
    windowWidth = width;
    sidebarwidth = width * 0.25;
    isX = false;
    isGridDraw = true;
    isPointInfo = true;
    isLinesInfo = true;
    for (int i = 0; i < MAX_POINTS_SIZE; i++) {
        points[i].deleted = true;
    }
    for (int i = 0; i < MAX_LINES_SIZE; i++) {
        lines[i].deleted = true;
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
        isX = isX ? false : true; 
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
}
void Editor::UpdateMirror() {
    if (mode == Mirror) {
        auto pos = GetMousePosition();
        if (IsKeyPressed(KEY_X))
            if (!isX)
                isX = true;
            else 
                isX = false;
        painter->DrawMirrorLine(pos,isX);
        if (!selected.empty()) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
                Vector2 zero = {0,0};
                if (isX)
                    transformer->MirrorPoints(selected,zero,pos,true);
                else
                    transformer->MirrorPoints(selected,zero,pos,false);
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
            selected = selector->RectangleSelect(points,MAX_POINTS_SIZE,start,end);
            std::cout << "Were selected: " << selected.size() << std::endl;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
            //std::cout << "released" << std::endl;
            selector->ClearPointsSelection(points,MAX_POINTS_SIZE);
        }
    }
}
void Editor::UpdatePoints() {
    if (mode == Create) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < MAX_POINTS_SIZE; i++) {
                if (!CheckCollisionPointCircle(GetMousePosition(), points[i].pos, 10))
                    if (points[i].deleted) {
                        points[i].pos = GetMousePosition();
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
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) 
                        && (CheckCollisionPointCircle(GetMousePosition(),points[i].pos, 10) 
                        || points[i].selected)) {
                            editPoint = i;
                            points[i].selected = true;
                    }
                    else if (CheckCollisionPointCircle(GetMousePosition(),points[i].pos, 10)) {
                        points[i].focused = true;
                    }
                    else {
                        points[i].focused = false;
                    }
                    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                        editPoint = -1;
                        points[i].selected = false;
                    }
            }
            if (editPoint >= 0)
                if (GetMousePosition().x < windowWidth-10 && GetMousePosition().x > 0+10 
                                && GetMousePosition().y < windowHeight-10 && GetMousePosition().y > 0+10)
                    points[editPoint].pos = GetMousePosition();
        } else {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                firstPoint = GetMousePosition();
                transformer->setInitial(selected);
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (secondPoint.x != GetMousePosition().x || secondPoint.y != GetMousePosition().y) {
                    secondPoint = GetMousePosition();
                    MovePoints(selected,firstPoint,secondPoint);
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
            if (CheckCollisionPointCircle(GetMousePosition(),points[i].pos, 10))
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    points[i].deleted = true;
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
                RotatePoints(selected,firstPoint,secondPoint);
                DrawLine(firstPoint.x,firstPoint.y,secondPoint.x,secondPoint.y,WHITE);
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && GetMousePosition().x < windowWidth-sidebarwidth) {
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
                ScalePoints(selected,firstPoint,secondPoint,true);
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
                ScalePoints(selected,firstPoint,secondPoint,false);
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
        selector->Link(lines,points,MAX_POINTS_SIZE,MAX_LINES_SIZE);
    }
    if (mode == Edit) {
        for (int i = 0; i < MAX_LINES_SIZE; i++) {
            if (!lines[i].deleted)
                if (CheckCollisionPointLine(GetMousePosition(),lines[i].startPoint->pos, lines[i].endPoint->pos, 2)) {
                    lines[i].focused = true;
                }
                else {
                    lines[i].focused = false;
                }
        }
    }
    if (mode == Delete) {
        Vector2 pos = GetMousePosition();
        for (int i = 0; i < MAX_LINES_SIZE; i++) 
            if (!lines[i].deleted)
                if (CheckCollisionPointLine(pos,lines[i].startPoint->pos, 
                                lines[i].endPoint->pos,4)) {                        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) 
                        lines[i].deleted = true;
                }
    }
}
void Editor::MovePoints(std::vector<Point*> selected, Vector2 firstPoint, Vector2 secondPoint) {
    transformer->MovePoints(selected,firstPoint,secondPoint,true);
}
void Editor::RotatePoints(std::vector<Point*> selected, Vector2 firstPoint, Vector2 secondPoint) {
    transformer->RotatePoints(selected,firstPoint,secondPoint,true);
}
void Editor::ScalePoints(std::vector<Point*> selected, Vector2 firstPoint, Vector2 secondPoint,bool isGeneral) {
    transformer->ScalePoints(selected,firstPoint,secondPoint,true,isGeneral);
}
void Editor::DrawFrame() {
    if (isGridDraw)
        painter->DrawGrid(40);
    painter->DrawLines(lines,MAX_LINES_SIZE);
    painter->DrawPoints(points,MAX_POINTS_SIZE);
    painter->DrawText(mode,points,lines,MAX_POINTS_SIZE,MAX_LINES_SIZE);
    if (isPointInfo)
        painter->DrawPointsInfo(points,MAX_POINTS_SIZE);
    if (isLinesInfo)
        painter->DrawLinesInfo(lines,MAX_LINES_SIZE);
    painter->DrawSideInterface(windowWidth*0.25);
    painter->DrawBottomInterface(lines,MAX_LINES_SIZE);
    painter->DrawSelectedInfo(selected.size());
}
Point* Editor::CreateNewPoint2D(Vector2 pos) {
    for (int i =0; i < MAX_POINTS_SIZE; i++) {
        if (points[i].deleted) {
            points[i].pos      = pos;
            points[i].selected = false;
            points[i].focused  = false;
            points[i].deleted  = false;
            return &points[i];
        }
    }        return nullptr;
}
Point* Editor::CreateNewPoint3D(Vector3 pos) {
}
bool Editor::CheckIfLineExist(Point* p1, Point* p2) {
    for (int i = 0; i < MAX_LINES_SIZE; i++) {
        if (lines[i].startPoint == p1 && lines[i].endPoint == p2 || lines[i].startPoint == p2 && lines[i].endPoint == p1) {
            return true;
        }
    }
    return false;
}
std::vector<Point*> Editor::GetPointAllLines(Point*& point) {
    std::vector<Point*> res;
    for (int i = 0; i < MAX_POINTS_SIZE; i++) {
        if (CheckIfLineExist(point, &points[i])) {
            res.push_back(&points[i]);
        }
    }
    return res;
}
Line* Editor::CreateLine(Point* p1, Point* p2) {
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
    return nullptr;
}
void Editor::CopyPaste() {
    std::vector<Point*> new_selected;
    if (!selected.empty()) {
        for (int i = 0; i < selected.size(); i++) {
            Vector2 pos = {selected[i]->pos.x+5,selected[i]->pos.y+5};
            new_selected.push_back(CreateNewPoint2D(pos));
            
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
