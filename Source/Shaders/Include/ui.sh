#ifndef UI_H
#define UI_H

uniform vec4 u_uiElem[4];

#define UI_ELEM_POSITION_X u_uiElem[0][0]
#define UI_ELEM_POSITION_Y u_uiElem[0][1]
#define UI_ELEM_SIZE_X u_uiElem[0][2]
#define UI_ELEM_SIZE_Y u_uiElem[0][3]
#define UI_ELEM_H_ALIGN u_uiElem[1][0]
#define UI_ELEM_V_ALIGN u_uiElem[1][1]
#define UI_ELEM_H_ANCHOR u_uiElem[1][2]
#define UI_ELEM_V_ANCHOR u_uiElem[1][3]
#define UI_ELEM_V_ANCHOR u_uiElem[1][3]
#define UI_ELEM_COLOR u_uiElem[2]
#define UI_ELEM_PERCENT u_uiElem[3][0]

#endif
