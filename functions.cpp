//
// Created by wumuzhe on 2020/7/18.
//
#include "mask_simulation.h"
#include <math.h>

static float hspt_capacity=100;
static float average_cure_day=5;
void initialization(_SYS *sys1)
{

    sys1->city_num=2;
    //sys1->time

    for (int i = 0; i < 2; ++i)
    {
        sys1->city[i] = (_CITY *)malloc(sizeof(_CITY));
        sys1->city[i]->sus_num = 995;
        sys1->city[i]->inf_num = 5;
        sys1->city[i]->rec_num = 0;
        sys1->city[i]->total_num=sys1->city[i]->sus_num+sys1->city[i]->inf_num+sys1->city[i]->rec_num;

        if(i==0){
            sys1->city[i]->prod_rt = 3000;
        } else{
            sys1->city[i]->prod_rt = 1200;
        }


        sys1->city[i]->cur_msk_num = 500;
        sys1->city[i]->hspt_num = 10;
        sys1->city[i]->is_produce = 1;



    }
    parameter_renew(sys1);

}

void mask_require(_SYS *sys1)
{
    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->req_msk_num = (float)1.2*(1 * sys1->city[i]->sus_num +
                                                 1 *  sys1->city[i]->rec_num +
                                                 2 * sys1->city[i]->inf_num);
    }
}

void parameter_renew(_SYS *sys1)
{
    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->eff_rt = sys1->city[i]->cur_msk_num > (sys1->city[i]->total_num)?
                                0 : 1-exp(-(sys1->city[i]->total_num-sys1->city[i]->cur_msk_num)/ (float)pow(sys1->city[i]->R,2));

        sys1->city[i]->rec_rt = sys1->city[i]->inf_num < sys1->city[i]->hspt_num*hspt_capacity?
                                (float)1/(average_cure_day*24) : 1/(average_cure_day*24 + average_cure_day*24 * (1 - exp(-(hspt_capacity*sys1->city[i]->hspt_num-sys1->city[i]->inf_num) / hspt_capacity*sys1->city[i]->hspt_num)));
    }
}

void sir_renew(_SYS *sys1, float delta_t)
{

    for (int i = 0; i < sys1->city_num; ++i)
    {
        float delta_sus,delta_inf,delta_rec;
        delta_sus= -(sys1->city[i]->eff_rt * sys1->city[i]->sus_num * sys1->city[i]->inf_num /
                     sys1->city[i]->total_num) *
                   delta_t;
        delta_inf=(sys1->city[i]->eff_rt * sys1->city[i]->sus_num * sys1->city[i]->inf_num /
                   sys1->city[i]->total_num -
                   sys1->city[i]->rec_rt * sys1->city[i]->inf_num) *
                  delta_t;
        delta_rec= (sys1->city[i]->rec_rt * sys1->city[i]->inf_num) *
                   delta_t;
        sys1->city[i]->sus_num+=delta_sus;
        sys1->city[i]->inf_num+=delta_inf;
        sys1->city[i]->rec_num+=delta_rec;

    }
}

void mask_change(_SYS *sys1, int isTransport,float delta_t)
{

    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->cur_msk_num += sys1->city[i]->prod_rt*delta_t;
    }
    for(int i=0;i<sys1->city_num;++i){
        sys1->city[i]->cur_msk_num-=(1 * sys1->city[i]->sus_num +
                                     1 * sys1->city[i]->rec_num +
                                     2 * sys1->city[i]->inf_num)*delta_t;
    }
    if (isTransport==0)
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

