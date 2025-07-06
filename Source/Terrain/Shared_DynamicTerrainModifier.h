#pragma once
#ifndef SHARED_SHADER
    #include <glm/glm.hpp>
    #define VEC2 glm::vec2
#else
    #define VEC2 vec2
#endif

#define DYN_MOD_MAX 16
#define DYN_MOD_POS_X(modifier) (modifier)[0][0]
#define DYN_MOD_POS_Y(modifier) (modifier)[0][1]
#define DYN_MOD_POS_Z(modifier) (modifier)[0][2]
#define DYN_MOD_IS_ACTIVE(modifier) ((modifier)[0][3] > 0.0f)

#ifndef SHARED_SHADER
struct DynamicTerrainModifier {
    glm::vec3 position;
    union {
        bool active;
        float activePadding;
    };
    
    glm::vec4& operator[](int index) const {
        return (*(glm::mat4*)(this))[index];
    }
};
#endif
