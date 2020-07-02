/**
 * @file graphing.cpp
 * @author Oisin O'Connell
 * @date 16 Jun 2020
 * @brief graphing the wavelengths and intensities of the photon pre- and post-
 * collision to visualize the compton shift
 *
 * @todo move headers out
 */

#include <gnuplot.h>
#include <cmath>
#include <graphing.hpp>
#include <sstream>



void graph_compton_shift(long double lambda_prime,
			 long double lambda_naught,
			 long double e_naught,
			 long double e_prime)
{
	system("killall gnuplot > /dev/null 2>&1");
	GnuplotPipe gp;
	
	long double b_value_naught = 2 * M_PI / lambda_naught;
	long double b_value_prime = 2 * M_PI / lambda_prime;
	
	gp.sendLine("set xlabel \"Wavelength (meters)\"");
	gp.sendLine("set ylabel \"Energy (joules)\"");

	std::ostringstream range_sstr;
	range_sstr << "set xrange [0:" << lambda_naught * 2 << "]";
	gp.sendLine(range_sstr.str());
	
	std::cout << "rangestr : " << range_sstr.str() << '\n';

	std::ostringstream photon_naught_sstr;
	photon_naught_sstr << "plot "
			   << e_naught
			   << " * cos("
			   << b_value_naught
			   << "*x) title \"Incident photon\"";

	std::cout << "sstr1: "<< photon_naught_sstr.str() << '\n';
	std::ostringstream photon_prime_sstr;
	photon_prime_sstr << "replot "
			  << e_prime
			  << " * cos("
			  << b_value_prime
			  << "*x) title \"Deflected photon\"";

	std::cout << "sstr2: " << photon_prime_sstr.str() << '\n';
		
	gp.sendLine(photon_naught_sstr.str());
	gp.sendLine(photon_prime_sstr.str());
}
