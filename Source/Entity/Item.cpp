#include "Item.h"

std::string GetItemName(Item item) {
    switch (item) {
        case I_None:
            return "None";

        case I_Homing:
            return "Homing";

        default:
            return "INVALID ITEM";
    }
}

int GetItemMax(Item item) {
    switch (item) {
        case I_None:
            return 0;

        case I_Homing:
            return 6;

        default:
            return 0;
    }
}
