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

/** 
 * @brief set compton event's phi value (angle (deg) between scattered photon
 * and insonant direction of photon)
 */
void ComptonEvent::setPhi(long double phi)
{
	this->phi = phi;
}

/** 
 * @brief set photon wavelength (meters) post collision
 */
void ComptonEvent::setLambdaPrime()
{
	photon.lambda_prime =
		photon.lambda_naught +
		PLANCK_CONSTANT / (M_NAUGHT * SPEED_OF_LIGHT) *
		(1 - cos(phi));
	
	std::cout << "set lambda prime to: " << photon.lambda_prime
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

	std::cout << "original photon momentum (kg * m/s) " <<
		photon.p_photon_naught << '\n';
	std::cout << "post-collision photon momentum (kg * m/s) " << photon.p_photon_prime << '\n';
    
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

	std::cout << "E photon naught (joules) = " << photon.E_photon << '\n';
	std::cout << "E photon prime (joules) = " << photon.E_photon_prime << '\n';
}

/** 
 * @brief set electron energy (joules)
 * , E_e = photon energy pre collision - photon
 * energy post collision
 */
void ComptonEvent::setElectronEnergy()
{
	this->electron.E_sub_e = photon.E_photon - photon.E_photon_prime;
	std::cout << "electron kinetic energy (joules) is " << electron.E_sub_e 
	      << '\n';
}

/** 
 * @brief set electron velocity (m/s), v = sqrt(2KE / electron mass)
 */
void ComptonEvent::setElectronVelocity()
{
	electron.velocity = sqrt(2 * electron.E_sub_e / M_NAUGHT);
	std::cout << "electron velocity: " << electron.velocity << " m/s" << '\n';
}  

/** 
 * @brief set electron momentum, p = electron mass * electron velocity
 */
void ComptonEvent::setElectronMomentum()
{
	electron.momentum = M_NAUGHT * electron.velocity;
	
	std::cout << "electron momentum: " << electron.momentum <<
		" kg * m / s" << '\n';
}

/** @brief essentially here we just set the momentum of the photon in the y 
 * direction and the momentum of the electron in the y direction equal to each
 * other and solve for theta since momentum of photon in y direction = momentum  * sin(phi) momentum of electron in y direction = electron momentum * 
 * sin(theta) we solve the equation photon momentum * sin(phi) = electron 
 * momentum * sin(theta) and since we are given phi, we can solve for theta
 */
void ComptonEvent::setElectronScatterAngleTheta()
{
	
	electron.theta = asin(photon.p_photon_prime * sin(phi)
			      / electron.momentum) * 180 / M_PI;
	
	std::cout << "set electron scatter angle (theta) to "
		  << electron.theta << '\n';
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

int main()
{
  // test cases go here for now, our "main" will later just be in
  // the graphical section of this project
  ComptonEvent c{30};

  ComptonGraphValues graph_vals = c.getComptonGraphValues();

  graph_compton_shift(graph_vals.lambda_prime,
		      graph_vals.lambda_naught,
		      graph_vals.E_photon_naught,
		      graph_vals.E_photon_prime);
}

