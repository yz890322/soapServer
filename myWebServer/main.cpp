#include <iostream>
#include "mainclass.h"
#include "packdata.h"

using namespace std;

int main()
{
    MainClass *p = new MainClass;
    while(1)
    {
        Sleep(1000);
    }
    delete p;
    p = NULL;
    cout << "webService end!" << endl;
    return 0;
}


