#include "mask_simulation.h"
#ifdef WINDOWS
#include <WinDef.h>
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#else
#include <cmath>
#endif // WINDOWS
#include <iostream>
extern float PI;
extern float real_scale;

extern time_t delta_time;

static float hspt_capacity = 1000;
static float average_cure_day = 5;

extern float threshold_msk_percent;
extern bool is_transport1;
extern bool is_transport2;
extern int minor_trans_num;
extern float mask_use_rt;
void mask_require(_SYS *sys1)
{
    for (int i = 0; i < sys1->city_num; ++i)
    {
        //As time goes on, less people will be going out and wearing masks.

        if (delta_time < 3600 * 24 * 5)
        {
            mask_use_rt = 1;
        }
        else if (delta_time < 3600 * 24 * 20)
        {
            mask_use_rt = 1 - (float)0.9 * ((float)delta_time - 3600 * 24 * 5) / (3600 * 24 * 15);
        }
        else
        {
            mask_use_rt = 0.9;
        }
        sys1->city[i]->req_msk_num = mask_use_rt * ((float)1 * sys1->city[i]->sus_num +
                                                    (float)0.5 * sys1->city[i]->rec_num +
                                                    (float)2 * sys1->city[i]->inf_num);
    }
}

void parameter_renew(_SYS *sys1)
{
    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->eff_rt = eff_rt_calculator(sys1->city[i]->sus_num, sys1->city[i]->rec_num, sys1->city[i]->cur_msk_num,sys1->city[i]->req_msk_num, (float)sys1->city[i]->R * real_scale);
        sys1->city[i]->rec_rt = rec_rt_calculator(sys1->city[i]->hspt_num, sys1->city[i]->inf_num);
    }
}

float eff_rt_calculator(float sus_num, float rec_num, float cur_msk_num,float req_msk_num, float radius)
{
    float eff_rt = 0;
    float msk_num_for_sus=cur_msk_num*sus_num*mask_use_rt/req_msk_num;
    if (msk_num_for_sus < sus_num)
    {
        float total_area = (float)pow(radius, 2) * PI * 0.3;
        float unit_area = (float)pow(1, 2) * PI;
        eff_rt = 1 / (total_area * (sus_num - msk_num_for_sus)) * ((sus_num - msk_num_for_sus) - 1 - total_area / unit_area > 0 ? total_area / unit_area * (total_area + unit_area) / 2 + ((sus_num - msk_num_for_sus) - 1 - total_area / unit_area) * total_area : ((sus_num - msk_num_for_sus) / 2) * ((sus_num - msk_num_for_sus) - 1) * unit_area);
    }
    //        Equivalently
    //        for(int i=1;i<=(int)(sus_num-mask_num);++i){
    //            eff_rt=min(eff_rt+1/sus_num*infectious_area/total_area,(float)1);
    //            infectious_area=min(infectious_area+unit_area,total_area);
    //        }
    return eff_rt;
}

float rec_rt_calculator(float hspt_num, float inf_num)
{
    float rec_rt;
    if (inf_num < hspt_num * hspt_capacity)
    {
        if (delta_time < 10 * 24 * 3600)
        {
            //The first 10 days have lower recover rate
            rec_rt = (float)(delta_time) / (10 * 24 * 3600) / (average_cure_day * 24);
        }
        else
        {
            rec_rt = 1 / (average_cure_day * 24);
        }
    }
    else
    {
        if (delta_time < 10 * 24 * 3600)
        {
            //The first 10 days have lower recover rate
            rec_rt = (float)(delta_time) / (10 * 24 * 3600) / (average_cure_day * 24 + average_cure_day * 24 * (inf_num - hspt_capacity * hspt_num) / inf_num);
        }
        else
        {
            rec_rt = 1 / (average_cure_day * 24 + average_cure_day * 24 * (1 - exp(-(inf_num - hspt_capacity * hspt_num) / hspt_capacity * hspt_num)));
        }
    }

    return rec_rt;
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
        sys1->city[i]->sus_num = max((float)0, sys1->city[i]->sus_num + delta_sus);
        sys1->city[i]->inf_num = max((float)0, sys1->city[i]->inf_num + delta_inf);
        sys1->city[i]->rec_num = max((float)0, sys1->city[i]->rec_num + delta_rec);
    }
}

