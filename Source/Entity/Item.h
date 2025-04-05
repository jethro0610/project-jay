#pragma once
#include <string>
enum Item {
    I_None = 0,
    I_Homing = 1,
};

std::string GetItemName(Item item);
