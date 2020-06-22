/**
 * @file ComptonEventWindow.cpp
 * @author Oisin O'Connell
 * @date 15 Jun 2020
 * @brief gtk user input management
 
 * @todo this file will contain the GTK code that allows us to
 * have multiple threads displaying information based on the input we get from
 * the user - likely one thread will accept input from the slider and
 * get a struct containing the calculated result, while another thread
 * (later on) will display a graph of the lamda-prime wavelength
 * (or perhaps something more interesting)

 * Make sure you have gtk+-3.0 installed, then you can compile with:
 * g++ `pkg-config --cflags gtk+-3.0` ComptonEventWindow.cpp -o input `pkg-config --libs gtk+-3.0`

 */

#include <ComptonEventWindow.hpp>
#include <sstream>
#include <iomanip>
#include <gnuplot.h>

namespace ComptonEventValues {
	long double theta;
	long double lambda_prime;
};

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
void scale_updated(GtkRange *scale, GtkLabel *theta_entry);
void insert_theta_event(GtkEditable *theta_entry, const gchar *text, gint length, gint *pos);
void insert_lambda_event(GtkEditable *theta_entry, const gchar *text,
			 gint length, gint *pos);
void submit_clicked(GtkWidget *button, struct args *multi_arg);

/* Create the result labels */
void create_result_labels(GtkWidget *outer_box,
			  GtkWidget *result_box,
			  struct result_labels *results);

/* Function that updates all of the results whenever a new set of values
 * is entered. Called by the submit_clicked callback. */
void set_result_labels(struct result_labels *results, ComptonEvent event);

void create_calculation_window()
{
	GtkWidget *win;
	GtkWidget *scale, *theta_label;
	GtkWidget *outer_box, *scale_info_box, *theta_entry_box,
		*lambda_entry_box;
	GtkWidget *theta_entry, *theta_entry_label;
	GtkWidget *lambda_entry, *lambda_entry_label;
	GtkWidget *submit;
	GtkAdjustment *adjustment;
	GtkWidget *title_description;
	
	gtk_init(0, NULL);

	// window setup
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (win),
			     basename("Compton Scattering Simulator"));
	gtk_window_set_default_size(GTK_WINDOW (win), 1000, 100);
	gtk_container_set_border_width(GTK_CONTAINER (win), 10);
	g_signal_connect(G_OBJECT (win), "destroy",
			 G_CALLBACK (gtk_main_quit), NULL);

	// create the outermost box that we pack everything else into
	outer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

	// add the title for the data entry section
	GtkWidget *data_entry_title = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(data_entry_title), "<b>Data entry: </b>");	
	gtk_label_set_xalign(GTK_LABEL(data_entry_title), 0.0);
	gtk_box_pack_start(GTK_BOX(outer_box), data_entry_title, TRUE, TRUE, 0);
	
	// add the usage description
	title_description = gtk_text_view_new();
	title_description = gtk_label_new ("Use the text box or the slider to change the value of the scatter angle (theta). Enter the wavelength of the incident photon (lambda) using the second\n text box.");
	gtk_label_set_xalign(GTK_LABEL(title_description), 0.0f);
	gtk_box_pack_start(GTK_BOX(outer_box), title_description, TRUE, TRUE, 0);
	
	// create the scale and the label for it
	adjustment = gtk_adjustment_new(180.0, 0.0, 360.0, 0.1, 10.0, 0.0);
	scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
	gtk_scale_set_digits (GTK_SCALE (scale), 2);
	gtk_scale_set_value_pos(GTK_SCALE (scale), GTK_POS_TOP);
	gtk_scale_set_draw_value(GTK_SCALE (scale), TRUE);
	theta_label = gtk_label_new("Adjust the slider to set the scatter angle (theta).");
	scale_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_pack_start(GTK_BOX(scale_info_box), theta_label, FALSE,
			   FALSE, 0);	
	gtk_box_pack_start(GTK_BOX(scale_info_box), scale, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(outer_box), scale_info_box, TRUE, TRUE, 0);

	// create the alternate textbox usable for theta entry and its label
	theta_entry = gtk_entry_new();
	theta_entry_label = gtk_label_new("Alternatively, enter the scatter angle manually:");
	gtk_entry_set_text(GTK_ENTRY(theta_entry), "180.0");
	gtk_widget_set_halign(theta_entry, GTK_ALIGN_START);
	theta_entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_pack_start(GTK_BOX(theta_entry_box), theta_entry_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(theta_entry_box), theta_entry, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(outer_box), theta_entry_box, TRUE, TRUE, 0);
	
	// create the text box for lambda entry and its label
	lambda_entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	lambda_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(lambda_entry), "10");
	lambda_entry_label = gtk_label_new("Enter the pre-collision photon wavelength (lambda) (picometers):");
	gtk_widget_set_halign(lambda_entry, GTK_ALIGN_START);
	
	gtk_box_pack_start(GTK_BOX(lambda_entry_box), lambda_entry_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(lambda_entry_box), lambda_entry, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(outer_box), lambda_entry_box, TRUE, TRUE, 0);

	
	// create the submit button
	submit = gtk_button_new_with_label("Submit");
	gtk_box_pack_start(GTK_BOX(outer_box), submit, TRUE, TRUE, 10);
	

	// add the title for the results section
	GtkWidget *results_title = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(results_title), "<b>Results: </b>");
	gtk_label_set_xalign(GTK_LABEL(results_title), 0.0);
	gtk_box_pack_start(GTK_BOX(outer_box), results_title, TRUE, TRUE, 0);


	// create the result labels, which we will use to output our findings
	GtkWidget *result_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	struct result_labels *results = g_new0(struct result_labels, 1);
	create_result_labels(outer_box, result_box, results);
	
	// pass functions to be called (callbacks) when new values are entered
	g_signal_connect(G_OBJECT(scale), "value-changed",
			 G_CALLBACK (scale_updated),
			 (gpointer) theta_entry);
	
	g_signal_connect(G_OBJECT(theta_entry), "insert-text",
			 G_CALLBACK(insert_theta_event), NULL);
	
	g_signal_connect(G_OBJECT(lambda_entry), "insert-text",
			 G_CALLBACK(insert_lambda_event), NULL);

	// need to have a makeshift struct here to pass both of the args
	// to the submit function
	struct args *multi_arg = g_new0(struct args, 1);
	multi_arg->lambda_val = lambda_entry;
	multi_arg->theta_val = theta_entry;
	multi_arg->results = results;
	g_signal_connect(G_OBJECT(submit), "clicked",
			 G_CALLBACK(submit_clicked),
			 multi_arg);
	
	gtk_container_add(GTK_CONTAINER (win), outer_box);
	gtk_widget_show_all(win);
	gtk_main();
}

