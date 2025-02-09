#ifndef WORLD_TEXT_H
#define WORLD_TEXT_H

uniform vec4 u_worldText[2];

#define WORLD_TEXT_POSITION u_worldText[0].xyz
#define WORLD_TEXT_SIZE u_worldText[0].w
#define WORLD_TEXT_COUNT u_worldText[1].x
#define WORLD_TEXT_H_ALIGN u_worldText[1].y
#define WORLD_TEXT_V_ALIGN u_worldText[1].z

#endif
