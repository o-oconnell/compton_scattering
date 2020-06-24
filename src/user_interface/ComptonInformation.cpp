#include <ComptonInformation.hpp>

/**
 * @brief Creates the GTK window and adds the text and image to it
 */
void create_information_window()
{
	GtkWidget *outer_box;
	GtkWidget *compton_image;
	GtkWidget *blurb;
	GtkWidget *close_button;
	gtk_init(0, NULL);

	// window setup
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (win),
			     basename("Compton Scattering Information"));
	gtk_window_set_default_size(GTK_WINDOW (win), 554, 377);
	gtk_container_set_border_width(GTK_CONTAINER (win), 10);
	g_signal_connect(G_OBJECT (win), "destroy",
			 G_CALLBACK (gtk_main_quit), NULL);

	// create the outermost box that we pack everything else into
	outer_box =  gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

	// create and pack the image
	compton_image = gtk_image_new_from_file("include/compton-scattering-final-border.png");
	gtk_box_pack_start(GTK_BOX(outer_box), compton_image, TRUE, TRUE, 0);

	blurb = gtk_label_new("This program models the Compton effect, Arthur Compton\'s discovery\n that the collision of a photon with a charged particle leads to scattering of\n both particles and a decrease in the photon\'s wavelength. Compton showed\n that conservation of energy could be applied to a photon (which has no mass but still\n has energy due to its momentum) to find the relationship between the pre-collision\n photon wavelength (λ), post-collision photon wavelength (λ'),\n and scattering angle (θ). Compton used the relativistic form of Einstein's equation\n E = mc^2 = sqrt(p^2c^2 + m^2c^4) to find the equation λ' =  λ + (1-cosθ) * h / mc.\nClick \"Go to the program\" to calculate the relevant values from a photon-electron collision");
	gtk_box_pack_start(GTK_BOX(outer_box), blurb, TRUE, TRUE, 0);
	
	// create and pack the close button
	close_button = gtk_button_new_with_label("Go to the program");
	g_signal_connect(G_OBJECT(close_button), "clicked",
			 G_CALLBACK(close_button_clicked),
			 win);
	gtk_box_pack_start(GTK_BOX(outer_box), close_button, TRUE, TRUE, 0);
	
	gtk_container_add(GTK_CONTAINER (win), outer_box);
	gtk_widget_show_all(win);
	gtk_main();
}

/**
 * @brief Closes the window when the "Go to the program" button is clicked
 * @param close_button the GTK button widget
 * @param data the GTK window widget
 */
void close_button_clicked(GtkWidget *close_button, gpointer data)
{
	gtk_window_close(GTK_WINDOW(data));
}
