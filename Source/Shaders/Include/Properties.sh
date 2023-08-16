#ifndef PROPERTIES_H
#define PROPERTIES_H
uniform mat4 u_materialProps;

#define PROP_COLOR u_materialProps[1]
#define PROP_SPEC_THRESH u_materialProps[0][0]
#define PROP_SPEC_BRIGHTNESS u_materialProps[0][1]
#endif
