//
// Created by wumuzhe on 2020/7/18.
//

#ifndef CLION_MASK_SIMULATION_H
#define CLION_MASK_SIMULATION_H
#include<string>



// define some const
//static float PI=3.141592653;
//
//static float all_scale=1.0f;

using namespace std;

typedef struct CITY
{
    string name;

    /*for plotting*/
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
    string text1;
    string text2;

    /* SIR model
     *
    / Effective Contact Rate: eff_rt= a + b*(1-e^(-(sus_num + inf_num + rec_num)/(radius*radius*cur_msk_num)))
    / Recovery Rate: rec_rt= 1/(15+15(1-e^(-inf_num/hspt_num)
     * these two parameters applies the model 1-e^(-x)
    */
    float sus_num; //those susceptible but not yet infected with the disease
    float inf_num; //infectious (infected) individuals
    float rec_num; //those individuals who have recovered and have immunity
    //Only display the integer part of the number!

    int is_produce;//whether or not the city produce masks in large scale
    int prod_rt;//the increment of masks for every period of time
    int cur_msk_num;//current mask number
    int req_msk_num;//required mask number
    int hspt_num;//hospital number
    float eff_rt;//Effective Contact Rate
    float rec_rt;//Recovery rate

} _CITY;

typedef struct SYS
{
    _CITY *city[2];
    int city_num = 2;
    string time;

} _SYS;

/*function prototypes
 */

//initialize the system
void initialization(_SYS *sys1);

// calculate the required mask number for each city
void mask_require(_SYS *sys1);

// calculate eff_rt, rec_rt in SIR model
void parameter_renew(_SYS *sys1);

// calculate sus_num, inf_num, rec_num in SIR model
void sir_renew(_SYS *sys1, float delta_t);

// mask consumed, masks produced, masks transported
void mask_change(_SYS *sys1, int isTransport);

#endif //CLION_MASK_SIMULATION_H
