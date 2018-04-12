#include <iostream>
#include <string>

int main()
{
	
	std::string device = "/dev/video0";
	std::string command = "fswebcam --device " + device + " out.png";

	system(command.c_str());

	return 0;
}
