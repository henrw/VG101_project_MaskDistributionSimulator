#include "GL/glut.h"
#include "mask_simulation.h"
#ifdef WINDOWS
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#else
#include <cmath>
#endif // WINDOWS

#include <ctime>
#include <string>
#include <iostream>
extern int end_check_result;
extern int draw_road_switch;
extern float threshold_msk_percent;
extern bool is_transport1;
extern bool is_transport2;
std::string month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
extern float PI;

extern int history_inf_cache_count;
//Convert the x coordinate of screen to the coordinates in OpenGL
GLfloat ParseOpenGLX(int x)
{
    return ((float)x * all_scale / screen_width - 0.5f) * 2;
}

//Convert the y coordinate of screen to the coordinates in OpenGL
GLfloat ParseOpenGLY(int y)
{
    return -((float)y * all_scale / screen_height - 0.5f) * 2;
}

//Draw general strings
void DrawString(std::string str, int x_offset, int y_offset, void *font = GLUT_BITMAP_HELVETICA_18)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(ParseOpenGLX(x_offset), ParseOpenGLY(y_offset));
    for (int i = 0; i < str.length(); i++)
        glutBitmapCharacter(font, str[i]);
}

//Draw rotatable strings
void DrawString2(std::string str, int x_offset, int y_offset, float angle, void *font = GLUT_STROKE_ROMAN)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glLoadIdentity();
    glTranslatef(ParseOpenGLX(x_offset), ParseOpenGLY(y_offset), 0);
    glScalef(0.00017, 0.0002, 0.0002);
    glRotated(angle, 0.0, 0.0, 1.0);
    glLineWidth(1);
    for (int i = 0; i < str.length(); i++)
        glutStrokeCharacter(font, str[i]);
    glLoadIdentity();
}

//���㵱ǰʮ����������Ϊ��λ��
int compute_number_level(int num)
{
	int val = num;
	int i = 0;
	for ( ; val != 0 ; ++i)
	{
		val = val / 10;
	}

	return i;
}

float inverse_lerp(float v1, float v2, float v)
{
	return (v - v1) / (v2 - v1);
}

float lerp(float v1, float v2, float t)
{
	return v1 + (v2 - v1) * t;
}

//Calculate the RGB values for city plotting
float color_calculate(float inf_num,float total_num)
{
	const int max_val = 220, min_val = 70;
	auto val = max_val - 1000 / (float)threshold_infection_percent/total_num * (max_val - min_val);

	//�����Ⱦ������ֵ�Ǽ�λ��
	auto threshold_infection_percent_level = compute_number_level((int)total_num*threshold_infection_percent/1000);
	//���㵱ǰ��Ⱦ�����Ǽ�λ��
	auto inf_num_level = compute_number_level(inf_num);

	//���㵱ǰ��Ⱦ�������ڵ���ɫ����
	auto down_val = max_val - inf_num_level / (float)threshold_infection_percent_level * (max_val - min_val);
	auto upper_val = max_val - (inf_num_level - 1) / (float)threshold_infection_percent_level * (max_val - min_val);

	//��ֵ���㵱ǰ��ɫ
	val = lerp(down_val, upper_val, inverse_lerp(pow(10, inf_num_level - 1), pow(10, inf_num_level), inf_num));

	return val;

}

