#include <iostream>
#include "mask_simulation.h"
#include <windows.h>
#include <math.h>
using namespace std;

int main()
{
    _SYS *sys1;
    sys1 = (_SYS *)malloc(sizeof(_SYS));
    initialization(sys1);
    float delta_t=0.01;
    float total_t=0;
    while(total_t<24*2){//30 days
        sir_renew(sys1,delta_t);
        parameter_renew(sys1);
        mask_require(sys1);
        mask_change(sys1,(int)(total_t*1/delta_t)%(int)(12*1/delta_t),delta_t);//transport every 12 hours
        total_t+=delta_t;


        Sleep(5);

    if((int)(total_t*1/delta_t)%(int)(1/delta_t)==0)
        cout<<"hour"<<(int)((total_t*1/delta_t)/(1/delta_t))<<" "<<round(sys1->city[1]->sus_num)<<" "<<round(sys1->city[1]->inf_num)<<" "<<sys1->city[1]->total_num-round(sys1->city[1]->inf_num)-round(sys1->city[1]->sus_num)<<"\n";

    }
}
