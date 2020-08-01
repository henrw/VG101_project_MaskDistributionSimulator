#include "GL/glut.h"
#include "mask_simulation.h"

//Index of the selection of cities during input.
//0 refers to unselected; 1 refers to the major city selected; 2 refers to other cities selected.
int selected_city_input = 0;

void AddString(std::string &str, int &val, unsigned char key)
{
    str += key;
    val = atoi(str.c_str());
    char temp[256];
    _itoa(val, temp, 10);
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
        if (x >= major_city_input_x1 && x <= major_city_input_x2 && y >= major_city_input_y1 && y <= major_city_input_y2)
        {
            selected_city_input = 1;
            current_major_input_color = select_input_color;
            current_other_input_color = normal_input_color;
        }
        else if (x >= other_city_input_x1 && x <= other_city_input_x2 && y >= other_city_input_y1 && y <= other_city_input_y2)
        {
            selected_city_input = 2;
            current_major_input_color = normal_input_color;
            current_other_input_color = select_input_color;
        }
        else
        {
            selected_city_input = 0;
            current_major_input_color = normal_input_color;
            current_other_input_color = normal_input_color;
        }

        //The initialization will be done if "OK" button is clicked.
        if (x >= ok_button_x1 && x <= ok_button_x2 && y >= ok_button_y1 && y <= ok_button_y2)
        {
            InitializeTime();
            InitializeCityDatas();
            is_inputed_values = true;
        }
    }
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
                //determining whether the cursor is in the circle (city)
                int delta_x = city_infos[i].center_x - x;
                int delta_y = (int) ((float)(city_infos[i].center_y - y) / city_infos[i].y_scale);
                int distance_diff = delta_x * delta_x + delta_y * delta_y - city_infos[i].R * city_infos[i].R;
                //if within the circle (judging from the distance between the cursor and the center of city)
                if (distance_diff < 0)
                {
                    dialog_city_index = i;
                }
            }
        }
        else
        {
            dialog_city_index = -1;
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
}
