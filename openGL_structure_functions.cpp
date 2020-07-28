#include "mask_simulation.h"
#include "GL/glut.h"
#include <iostream>
#include <iomanip>

//testing
extern int hours_per_trans;

extern bool is_hour_changed;
extern int pre_hour;

extern int inf_refresh_cycle;
extern int inf_pre_cycle_pos;
extern bool need_refresh_inf;

extern bool is_translating;

extern int history_inf_cache_count;
//refresh data
void RefreshDatas()
{
    Sleep(100);
    //compute the current time
    int current_hour = (current_virtual_time - start_virtual_time) / 3600;
    is_hour_changed = current_hour != pre_hour;
    pre_hour = current_hour;

    //compute the refreshing period of function plotting
    int current_cycle_pos = (current_virtual_time - start_virtual_time) / inf_refresh_cycle;
    need_refresh_inf = current_cycle_pos != inf_pre_cycle_pos;
    inf_pre_cycle_pos = current_cycle_pos;

    auto sub_virtual_time = (current_virtual_time - start_virtual_time) % (hours_per_trans * 3600);
    //if it it transmission time
    is_translating = current_hour!=0 && sub_virtual_time < 3600;

    float delta = current_virtual_time - pre_virtual_time;
    //time interval->hours
    auto delta_t = delta / 3600.0f;

    _SYS sys_instance;
    sys_instance.city_num = city_count;
    for (int i = 0; i < city_count; ++i)
    {
        auto &city = city_infos[i];
        sys_instance.city[i] = &city;
    }

    parameter_renew(&sys_instance);
    sir_renew(&sys_instance, delta_t);

    mask_require(&sys_instance);
    mask_change(&sys_instance, is_translating ? 0 : 1, delta_t);

    for (int i = 0; i < city_count; ++i)
    {
        auto &city = city_infos[i];
        if (is_hour_changed)
        {
            city.pre_hour_mask_num = city.current_hour_mask_num;

            if (city.pre_hour_mask_num == -1)
            {
                city.pre_hour_mask_num = city.cur_msk_num;
            }
            city.current_hour_mask_num = city.cur_msk_num;
        }

        if (need_refresh_inf)
        {

            city.history_inf_nums.push_back(city.inf_num);

            if (city.history_inf_nums.size() >= history_inf_cache_count)
            {
                city.history_inf_nums.erase(city.history_inf_nums.begin());
            }
        }
    }
}

//MAIN FUNCTION OF DRAWING
void Show()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glColor3f(100.0f / 255.0f, 129.0f / 255.0f, 192.0f / 255.0f);
    glRectf(ParseOpenGLX(0), ParseOpenGLY(0), ParseOpenGLX(screen_width), ParseOpenGLY(screen_height));

    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(ParseOpenGLX(10), ParseOpenGLY(25), ParseOpenGLX(690), ParseOpenGLY(490));

    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(ParseOpenGLX(700), ParseOpenGLY(25), ParseOpenGLX(1070), ParseOpenGLY(490));



    if (is_inputed_values)
    {
        pre_virtual_time = current_virtual_time;
        ComputeVirtualTime();
        RefreshDatas();
        ShowCities();
        DrawCityDatas();
        ShowDate();
        ShowDialog();
    }
    else
    {
        DrawInputUI();
    }

    glutSwapBuffers();
}

void IdleFunc()
{
    glutPostRedisplay();
}
