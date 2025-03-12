#include "../headers/Database.h"
#include "../headers/User.h"
#include "../headers/Color.h"
#include "../headers/Timer.h"
#include "../headers/TaskManager.h"
#include <iostream>
#include <ctime>
#include <iomanip>

//! MAIN
int main()
{
    // Cleaning up in progress lesgoo 🧹
    TaskManager taskManager("database.db");
    taskManager.start();
    return 0;
}
