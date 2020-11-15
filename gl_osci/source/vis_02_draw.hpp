#ifndef VIS_02_DRAW_H
#define VIS_02_DRAW_H
#include "utils.h"
;
#include "globals.h"
;
#include <algorithm>
;
void uploadTex(const void *image, int w, int h);
int screen_width();
int screen_height();
glm::vec2 get_mouse_position();
void draw_circle(float sx, float sy, float rad);
void initDraw();
void world_to_screen(const glm::vec2 &v, int &screeni, int &screenj);
void screen_to_world(int screeni, int screenj, glm::vec2 &v);
void cleanupDraw();
void drawFrame();
#endif