void DrawInputUI()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(ParseOpenGLX(10), ParseOpenGLY(10), ParseOpenGLX(1070), ParseOpenGLY(490));

    DrawInfoLogo();

	auto base_y = input_ui_base_y;
    DrawString("Mask Distribution Simulator for Hubei Province during COVID-19", 280, 70, GLUT_BITMAP_HELVETICA_18);
    DrawString("Please enter the mask production capacity (pieces per day) of:", 290, 170 + base_y, GLUT_BITMAP_HELVETICA_18);
    DrawString("major industrial city*:", 290, 210 + base_y, GLUT_BITMAP_HELVETICA_18);
    DrawString("(Xiantao)", 290, 230 + base_y, GLUT_BITMAP_HELVETICA_18);
	DrawString("other cities:", 290, 290 + base_y, GLUT_BITMAP_HELVETICA_18);
	DrawString("tolerable infection ratio:", 290, 373 + base_y);
    DrawString("X 1/10000", 660, 370 + base_y,GLUT_BITMAP_HELVETICA_12);
    DrawString("Example: input \"10\" to set that the simulation ends when the infection ratio reaches 0.001 in any one of the cities", 290, 420 + base_y,GLUT_BITMAP_HELVETICA_12);
    DrawString("             scale for reference: for Wuhan,9785392(population) * 10 * 1/10000 = 9785", 290, 440 + base_y,GLUT_BITMAP_HELVETICA_12);
    glColor3f(current_major_input_color, current_major_input_color, current_major_input_color);
    //main city
    glRectf(ParseOpenGLX(major_city_input_x1), ParseOpenGLY(major_city_input_y1 + base_y), ParseOpenGLX(major_city_input_x2), ParseOpenGLY(major_city_input_y2 + base_y));
    DrawString(major_city_string.c_str(), 570, 215 + base_y, GLUT_BITMAP_HELVETICA_18);

    glColor3f(current_other_input_color, current_other_input_color, current_other_input_color);
    //other cities
    glRectf(ParseOpenGLX(other_city_input_x1), ParseOpenGLY(other_city_input_y1 + base_y), ParseOpenGLX(other_city_input_x2), ParseOpenGLY(other_city_input_y2 + base_y));
    DrawString(other_city_string.c_str(), 570, 295 + base_y, GLUT_BITMAP_HELVETICA_18);

	glColor3f(threshold_infection_input_color, threshold_infection_input_color, threshold_infection_input_color);
	//threshold
	glRectf(ParseOpenGLX(threshold_infection_percent_input_x1), ParseOpenGLY(threshold_infection_percent_input_y1 + base_y), ParseOpenGLX(threshold_infection_percent_input_x2), ParseOpenGLY(threshold_infection_percent_input_y2 + base_y));
	DrawString(threshold_infection_percent_string.c_str(), 550, 375 + base_y, GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.5, 0.5, 0.5);
    glRectf(ParseOpenGLX(ok_button_x1), ParseOpenGLY(ok_button_y1 + base_y), ParseOpenGLX(ok_button_x2), ParseOpenGLY(ok_button_y2 + base_y));
    //OK button
    DrawString("OK", 685, 480 + base_y, GLUT_BITMAP_HELVETICA_18);
}


void DrawSmallTriangle(int x_offset, int y_offset)
{
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.0f);
    float length = 15;
    glVertex2f(ParseOpenGLX(x_offset), ParseOpenGLY(y_offset-length/sqrt(3)));
    glVertex2f(ParseOpenGLX(x_offset-length/2), ParseOpenGLY(y_offset+length/sqrt(3)/2));
    glVertex2f(ParseOpenGLX(x_offset+length/2), ParseOpenGLY(y_offset+length/sqrt(3)/2));
    glEnd();
}


void DrawSmallSquare(int x_offset, int y_offset)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    int length = 4;
    glRectf(ParseOpenGLX(x_offset - length), ParseOpenGLY(y_offset - length), ParseOpenGLX(x_offset + length), ParseOpenGLY(y_offset + length));
}

