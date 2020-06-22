/**
 * @file ComptonEvent.hpp
 * @author Oisin O'Connell
 * @date 10 Jun 2020
 * @brief ComptonEvent class declaration. This class will contain all of the
 * member function declarations
 * and parameters that we need to calculate numbers for the
 * graphical part of the application.
 *
 *
 * @todo Update for relativistic velocities (>10% speed of light)
 */

#ifndef COMPTON_EVENT_H
#define COMPTON_EVENT_H

#include <iostream>
#include <cmath>

// container used to return necessary values when graphing compton
// shift
struct ComptonGraphValues {
	long double E_photon_naught;
	long double E_photon_prime;
	long double lambda_naught;
	long double lambda_prime;
};

// contains ALL of the values that we calculate, used to update the result
// on the UI
struct ComptonResultValues {
	long double theta;
	long double lambda_naught;
	long double lambda_prime;
	long double photon_energy_naught;
	long double photon_energy_prime;
	long double photon_momentum_naught;
	long double photon_momentum_prime;
	long double electron_energy;
	long double electron_velocity;
	long double electron_momentum;
	long double electron_scatter_angle;
};

class ComptonEvent {
private:
	long double theta;
  
	struct Photon {
		// pre and post collision wavelength
		long double lambda_naught = 9E-12; // (9 picometers in meters)
		long double lambda_prime; 

		// momentum of the photon before and after the collision
		// since the photon transfers momentum to the electron due to
		// conservation of momentum
		long double p_photon_naught;
		long double p_photon_prime;

		// energy of the photon pre- and post-collision
		long double E_photon;
		long double E_photon_prime;
	} photon;
  
	struct Electron {
		// energy of the electron post-collision
		long double E_sub_e;

		// electron momentum post-collision
		long double p_sub_e;

		// post collision velocity
		long double velocity;

		// direction of the electron after the collision
		long double phi;

		// post collision momentum
		long double momentum;
    
	} electron;	
public:
	ComptonEvent(long double theta, long double lambda_naught) :
		theta{theta}
	{
		photon.lambda_naught = picometersToMeters(lambda_naught);
		setTheta(theta / (180 / M_PI));
		setLambdaPrime();
		setPhotonEnergy();
		setPhotonMomentum();
		setElectronEnergy();
		setElectronVelocity();
		setElectronMomentum();
		setElectronScatterAnglePhi();
	}


	ComptonResultValues getResults();
	long double picometersToMeters(long double val);
	void setTheta(long double theta);
	void setLambdaPrime();
	void setPhotonMomentum();
	void setPhotonEnergy();
	void setElectronEnergy();
	void setElectronVelocity();
	void setElectronMomentum();
	void setElectronScatterAnglePhi();
	long double getThetaInDegrees();
	ComptonGraphValues getComptonGraphValues();
};

#endif
