# Compton Scattering Project
A simple modeling program demonstrating Compton scattering and photon behavior as a particle of no mass that has momentum.

Uses GTK windows to provide a basic introduction to Compton scattering. Allows the user to graph different compton shifts by providing values for the angle between the incident photon and its direction after collision (theta) and the photon's initial wavelength (lambda).

Compile and run with:

$ make

$ ./compton_program


Depends: gnuplot-cpp (https://github.com/martinruenz/gnuplot-cpp), GTK+3.0, gnuplot

Files:
src/computation/ComptonEvent.cpp - contains the calculation functions for a collision event.
src/user_interface/ComptonEventWindow.cpp - contains the GTK code to display the window that allows calculation input.
src/user_interface/ComptonInformation.cpp - contains the GTK code to display a quick informational window with a blurb on Compton scattering.
src/main/main.cpp - calls the functions to display the informational window followed by the computation window.