//the road
void DrawRoad(int start_index, int end_index, int min_diff_val, int max_diff_val,int mode)
{

    if(is_transport2)
    {
        if(city_infos[start_index].transport_num>city_infos[end_index].transport_num)
        {
            draw_road_switch=1;
        } else
        {
            draw_road_switch=0;
        }
    }
    int swap;
    if(mode==2&&draw_road_switch){
        swap=start_index;
        start_index=end_index;
        end_index=swap;
    }
    const auto &start_city = city_infos[start_index];
    const auto &end_city = city_infos[end_index];

	int	start_city_x = start_city.hubei_center_x;
	int	start_city_y = start_city.hubei_center_y;
	int	end_city_x = end_city.hubei_center_x;
	int	end_city_y = end_city.hubei_center_y;

    int delta_x = end_city_x - start_city_x;
    int delta_y = end_city_y - start_city_y;

    float length = sqrt(delta_x * delta_x + delta_y * delta_y);
    float normal_delta_x = delta_x / length;
    float normal_delta_y = delta_y / length;
    float road_width;

    if(mode==1) {glColor3f(0.9f, 0.0f, 0.0f);road_width=2.5;}
    if(mode==2) {glColor3f(0.9f, 0.8f, 0.0f);road_width=1.5;}

    glBegin(GL_POLYGON);

    int inner_road_width = 0.5 * road_width;
    int inner_road_bold_width = road_width - inner_road_width;
    //the outer side of the road
    //	starting point
    int start_x_1 = start_city_x - normal_delta_y * road_width;
    int start_y_1 = start_city_y + normal_delta_x * road_width;
    int start_x_2 = start_city_x + normal_delta_y * road_width;
    int start_y_2 = start_city_y - normal_delta_x * road_width;
    glVertex2f(ParseOpenGLX(start_x_1), ParseOpenGLY(start_y_1));
    glVertex2f(ParseOpenGLX(start_x_2), ParseOpenGLY(start_y_2));
    //ending point
    int end_x_1 = end_city_x + (-normal_delta_x * 2 + normal_delta_y) * road_width;
    int end_y_1 = end_city_y + (-normal_delta_y * 2 - normal_delta_x) * road_width;
    int end_x_2 = end_city_x + (-normal_delta_x * 2 - normal_delta_y) * road_width;
    int end_y_2 = end_city_y + (-normal_delta_y * 2 + normal_delta_x) * road_width;
    glVertex2f(ParseOpenGLX(end_x_1), ParseOpenGLY(end_y_1));
    glVertex2f(ParseOpenGLX(end_city_x), ParseOpenGLY(end_city_y));
    glVertex2f(ParseOpenGLX(end_x_2), ParseOpenGLY(end_y_2));
    glEnd();
    //the inner side of the road

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(ParseOpenGLX(start_x_1 + (normal_delta_x + normal_delta_y) * inner_road_bold_width), ParseOpenGLY(start_y_1 + (-normal_delta_x + normal_delta_y) * inner_road_bold_width));
    glVertex2f(ParseOpenGLX(start_x_2 + (normal_delta_x - normal_delta_y) * inner_road_bold_width), ParseOpenGLY(start_y_2 + (normal_delta_x + normal_delta_y) * inner_road_bold_width));
    glVertex2f(ParseOpenGLX(end_x_1 + (-normal_delta_y) * inner_road_bold_width), ParseOpenGLY(end_y_1 + (normal_delta_x)*inner_road_bold_width));
    glVertex2f(ParseOpenGLX(end_city_x - normal_delta_x * inner_road_bold_width * 1.5), ParseOpenGLY(end_city_y - normal_delta_y * inner_road_bold_width * 1.5));
    glVertex2f(ParseOpenGLX(end_x_2 + (+normal_delta_y) * inner_road_bold_width), ParseOpenGLY(end_y_2 + (-normal_delta_x) * inner_road_bold_width));
    glEnd();

    //rolling animation(car)
    glColor3f(0.3f, 0.3f, 0.3f);

    int tag_length = 15;
    int half_tag_length = tag_length / 2;

    // display the animation through timestamp(ms)
    int current_pos = 10;
    if (is_transport1&&mode==1||is_transport2&&mode==2)
    {
        DrawRoadText(start_index,mode);
        DrawRoadText(end_index,mode);
        //translation speed
        auto diff = end_city.current_hour_mask_num - end_city.pre_hour_mask_num;

        int t = 0;

        if ((max_diff_val - min_diff_val) != 0)
        {
            t = (diff - min_diff_val) / (max_diff_val - min_diff_val);
        }
        //max_speed min_speed
        float max_speed = 15;
        float min_speed = 2;
        float speed = (max_speed - min_speed) * t + min_speed;
        current_pos += (int(clock() / (100.0f / speed)) % 30);
    }

    while (end_check_result ==-2)
    {
        int farest_x_delta = normal_delta_x * (current_pos + tag_length + half_tag_length);
        int farest_y_delta = normal_delta_y * (current_pos + tag_length + half_tag_length);
        int current_length = sqrt(farest_x_delta * farest_x_delta + farest_y_delta * farest_y_delta);

        //drawing small arrow repeatedly until reaching the ending point
        if (current_length < length - 10)
        {
            if(mode==1) glColor3f(0.9f, 0.0f, 0.0f);
            if(mode==2) glColor3f(0.9f, 0.8f, 0.0f);
            glBegin(GL_POLYGON);
            glVertex2f(ParseOpenGLX(start_city_x + normal_delta_x * (current_pos + half_tag_length)), ParseOpenGLY(start_city_y + normal_delta_y * (current_pos + half_tag_length)));
            glVertex2f(ParseOpenGLX(start_x_1 + normal_delta_x * current_pos), ParseOpenGLY(start_y_1 + normal_delta_y * current_pos));
            glVertex2f(ParseOpenGLX(start_x_1 + normal_delta_x * (current_pos + tag_length)), ParseOpenGLY(start_y_1 + normal_delta_y * (current_pos + tag_length)));
            glVertex2f(ParseOpenGLX(start_city_x + farest_x_delta), ParseOpenGLY(start_city_y + farest_y_delta));
            glVertex2f(ParseOpenGLX(start_x_2 + normal_delta_x * (current_pos + tag_length)), ParseOpenGLY(start_y_2 + normal_delta_y * (current_pos + tag_length)));
            glVertex2f(ParseOpenGLX(start_x_2 + normal_delta_x * current_pos), ParseOpenGLY(start_y_2 + normal_delta_y * current_pos));
            glEnd();

            current_pos += 30;
        }
        else
        {
            break;
        }
    }
}

