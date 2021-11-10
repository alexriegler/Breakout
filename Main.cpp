#include "Breakout.h"

#include <iostream>

int main()
{
	try
	{
		ar::Breakout breakout{};
		breakout.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}