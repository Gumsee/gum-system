#pragma once
#include <Maths/vec.h>

class Curses
{
private:
    ivec2 v2ConsoleSize;
    int iCurrentChar;

public:
    Curses();
    ~Curses();

    void update();
    void printAt(std::string msg, ivec2 pos);

    // Setter
    void hideInput(bool hide);

    // Getter
    ivec2 getConsoleSize();
};