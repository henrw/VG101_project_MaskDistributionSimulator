#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "GL/glut.h"

//#include <ctime>
//#include <iostream>
#include "mask_simulation.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int cnt = 0;

int half_screen_width = screen_width / 2;
int half_screen_height = screen_height / 2;

//the ratio of the height and the wide
float radio = screen_width / screen_height;

bool is_transport1 = false;
bool is_transport2 = false;
bool is_hour_changed = false;

bool is_inputed_values = false;

//the maximum day number that the function plotting shows
int history_inf_cache_count = 220;

int current_hour = 0;
int pre_hour = -1;

//the period that the infected people be updated
int inf_refresh_cycle = 60 * 5;
int inf_pre_cycle_pos = 0;
bool need_refresh_inf = false;

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(screen_width, screen_height);
    glutCreateWindow("Mask Distribution Simulator");
    glutDisplayFunc(Show);
    glutMouseFunc(MouseFunc);
    glutKeyboardFunc(KeyboardFuncInputValues);
    glutIdleFunc(IdleFunc);
    glutMainLoop();
    return 0;
}