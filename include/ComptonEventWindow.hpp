#ifndef COMPTON_EVENT_WINDOW_H
#define COMPTON_EVENT_WINDOW_H

#include <gtk/gtk.h>
#include <ComptonEvent.hpp>
#include <graphing.hpp>

void scale_updated(GtkRange *scale, GtkLabel *theta_label);
void create_calculation_window();

#endif
