#ifndef PROPERTIES_SH
#define PROPERTIES_SH

uniform vec4 u_mProps[4];

#define PROP_COLOR u_mProps[0]
#define PROP_TEXSCALE u_mProps[1].xy

#define PROP_SPEC_POWER u_mProps[1].z
#define PROP_SPEC_THRESH u_mProps[1].w
#define PROP_SPEC_BRIGHTNESS u_mProps[2].x
#define PROP_FRES_POWER u_mProps[2].y
#define PROP_FRES_SCALE u_mProps[2].z
#define PROP_FRES_BRIGHTNESS u_mProps[2].w

#define PROP_VARIATION_FREQUENCY u_mProps[3].x
#define PROP_VARIATION_MIN u_mProps[3].y
#define PROP_VARIATION_MAX u_mProps[3].z
#define PROP_VARIATION_POWER u_mProps[3].w

#define PROP_VOLUMETRIC_ORIGIN (u_mProps[0].xyz)
#define PROP_VOLUMETRIC_RADIUS (u_mProps[0].w)
#define PROP_VOLUMETRIC_STEP_DIST (u_mProps[1].x)
#define PROP_VOLUMETRIC_LIGHT_ACCUM (u_mProps[1].y)
#define PROP_VOLUMETRIC_FALLOFF_HEIGHT (u_mProps[1].z)
#define PROP_VOLUMETRIC_COLOR (u_mProps[2])

#endif
