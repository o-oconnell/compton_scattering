/**
 * @file main.cpp
 * @author Oisin O'Connell
 * @date 25 Jun 2020
 * @brief main program, calls the function that creates the informational
 * window followed by the function that creates the calculation window
 * 
 */

#include <ComptonEventWindow.hpp>
#include <ComptonInformation.hpp>

int main()
{
	create_information_window();
	create_calculation_window();
}
