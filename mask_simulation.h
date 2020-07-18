//
// Created by wumuzhe on 2020/7/18.
//

#ifndef CLION_MASK_SIMULATION_H
#define CLION_MASK_SIMULATION_H
#include<string>



// define some const
static const float k_msk_req = 1.5;

using namespace std;

typedef struct CITY
{
    /*for plotting*/
    float location[2];
    float radius; //size of the city

    /* SIR model
    / Effective Contact Rate: eff_rt= const + k1*(sus_num + inf_num + rec_num)/(2*pi*radius*radius)/cur_msk_num
    / Recovery Rate: rec_rt= k2*15/hspt_num
    /   written as functions
    */
    float sus_num; //those susceptible but not yet infected with the disease
    float inf_num; //infectious (infected) individuals
    float rec_num; //those individuals who have recovered and have immunity
    //Only display the integer part of the number!

    int is_produce;
    int prod_rt;
    int cur_msk_num;
    int req_msk_num;

    int hspt_num; //hospital number

    float beta;
    float gamma;

} _CITY;

typedef struct SYS
{
    _CITY *city[2];//city[0] is the mask production city;
    int city_num = 2;
    string time;

} _SYS;


void initialization(_SYS *sys1);
void mask_require(_SYS *sys1);
void parameter_renew(_SYS *sys1);
void sir_renew(_SYS *sys1, float delta_t);
void mask_change(_SYS *sys1, int isTransport);
#endif //CLION_MASK_SIMULATION_H
