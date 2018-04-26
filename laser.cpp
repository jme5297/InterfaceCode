#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv)
{
    while(1)
    {
        usleep(100000);

	    int digit = -1;
	    ifstream laser;
	std::string loc = "/sys/class/gpio/gpio";
	loc.append(argv[1]);
	loc.append("/value");
	    laser.open(loc);

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
	        continue;
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
    
    return 0;
}



