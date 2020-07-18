#include <Windows.h>
#include "GL/glut.h"
#include <math.h>
#include <string>
#include <time.h>
#include <ctime>
#include <iostream>

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

struct City
{
	//city name
	std::string name;
	//the origin point of circle
	int center_x;
	int center_y;
	//the point where the car begins moving
	int square_x;
	int square_y;
	//the point where the city name is printed at
	int name_x;
	int name_y;

	//radius of circle
	int R;
	//temporarily ignore this one
	float k;
	//the car speed to this city
	float speed;
	//the text in dialogue box
	std::string text1;
	std::string text2;
};

float PI = 3.141592653;

//scaling
float all_scale = 1.0f;

//the width and height of the screen
int screen_width = 1000 * all_scale;
int screen_height = 500 * all_scale;

int half_screen_width = screen_width / 2;
int half_screen_height = screen_height / 2;

//ration of the width and height
float radio = screen_width / screen_height;

//total city number
const int city_count = 8;
//coordinate information of city
City city_infos[city_count] = { 
	{ "A", 178, 198, 179, 194, 138, 200, 75, 1.5, 10, "1 2 3 4 5 6....", "1 2 3 4 5 6...." },
	{ "B", 355, 139, 378, 158, 337, 132, 82, 1.3, 8, "7 8 9 0 1 2....", "7 8 9 0 1 2...." },
	{ "C", 540, 126, 532, 126, 572, 139, 58, 1.1, 6, "3 4 5 6 7 8....", "3 4 5 6 7 8...." },
	{ "D", 264, 301, 271, 307, 270, 287, 70, 1.5, 2, "9 0 1 2 3 4....", "9 0 1 2 3 4...." },
	{ "E", 387, 262, 385, 253, 406, 281, 39, 1.6, 7, "5 6 7 8 9 0....", "5 6 7 8 9 0...." },
	{ "F", 531, 261, 519, 231, 557, 284, 65, 1.1, 9, "3 4 5 6 7 8....", "3 4 5 6 7 8...." },
	{ "G", 138, 384, 153, 338, 132, 401, 95, 1.5, 3, "9 0 1 2 3 4....", "9 0 1 2 3 4...." },
	{ "H", 409, 368, 354, 355, 438, 402, 90, 1.6, 4, "5 6 7 8 9 0....", "5 6 7 8 9 0...." }, };

//scale the y axis of city (to draw ellipse)
float yscale = 0.85f;

//city index in dialog box
int dialog_city_index = -1;

//timestamp(to calculate time)
time_t start_time;
time_t pre_time;
time_t virtual_start_time;

//transform the default coordinate to opengl's coordinate (x coordinate)
GLfloat ParseOpenGLX(int x)
{
	return ((float)x * all_scale / screen_width - 0.5f) * 2;
}

//transform the default coordinate to opengl's coordinate (y coordinate)
GLfloat ParseOpenGLY(int y)
{
	return -((float)y * all_scale / screen_height - 0.5f) * 2;
}

//generate normal string
void DrawString(std::string str, int x_offset, int y_offset, void* font = GLUT_BITMAP_HELVETICA_18)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(ParseOpenGLX(x_offset), ParseOpenGLY(y_offset));
	for (int i = 0; i < str.length(); i++)
		glutBitmapCharacter(font, str[i]);
}

//generate rotated string 
void DrawString2(std::string str, int x_offset, int y_offset, float angle, void* font = GLUT_STROKE_ROMAN)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glLoadIdentity();
	glTranslatef(ParseOpenGLX(x_offset), ParseOpenGLY(y_offset), 0);
	glScalef(0.0003, 0.0003, 0.0003);
	glRotated(angle, 0.0, 0.0, 1.0);
	glLineWidth(2);
	for (int i = 0; i < str.length(); i++)
		glutStrokeCharacter(font, str[i]);
	glLoadIdentity();
}

//draw the beginning point of the road (a small square)
void DrawSmallSquare(int x_offset, int y_offset)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	int length = 7;
	glRectf(ParseOpenGLX(x_offset - length), ParseOpenGLY(y_offset - length), ParseOpenGLX(x_offset + length), ParseOpenGLY(y_offset + length));
}

//draw the circle of city
void DrawCircle(int r, int x_offset, int y_offset)
{
	glColor3f(225.0f / 255.0f, 235.0f / 255.0f, 247.0f / 255.0f);
	int n = 200;
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
	{
		GLfloat x = ParseOpenGLX(r *cos(2 * PI / n * i) + x_offset);
		//elipse
		GLfloat y = ParseOpenGLY(r * yscale *sin(2 * PI / n * i) + y_offset);
		glVertex2f(x, y);
	}
	glEnd();
}

