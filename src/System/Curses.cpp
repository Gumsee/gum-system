#include "Curses.h"
#include <curses.h>
#include <ncurses.h>

Curses::Curses()
{
    initscr();
    update();
    cbreak();
}

Curses::~Curses()
{
    endwin();
}

void Curses::update()
{
    refresh();
    getch();
    getmaxyx(stdscr, v2ConsoleSize.x, v2ConsoleSize.y);
}

void Curses::printAt(std::string msg, ivec2 pos)
{
    mvprintw(pos.x, pos.y, "%s", msg.c_str());
}


//
// Setter
// 
void hideInput(bool hide)
{
    if(hide)
        noecho();
    else
        echo();
}

//
// Getter
//
ivec2 Curses::getConsoleSize() { return this->v2ConsoleSize; }