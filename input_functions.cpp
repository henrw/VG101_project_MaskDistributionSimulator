#include "GL/glut.h"
#include "mask_simulation.h"
#include <cmath>

//Index of the selection of cities during input.
//0 refers to unselected; 1 refers to the major city selected; 2 refers to other cities selected; 3 refers to infected threshold selected.
int selected_city_input = 0;

void AddString(std::string &str, int &val, unsigned char key)
{
    str += key;
    val = atoi(str.c_str());
    char temp[256];
#ifdef WINDOWS
	_itoa_s(val, temp, 10);
#else
	_itoa(val, temp, 10);
#endif // WINDOWS

    str = temp;
}

void RemoveStringOneChar(std::string &str, int &val)
{
    if (!str.empty())
    {
        str = str.substr(0, str.size() - 1);
        if (str.empty())
        {
            str = "0";
        }
        val = atoi(str.c_str());
    }
}

void KeyboardFuncInputValues(unsigned char key, int x, int y)
{
    if (key >= '0' && key <= '9')
    {
        switch (selected_city_input)
        {
            case 1:
                AddString(major_city_string, major_city_prod_rt, key);
                break;
            case 2:
                AddString(other_city_string, other_city_prod_rt, key);
                break;
			case 3:
				AddString(threshold_infection_percent_string, threshold_infection_percent, key);
				break;
            default:
                break;
        }
    }

    //If tapping "Backspace"
    if (key == 8)
    {
        switch (selected_city_input)
        {
            case 1:
                RemoveStringOneChar(major_city_string, major_city_prod_rt);
                break;
            case 2:
                RemoveStringOneChar(other_city_string, other_city_prod_rt);
				break;
			case 3:
				RemoveStringOneChar(threshold_infection_percent_string, threshold_infection_percent);
				break;
            default:
                break;
        }
    }
}

//The function for mouse activity when there are input values
void MouseFuncInputValues(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (x >= major_city_input_x1 && x <= major_city_input_x2 && y >= major_city_input_y1 + input_ui_base_y && y <= major_city_input_y2 + input_ui_base_y)
        {
            selected_city_input = 1;
            current_major_input_color = select_input_color;
			current_other_input_color = normal_input_color;
			threshold_infection_input_color = normal_input_color;
        }
        else if (x >= other_city_input_x1 && x <= other_city_input_x2 && y >= other_city_input_y1 + input_ui_base_y && y <= other_city_input_y2 + input_ui_base_y)
        {
            selected_city_input = 2;
            current_major_input_color = normal_input_color;
			current_other_input_color = select_input_color;
			threshold_infection_input_color = normal_input_color;
        }
		else if (x >= threshold_infection_percent_input_x1 && x <= threshold_infection_percent_input_x2 && y >= threshold_infection_percent_input_y1 + input_ui_base_y && y <= threshold_infection_percent_input_y2 + input_ui_base_y)
		{
			selected_city_input = 3;
			current_major_input_color = normal_input_color;
			current_other_input_color = normal_input_color;
			threshold_infection_input_color = select_input_color;
		}
        else
        {
            selected_city_input = 4;
            current_major_input_color = normal_input_color;
            current_other_input_color = normal_input_color;
			threshold_infection_input_color = normal_input_color;
        }

        //The initialization will be done if "OK" button is clicked.
        if (x >= ok_button_x1 && x <= ok_button_x2 && y >= ok_button_y1 + input_ui_base_y && y <= ok_button_y2 + input_ui_base_y)
        {
            InitializeTime();
			InitializeCityDatas();
			InitializeOutlineDatas();
            is_inputed_values = true;
        }
    }
}

float TriangleArea(std::pair<int, int> p1, std::pair<int, int> p2, std::pair<int, int> p3)//���׹�ʽ
{
	float AB, BC, AC, P;
	AB = sqrt(pow(p2.first - p1.first, 2) + pow(p2.second - p1.second, 2));
	AC = sqrt(pow(p3.first - p1.first, 2) + pow(p3.second - p1.second, 2));
	BC = sqrt(pow(p3.first - p2.first, 2) + pow(p3.second - p2.second, 2));
	P = (AB + AC + BC) / 2;

	return sqrt(P*(P - AB)*(P - AC)*(P - BC));
}

bool IsInTriangle(std::pair<int, int> A, std::pair<int, int> B, std::pair<int, int> C, std::pair<int, int> D)
{
	float S1, S2, S3, Ssum;
	S1 = TriangleArea(A, B, D);
	S2 = TriangleArea(A, C, D);
	S3 = TriangleArea(B, C, D);
	Ssum = TriangleArea(A, B, C);

	if (0.1 > fabs(Ssum - S1 - S2 - S3))//ע�����ֵ
		return true;
	else
		return false;
}

//The function for mouse actions in the mode of showing dialog
void MouseFuncDialog(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (dialog_city_index < 0)
        {
            for (int i = 0; i < city_count; i++)
            {
					const auto& city = city_infos[i];
					for (int triangle_i = 0 ; triangle_i < city.city_outline_index.size() ; triangle_i += 3)
					{
						auto index_0 = city.city_outline_index[triangle_i];
						auto index_1 = city.city_outline_index[triangle_i + 1];
						auto index_2 = city.city_outline_index[triangle_i + 2];
						if (IsInTriangle(city.city_outline[index_0], city.city_outline[index_1], city.city_outline[index_2], std::make_pair(x, y)))
						{
							dialog_city_index = i;
							return;
						}
					}
            }
        }
        else
        {
            dialog_city_index = -1;
        }
    }
}

void MouseFuncInfo(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(is_info_shown==0){
            if(pow(x-info_x,2)+pow(y-info_y,2)<info_R*info_R){
                is_info_shown=1;
            }
        }
        else
        {
            is_info_shown=0;
        }
    }
}

//Structural function for mouse functions
void MouseFunc(int button, int state, int x, int y)
{
    if (is_inputed_values)
    {
        MouseFuncDialog(button, state, x, y);
    }
    else
    {
        MouseFuncInputValues(button, state, x, y);
    }
    MouseFuncInfo(button, state, x, y);
}
