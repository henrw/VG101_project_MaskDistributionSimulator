#include "mask_simulation.h"

int end_check_result =-2;
float PI = 3.141592653;

float real_scale=55.4*1000/66;

float all_scale = 1.0f;

int screen_width = 1080 * all_scale;
int screen_height = 500 * all_scale;

//const int major_city_index=8;

int hours_per_trans1 = 6;
int hours_per_trans2 = 2;
int minor_trans_num=10000;

int dialog_city_index = -1;

_CITY city_infos[city_count] = {
		{"Shiyan", "A",		223, 109,223, 104,0, 169, 131, 143 / 2, 3340843, 3340843 - 177, 177, 0, 5, 100000},
		{"Xiangyang", "B",	326, 166,326, 161,0, 320, 140, 129 / 2, 5500307, 5500307 - 347, 347, 0, 5, 100000},
		{"Suizhou", "C",	432, 195,432, 190,0, 424, 165, 66 / 2, 2162222, 2162222 - 304, 304, 0, 2, 100000},
		{"Jingmen", "D",	335, 272,335, 267,0, 371, 260, 104 / 2, 2873687, 2873687 - 251, 251, 0, 3, 100000},
		{"Xiaogan", "E",	477, 274,477, 269,0, 465, 239, 50 / 2, 4814542, 4814542 - 628, 628, 0, 4, 100000},
		{"Huanggang", "F",	571, 310,571, 305,0, 575, 278, 85 / 2, 6162072, 6162072 - 726, 726, 0, 2, 100000},
		{"Enshi", "G",		116, 349,116, 344,0, 120, 329, 150 / 2, 3290294, 3290294 - 87, 87, 0, 3, 100000},
		{"Yichang", "H",	269, 302,269, 297,0, 255, 269, 120 / 2, 4059686, 4059686 - 276, 276, 0, 4, 100000},
		{"Xiantao,etc.","I",420, 321,420, 316,0, 394, 340, 100 / 2, 3540275, 3540275 - 206, 206, 0, 5, 100000},
		{"Wuhan", "J",		511, 306,511, 301,0, 505, 283, 50 / 2, 9785392, 9785392 - 3215, 3215, 0, 24, 100000},
		{"Jingzhou", "K",	344, 343,344, 338,0, 369, 380, 140 / 2, 5691707, 5691707 - 287, 287, 0, 9, 100000},
		{"Xianning", "L",	518, 383,518, 378,0, 501, 397, 84 / 2, 2462583, 2462583 - 206, 206, 0, 1, 100000},
		{"Huangshi", "M",	576, 350,576, 345,0, 560, 370, 44 / 2, 2429318, 2429318 - 209, 209, 0, 2, 100000},
};

time_t start_time;
time_t virtual_start_time;
time_t current_virtual_time;
time_t pre_virtual_time;
time_t start_virtual_time;
time_t delta_time;

float current_major_input_color = normal_input_color;
float current_other_input_color = normal_input_color;
float threshold_infection_input_color = normal_input_color;

std::string major_city_string = "30000000";
std::string other_city_string = "300000";
std::string threshold_infection_percent_string = "10";

int major_city_prod_rt = 30000000;
int other_city_prod_rt = 300000;
int threshold_infection_percent = 10;

//Under this percentage masks won't be transported anyhow
float threshold_msk_percent=0.00033;

int draw_road_switch=0;
float mask_use_rt=1;

int is_info_shown=0;