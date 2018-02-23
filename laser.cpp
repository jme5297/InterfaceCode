#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

inline void Detect()
{
    int digit = -1;
    ifstream laser;
    laser.open("/sys/class/gpio/gpio60/value");
    
    if (laser.is_open())
    {
        while (laser.good() )
        {
            laser >> digit;
        }
    }
    
   // laser.close();
    
    else 
    {
        cout << "Unable to read the data\n";
        return;
    }
    
    if (digit == 0) //obstacle detected
    {
        cout<<"Obstacle detected"<<endl;
    }
    else if (digit == 1) //no obstacle detect
    {
        cout<<" No obstacle detected"<<endl;
    }
}

int main()
{
    while(1)
    {
        usleep(1);
        Detect();
    }
    
    return 0;
}



