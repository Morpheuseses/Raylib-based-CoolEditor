#include "Editor.hpp"

int main() {
    int windowHeight = 900;
    int windowWidth = 1600;
    Editor* editor = new Editor(windowHeight,windowWidth,FLAG_MSAA_4X_HINT);
    editor->Init("SomeEditor",60);
    return 0;
}