#pragma once
#include <tuple>
#include "ExpandComponents.h"

// Create forward declarations of the types
#define COMPONENTEXPANSION(TYPE, VAR) class TYPE;
#define TAILEXPANSION(TYPE, VAR) class TYPE;
EXPANDCOMPONENTS
#undef COMPONENTEXPANSION 
#undef TAILEXPANSION

class ComponentList {
    std::tuple<
        #define COMPONENTEXPANSION(TYPE, VAR) TYPE&,
        #define TAILEXPANSION(TYPE, VAR) TYPE&
        EXPANDCOMPONENTS
        #undef COMPONENTEXPANSION 
        #undef TAILEXPANSION
    > tuple_;

public:
    ComponentList(
        #define COMPONENTEXPANSION(TYPE, VAR) TYPE& VAR,
        #define TAILEXPANSION(TYPE, VAR) TYPE& VAR
        EXPANDCOMPONENTS
        #undef COMPONENTEXPANSION 
        #undef TAILEXPANSION
    ) 
    :tuple_(
        #define COMPONENTEXPANSION(TYPE, VAR) VAR,
        #define TAILEXPANSION(TYPE, VAR) VAR
        EXPANDCOMPONENTS
        #undef COMPONENTEXPANSION 
        #undef TAILEXPANSION
    ) {

    }
    template <class T>
    T& Get() {
        return std::get<T&>(tuple_);
    }
};