void submit_clicked(GtkWidget *button, struct args *multi_arg)
{
	GtkEntryBuffer *theta_buf = gtk_entry_get_buffer
		(GTK_ENTRY(multi_arg->theta_val));
	GtkEntryBuffer *lambda_buf = gtk_entry_get_buffer
		(GTK_ENTRY(multi_arg->lambda_val));
	
	const gchar *theta = gtk_entry_buffer_get_text(theta_buf);
	const gchar *lambda = gtk_entry_buffer_get_text(lambda_buf);

	ComptonEvent c{atof(theta), atof(lambda)};
	ComptonGraphValues graph_vals = c.getComptonGraphValues();

	graph_compton_shift(graph_vals.lambda_prime,
			    graph_vals.lambda_naught,
			    graph_vals.E_photon_naught,
			    graph_vals.E_photon_prime);
	
	set_result_labels(multi_arg->results, c);
}

void insert_lambda_event(GtkEditable *theta_entry, const gchar *text,
			gint length, gint *pos)
{
	for (int i = 0; i < length; ++i) {
		if(text[i] == '.')
			continue;
		if (!isdigit(text[i])) {
			g_signal_stop_emission_by_name(G_OBJECT(theta_entry),
						       "insert-text");
			return;
		}
	}
}

void insert_theta_event(GtkEditable *theta_entry, const gchar *text,
			gint length, gint *pos)
{
	for (int i = 0; i < length; ++i) {
		if(text[i] == '.')
			continue;
		if (!isdigit(text[i])) {
			g_signal_stop_emission_by_name(G_OBJECT(theta_entry),
						       "insert-text");
			return;
		}
	}
}

void scale_updated(GtkRange *scale, GtkLabel *theta_entry)
{
	gtk_entry_set_text(GTK_ENTRY(theta_entry),
			   std::to_string(gtk_range_get_value(scale)).c_str());
	ComptonEventValues::theta = gtk_range_get_value(scale);
}

/* Create the result labels */
void create_result_labels(GtkWidget *outer_box,
			  GtkWidget *result_box,
			  struct result_labels *results)
{
	results->theta = gtk_label_new("Scattering angle (theta): 0.0");
	results->lambda =
		gtk_label_new("Lambda naught (pre-collision wavelength): 0.0");
	results->lambda_prime =
		gtk_label_new("Lambda prime (post-collision wavelength): 0.0");
	results->photon_energy_naught =
		gtk_label_new("Pre-collision photon energy (joules): 0.0");
	results->photon_energy_prime =
		gtk_label_new("Post-collision photon energy (joules): 0.0");
	results->photon_momentum_naught =
		gtk_label_new("Pre-collision photon momentum (kg * m/s): 0.0");
	results->photon_momentum_prime =
		gtk_label_new("Post-collision photon momentum (kg * m/s): 0.0");
	results->electron_energy =
		gtk_label_new("Electron kinetic energy (joules): 0.0");
	results->electron_velocity =
		gtk_label_new("Electron velocity (m/s): 0.0");
	results->electron_momentum =
		gtk_label_new("Electron momentum (kg * m/s): 0.0");
	results->electron_scatter_angle =
		gtk_label_new("Electron scatter angle (phi): 0.0");