//draw a road
void DrawRoad(int start_index, int end_index)
{
	const City& start_city = city_infos[start_index];
	const City& end_city = city_infos[end_index];
	int delta_x = end_city.square_x - start_city.square_x;
	int delta_y = end_city.square_y - start_city.square_y;

	float length = sqrt(delta_x * delta_x + delta_y * delta_y);
	float normal_delta_x = delta_x / length;
	float normal_delta_y = delta_y / length;

	glColor3f(0.9f, 0.9f, 0.0f);
	glBegin(GL_POLYGON);
	int road_width = 7;
	int inner_road_width = 0.5 * road_width;
	int inner_road_bold_width = road_width - inner_road_width;
	
	int start_x_1 = start_city.square_x - normal_delta_y * road_width;
	int start_y_1 = start_city.square_y + normal_delta_x * road_width;
	int start_x_2 = start_city.square_x + normal_delta_y * road_width;
	int start_y_2 = start_city.square_y - normal_delta_x * road_width;
	glVertex2f(ParseOpenGLX(start_x_1), ParseOpenGLY(start_y_1));
	glVertex2f(ParseOpenGLX(start_x_2), ParseOpenGLY(start_y_2));
	
	int end_x_1 = end_city.square_x + (-normal_delta_x * 2 + normal_delta_y) * road_width;
	int end_y_1 = end_city.square_y + (-normal_delta_y * 2 - normal_delta_x) * road_width;
	int end_x_2 = end_city.square_x + (-normal_delta_x * 2 - normal_delta_y) * road_width;
	int end_y_2 = end_city.square_y + (-normal_delta_y * 2 + normal_delta_x) * road_width;
	glVertex2f(ParseOpenGLX(end_x_1), ParseOpenGLY(end_y_1));
	glVertex2f(ParseOpenGLX(end_city.square_x), ParseOpenGLY(end_city.square_y));
	glVertex2f(ParseOpenGLX(end_x_2), ParseOpenGLY(end_y_2));
	glEnd();
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(ParseOpenGLX(start_x_1 + (normal_delta_x + normal_delta_y) * inner_road_bold_width), ParseOpenGLY(start_y_1 + (-normal_delta_x + normal_delta_y) * inner_road_bold_width));
	glVertex2f(ParseOpenGLX(start_x_2 + (normal_delta_x - normal_delta_y) * inner_road_bold_width), ParseOpenGLY(start_y_2 + (normal_delta_x + normal_delta_y) * inner_road_bold_width));
	glVertex2f(ParseOpenGLX(end_x_1 + (- normal_delta_y) * inner_road_bold_width), ParseOpenGLY(end_y_1 + (normal_delta_x) * inner_road_bold_width));
	glVertex2f(ParseOpenGLX(end_city.square_x - normal_delta_x * inner_road_bold_width * 1.5), ParseOpenGLY(end_city.square_y - normal_delta_y * inner_road_bold_width * 1.5));
	glVertex2f(ParseOpenGLX(end_x_2 + (+ normal_delta_y) * inner_road_bold_width), ParseOpenGLY(end_y_2 + (-normal_delta_x) * inner_road_bold_width));
	glEnd();

	//simulation
	glColor3f(0.3f, 0.3f, 0.3f);
	
	int tag_length = 15;
	int half_tag_length = tag_length / 2;

	//play the simulation using the timestamp,and loop it
	int current_pos = 10 + (int(clock() /(100 / end_city.speed)) % 30);
	
	while (true)
	{
		int farest_x_delta = normal_delta_x * (current_pos + tag_length + half_tag_length);
		int farest_y_delta = normal_delta_y * (current_pos + tag_length + half_tag_length);
		int current_length = sqrt(farest_x_delta * farest_x_delta + farest_y_delta * farest_y_delta);
		
		//draw the small arrow
		if (current_length < length - 10)
		{
			glColor3f(0.9f, 0.9f, 0.0f);
			glBegin(GL_POLYGON);
			glVertex2f(ParseOpenGLX(start_city.square_x + normal_delta_x * (current_pos + half_tag_length)), ParseOpenGLY(start_city.square_y + normal_delta_y * (current_pos + half_tag_length)));
			glVertex2f(ParseOpenGLX(start_x_1 + normal_delta_x * current_pos), ParseOpenGLY(start_y_1 + normal_delta_y * current_pos));
			glVertex2f(ParseOpenGLX(start_x_1 + normal_delta_x * (current_pos + tag_length)), ParseOpenGLY(start_y_1 + normal_delta_y * (current_pos + tag_length)));
			glVertex2f(ParseOpenGLX(start_city.square_x + farest_x_delta), ParseOpenGLY(start_city.square_y + farest_y_delta));
			glVertex2f(ParseOpenGLX(start_x_2 + normal_delta_x * (current_pos + tag_length)), ParseOpenGLY(start_y_2 + normal_delta_y * (current_pos + tag_length)));
			glVertex2f(ParseOpenGLX(start_x_2 + normal_delta_x * current_pos), ParseOpenGLY(start_y_2 + normal_delta_y * current_pos));
			glEnd();

			current_pos += 30;
		}
		else
		{
			break;
		}
	}
}

