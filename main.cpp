#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "process.h"




int main(int argc, char** argv)
{
	if( argc !=3 )
	{
		std::cout << "2 file paths needed" << std::endl;
		return 0;
	}
	else{
		process pr;
		pr.trimmer( argv );
		return 0;

	}
}