void DrawMap(const CITY& city)
{

	glBegin(GL_TRIANGLES);
	auto inf_num = city.inf_num;
    auto total_num = city.total_num;
	glColor3f((color_calculate(inf_num,total_num) + 5) / 255.0f, (color_calculate(inf_num,total_num) + 15) / 255.0f, (color_calculate(inf_num,total_num) + 27) / 255.0f);
	for (int point_i = 0; point_i < (int)city.city_outline_index.size(); ++point_i)
	{
		const auto& index = city.city_outline[city.city_outline_index[point_i]];
		glVertex2f(ParseOpenGLX(index.first), ParseOpenGLY(index.second));
	}
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	for (int point_i = 0; point_i < city.city_outline.size(); ++point_i)
	{
		const auto& point = city.city_outline[point_i];
		glVertex2f(ParseOpenGLX(point.first), ParseOpenGLY(point.second));
	}
	glEnd();
}

//drawing
void ShowCities()
{
    //draw cities
    for (int i = 0; i < city_count; i++)
    {
        const auto &city = city_infos[i];

			DrawMap(city);
			DrawString(city.name, city.name_x, city.name_y, GLUT_BITMAP_HELVETICA_12);
	}

    int min_diff_val = 10000000, max_diff_val = 0;
    //computing the maximum speed and the minimum speed
    for (int i = 1; i < city_count; i++)
    {
        const auto &city = city_infos[i];
        auto diff = city.current_hour_mask_num - city.pre_hour_mask_num;
        if (diff < min_diff_val)
            min_diff_val = diff;
        if (diff > max_diff_val)
            max_diff_val = diff;
    }

    //draw roads
    DrawRoad(1, 0, min_diff_val, max_diff_val,1);
    DrawRoad(3, 1, min_diff_val, max_diff_val,1);
    DrawRoad(major_city_index, 2, min_diff_val, max_diff_val,1);
    DrawRoad(major_city_index, 3, min_diff_val, max_diff_val,1);
    DrawRoad(major_city_index, 4, min_diff_val, max_diff_val,1);
    DrawRoad(9, 5, min_diff_val, max_diff_val,1);
    DrawRoad(7, 6, min_diff_val, max_diff_val,1);
    DrawRoad(10, 7, min_diff_val, max_diff_val,1);
    DrawRoad(major_city_index, 9, min_diff_val, max_diff_val,1);
    DrawRoad(major_city_index, 10, min_diff_val, max_diff_val,1);
    DrawRoad(9, 11, min_diff_val, max_diff_val,1);
    DrawRoad(9, 12, min_diff_val, max_diff_val,1);

    DrawRoad(9, 4, min_diff_val, max_diff_val,2);
    DrawRoad(4, 2, min_diff_val, max_diff_val,2);
    DrawRoad(2, 3, min_diff_val, max_diff_val,2);
    DrawRoad(3, 10, min_diff_val, max_diff_val,2);
    DrawRoad(5, 12, min_diff_val, max_diff_val,2);
    DrawRoad(12, 11, min_diff_val, max_diff_val,2);

    for (int i = 0; i < city_count; i++)
    {
        const auto &city = city_infos[i];
        if(i==major_city_index){
            DrawSmallTriangle(city.hubei_center_x, city.hubei_center_y);
        } else{
            DrawSmallSquare(city.hubei_center_x, city.hubei_center_y);
        }

    }

	DrawString("Hubei Province, China", 40, 480, GLUT_BITMAP_HELVETICA_18);
}