	gtk_label_set_xalign(GTK_LABEL(results->theta), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->lambda), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->lambda_prime), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->photon_energy_naught), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->photon_energy_prime), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->photon_momentum_naught), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->photon_momentum_prime), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->electron_energy), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->electron_velocity), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->electron_momentum), 0.0);
	gtk_label_set_xalign(GTK_LABEL(results->electron_scatter_angle), 0.0);
									
	gtk_box_pack_start(GTK_BOX(result_box), results->theta, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->lambda, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->lambda_prime, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->photon_energy_naught, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->photon_energy_prime, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->photon_momentum_naught, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->photon_momentum_prime, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->electron_energy, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->electron_velocity, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->electron_momentum, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(result_box), results->electron_scatter_angle, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(outer_box), result_box, FALSE, FALSE, 0);
}

/* Function that updates all of the results whenever a new set of values
 * is entered. Called by the submit_clicked callback. */
void set_result_labels(struct result_labels *results, ComptonEvent event)
{
	ComptonResultValues eventResult = event.getResults();
 
	std::stringstream theta_result;
	theta_result << "Scattering angle (theta): "
		     << std::setprecision(5)
		     << eventResult.theta;
	
	gtk_label_set_text(GTK_LABEL(results->theta), theta_result.str().c_str());

	std::stringstream lambda_naught_result;
        lambda_naught_result << "Lambda naught (pre-collision wavelength): "
			     << std::setprecision(5)
			     << std::scientific
			     << eventResult.lambda_naught;
	
	gtk_label_set_text(GTK_LABEL(results->lambda), lambda_naught_result.str().c_str());

	std::stringstream lambda_prime_result;
        lambda_prime_result << "Lambda prime (post-collision wavelength): "
			     << std::setprecision(5)
			     << std::scientific
			     << eventResult.lambda_prime;
	
	gtk_label_set_text(GTK_LABEL(results->lambda_prime), lambda_prime_result.str().c_str());

	std::stringstream photon_energy_naught_result;
        photon_energy_naught_result << "Pre-collision photon energy (joules): "
			     << std::setprecision(5)
			     << std::scientific
			     << eventResult.photon_energy_naught;

	gtk_label_set_text(GTK_LABEL(results->photon_energy_naught), photon_energy_naught_result.str().c_str());
	
	std::stringstream photon_energy_prime_result;
        photon_energy_prime_result << "Post-collision photon energy (joules): "
			     << std::setprecision(5)
			     << std::scientific
			     << eventResult.photon_energy_prime;

	gtk_label_set_text(GTK_LABEL(results->photon_energy_prime), photon_energy_prime_result.str().c_str());

	std::stringstream photon_momentum_naught_result;
	photon_momentum_naught_result << "Pre-collision photon momentum (kg * m/s): "
				      << std::setprecision(5)
				      << std::scientific
				      << eventResult.photon_momentum_naught;

	gtk_label_set_text(GTK_LABEL(results->photon_momentum_naught), photon_momentum_naught_result.str().c_str());

	std::stringstream photon_momentum_prime_result;
	photon_momentum_prime_result << "Post-collision photon momentum (kg * m/s): "
				      << std::setprecision(5)
				      << std::scientific
				      << eventResult.photon_momentum_prime;
	
	gtk_label_set_text(GTK_LABEL(results->photon_momentum_prime), photon_momentum_prime_result.str().c_str());
	
	std::stringstream electron_energy_result;
	electron_energy_result << "Electron kinetic energy (joules): "
			       << std::setprecision(5)
			       << std::scientific
			       << eventResult.electron_energy;

	gtk_label_set_text(GTK_LABEL(results->electron_energy), electron_energy_result.str().c_str());

	std::stringstream electron_velocity_result;
	electron_velocity_result << "Electron velocity (m/s): "
			       << std::setprecision(5)
			       << std::scientific
			       << eventResult.electron_velocity;
	
	gtk_label_set_text(GTK_LABEL(results->electron_velocity), electron_velocity_result.str().c_str());

	std::stringstream electron_momentum_result;
	electron_momentum_result << "Electron momentum (kg * m/s): "
				 << std::setprecision(5)
				 << std::scientific
				 << eventResult.electron_momentum;
	
	gtk_label_set_text(GTK_LABEL(results->electron_momentum), electron_momentum_result.str().c_str());

	std::stringstream electron_scatter_angle_result;
	electron_scatter_angle_result << "Electron scatter angle (phi): "
				 << std::setprecision(5)
				 << std::scientific
				 << eventResult.electron_scatter_angle;
	
	gtk_label_set_text(GTK_LABEL(results->electron_scatter_angle), electron_scatter_angle_result.str().c_str());
}
