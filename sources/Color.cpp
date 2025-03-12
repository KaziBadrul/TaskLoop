#include "../headers/Color.h"

void setconsolecolor(int textColor, int bgColor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (bgColor * 16)));
}

void SetColor(int textColor)
{
    std::cout << "\033[" << textColor << "m";
}

void ResetColor()
{
    std::cout << "\033[0m";
}