//show the data
void DrawCityDatas()
{
    DrawString("City", 710, 80, GLUT_BITMAP_HELVETICA_12);
    DrawString("Citizens", 780, 80, GLUT_BITMAP_HELVETICA_12);
    DrawString("infected", 850, 80, GLUT_BITMAP_HELVETICA_12);
    DrawString("recovered", 920, 80, GLUT_BITMAP_HELVETICA_12);
    DrawString("masks", 990, 80, GLUT_BITMAP_HELVETICA_12);
    for (int i = 0; i < city_count; i++)
    {
        const auto &city = city_infos[i];
        int row = i;
        //        switch (row)
        //        {
        //            case 0:
        //                row = 1;
        //                break;
        //            case 1:
        //                row = 0;
        //            default:
        //                break;
        //        }//Now the order doesn't matter
        int y_pos = 110 + 30 * row;
        char total_num_str[128];
        char inf_num_str[128];
        char rec_num_str[128];
        char cur_msk_num_str[128];

#ifdef WINDOWS
		_itoa_s(city.total_num, total_num_str, 10);
		_itoa_s(city.inf_num, inf_num_str, 10);
		_itoa_s(city.rec_num, rec_num_str, 10);
		_itoa_s(cur_msk_num, cur_msk_num_str, 10);
#else
		_itoa(city.total_num, total_num_str, 10);
		_itoa(city.inf_num, inf_num_str, 10);
		_itoa(city.rec_num, rec_num_str, 10);
		_itoa(city.cur_msk_num+threshold_msk_percent*city.total_num, cur_msk_num_str, 10);
#endif // WINDOWS
        DrawString(city.name, 710, y_pos, GLUT_BITMAP_HELVETICA_12);
        DrawString(total_num_str, 780, y_pos, GLUT_BITMAP_HELVETICA_12);
        DrawString(inf_num_str, 850, y_pos, GLUT_BITMAP_HELVETICA_12);
        DrawString(rec_num_str, 920, y_pos, GLUT_BITMAP_HELVETICA_12);
        DrawString(cur_msk_num_str, 990, y_pos, GLUT_BITMAP_HELVETICA_12);
    }
}

float MaxHistoryInfNums(const CITY& city)
{
	float max_val = 0;
	for (int i = 0 ; i < city.history_inf_nums.size() ; ++i)
	{
		auto inf_num = city.history_inf_nums[i];
		max_val = max(max_val, inf_num);
	}
	return max_val;
}

