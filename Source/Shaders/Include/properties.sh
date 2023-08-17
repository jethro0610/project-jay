#ifndef PROPERTIES_H
#define PROPERTIES_H

uniform mat4 u_materialProps;

#define PROP_COLOR u_materialProps[3]

#define PROP_SPEC_POWER u_materialProps[0][0]
#define PROP_SPEC_THRESH u_materialProps[0][1]
#define PROP_SPEC_BRIGHTNESS u_materialProps[0][2]

#define PROP_FRES_POWER u_materialProps[1][0]
#define PROP_FRES_SCALE u_materialProps[1][1]
#define PROP_FRES_BRIGHTNESS u_materialProps[1][2]

#endif
