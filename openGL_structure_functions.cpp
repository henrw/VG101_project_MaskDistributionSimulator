#include "mask_simulation.h"
#include "GL/glut.h"
#include <iostream>
#include <iomanip>

//testing
extern int end_check_result;

extern int hours_per_trans1;
extern int hours_per_trans2;

extern bool is_hour_changed;
extern int pre_hour;

extern int inf_refresh_cycle;
extern int inf_pre_cycle_pos;
extern bool need_refresh_inf;

extern bool is_transport1;
extern bool is_transport2;

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

    auto sub_virtual_time1 = (current_virtual_time - start_virtual_time) % (hours_per_trans1 * 3600);
    auto sub_virtual_time2 = (current_virtual_time - start_virtual_time) % (hours_per_trans2 * 3600);
    //if it it transmission time
    is_transport1 = current_hour!=0 && sub_virtual_time1 < 3600;
    is_transport2 = current_hour!=0 && sub_virtual_time2 < 3600&&!is_transport1;
    float delta = (float) current_virtual_time - pre_virtual_time;
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
    mask_change(&sys_instance, delta_t);

    for (int i = 0; i < city_count; ++i)
    {
        auto &city = city_infos[i];
        if (is_hour_changed)
        {
            city.pre_hour_mask_num = city.current_hour_mask_num;

            if (city.pre_hour_mask_num == -1)
            {
                city.pre_hour_mask_num = (int) city.cur_msk_num;
            }
            city.current_hour_mask_num = (int) city.cur_msk_num;
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
    glRectf(ParseOpenGLX(10), ParseOpenGLY(10), ParseOpenGLX(690), ParseOpenGLY(40));
    DrawString("@",20,30,GLUT_BITMAP_HELVETICA_18);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(ParseOpenGLX(10), ParseOpenGLY(45), ParseOpenGLX(690), ParseOpenGLY(490));

    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(ParseOpenGLX(700), ParseOpenGLY(10), ParseOpenGLX(1070), ParseOpenGLY(490));

    DrawInfoLogo();

	if (is_inputed_values)
	{
		end_check_result = end_check();
		if(end_check_result ==-2)
		{
				pre_virtual_time = current_virtual_time;
				ComputeVirtualTime();
				RefreshDatas();
		}
		else if(end_check_result ==-1)
		{
			DrawString("There is currently no infected person in Hubei Province. The simulation ends.",\
					40,30,GLUT_BITMAP_HELVETICA_18);
		}
		else
		{
			char end_str[100];
			sprintf_s(end_str, "%d ten-thousand(s) of citizens are infected in %s. The simulation ends.", threshold_infection_percent, city_infos[end_check_result].name.c_str());
			DrawString(end_str,40,30,GLUT_BITMAP_HELVETICA_18);
		}
        ShowCities();
        DrawCityDatas();
        ShowDate();
        ShowDialog();
        ShowInfo();
	}
	else
	{
		DrawInputUI();
        ShowInfo();
        DrawString("<--Click to see the guide",info_x+info_R+3,info_y+4,GLUT_BITMAP_HELVETICA_12);
	}
    glutSwapBuffers();
}

void IdleFunc()
{
    glutPostRedisplay();
}

int end_check()
{
    for(int i=0;i<city_count;++i){
        if(city_infos[i].inf_num>=(float)threshold_infection_percent/10000*city_infos[i].total_num){
            return i;
        }
    }

    bool is_win=true;
    for (int i=0;i<city_count;++i){
        if(city_infos[i].inf_num>=1){
            is_win=false;
        }
    }
    if(is_win){
        return -1;
    }

    return -2;
}