//city's dialog
void ShowDialog()
{
    if (dialog_city_index >= 0)
    {
        const auto &city_info = city_infos[dialog_city_index];

        glColor3f(0.9f, 0.9f, 0.9f);
        int x1 = 300, x2 = screen_width - 300, y1 = 50, y2 = screen_height - 50;
        glRectf(ParseOpenGLX(x1), ParseOpenGLY(y1), ParseOpenGLX(x2), ParseOpenGLY(y2));

        //title
        std::string city = "City ";
        city += city_info.name;
        DrawString(city, x1 + 180, y1 + 30, GLUT_BITMAP_HELVETICA_18);

        //axis

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);

        int x_origin = x1 + 80;
        int y_origin = y2 - 100;
        int x_end = x2 - 80;
        int y_end = y1 + 100;
        //x_axis
        glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_origin));
        glVertex2f(ParseOpenGLX(x_end), ParseOpenGLY(y_origin));
        glVertex2f(ParseOpenGLX(x_end), ParseOpenGLY(y_origin));
        glVertex2f(ParseOpenGLX(x_end - 10), ParseOpenGLY(y_origin - 10));
        glVertex2f(ParseOpenGLX(x_end), ParseOpenGLY(y_origin));
        glVertex2f(ParseOpenGLX(x_end - 10), ParseOpenGLY(y_origin + 10));
        //y_axis
        glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_origin));
        glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_end));
        glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_end));
        glVertex2f(ParseOpenGLX(x_origin + 10), ParseOpenGLY(y_end + 10));
        glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_end));
        glVertex2f(ParseOpenGLX(x_origin - 10), ParseOpenGLY(y_end + 10));
        glEnd();

        //labeling axes
        DrawString("Persons", x_origin - 30, y_end - 10, GLUT_BITMAP_HELVETICA_12);
        DrawString("0", x_origin - 10, y_origin, GLUT_BITMAP_HELVETICA_12);
        char total_num_str[20];
#ifdef WINDOWS
		_itoa_s(city_info.total_num, total_num_str, 10);
#else
		_itoa(city_info.total_num, total_num_str, 10);
#endif
		auto max_inf_num = MaxHistoryInfNums(city_info);
		auto upper_limit = min(int(100.0f * (max_inf_num / city_info.total_num)) + 1, 100);
		char y_description[128];
		snprintf(y_description, 128, "%d%%", upper_limit);
        DrawString(y_description, x_origin - 60, y_end + 10, GLUT_BITMAP_HELVETICA_12);
        DrawString("Time", x_end, y_origin + 10, GLUT_BITMAP_HELVETICA_12);

        //function plotting
        glBegin(GL_LINE_STRIP);
        glVertex2f(ParseOpenGLX(x_origin), ParseOpenGLY(y_origin));

		auto upper = float(upper_limit) / 100.0f *  city_info.total_num;
        for (int i = 0; i < history_inf_cache_count && i < city_info.history_inf_nums.size(); i++)
        {
            //            int y = y_origin - city_info.history_inf_nums[i];
			//compute infect people percent
			float percent = city_info.history_inf_nums[i] / upper;
			int y = y_origin - (y_origin - y_end) * percent;
            glVertex2f(ParseOpenGLX(i + x_origin), ParseOpenGLY(y));
        }
        glEnd();

        //text
        char inf_num_str[128];
#ifdef WINDOWS
        _itoa_s(city_info.inf_num, inf_num_str, 10);
#else
		_itoa(city_info.inf_num, inf_num_str, 10);
#endif
        char rec_rt_str[128];
        snprintf(rec_rt_str, 128, "%7f", city_info.rec_rt);
        DrawString(std::string("infected people:") + inf_num_str, x_origin, y_origin + 40, GLUT_BITMAP_HELVETICA_18);
        DrawString(std::string("Recovery Rate:") + rec_rt_str, x_origin, y_origin + 70, GLUT_BITMAP_HELVETICA_18);
    }
}

