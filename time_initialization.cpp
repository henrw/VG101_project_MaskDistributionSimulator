#include "mask_simulation.h"
#include "ctime"

//计算虚拟时间
void ComputeVirtualTime()
{
    time_t tnow;
    time(&tnow);
    int real = 2;
    int real_delta_time = tnow - start_time;
    real_delta_time = real_delta_time / real * real;

    //按照 real 秒等于1小时换算从程序启动到当前的时间
    time_t delta_time = (real_delta_time) * (3600 / real);

    current_virtual_time = virtual_start_time + delta_time;
}

//initialize time
void InitializeTime()
{
    struct tm sttm;
    memset(&sttm, 0, sizeof(sttm));
    sttm.tm_year = 2020 - 1900;
    sttm.tm_mon = 1;
    sttm.tm_mday = 1;
    sttm.tm_hour = 0;
    sttm.tm_min = 0;
    virtual_start_time = mktime(&sttm);

    time(&start_time);

    //compute virtual time
    ComputeVirtualTime();
    start_virtual_time = current_virtual_time;
}

//initialize city data
void InitializeCityDatas()
{
    _SYS sys_instance;
    for (int i = 0; i < city_count; ++i)
    {
        auto &city = city_infos[i];
        sys_instance.city[i] = &city;

        if (i == major_city_index)
        {
            sys_instance.city[i]->prod_rt = (float)major_city_prod_rt;
        }
        else
        {
            sys_instance.city[i]->prod_rt = (float)other_city_prod_rt;
        }
    }
    parameter_renew(&sys_instance);
}