//combined previous functions to draw the frame
void ShowCities()
{
	//draw cities
	for (int i = 0 ; i < city_count ; i++)
	{
		const City& city = city_infos[i];
		DrawCircle(city.R, city.center_x, city.center_y);
		DrawSmallSquare(city.square_x, city.square_y);
		DrawString(city.name, city.name_x, city.name_y, GLUT_BITMAP_TIMES_ROMAN_24);
	}

	//draw roads
	DrawRoad(1, 0);
	DrawRoad(1, 2);
	DrawRoad(1, 3);
	DrawRoad(1, 4);
	DrawRoad(1, 5);
	DrawRoad(1, 6);
	DrawRoad(1, 7);
}


void DrawCityDatas()
{
	DrawString("City", 710, 80); DrawString("Citizens", 760, 80); DrawString("infected", 840, 80); DrawString("masks", 925, 80);
	DrawString("A", 710, 120); DrawString("100000", 760, 120); DrawString("100", 840, 120); DrawString("130000", 925, 120);
	DrawString("B", 710, 160); DrawString("120000", 760, 160); DrawString("150", 840, 160); DrawString("160000", 925, 160);
	DrawString("C", 710, 200); DrawString("75000", 760, 200); DrawString("110", 840, 200); DrawString("170000", 925, 200);
	DrawString("D", 710, 240); DrawString("75000", 760, 240); DrawString("120", 840, 240); DrawString("190000", 925, 240);
	DrawString("E", 710, 280); DrawString("50000", 760, 280); DrawString("170", 840, 280); DrawString("110000", 925, 280);
	DrawString("F", 710, 320); DrawString("100000", 760, 320); DrawString("110", 840, 320); DrawString("120000", 925, 320);
	DrawString("G", 710, 360); DrawString("100000", 760, 360); DrawString("140", 840, 360); DrawString("150000", 925, 360);
	DrawString("H", 710, 400); DrawString("100000", 760, 400); DrawString("130", 840, 400); DrawString("170000", 925, 400);
}

//draw  the pop-up dialog box
void ShowDialog()
{
	if (dialog_city_index >= 0)
	{
		const City& city_info = city_infos[dialog_city_index];

		glColor3f(0.9f, 0.9f, 0.9f);
		int x1 = 300, x2 = screen_width - 300, y1 = 50, y2 = screen_height - 50;
		glRectf(ParseOpenGLX(x1), ParseOpenGLY(y1), ParseOpenGLX(x2), ParseOpenGLY(y2));

		//title
		std::string city = "City ";
		city += city_info.name;
		DrawString(city, x1 + 180, y1 + 30);

		//visualize the infection function

		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);

		int x_origin = x1 + 80;
		int y_origin = y2 - 100;
		int x_end = x2 - 80;
		int y_end = y1 + 100;
		//x axis
		glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_origin));
		glVertex2f(ParseOpenGLX(x_end), ParseOpenGLY(y_origin));
		glVertex2f(ParseOpenGLX(x_end), ParseOpenGLY(y_origin));
		glVertex2f(ParseOpenGLX(x_end - 10), ParseOpenGLY(y_origin - 10));
		glVertex2f(ParseOpenGLX(x_end), ParseOpenGLY(y_origin));
		glVertex2f(ParseOpenGLX(x_end - 10), ParseOpenGLY(y_origin + 10));
		//y axis					 
		glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_origin));
		glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_end));
		glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_end));
		glVertex2f(ParseOpenGLX(x_origin + 10), ParseOpenGLY(y_end + 10));
		glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_end));
		glVertex2f(ParseOpenGLX(x_origin - 10), ParseOpenGLY(y_end + 10));
		glEnd();
		//function
		glBegin(GL_LINE_STRIP);
		glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_origin));
		for (int i = 0; i <= x_end - x_origin + 1; i++)
		{
			int y = y_origin - city_info.k * i;
			if (y >= y_end)
			{
				glVertex2f(ParseOpenGLX(i + x_origin), ParseOpenGLY(y_origin - city_info.k * i));
			}
		}
		glEnd();

		//text
		DrawString(city_info.text1, x_origin, y_origin + 40);
		DrawString(city_info.text1, x_origin, y_origin + 70);
	}
}

