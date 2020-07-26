#include "mask_simulation.h"
#include <math.h>

static float hspt_capacity = 1000;
static float average_cure_day = 5;
//void initialization(_SYS *sys1)
//{
//    //sys1->time
//
//    for (int i = 0; i < sys1->city_num; ++i)
//    {
//        sys1->city[i]->sus_num = 995;
//        sys1->city[i]->inf_num = 5;
//        sys1->city[i]->rec_num = 0;
//        sys1->city[i]->total_num=sys1->city[i]->sus_num+sys1->city[i]->inf_num+sys1->city[i]->rec_num;
//
//        //i==5 Huanggang
//        if(i==5){
//            sys1->city[i]->prod_rt = (float)major_city_prod_rt;
//        } else{
//            sys1->city[i]->prod_rt = (float)other_city_prod_rt;
//        }
//
//
//        sys1->city[i]->cur_msk_num = 500;
//        sys1->city[i]->hspt_num = 10;
//        sys1->city[i]->is_produce = 1;
//
//
//
//    }
//    parameter_renew(sys1);
//
//}

void mask_require(_SYS *sys1)
{
    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->req_msk_num = (float)1.2 * (1 * sys1->city[i]->sus_num +
                                                   1 * sys1->city[i]->rec_num +
                                                   2 * sys1->city[i]->inf_num);
    }
}

void parameter_renew(_SYS *sys1)
{
    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->eff_rt = sys1->city[i]->cur_msk_num > (sys1->city[i]->sus_num) ? 0 : 0.2 * (1 - exp(-(sys1->city[i]->sus_num - sys1->city[i]->cur_msk_num) / (float)pow(sys1->city[i]->R, 2)));

        sys1->city[i]->rec_rt = sys1->city[i]->inf_num < sys1->city[i]->hspt_num * hspt_capacity ? (float)1 / (average_cure_day * 24) : 1 / (average_cure_day * 24 + average_cure_day * 24 * (1 - exp(-(sys1->city[i]->inf_num - hspt_capacity * sys1->city[i]->hspt_num) / hspt_capacity * sys1->city[i]->hspt_num)));
    }
}

void sir_renew(_SYS *sys1, float delta_t)
{

    for (int i = 0; i < sys1->city_num; ++i)
    {
        float delta_sus, delta_inf, delta_rec;
        delta_sus = -(sys1->city[i]->eff_rt * sys1->city[i]->sus_num * sys1->city[i]->inf_num /
                      sys1->city[i]->total_num) *
                    delta_t;
        delta_inf = (sys1->city[i]->eff_rt * sys1->city[i]->sus_num * sys1->city[i]->inf_num /
                     sys1->city[i]->total_num -
                     sys1->city[i]->rec_rt * sys1->city[i]->inf_num) *
                    delta_t;
        delta_rec = (sys1->city[i]->rec_rt * sys1->city[i]->inf_num) *
                    delta_t;
        sys1->city[i]->sus_num += delta_sus;
        sys1->city[i]->inf_num += delta_inf;
        sys1->city[i]->rec_num += delta_rec;
    }
}

void mask_change(_SYS *sys1, int isTransport, float delta_t)
{

    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->cur_msk_num += sys1->city[i]->prod_rt * delta_t / (float)24;
    }
    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->cur_msk_num -= ((float)1 * sys1->city[i]->sus_num +
                                       (float)0.5 * sys1->city[i]->rec_num +
                                       (float)2 * sys1->city[i]->inf_num) *
                                      delta_t / (float)24;
    }
    if (isTransport == 0)
    {
        for (int i = 0; i < sys1->city_num; ++i)
        {
            if (i != major_city_index)
            {
                if (sys1->city[i]->req_msk_num > sys1->city[i]->cur_msk_num && sys1->city[major_city_index]->cur_msk_num > sys1->city[major_city_index]->req_msk_num)
                {
                    sys1->city[major_city_index]->cur_msk_num -= (sys1->city[i]->req_msk_num - sys1->city[i]->cur_msk_num) * delta_t;
                    sys1->city[i]->cur_msk_num += (sys1->city[i]->req_msk_num - sys1->city[i]->cur_msk_num) * delta_t;
                }
            }
        }
    }
}
