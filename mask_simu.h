#ifndef MASK_SIMU_H
#define MASK_SIMU_H

/*
TO-DO include header files
*/

// define some const
#define K1 1
#define K2 2

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
    int sus_num; //those susceptible but not yet infected with the disease
    int inf_num; //infectious (infected) individuals
    int rec_num; //those individuals who have recovered and have immunity

    int cur_msk_num;
    // required mask number write as a function
    int hspt_num;//hospital number

} _CITY;

typedef struct SYS
{
    _CITY city[13];
    char time;

} _SYS;

#endif
