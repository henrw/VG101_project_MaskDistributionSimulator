#include <iostream>
#include "mask_simulation.h"
#include <windows.h>
#include <windows.h>
using namespace std;

int main()
{
    _SYS *sys1;
    sys1 = (_SYS *)malloc(sizeof(_SYS));
    initialization(sys1);
    float delta_t=0.1;
    float total_t=0;
    while(total_t<100){
        sir_renew(sys1,delta_t);
        parameter_renew(sys1);
        mask_require(sys1);
        mask_change(sys1,((int)total_t*10)%5);
        total_t+=delta_t;
        cout<<sys1->city[0]->sus_num<<" "<<sys1->city[0]->inf_num<<" "<<sys1->city[0]->rec_num<<"\n";
        Sleep(100);

    }
}
