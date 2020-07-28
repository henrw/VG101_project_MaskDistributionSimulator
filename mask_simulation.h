
#ifndef CLION_MASK_SIMULATION_H
#define CLION_MASK_SIMULATION_H
#include <string>
#include <vector>
#include "GL/glut.h"
#include <string>

using namespace std;

//Structure define
typedef struct CITY
{
    std::string name;
    std::string label;

    /*Parameters for plotting*/

    //The origin point of circle
    int center_x;
    int center_y;

    //The point where the car begins moving
    int square_x;
    int square_y;

    //The point where the string is printed
    int string_x;
    int string_y;
    int rotation;

    //the point where the city name is printed at
    int name_x;
    int name_y;

    //Radius of circle
    int R;
    //城市Y轴缩放，用来画椭圆
    float y_scale;

    /* SIR model
     *
    / Effective Contact Rate: eff_rt= a + b*(1-e^(-(sus_num + inf_num + rec_num)/(radius*radius*cur_msk_num)))
    / Recovery Rate: rec_rt= 1/(15+15(1-e^(-inf_num/hspt_num)
     * these two parameters applies the model 1-e^(-x)
    */
    float total_num;
    float sus_num; //Those susceptible but not yet infected with the disease
    float inf_num; //Infectious (infected) individuals
    float rec_num; //Those individuals who have recovered and have immunity

    //Only display the integer part of the number!

    float hspt_num; //Hospital number

    float cur_msk_num; //Current mask number
    float req_msk_num; //Required mask number
    float transport_num;

    float eff_rt; //Effective Contact Rate
    float rec_rt; //Recovery rate

    //the mask number in previous hour
    int pre_hour_mask_num;
    //the mask number in current hour
    int current_hour_mask_num = -1;
    //history data of the infected people
    std::vector<float> history_inf_nums;
    float prod_rt; //The increment of masks for every period of time
} _CITY;

typedef struct SYS
{
    _CITY *city[13];
    int city_num = 13;
    string time;

} _SYS;

//Global values

//缩放比例
extern float all_scale;

//屏幕宽高
extern int screen_width;
extern int screen_height;

//标记是否已经输入了数值
extern bool is_inputed_values;

//城市数量
const int city_count = 13;

//显示对话框的城市序号
extern int dialog_city_index;

//城市信息
extern _CITY city_infos[city_count];

//时间戳，用来计算日期
extern time_t start_time;
extern time_t virtual_start_time;
extern time_t current_virtual_time;
extern time_t pre_virtual_time;
extern time_t start_virtual_time;

//选中后的输入框颜色
const float select_input_color = 0.8;
//正常状态的输入框颜色
const float normal_input_color = 0.6;

//当前输入框颜色
extern float current_major_input_color;
extern float current_other_input_color;
const int major_city_index = 8;

const int major_city_input_x1 = 460;
const int major_city_input_y1 = 175;
const int major_city_input_x2 = 750;
const int major_city_input_y2 = 245;

const int other_city_input_x1 = 460;
const int other_city_input_y1 = 255;
const int other_city_input_x2 = 750;
const int other_city_input_y2 = 325;

const int ok_button_x1 = 580;
const int ok_button_y1 = 350;
const int ok_button_x2 = 800;
const int ok_button_y2 = 420;

extern std::string major_city_string;
extern std::string other_city_string;

extern int major_city_prod_rt;
extern int other_city_prod_rt;

extern std::string month[12];

/*function prototypes
 */

//initialize the system
void initialization(_SYS *sys1);

//Calculate the required mask number for each city
//Input: the pointer of system
//Changes: sys1->city[ALL]->req_msk_num (required mask number) updated
void mask_require(_SYS *sys1);

//Calculate eff_rt, rec_rt in SIR model
//Input: the pointer of system
//Changes: sys1->city[ALL]->eff_rt (effective contact rate) & rec_rt (recovery rate) updated
void parameter_renew(_SYS *sys1);

float eff_rt_calculator(float sus_num,float rec_num,float cur_msk_num,float req_msk_num, float radius);

float rec_rt_calculator(float hspt_num,float inf_num);

//Calculate sus_num, inf_num, rec_num in SIR model
//Input: the pointer of system
//Changes: sys1->city[ALL]->sus_num (number of suspected people) & inf_num (...) & rec_num (...) updated
void sir_renew(_SYS *sys1, float delta_t);

//Mask consumed, masks produced, masks transported
//Input: the pointer of system
//Changes: sys1->city[ALL]->cur_msk_num (current mask number) updated
void mask_change(_SYS *sys1, int isTransport, float delta_t);

int find_order(int* worst_order,_SYS *sys1);

//Graphing Part

//屏幕坐标转换到OpenGL坐标，转换X轴
GLfloat ParseOpenGLX(int x);

//屏幕坐标转换到OpenGL坐标，转换Y轴
GLfloat ParseOpenGLY(int y);

//绘制字符串
void DrawString(std::string str, int x_offset, int y_offset, void *font);

//绘制可旋转的字符串
void DrawString2(std::string str, int x_offset, int y_offset, float angle, void *font);

void DrawInputUI();

//Time initialization functions

//计算虚拟时间
void ComputeVirtualTime();

//initialize time
void InitializeTime();

//initialize city data
void InitializeCityDatas();

//Input functions

void AddString(std::string &str, int &val, unsigned char key);

void RemoveStringOneChar(std::string &str, int &val);

void KeyboardFuncInputValues(unsigned char key, int x, int y);

//输入数值的鼠标响应事件
void MouseFuncInputValues(int button, int state, int x, int y);

//弹出对话框的鼠标响应时间
void MouseFuncDialog(int button, int state, int x, int y);

//鼠标操作回调函数
void MouseFunc(int button, int state, int x, int y);

/*OpenGL basic functions*/

//small square in the city
void DrawSmallSquare(int x_offset, int y_offset);

//circle (the shape of the city)
void DrawCircle(int r, int x_offset, int y_offset, int inf_num, float y_scale);

//the road
void DrawRoad(int start_index, int end_index, int min_diff_val, int max_diff_val);

//drawing
void ShowCities();

//show the data
void DrawCityDatas();

//city's dialog
void ShowDialog();

//how time
void ShowDate();

//the digit in the road
void DrawRoadText(int index,int diff);

/*OpenGL structure functions*/

//refresh data
void RefreshDatas();

//MAIN FUNCTION OF DRAWING
void Show();

void IdleFunc();

#endif //CLION_MASK_SIMULATION_H
