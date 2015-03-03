#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <stdlib.h>
#include <algorithm>
#include "process.h"




int main(int argc, char** argv)
{
	if( argc !=3 )
	{
		std::cout << "2 file paths needed" << std::endl;
		return 0;
	}
	else{
		double time1 = time(NULL);
		process pr;
		//std::cout << argc << std::endl;
		pr.trimmer( argv );
		double time2 = time(NULL);
		std::cout << time2 - time1 << std::endl;
		return 0;

	}
}