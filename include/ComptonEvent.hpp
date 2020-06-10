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
#include <globals.hpp>

class ComptonEvent {

private:

  long double phi;
  
  struct Photon {
    // pre and post collision wavelength
    const long double lambda_naught = 2E-11; // (9 picometers in meters)
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
    long double theta;

    // post collision momentum
    long double momentum;
    
  } electron;
  
public:

  ComptonEvent(long double phi) : phi{phi}
  {
    setPhi(phi / (180 / M_PI));
    setLambdaPrime();
    setPhotonEnergy();
    setPhotonMomentum();
    setElectronEnergy();
    setElectronVelocity();
    setElectronMomentum();
    setElectronScatterAngleTheta();
  }
  
  void setPhi(long double phi)
  {
    this->phi = phi;
  }
  
  void setLambdaPrime()
  {
    photon.lambda_prime =
      photon.lambda_naught +
      PLANCK_CONSTANT / (M_NAUGHT * SPEED_OF_LIGHT) *
      (1 - cos(phi));

    std::cout << "set lambda prime to: " << photon.lambda_prime
	      << " meters" << '\n';
  }

  void setPhotonMomentum()
  {
    // using de Broglie equation: p = h / lambda
    photon.p_photon_naught = PLANCK_CONSTANT / photon.lambda_naught;
    photon.p_photon_prime = PLANCK_CONSTANT / photon.lambda_prime;

    std::cout << "original photon momentum (kg * m/s) " <<
      photon.p_photon_naught << '\n';
    std::cout << "post-collision photon momentum (kg * m/s) " << photon.p_photon_prime << '\n';
    
  }

  void setPhotonEnergy()
  {
    // E = hc / lambda
    photon.E_photon =
      PLANCK_CONSTANT * SPEED_OF_LIGHT / photon.lambda_naught;
    
    photon.E_photon_prime =
      PLANCK_CONSTANT * SPEED_OF_LIGHT / photon.lambda_prime;

    std::cout << "E photon naught (joules) = " << photon.E_photon << '\n';
    std::cout << "E photon prime (joules) = " << photon.E_photon_prime << '\n';
  }

  void setElectronEnergy()
  {
    this->electron.E_sub_e = photon.E_photon - photon.E_photon_prime;
    std::cout << "electron kinetic energy (joules) is " << electron.E_sub_e 
	      << '\n';
  }

  void setElectronVelocity()
  {
    electron.velocity = sqrt(2 * electron.E_sub_e / M_NAUGHT);
    std::cout << "electron velocity: " << electron.velocity << " m/s" << '\n';
  }  
  
  void setElectronMomentum()
  {
    electron.momentum = M_NAUGHT * electron.velocity;

    std::cout << "electron momentum: " << electron.momentum << " kg * m / s" <<
      '\n';
  }

  void setElectronScatterAngleTheta()
  {
    // essentially here we just set the momentum of the photon
    // in the y direction and the momentum of the electron in the y direction
    // equal to each other and solve for theta

    // since momentum of photon in y direction = momentum * sin(phi)
    // momentum of electron in y direction = electron momentum * sin(theta)

    // we solve the equation photon momentum * sin(phi) = electron momentum
    // * sin(theta)
    // and since we are given phi, we can solve for theta

    electron.theta = asin(photon.p_photon_prime * sin(phi)
			  / electron.momentum) * 180 / M_PI;

    std::cout << "set electron scatter angle (theta) to "
	      << electron.theta << '\n';
  }
  
};

#endif
