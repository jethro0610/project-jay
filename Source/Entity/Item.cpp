#include "Item.h"

std::string GetItemName(Item item) {
    switch (item) {
        case I_Homing:
            return "Homing";

        case I_None:
            return "None";

        default:
            return "INVALID ITEM";
    }
}
