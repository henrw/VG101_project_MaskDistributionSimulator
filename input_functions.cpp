#include "GL/glut.h"
#include "mask_simulation.h"

//选中的输入框序号，0代表都没选中，1代表选中主城市，2代表选中其他城市
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

    //如果按下Backspace健
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

//输入数值的鼠标响应事件
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

        //如果点击OK按钮，则进行初始化，进入主程序
        if (x >= ok_button_x1 && x <= ok_button_x2 && y >= ok_button_y1 && y <= ok_button_y2)
        {
            InitializeTime();
            InitializeCityDatas();
            is_inputed_values = true;
        }
    }
}

//弹出对话框的鼠标响应时间
void MouseFuncDialog(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (dialog_city_index < 0)
        {
            for (int i = 0; i < city_count; i++)
            {
                //计算鼠标点击位置是否在圆里
                int delta_x = city_infos[i].center_x - x;
                int delta_y = (city_infos[i].center_y - y) / city_infos[i].y_scale;
                int distance_diff = delta_x * delta_x + delta_y * delta_y - city_infos[i].R * city_infos[i].R;
                //如果鼠标点击位置到圆心距离小于半径,则显示对话框
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

//鼠标操作回调函数
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
