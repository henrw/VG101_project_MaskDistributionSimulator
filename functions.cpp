//
// Created by wumuzhe on 2020/7/18.
//
#include "mask_simulation.h"
#include <math.h>

void initialization(_SYS *sys1)
{

    sys1->city_num=2;
    //sys1->time

    for (int i = 0; i < 2; ++i)
    {
        sys1->city[i] = (_CITY *)malloc(sizeof(_CITY));
        sys1->city[i]->sus_num = 1000;
        sys1->city[i]->inf_num = 1;
        sys1->city[i]->rec_num = 0;
        sys1->city[i]->prod_rt = 1200;
        sys1->city[i]->radius = 5;
        sys1->city[i]->cur_msk_num = 1000;
        sys1->city[i]->hspt_num = 10;
        sys1->city[i]->is_produce = 1;
        sys1->city[i]->location[0] = 1;
        sys1->city[i]->location[1] = 1;


    }
    parameter_renew(sys1);

}

void mask_require(_SYS *sys1)
{
    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->req_msk_num = (int)(1.2*(1 * sys1->city[i]->sus_num +
                                                1 *  sys1->city[i]->rec_num +
                                                2 * sys1->city[i]->inf_num));
    }
}

void parameter_renew(_SYS *sys1)
{
    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->beta = 0.3 + 0.7*(1 - exp((sys1->city[i]->rec_num
                                                  + sys1->city[i]->sus_num
                                                  + sys1->city[i]->inf_num)
                                                 / pow(sys1->city[i]->radius,2)
                                                 /sys1->city[i]->cur_msk_num));

        sys1->city[i]->gamma = 1/(15 + 15 * (1 - exp(-sys1->city[i]->inf_num / (float)sys1->city[i]->hspt_num)));
    }
}

void sir_renew(_SYS *sys1, float delta_t)
{

    for (int i = 0; i < sys1->city_num; ++i)
    {
        float delta_sus,delta_inf,delta_rec;
        delta_sus= -(sys1->city[i]->beta * sys1->city[i]->sus_num * sys1->city[i]->inf_num /
                     (sys1->city[i]->rec_num + sys1->city[i]->sus_num + sys1->city[i]->inf_num)) *
                   delta_t;
        delta_inf=(sys1->city[i]->beta * sys1->city[i]->sus_num * sys1->city[i]->inf_num /
                   (sys1->city[i]->rec_num + sys1->city[i]->sus_num + sys1->city[i]->inf_num) -
                   sys1->city[i]->gamma * sys1->city[i]->inf_num) *
                  delta_t;
        delta_rec= (sys1->city[i]->gamma * sys1->city[i]->inf_num) *
                   delta_t;
        sys1->city[i]->sus_num+=delta_sus;
        sys1->city[i]->inf_num+=delta_inf;
        sys1->city[i]->rec_num+=delta_rec;

    }
}

void mask_change(_SYS *sys1, int isTransport)
{

    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->cur_msk_num += sys1->city[i]->prod_rt;
    }
    for(int i=0;i<sys1->city_num;++i){
        sys1->city[i]->cur_msk_num-=(1 * (int)sys1->city[i]->sus_num +
                                     1 * (int) sys1->city[i]->rec_num +
                                     2 * (int)sys1->city[i]->inf_num);
    }
    if (isTransport)
    {
        for (int i = 1; i < sys1->city_num; ++i)
        {
            if (sys1->city[i]->req_msk_num > sys1->city[i]->cur_msk_num
                &&sys1->city[0]->cur_msk_num>sys1->city[0]->req_msk_num)
            {
                sys1->city[0]->cur_msk_num -= sys1->city[i]->req_msk_num - sys1->city[i]->cur_msk_num;
                sys1->city[i]->cur_msk_num = sys1->city[i]->req_msk_num;
                //plot
            }
        }
    }
}