//show the simulated time
void ShowDate()
{
	{
		//the outline border of the box
		glColor3f(0.0f, 0.0f, 0.0f);
		glRectf(ParseOpenGLX(383), ParseOpenGLY(427), ParseOpenGLX(683), ParseOpenGLY(483));
		glColor3f(1.0f, 1.0f, 1.0f);
		glRectf(ParseOpenGLX(383 + 2), ParseOpenGLY(427 + 2), ParseOpenGLX(683 - 2), ParseOpenGLY(483 - 2));
		glColor3f(0.0f, 0.0f, 0.0f);

		//culcalate the simulated time
		time_t tnow;
		time(&tnow);
		    //usnig 30 seconds=one day
		time_t delta_time = (tnow - start_time) * (86400 / 30);
		
		time_t current_virtual_time = virtual_start_time + delta_time;
		struct tm t;
		localtime_s(&t, &current_virtual_time);
		char data_str[128];
		sprintf_s(data_str, "%02d:%02d  %s  %d, %d", t.tm_hour, t.tm_min, "March", t.tm_mday, t.tm_year + 1900);
		
		DrawString(data_str, 426, 465, GLUT_BITMAP_TIMES_ROMAN_24);
		
		pre_time = tnow;
	}
}

//show the length of the road
void DrawRoadText()
{
	DrawString2("10000", 222, 167, 20);//A
	DrawString2("20000", 400, 135, 20);//C
	DrawString2("22000", 324, 298, 70);//D
	DrawString2("20000", 396, 190, -85);//E
	DrawString2("20000", 435, 175, -40);//F
	DrawString2("30000", 195, 277, 55);//G
	DrawString2("22000", 381, 290, -90);//H
}

//THE MAIN DRAWING FUNCTION 
void Show()
{
	glClear(GL_COLOR_BUFFER_BIT); 
	glDisable(GL_DEPTH_TEST);
	
	glColor3f(100.0f / 255.0f, 129.0f / 255.0f, 192.0f / 255.0f);
	glRectf(ParseOpenGLX(0), ParseOpenGLY(0), ParseOpenGLX(screen_width), ParseOpenGLY(screen_height));
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectf(ParseOpenGLX(10), ParseOpenGLY(25), ParseOpenGLX(690), ParseOpenGLY(490));

	
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectf(ParseOpenGLX(700), ParseOpenGLY(25), ParseOpenGLX(990), ParseOpenGLY(490));
	
	ShowCities();
	DrawCityDatas();
	ShowDate();
	DrawRoadText();
	ShowDialog();

	glutSwapBuffers();
}

//mouse manipulaton
void MouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (dialog_city_index < 0)
		{
			for (int i = 0; i < city_count; i++)
			{
				
				int delta_x = city_infos[i].center_x - x;
				int delta_y = (city_infos[i].center_y - y) / yscale;
				int distance_diff = delta_x * delta_x + delta_y * delta_y - city_infos[i].R * city_infos[i].R;
				
				if (distance_diff < 0)
				{
					dialog_city_index = i;
				}
			}
		}
		else
		{
			dialog_city_index = -1;
		}
	}
}

//initalize the timestamp
void InitializeTime()
{
	struct tm sttm;
	memset(&sttm, 0, sizeof(sttm));
	sttm.tm_year = 2020 - 1900;
	sttm.tm_mon = 1 - 1;
	sttm.tm_mday = 24;
	sttm.tm_hour = 12;
	sttm.tm_min = 1;
	virtual_start_time = mktime(&sttm);

	time(&start_time);
}

void IdleFunc()
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	InitializeTime();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 500);
	glutCreateWindow("Graph");
	glutDisplayFunc(Show);
	glutMouseFunc(MouseFunc);
	glutIdleFunc(IdleFunc);
	glutMainLoop();
	return 0;
}