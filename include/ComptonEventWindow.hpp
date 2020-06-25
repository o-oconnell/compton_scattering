#ifndef COMPTON_EVENT_WINDOW_H
#define COMPTON_EVENT_WINDOW_H

#include <gtk/gtk.h>
#include <ComptonEvent.hpp>
#include <graphing.hpp>
#include <sstream>
#include <iomanip>
#include <gnuplot.h>
#include <iostream>

struct result_labels {
	GtkWidget *theta;
	GtkWidget *lambda;
	GtkWidget *lambda_prime;
	GtkWidget *photon_energy_naught;
	GtkWidget *photon_energy_prime;
	GtkWidget *photon_momentum_naught;
	GtkWidget *photon_momentum_prime;
	GtkWidget *electron_energy;
	GtkWidget *electron_velocity;
	GtkWidget *electron_momentum;
	GtkWidget *electron_scatter_angle;
};

struct args {
	GtkWidget *theta_val;
	GtkWidget *lambda_val;
	struct result_labels *results;
};


/* Callbacks */
/**
 * @brief updates the theta text box whenever the scale is updated
 * @param scale the theta slider
 * @param theta_entry the label for the theta slider
 */
void scale_updated(GtkRange *scale, GtkLabel *theta_entry);

/**
 * @brief prevents the entry of any non-numeric characters for theta
 * @param scale the text box
 * @param theta_entry the text that was added to the text box
 * @param length the length of the added text
 * @param pos the length of the added text
 */
void insert_theta_event(GtkEditable *theta_entry, const gchar *text, gint length, gint *pos);

/**
 * @brief prevents the entry of any non-numeric characters for lambda
 * @param scale the text box
 * @param theta_entry the text that was added to the text box
 * @param length the length of the added text
 * @param pos the length of the added text
 */
void insert_lambda_event(GtkEditable *theta_entry, const gchar *text,
			 gint length, gint *pos);

/**
 * @brief passes the inputted args to the graphing function
 * @param button the input button
 * @param multi_arg struct containing the result struct along with the 
 * lambda and theta text field entries
 */
void submit_clicked(GtkWidget *button, struct args *multi_arg);

/**
 * @brief creates all of the result labels for the calculation results
 * @param outer_box the outer GTK box containing all of the boxes
 * @param result box the GTK box containing all of the results
 * @param results a struct containing all of the result label widgets
 */
void create_result_labels(GtkWidget *outer_box,
			  GtkWidget *result_box,
			  struct result_labels *results);

/**
 * @brief updates the result labels whenever the submit button is clicked
 * @param results all of the result label widgets in a struct
 * @param event the ComptonEvent which we can query to get the results
 */
void set_result_labels(struct result_labels *results, ComptonEvent event);

/** 
 * @brief creates the window that allows user input to generate calculations
 */
void create_calculation_window();


#endif