void ShowInfo()
{
    if(is_info_shown==1){

        int x1 = info_start_x, x2 = screen_width - info_start_x, y1 = info_start_y, y2 = screen_height - info_start_y;
        glColor3f(0,0,0);
        glRectf(ParseOpenGLX(x1), ParseOpenGLY(y1), ParseOpenGLX(x2), ParseOpenGLY(y2));
        glColor3f(1,1,1);
        glRectf(ParseOpenGLX(x1+1), ParseOpenGLY(y1+1), ParseOpenGLX(x2-1), ParseOpenGLY(y2-1));
        int linespace=20;
        DrawString("Welcome!",info_start_x+info_left_space,info_start_y+2*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("This program tends to simulate a scientific mask redistribution in Hubei Province, China, during the Covid-19.",info_start_x+info_left_space,info_start_y+3*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("Note that in our model, Xiantao is deemed as the major mask production city with a higher production capacity",info_start_x+info_left_space,info_start_y+4.5*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("(rich in fabric production). Other cities have a lower production capacity as there are less mask production.",info_start_x+info_left_space,info_start_y+5.5*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("(Setting the mask production capacities and tolerable infected ratio will lead you to the next frame)",info_start_x+info_left_space,info_start_y+7*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("-----------------------------------------------------------------------------",info_start_x+info_left_space,info_start_y+8*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("You can see a message box on the upper left, the map on the left and an information list on the right:",info_start_x+info_left_space,info_start_y+9*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("    -Message box tells the end of simulation",info_start_x+info_left_space,info_start_y+10*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("    -Red road indicates the mask transportation from the major production city (every 6 hrs)",info_start_x+info_left_space,info_start_y+11*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("     Yellow road indicates the mask transportation between some non-major cities (every 2 hrs)",info_start_x+info_left_space,info_start_y+12*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("    -Information list records the immediate data of different cities.",info_start_x+info_left_space,info_start_y+13*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("More features you might like to know:",info_start_x+info_left_space,info_start_y+14.5*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("    -click the cities on map to inspect the trending of the infected number",info_start_x+info_left_space,info_start_y+15.5*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("    -judge the seriousness of infection by the color of cities on the map",info_start_x+info_left_space,info_start_y+16.5*linespace,GLUT_BITMAP_HELVETICA_12);
        DrawString("Enjoy it!",info_start_x+info_left_space,info_start_y+18*linespace,GLUT_BITMAP_HELVETICA_12);


    }
}

void DrawInfoLogo()
{
    int n = 200;
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < n; i++)
    {
        GLfloat x = ParseOpenGLX(info_R *cos(2 * PI / n * i) + info_x);
        GLfloat y = ParseOpenGLY(info_R *sin(2 * PI / n * i) + info_y);
        glVertex2f(x, y);
    }
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int i = 0; i < n; i++)
    {
        GLfloat x = ParseOpenGLX((info_R-2) *cos(2 * PI / n * i) + info_x);
        GLfloat y = ParseOpenGLY((info_R-2) *sin(2 * PI / n * i) + info_y);
        glVertex2f(x, y);
    }
    glEnd();

    DrawString("i",info_x-3,info_y+7,GLUT_BITMAP_TIMES_ROMAN_24);
}

//show the virtual time
void ShowDate()
{
    {
        struct tm t;
        localtime_s(&t, &current_virtual_time);
        char data_str[128];
        sprintf_s(data_str, "%02d:%02d  %s.  %d, %d", t.tm_hour, t.tm_min, month[t.tm_mon].c_str(), t.tm_mday, t.tm_year + 1900);

        DrawString(data_str, 520, 480, GLUT_BITMAP_HELVETICA_18);
    }
}

//the digit in the road
void DrawRoadText(int index,int mode)
{
    if(city_infos[index].transport_num==0&&mode==2){return;}
    char diff_str[12];
    if (city_infos[index].transport_num>=0){
#ifdef WINDOWS
        _itoa_s((int)city_infos[index].transport_num, diff_str+1, 11, 10);
#else
		_itoa((int)city_infos[index].transport_num, diff_str + 1, 10);
#endif

        diff_str[0]='+';
    }
    else if(city_infos[index].transport_num<0)
    {
#ifdef WINDOWS
        _itoa_s((int)city_infos[index].transport_num, diff_str, 10);
#else
		_itoa((int)city_infos[index].transport_num, diff_str, 10);
#endif
    }
    DrawString2(diff_str, city_infos[index].string_x,city_infos[index].string_y,city_infos[index].rotation,GLUT_STROKE_ROMAN);


}
