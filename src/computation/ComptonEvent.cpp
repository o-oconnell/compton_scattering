/**
 * @file ComptonEvent.cpp
 * @author Oisin O'Connell
 * @date 10 Jun 2020
 * @brief ComptonEvent class member function definitions
 * 
 * @todo Update for relativistic velocities (>10% speed of light),
 * move function definitions into this file (done)
 * move main to a central control program, and make
 * a dir dedicated to testing
 */

#include <ComptonEvent.hpp>
#include <graphing.hpp>
#include <globals.hpp>

/** 
 * @brief Creates and returns a struct containing all of the ComptonEvent's
 * values. 
 * @return a ComptonResultValues struct.
 */
ComptonResultValues ComptonEvent::getResults()
{
	ComptonResultValues result = {
				      getThetaInDegrees(),
				      photon.lambda_naught,
				      photon.lambda_prime,
				      photon.E_photon,
				      photon.E_photon_prime,
				      photon.p_photon_naught,
				      photon.p_photon_prime,
				      electron.E_sub_e,
				      electron.velocity,
				      electron.momentum,
				      electron.phi
	};
	return result;
}

/** 
 * @brief converts the theta value to degrees since sin() in c takes radians.
 * @return the value in degrees.
 */
long double ComptonEvent::getThetaInDegrees()
{
	return theta * (180 / M_PI);
}

/** 
 * @brief converts picometers to meters since we take an incident ray value
 * in picometers
 * @return the value in meters.
 */
long double ComptonEvent::picometersToMeters(long double val)
{
	return val * pow(10, -12);
}

/** 
 * @brief set compton event's phi value (angle (deg) between scattered photon
 * and insonant direction of photon)
 */
void ComptonEvent::setTheta(long double theta)
{
	this->theta = theta;
}

/** 
 * @brief set photon wavelength (meters) post collision
 */
void ComptonEvent::setLambdaPrime()
{
	photon.lambda_prime =
		photon.lambda_naught +
		PLANCK_CONSTANT / (M_NAUGHT * SPEED_OF_LIGHT) *
		(1 - cos(theta));
	
	std::cout << "Set lambda prime (post-collision wavelength) to: "
		  << photon.lambda_prime
		  << " meters" << '\n';
}

/** 
 * @brief set photon momentum (kg * m/s) 
 * pre and post collision, p_naught = h / lambda
 * naught, p_prime = h / lambda prime
 */
void ComptonEvent::setPhotonMomentum()
{
	// using de Broglie equation: p = h / lambda
	photon.p_photon_naught = PLANCK_CONSTANT / photon.lambda_naught;
	photon.p_photon_prime = PLANCK_CONSTANT / photon.lambda_prime;

	std::cout << "Pre-collision photon momentum (kg * m/s) " <<
		photon.p_photon_naught << '\n';
	std::cout << "Post-collision photon momentum (kg * m/s) " << photon.p_photon_prime << '\n';
    
}

/** 
 * @brief set photon energy (joules) pre and post collision, 
 * E_photon = hc / lambda,
 * E_photon_prime = hc / lambda prime
 */
void ComptonEvent::setPhotonEnergy()
{
	// E = hc / lambda
	photon.E_photon =
		PLANCK_CONSTANT * SPEED_OF_LIGHT / photon.lambda_naught;
    
	photon.E_photon_prime =
		PLANCK_CONSTANT * SPEED_OF_LIGHT / photon.lambda_prime;

	std::cout << "Pre-collision photon energy (joules) = "
		  << photon.E_photon << '\n';
	std::cout << "Post-collision photon energy (joules) = "
		  << photon.E_photon_prime << '\n';
}

/** 
 * @brief set electron energy (joules)
 * , E_e = photon energy pre collision - photon
 * energy post collision
 */
void ComptonEvent::setElectronEnergy()
{
	this->electron.E_sub_e = photon.E_photon - photon.E_photon_prime;
	std::cout << "Electron kinetic energy (joules) = " << electron.E_sub_e 
	      << '\n';
}

/** 
 * @brief set electron velocity (m/s), v = sqrt(2KE / electron mass)
 */
void ComptonEvent::setElectronVelocity()
{
	electron.velocity = sqrt(2 * electron.E_sub_e / M_NAUGHT);
	std::cout << "Electron velocity (m/s): " << electron.velocity << '\n';
}  

/** 
 * @brief set electron momentum, p = electron mass * electron velocity
 */
void ComptonEvent::setElectronMomentum()
{
	electron.momentum = M_NAUGHT * electron.velocity;
	
	std::cout << "Electron momentum (kg * m/s): " << electron.momentum
		  << '\n';
}

/** @brief essentially here we just set the momentum of the photon in the y 
 * direction and the momentum of the electron in the y direction equal to each
 * other and solve for phi since momentum of photon in y direction = momentum  * sin(theta) and momentum of electron in y direction = electron momentum * 
 * sin(theta) we solve the equation photon momentum * sin(theta) = electron 
 * momentum * sin(theta) and since we are given theta, we can solve for phi
 */
void ComptonEvent::setElectronScatterAnglePhi()
{
	electron.phi = asin(photon.p_photon_prime * sin(theta)
			      / electron.momentum) * 180 / M_PI;
	
	std::cout << "Set electron scatter angle (phi) to "
		  << electron.phi << '\n';
}

ComptonGraphValues ComptonEvent::getComptonGraphValues()
{
	ComptonGraphValues ComptonGraphContainer =
		{
		 photon.E_photon,
		 photon.E_photon_prime,
		 photon.lambda_naught,
		 photon.lambda_prime
		};

	return ComptonGraphContainer;
}

// int main()
// {
// 	long double theta = 0.0;
// 	std::cout << "Enter the scatter angle (theta)\n";
// 	std::cin >> theta;

// 	// test cases go here for now, our "main" will later just be in
// 	// the graphical section of this project
// 	ComptonEvent c{theta};
	
// 	ComptonGraphValues graph_vals = c.getComptonGraphValues();
  
// 	graph_compton_shift(graph_vals.lambda_prime,
// 			    graph_vals.lambda_naught,
// 			    graph_vals.E_photon_naught,
// 			    graph_vals.E_photon_prime);
// }

