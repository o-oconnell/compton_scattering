
/**
 * @file input.cpp
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
 * g++ `pkg-config --cflags gtk+-3.0` input.cpp -o input `pkg-config --libs gtk+-3.0`

 */

#include <gtk/gtk.h>

int main()
{
	GtkWidget *win;
	GtkWidget *scale;
	GtkAdjustment *adjustment;

	gtk_init(0, NULL);

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW (win),
			     basename("Compton Scattering Simulator"));
	
	//gtk_widget_set_size_request(win, WINWIDTH, WINHEIGHT);

	gtk_container_set_border_width(GTK_CONTAINER (win), 20);
	
	g_signal_connect(G_OBJECT (win), "destroy",
			 G_CALLBACK (gtk_main_quit), NULL);

	adjustment = gtk_adjustment_new(50.0, 0.0, 100.0, 0.1, 10.0, 0.0);
	scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);

	gtk_scale_set_digits (GTK_SCALE (scale), 2);

	gtk_scale_set_value_pos(GTK_SCALE (scale), GTK_POS_TOP);
	gtk_scale_set_draw_value(GTK_SCALE (scale), TRUE);
	gtk_container_add(GTK_CONTAINER (win), scale);
	gtk_widget_show_all(win);
	gtk_main();
}
