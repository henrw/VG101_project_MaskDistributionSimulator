#include "mask_simulation.h"



float PI = 3.141592653;

float real_scale=55.4*1000/66;

float all_scale = 1.0f;

int screen_width = 1080 * all_scale;
int screen_height = 500 * all_scale;

//const int major_city_index=8;

int hours_per_trans = 6;

int dialog_city_index = -1;

_CITY city_infos[city_count] = {
        {"Shiyan", "A", 189, 111, 221, 91,223, 91,-69, 169, 81, 143 / 2, (float)165 / 143, 3340843, 3340843 - 177, 177, 0, 5, 1000000},
        {"Xiangyang", "B", 320, 162, 323, 145,325, 145,-80, 320, 135, 129 / 2, (float)74 / 129, 5500307, 5500307 - 347, 347, 0, 5, 1000000},
        {"Suizhou", "C", 429, 161, 429, 175,415, 180,82, 429, 165, 66 / 2, (float)67 / 66, 2162222, 2162222 - 304, 304, 0, 2, 1000000},
        {"Jingmen", "D", 371, 239, 328, 248,328, 248,-53, 371, 238, 104 / 2, (float)63 / 104, 2873687, 2873687 - 251, 251, 0, 3, 1000000},
        {"Xiaogan", "E", 465, 228, 471, 249,451, 260,75, 465, 239, 50 / 2, (float)90 / 50, 4814542, 4814542 - 628, 628, 0, 4, 1000000},
        {"Huanggang", "F", 577, 248, 554, 288,504, 298,25, 554, 278, 85 / 2, (float)125 / 85, 6162072, 6162072 - 726, 726, 0, 2, 1000000},
        {"Enshi", "G", 120, 335, 165, 309,165, 309,0, 120, 319, 150 / 2, (float)110 / 140, 3290294, 3290294 - 87, 87, 0, 3, 1000000},
        {"Yichang", "H", 255, 275, 265, 279,265, 279,-20, 255, 269, 120 / 2, (float)115 / 110, 4059686, 4059686 - 276, 276, 0, 4, 1000000},
        {"Xiantao,etc.", "I", 409, 307, 409, 317,394, 350,0, 394, 340, 100 / 2, (float)70 / 100, 3540275, 3540275 - 206, 206, 0, 5, 100000000},
        {"Wuhan", "J", 505, 280, 503, 283,463, 288,35, 505, 273, 50 / 2, (float)76 / 50, 9785392, 9785392 - 3215, 3215, 0, 24, 1000000},
        {"Jingzhou", "K", 349, 388, 338, 365,338, 365,45, 369, 380, 140 / 2, (float)75 / 140, 5691707, 5691707 - 287, 287, 0, 9, 1000000},
        {"Xianning", "L", 491, 377, 509, 357,469, 337,-30, 501, 372, 84 / 2, (float)75 / 84, 2462583, 2462583 - 206, 206, 0, 1, 1000000},
        {"Huangshi", "M", 569, 335, 569, 320,529, 320,0, 569, 335, 44 / 2, (float)45 / 44, 2429318, 2429318 - 209, 209, 0, 2, 1000000},
};
string transported_num_str[13];

time_t start_time;
time_t virtual_start_time;
time_t current_virtual_time;
time_t pre_virtual_time;
time_t start_virtual_time;
time_t delta_time;

float current_major_input_color = normal_input_color;
float current_other_input_color = normal_input_color;

std::string major_city_string = "30000000";
std::string other_city_string = "300000";

int major_city_prod_rt = 30000000;
int other_city_prod_rt = 300000;