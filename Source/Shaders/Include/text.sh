#ifndef TEXT_H
#define TEXT_H

uniform vec4 u_textProps[3];

#define TEXT_POSITION_X u_textProps[0][0]
#define TEXT_POSITION_Y u_textProps[0][1]
#define TEXT_SCALE u_textProps[0][2]
#define TEXT_KERNING u_textProps[0][3]
#define TEXT_H_ALIGN u_textProps[1][0]
#define TEXT_V_ALIGN u_textProps[1][1]
#define TEXT_H_ANCHOR u_textProps[1][2]
#define TEXT_V_ANCHOR u_textProps[1][3]
#define TEXT_COUNT u_textProps[2][0]

#endif
