#include "mask_simulation.h"
#include "ctime"


extern time_t delta_time;
//Calculate the virtual time
void ComputeVirtualTime()
{
    time_t tnow;
    time(&tnow);
    int real_interval = 1;
    int real_delta_time = tnow - start_time;
    real_delta_time = real_delta_time / real_interval * real_interval;

    //A real_interval is equivalent to 1 virtual hour
    delta_time = (real_delta_time) * (3600 / real_interval);
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

//初始化外轮廓数据
void InitializeOutlineDatas()
{
	//Shiyan
	city_infos[0].city_outline.push_back(std::make_pair(115, 57));
	city_infos[0].city_outline.push_back(std::make_pair(232, 51));
	city_infos[0].city_outline.push_back(std::make_pair(290, 111));
	city_infos[0].city_outline.push_back(std::make_pair(221, 195));
	city_infos[0].city_outline.push_back(std::make_pair(139, 224));
	city_infos[0].city_outline.push_back(std::make_pair(116, 120));
	city_infos[0].city_outline.push_back(std::make_pair(171, 107));
	city_infos[0].city_outline.push_back(std::make_pair(115, 57));//done
	city_infos[0].city_outline_index.push_back(0);//
	city_infos[0].city_outline_index.push_back(1);
	city_infos[0].city_outline_index.push_back(2);
	city_infos[0].city_outline_index.push_back(0);//
	city_infos[0].city_outline_index.push_back(2);
	city_infos[0].city_outline_index.push_back(6);
	city_infos[0].city_outline_index.push_back(6);//
	city_infos[0].city_outline_index.push_back(2);
	city_infos[0].city_outline_index.push_back(3);
	city_infos[0].city_outline_index.push_back(3);//
	city_infos[0].city_outline_index.push_back(5);
	city_infos[0].city_outline_index.push_back(6);
	city_infos[0].city_outline_index.push_back(3);//
	city_infos[0].city_outline_index.push_back(4);
	city_infos[0].city_outline_index.push_back(5);
	//Xiangyang
	city_infos[1].city_outline.push_back(std::make_pair(290, 111));
	city_infos[1].city_outline.push_back(std::make_pair(221, 195));
	city_infos[1].city_outline.push_back(std::make_pair(234, 220));
	city_infos[1].city_outline.push_back(std::make_pair(310, 244));
	city_infos[1].city_outline.push_back(std::make_pair(378, 214));
	city_infos[1].city_outline.push_back(std::make_pair(405, 132));//done	
	city_infos[1].city_outline_index.push_back(0);//
	city_infos[1].city_outline_index.push_back(1);
	city_infos[1].city_outline_index.push_back(2);
	city_infos[1].city_outline_index.push_back(0);//
	city_infos[1].city_outline_index.push_back(2);
	city_infos[1].city_outline_index.push_back(3);
	city_infos[1].city_outline_index.push_back(0);//
	city_infos[1].city_outline_index.push_back(3);
	city_infos[1].city_outline_index.push_back(4);
	city_infos[1].city_outline_index.push_back(0);//
	city_infos[1].city_outline_index.push_back(4);
	city_infos[1].city_outline_index.push_back(5);
	////Suizhou
	city_infos[2].city_outline.push_back(std::make_pair(405, 132));
	city_infos[2].city_outline.push_back(std::make_pair(378, 214));
	city_infos[2].city_outline.push_back(std::make_pair(427, 240));
	city_infos[2].city_outline.push_back(std::make_pair(431, 226));
	city_infos[2].city_outline.push_back(std::make_pair(482, 226));
	city_infos[2].city_outline.push_back(std::make_pair(487, 187));
	city_infos[2].city_outline.push_back(std::make_pair(454, 126));//done	
	city_infos[2].city_outline_index.push_back(0);//
	city_infos[2].city_outline_index.push_back(1);
	city_infos[2].city_outline_index.push_back(2);
	city_infos[2].city_outline_index.push_back(0);//
	city_infos[2].city_outline_index.push_back(2);
	city_infos[2].city_outline_index.push_back(3);
	city_infos[2].city_outline_index.push_back(0);//
	city_infos[2].city_outline_index.push_back(3);
	city_infos[2].city_outline_index.push_back(4);
	city_infos[2].city_outline_index.push_back(0);//
	city_infos[2].city_outline_index.push_back(4);
	city_infos[2].city_outline_index.push_back(5);
	city_infos[2].city_outline_index.push_back(0);//
	city_infos[2].city_outline_index.push_back(5);
	city_infos[2].city_outline_index.push_back(6);
	////Jingmen
	city_infos[3].city_outline.push_back(std::make_pair(326, 314));
	city_infos[3].city_outline.push_back(std::make_pair(310, 244));
	city_infos[3].city_outline.push_back(std::make_pair(378, 214));
	city_infos[3].city_outline.push_back(std::make_pair(427, 240));
	city_infos[3].city_outline.push_back(std::make_pair(442, 250));
	city_infos[3].city_outline.push_back(std::make_pair(429, 283));
	city_infos[3].city_outline.push_back(std::make_pair(381, 293));
	city_infos[3].city_outline.push_back(std::make_pair(365, 332));//done
	city_infos[3].city_outline_index.push_back(0);//
	city_infos[3].city_outline_index.push_back(1);
	city_infos[3].city_outline_index.push_back(6);
	city_infos[3].city_outline_index.push_back(1);//
	city_infos[3].city_outline_index.push_back(5);
	city_infos[3].city_outline_index.push_back(6);
	city_infos[3].city_outline_index.push_back(1);//
	city_infos[3].city_outline_index.push_back(2);
	city_infos[3].city_outline_index.push_back(5);
	city_infos[3].city_outline_index.push_back(2);//
	city_infos[3].city_outline_index.push_back(3);
	city_infos[3].city_outline_index.push_back(4);
	city_infos[3].city_outline_index.push_back(2);//
	city_infos[3].city_outline_index.push_back(4);
	city_infos[3].city_outline_index.push_back(5);
	city_infos[3].city_outline_index.push_back(4);//
	city_infos[3].city_outline_index.push_back(5);
	city_infos[3].city_outline_index.push_back(6);
	city_infos[3].city_outline_index.push_back(0);//
	city_infos[3].city_outline_index.push_back(6);
	city_infos[3].city_outline_index.push_back(7);
	////Xiaogan
	city_infos[4].city_outline.push_back(std::make_pair(429, 283));
	city_infos[4].city_outline.push_back(std::make_pair(442, 250));
	city_infos[4].city_outline.push_back(std::make_pair(427, 240));
	city_infos[4].city_outline.push_back(std::make_pair(431, 226));
	city_infos[4].city_outline.push_back(std::make_pair(482, 226));
	city_infos[4].city_outline.push_back(std::make_pair(487, 187));
	city_infos[4].city_outline.push_back(std::make_pair(531, 200));
	city_infos[4].city_outline.push_back(std::make_pair(523, 234));
	city_infos[4].city_outline.push_back(std::make_pair(498, 242));
	city_infos[4].city_outline.push_back(std::make_pair(498, 283));
	city_infos[4].city_outline.push_back(std::make_pair(478, 287));
	city_infos[4].city_outline.push_back(std::make_pair(461, 325));
	city_infos[4].city_outline.push_back(std::make_pair(439, 324));//done
	city_infos[4].city_outline_index.push_back(0);//
	city_infos[4].city_outline_index.push_back(11);
	city_infos[4].city_outline_index.push_back(12);
	city_infos[4].city_outline_index.push_back(0);//
	city_infos[4].city_outline_index.push_back(10);
	city_infos[4].city_outline_index.push_back(11);
	city_infos[4].city_outline_index.push_back(0);//
	city_infos[4].city_outline_index.push_back(1);
	city_infos[4].city_outline_index.push_back(10);
	city_infos[4].city_outline_index.push_back(1);//
	city_infos[4].city_outline_index.push_back(2);
	city_infos[4].city_outline_index.push_back(3);
	city_infos[4].city_outline_index.push_back(8);//
	city_infos[4].city_outline_index.push_back(9);
	city_infos[4].city_outline_index.push_back(10);
	city_infos[4].city_outline_index.push_back(1);//
	city_infos[4].city_outline_index.push_back(3);
	city_infos[4].city_outline_index.push_back(4);
	city_infos[4].city_outline_index.push_back(1);//
	city_infos[4].city_outline_index.push_back(4);
	city_infos[4].city_outline_index.push_back(8);
	city_infos[4].city_outline_index.push_back(4);//
	city_infos[4].city_outline_index.push_back(7);
	city_infos[4].city_outline_index.push_back(8);
	city_infos[4].city_outline_index.push_back(4);//
	city_infos[4].city_outline_index.push_back(5);
	city_infos[4].city_outline_index.push_back(7);
	city_infos[4].city_outline_index.push_back(5);//
	city_infos[4].city_outline_index.push_back(6);
	city_infos[4].city_outline_index.push_back(7);
	city_infos[4].city_outline_index.push_back(1);//
	city_infos[4].city_outline_index.push_back(8);
	city_infos[4].city_outline_index.push_back(10);
	////Huanggang",
	city_infos[5].city_outline.push_back(std::make_pair(523, 234));
	city_infos[5].city_outline.push_back(std::make_pair(531, 200));
	city_infos[5].city_outline.push_back(std::make_pair(581, 211));
	city_infos[5].city_outline.push_back(std::make_pair(651, 262));
	city_infos[5].city_outline.push_back(std::make_pair(642, 306));
	city_infos[5].city_outline.push_back(std::make_pair(668, 379));
	city_infos[5].city_outline.push_back(std::make_pair(618, 379));
	city_infos[5].city_outline.push_back(std::make_pair(554, 316));
	city_infos[5].city_outline.push_back(std::make_pair(572, 272));
	city_infos[5].city_outline.push_back(std::make_pair(554, 250));//done

	city_infos[5].city_outline_index.push_back(0);//
	city_infos[5].city_outline_index.push_back(1);
	city_infos[5].city_outline_index.push_back(2);
	city_infos[5].city_outline_index.push_back(0);//
	city_infos[5].city_outline_index.push_back(2);
	city_infos[5].city_outline_index.push_back(9);
	city_infos[5].city_outline_index.push_back(9);//
	city_infos[5].city_outline_index.push_back(8);
	city_infos[5].city_outline_index.push_back(2);
	city_infos[5].city_outline_index.push_back(2);//
	city_infos[5].city_outline_index.push_back(3);
	city_infos[5].city_outline_index.push_back(8);
	city_infos[5].city_outline_index.push_back(3);//
	city_infos[5].city_outline_index.push_back(4);
	city_infos[5].city_outline_index.push_back(8);
	city_infos[5].city_outline_index.push_back(4);//
	city_infos[5].city_outline_index.push_back(7);
	city_infos[5].city_outline_index.push_back(8);
	city_infos[5].city_outline_index.push_back(4);//
	city_infos[5].city_outline_index.push_back(5);
	city_infos[5].city_outline_index.push_back(6);
	city_infos[5].city_outline_index.push_back(4);//
	city_infos[5].city_outline_index.push_back(6);
	city_infos[5].city_outline_index.push_back(7);
	////Enshi", "G"
	city_infos[6].city_outline.push_back(std::make_pair(221, 195));
	city_infos[6].city_outline.push_back(std::make_pair(139, 224));
	city_infos[6].city_outline.push_back(std::make_pair(170, 285));
	city_infos[6].city_outline.push_back(std::make_pair(104, 321));
	city_infos[6].city_outline.push_back(std::make_pair(42, 320));
	city_infos[6].city_outline.push_back(std::make_pair(27, 337));
	city_infos[6].city_outline.push_back(std::make_pair(36, 397));
	city_infos[6].city_outline.push_back(std::make_pair(92, 466));
	city_infos[6].city_outline.push_back(std::make_pair(136, 402));
	city_infos[6].city_outline.push_back(std::make_pair(204, 408));
	city_infos[6].city_outline.push_back(std::make_pair(190, 373));
	city_infos[6].city_outline.push_back(std::make_pair(194, 244));
	city_infos[6].city_outline.push_back(std::make_pair(234, 220));//done

	city_infos[6].city_outline_index.push_back(0);//
	city_infos[6].city_outline_index.push_back(1);
	city_infos[6].city_outline_index.push_back(12);
	city_infos[6].city_outline_index.push_back(1);//
	city_infos[6].city_outline_index.push_back(12);
	city_infos[6].city_outline_index.push_back(11);
	city_infos[6].city_outline_index.push_back(1);//
	city_infos[6].city_outline_index.push_back(2);
	city_infos[6].city_outline_index.push_back(11);
	city_infos[6].city_outline_index.push_back(2);//
	city_infos[6].city_outline_index.push_back(10);
	city_infos[6].city_outline_index.push_back(11);
	city_infos[6].city_outline_index.push_back(2);//
	city_infos[6].city_outline_index.push_back(3);
	city_infos[6].city_outline_index.push_back(10);
	city_infos[6].city_outline_index.push_back(3);//
	city_infos[6].city_outline_index.push_back(4);
	city_infos[6].city_outline_index.push_back(5);
	city_infos[6].city_outline_index.push_back(3);//
	city_infos[6].city_outline_index.push_back(5);
	city_infos[6].city_outline_index.push_back(6);
	city_infos[6].city_outline_index.push_back(3);//
	city_infos[6].city_outline_index.push_back(6);
	city_infos[6].city_outline_index.push_back(7);
	city_infos[6].city_outline_index.push_back(3);//
	city_infos[6].city_outline_index.push_back(8);
	city_infos[6].city_outline_index.push_back(7);
	city_infos[6].city_outline_index.push_back(3);//
	city_infos[6].city_outline_index.push_back(8);
	city_infos[6].city_outline_index.push_back(10);
	city_infos[6].city_outline_index.push_back(8);//
	city_infos[6].city_outline_index.push_back(9);
	city_infos[6].city_outline_index.push_back(10);
	city_infos[6].city_outline_index.push_back(8);//
	city_infos[6].city_outline_index.push_back(9);
	city_infos[6].city_outline_index.push_back(10);
	////Yichang", "
	city_infos[7].city_outline.push_back(std::make_pair(204, 408));
	city_infos[7].city_outline.push_back(std::make_pair(190, 373));
	city_infos[7].city_outline.push_back(std::make_pair(194, 244));
	city_infos[7].city_outline.push_back(std::make_pair(234, 220));
	city_infos[7].city_outline.push_back(std::make_pair(310, 244));
	city_infos[7].city_outline.push_back(std::make_pair(326, 314));
	city_infos[7].city_outline.push_back(std::make_pair(262, 374));//done

	city_infos[7].city_outline_index.push_back(0);//
	city_infos[7].city_outline_index.push_back(1);
	city_infos[7].city_outline_index.push_back(6);
	city_infos[7].city_outline_index.push_back(1);//
	city_infos[7].city_outline_index.push_back(2);
	city_infos[7].city_outline_index.push_back(6);
	city_infos[7].city_outline_index.push_back(2);//
	city_infos[7].city_outline_index.push_back(3);
	city_infos[7].city_outline_index.push_back(6);
	city_infos[7].city_outline_index.push_back(3);//
	city_infos[7].city_outline_index.push_back(4);
	city_infos[7].city_outline_index.push_back(6);
	city_infos[7].city_outline_index.push_back(4);//
	city_infos[7].city_outline_index.push_back(5);
	city_infos[7].city_outline_index.push_back(6);
	////Xiantao, etc
	city_infos[8].city_outline.push_back(std::make_pair(429, 283));
	city_infos[8].city_outline.push_back(std::make_pair(381, 293));
	city_infos[8].city_outline.push_back(std::make_pair(365, 332));
	city_infos[8].city_outline.push_back(std::make_pair(386, 360));
	city_infos[8].city_outline.push_back(std::make_pair(472, 346));
	city_infos[8].city_outline.push_back(std::make_pair(461, 325));
	city_infos[8].city_outline.push_back(std::make_pair(439, 324));//done

	city_infos[8].city_outline_index.push_back(0);//
	city_infos[8].city_outline_index.push_back(1);
	city_infos[8].city_outline_index.push_back(6);
	city_infos[8].city_outline_index.push_back(1);//
	city_infos[8].city_outline_index.push_back(2);
	city_infos[8].city_outline_index.push_back(6);
	city_infos[8].city_outline_index.push_back(2);//
	city_infos[8].city_outline_index.push_back(3);
	city_infos[8].city_outline_index.push_back(6);
	city_infos[8].city_outline_index.push_back(3);//
	city_infos[8].city_outline_index.push_back(4);
	city_infos[8].city_outline_index.push_back(6);
	city_infos[8].city_outline_index.push_back(4);//
	city_infos[8].city_outline_index.push_back(5);
	city_infos[8].city_outline_index.push_back(6);
	//Wuhan
	city_infos[9].city_outline.push_back(std::make_pair(478, 287));
	city_infos[9].city_outline.push_back(std::make_pair(461, 325));
	city_infos[9].city_outline.push_back(std::make_pair(472, 346));
	city_infos[9].city_outline.push_back(std::make_pair(532, 365));
	city_infos[9].city_outline.push_back(std::make_pair(554, 316));
	city_infos[9].city_outline.push_back(std::make_pair(572, 272));
	city_infos[9].city_outline.push_back(std::make_pair(554, 250));
	city_infos[9].city_outline.push_back(std::make_pair(523, 234));
	city_infos[9].city_outline.push_back(std::make_pair(498, 242));
	city_infos[9].city_outline.push_back(std::make_pair(498, 283));//done

	city_infos[9].city_outline_index.push_back(0);//
	city_infos[9].city_outline_index.push_back(1);
	city_infos[9].city_outline_index.push_back(2);
	city_infos[9].city_outline_index.push_back(0);//
	city_infos[9].city_outline_index.push_back(2);
	city_infos[9].city_outline_index.push_back(3);
	city_infos[9].city_outline_index.push_back(0);//
	city_infos[9].city_outline_index.push_back(3);
	city_infos[9].city_outline_index.push_back(4);
	city_infos[9].city_outline_index.push_back(0);//
	city_infos[9].city_outline_index.push_back(4);
	city_infos[9].city_outline_index.push_back(9);
	city_infos[9].city_outline_index.push_back(9);//
	city_infos[9].city_outline_index.push_back(4);
	city_infos[9].city_outline_index.push_back(5);
	city_infos[9].city_outline_index.push_back(9);//
	city_infos[9].city_outline_index.push_back(5);
	city_infos[9].city_outline_index.push_back(6);
	city_infos[9].city_outline_index.push_back(9);//
	city_infos[9].city_outline_index.push_back(6);
	city_infos[9].city_outline_index.push_back(7);
	city_infos[9].city_outline_index.push_back(9);//
	city_infos[9].city_outline_index.push_back(7);
	city_infos[9].city_outline_index.push_back(8);
	////Jingzhou
	city_infos[10].city_outline.push_back(std::make_pair(419, 424));
	city_infos[10].city_outline.push_back(std::make_pair(262, 374));
	city_infos[10].city_outline.push_back(std::make_pair(326, 314));
	city_infos[10].city_outline.push_back(std::make_pair(365, 332));
	city_infos[10].city_outline.push_back(std::make_pair(386, 360));
	city_infos[10].city_outline.push_back(std::make_pair(472, 346));
	city_infos[10].city_outline.push_back(std::make_pair(485, 362));//done

	city_infos[10].city_outline_index.push_back(0);//
	city_infos[10].city_outline_index.push_back(1);
	city_infos[10].city_outline_index.push_back(4);
	city_infos[10].city_outline_index.push_back(1);//
	city_infos[10].city_outline_index.push_back(2);
	city_infos[10].city_outline_index.push_back(3);
	city_infos[10].city_outline_index.push_back(1);//
	city_infos[10].city_outline_index.push_back(3);
	city_infos[10].city_outline_index.push_back(4);
	city_infos[10].city_outline_index.push_back(4);//
	city_infos[10].city_outline_index.push_back(5);
	city_infos[10].city_outline_index.push_back(6);
	city_infos[10].city_outline_index.push_back(4);//
	city_infos[10].city_outline_index.push_back(6);
	city_infos[10].city_outline_index.push_back(0);
	////Xianning
	city_infos[11].city_outline.push_back(std::make_pair(472, 346));
	city_infos[11].city_outline.push_back(std::make_pair(485, 362));
	city_infos[11].city_outline.push_back(std::make_pair(451, 401));
	city_infos[11].city_outline.push_back(std::make_pair(451, 439));
	city_infos[11].city_outline.push_back(std::make_pair(476, 470));
	city_infos[11].city_outline.push_back(std::make_pair(577, 416));
	city_infos[11].city_outline.push_back(std::make_pair(532, 365));//done

	city_infos[11].city_outline_index.push_back(0);//
	city_infos[11].city_outline_index.push_back(1);
	city_infos[11].city_outline_index.push_back(6);
	city_infos[11].city_outline_index.push_back(1);//
	city_infos[11].city_outline_index.push_back(2);
	city_infos[11].city_outline_index.push_back(6);
	city_infos[11].city_outline_index.push_back(2);//
	city_infos[11].city_outline_index.push_back(3);
	city_infos[11].city_outline_index.push_back(6);
	city_infos[11].city_outline_index.push_back(3);//
	city_infos[11].city_outline_index.push_back(4);
	city_infos[11].city_outline_index.push_back(6);
	city_infos[11].city_outline_index.push_back(4);//
	city_infos[11].city_outline_index.push_back(5);
	city_infos[11].city_outline_index.push_back(6);
	////Huangshi", 
	city_infos[12].city_outline.push_back(std::make_pair(577, 416));
	city_infos[12].city_outline.push_back(std::make_pair(532, 365));
	city_infos[12].city_outline.push_back(std::make_pair(554, 316));
	city_infos[12].city_outline.push_back(std::make_pair(618, 379));//done
	city_infos[12].city_outline_index.push_back(0);//
	city_infos[12].city_outline_index.push_back(1);
	city_infos[12].city_outline_index.push_back(2);
	city_infos[12].city_outline_index.push_back(0);//
	city_infos[12].city_outline_index.push_back(2);
	city_infos[12].city_outline_index.push_back(3);
}