void mask_change(_SYS *sys1, float delta_t)
{
    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->cur_msk_num = max((float)0,
                                         sys1->city[i]->cur_msk_num + sys1->city[i]->prod_rt * delta_t / (float)24);
    }

    for (int i = 0; i < sys1->city_num; ++i)
    {
        sys1->city[i]->cur_msk_num = max((float)0, sys1->city[i]->cur_msk_num -
                                                   sys1->city[i]->req_msk_num * delta_t / (float)24);
    }

    if (is_transport1)
    {

        //Find the order of urgency of transporting masks
        int worst_order[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int major_order = find_order(worst_order, sys1);

        for (int i = 0; i < sys1->city_num; ++i)
        {
            if (i < major_order && sys1->city[worst_order[i]]->req_msk_num > sys1->city[worst_order[i]]->cur_msk_num && sys1->city[major_city_index]->cur_msk_num > 0)
            {
                sys1->city[worst_order[i]]->transport_num = min(
                        sys1->city[worst_order[i]]->req_msk_num - sys1->city[worst_order[i]]->cur_msk_num,
                        sys1->city[major_city_index]->cur_msk_num);
                sys1->city[major_city_index]->cur_msk_num -= sys1->city[worst_order[i]]->transport_num;
                sys1->city[worst_order[i]]->cur_msk_num += sys1->city[worst_order[i]]->transport_num;
                sys1->city[major_city_index]->transport_num -= sys1->city[worst_order[i]]->transport_num;
            }
            else if (i == major_order)
            {
                continue;
            }
            else if (i > major_order && sys1->city[worst_order[i]]->req_msk_num > sys1->city[worst_order[i]]->cur_msk_num && sys1->city[major_city_index]->cur_msk_num > 0)
            {
                sys1->city[worst_order[i]]->transport_num = min(
                        sys1->city[worst_order[i]]->req_msk_num - sys1->city[worst_order[i]]->cur_msk_num,
                        sys1->city[major_city_index]->cur_msk_num - sys1->city[major_city_index]->req_msk_num);
                sys1->city[major_city_index]->cur_msk_num -= sys1->city[worst_order[i]]->transport_num;
                sys1->city[worst_order[i]]->cur_msk_num += sys1->city[worst_order[i]]->transport_num;
                sys1->city[major_city_index]->transport_num -= sys1->city[worst_order[i]]->transport_num;
            }
        }
    }
    else if(is_transport2)
    {

        if ((city_infos[5].cur_msk_num==0||city_infos[11].cur_msk_num==0||city_infos[12].cur_msk_num== 0)\
        &&(city_infos[5].cur_msk_num>= 3 * minor_trans_num|| city_infos[11].cur_msk_num>= 3 * minor_trans_num||city_infos[12].cur_msk_num>= 3 * minor_trans_num))
        {
            int sum1 = 0;
            if (city_infos[5].cur_msk_num == 0)
            {
                city_infos[5].cur_msk_num+=(city_infos[5].transport_num = minor_trans_num);
                sum1 += minor_trans_num;
            }
            if (city_infos[11].cur_msk_num == 0)
            {
                city_infos[11].cur_msk_num+=(city_infos[11].transport_num = minor_trans_num);
                sum1 += minor_trans_num;
            }
            if (city_infos[12].cur_msk_num == 0)
            {
                city_infos[12].cur_msk_num+=(city_infos[12].transport_num = minor_trans_num);
                sum1 += minor_trans_num;
            }
            int average_trans_num = sum1 / ((city_infos[5].cur_msk_num > 3 * minor_trans_num) + (city_infos[11].cur_msk_num > 3 * minor_trans_num) + (city_infos[12].cur_msk_num > 3 * minor_trans_num));
            if (city_infos[5].cur_msk_num >= 3 * minor_trans_num)
            {
                city_infos[5].cur_msk_num-=(city_infos[5].transport_num = -average_trans_num);
            }
            if (city_infos[11].cur_msk_num >= 3 * minor_trans_num)
            {
                city_infos[11].cur_msk_num-=(city_infos[11].transport_num = -average_trans_num);
            }
            if (city_infos[12].cur_msk_num >= 3 * minor_trans_num)
            {
                city_infos[12].cur_msk_num-=(city_infos[12].transport_num = -average_trans_num);
            }
        }
        if ((city_infos[10].cur_msk_num==0||city_infos[3].cur_msk_num==0||city_infos[2].cur_msk_num== 0||city_infos[4].cur_msk_num== 0||city_infos[9].cur_msk_num== 0)\
        &&(city_infos[10].cur_msk_num>= 5 * minor_trans_num|| city_infos[3].cur_msk_num>= 5 * minor_trans_num||city_infos[2].cur_msk_num>= 5 * minor_trans_num||city_infos[4].cur_msk_num>= 5 * minor_trans_num||city_infos[9].cur_msk_num>= 5 * minor_trans_num))
        {
            int sum1 = 0;
            if (city_infos[10].cur_msk_num == 0)
            {
                city_infos[10].cur_msk_num+=(city_infos[10].transport_num = minor_trans_num);
                sum1 += minor_trans_num;
            }
            if (city_infos[3].cur_msk_num == 0)
            {
                city_infos[3].cur_msk_num+=(city_infos[3].transport_num = minor_trans_num);
                sum1 += minor_trans_num;
            }
            if (city_infos[2].cur_msk_num == 0)
            {
                city_infos[2].cur_msk_num+=(city_infos[3].transport_num = minor_trans_num);
                sum1 += minor_trans_num;
            }
            if (city_infos[4].cur_msk_num == 0)
            {
                city_infos[4].cur_msk_num+=(city_infos[4].transport_num = minor_trans_num);
                sum1 += minor_trans_num;
            }
            if (city_infos[9].cur_msk_num == 0)
            {
                city_infos[9].cur_msk_num+=(city_infos[9].transport_num = minor_trans_num);
                sum1 += minor_trans_num;
            }
            int average_trans_num = sum1 / ((city_infos[10].cur_msk_num > 5 * minor_trans_num) + (city_infos[3].cur_msk_num > 5 * minor_trans_num) + (city_infos[2].cur_msk_num > 5 * minor_trans_num)+ (city_infos[4].cur_msk_num > 5 * minor_trans_num)+ (city_infos[9].cur_msk_num > 5 * minor_trans_num));
            if (city_infos[10].cur_msk_num >= 5 * minor_trans_num)
            {
                city_infos[10].cur_msk_num-=(city_infos[10].transport_num = -average_trans_num);
            }
            if (city_infos[3].cur_msk_num >= 5 * minor_trans_num)
            {
                city_infos[3].cur_msk_num-=(city_infos[3].transport_num = -average_trans_num);
            }
            if (city_infos[2].cur_msk_num >= 3 * minor_trans_num)
            {
                city_infos[2].cur_msk_num-=(city_infos[2].transport_num = -average_trans_num);
            }
            if (city_infos[4].cur_msk_num >= 3 * minor_trans_num)
            {
                city_infos[4].cur_msk_num-=(city_infos[4].transport_num = -average_trans_num);
            }
            if (city_infos[9].cur_msk_num >= 3 * minor_trans_num)
            {
                city_infos[9].cur_msk_num-=(city_infos[9].transport_num = -average_trans_num);
            }
        }
    }
    else
    {
        for (int i = 0; i < city_count; ++i)
        {
            sys1->city[i]->transport_num = 0;
        }
    }
}
int find_order(int *worst_order, _SYS *sys1)
{
    int major_order;
    float worst_rate;
    for (int i = 0; i < sys1->city_num; ++i)
    {
        worst_rate = -1 / average_cure_day * (float)pow(10, 12);
        for (int j = 0; j < sys1->city_num; ++j)
        {
            if (i == 0)
            {
                if ((sys1->city[j]->eff_rt * sys1->city[j]->sus_num * sys1->city[j]->inf_num /
                     sys1->city[j]->total_num -
                     sys1->city[j]->rec_rt * sys1->city[j]->inf_num) > worst_rate)
                {
                    worst_rate = (sys1->city[i]->eff_rt * sys1->city[j]->sus_num * sys1->city[j]->inf_num /
                                  sys1->city[i]->total_num -
                                  sys1->city[j]->rec_rt * sys1->city[j]->inf_num);
                    worst_order[i] = j;
                }
            }
            else
            {
                for (int k = 0; k < i; ++k)
                {
                    if (j == worst_order[k])
                    {
                        break;
                    }
                    if (k == i - 1)
                    {
                        if ((sys1->city[j]->eff_rt * sys1->city[j]->sus_num * sys1->city[j]->inf_num /
                             sys1->city[j]->total_num -
                             sys1->city[j]->rec_rt * sys1->city[j]->inf_num) > worst_rate)
                        {
                            worst_rate = (sys1->city[i]->eff_rt * sys1->city[j]->sus_num * sys1->city[j]->inf_num /
                                          sys1->city[i]->total_num -
                                          sys1->city[j]->rec_rt * sys1->city[j]->inf_num);
                            worst_order[i] = j;
                        }
                    }
                }
            }
        }
        if (worst_order[i] == major_city_index)
        {
            major_order = i;
        }
    }

    return major_order;
}