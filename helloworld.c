#include <stdlib.h>                          // for calloc
#include <string.h>                          // for strlen
#include <gtk/gtk.h>                         // for gtk functionality

/* Global constants */
const char* message = "Pressed %d times.";
const int MAX_COUNT = 10000;

/* Function prototypes */
void update_text(GtkWidget *widget, gpointer data);

/**
 * main
 */
int main (int argc, char **argv) {
  // local variables
  GtkWidget* window;                         // main display window
  GtkWidget* vbox;                           // vertical alignment box
  GtkWidget* btn;                            // button to press
  GtkWidget* txt;                            // textview for display
  GtkTextBuffer* buf;                        // textview text buffer

  // initialize gui, pull out gtk args, etc...
  gtk_init(&argc, &argv);
 
  // create main window, set title, position, default size and borders
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Press Counter");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 250, 50);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);

  // call gtk_main_quit on delete-event or destroy
  g_signal_connect(G_OBJECT(window), "delete-event", gtk_main_quit, NULL);
  g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);

  // create the textview, make it uneditable and justify center
  txt = gtk_text_view_new();
  gtk_text_view_set_editable((GtkTextView*) txt, FALSE);
  gtk_text_view_set_justification((GtkTextView*) txt, GTK_JUSTIFY_CENTER);

  // initialize the textview text buffer
  buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (txt));
  gtk_text_buffer_set_text (buf, "Press the button!", -1);

  // create the button, give it a label, and connect a callback listener for clicks
  btn = gtk_button_new_with_label("Press me");
  gtk_signal_connect(GTK_OBJECT(btn), "clicked", GTK_SIGNAL_FUNC(update_text), (gpointer) txt);

  // create vbox for vertical alignemnt
  vbox = gtk_vbox_new(TRUE,1);

  // add vbox to window
  gtk_container_add(GTK_CONTAINER(window), vbox);

  // pack the textview and button into the vbox
  gtk_box_pack_start(GTK_BOX(vbox), txt, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), btn, TRUE, TRUE, 0);

  // display window and all its children
  gtk_widget_show_all(window);
 
  // begin the gtk main ui loop
  gtk_main();

  return 0;
}


/**
 * update_text - change the textview text on button press
 * Parameters:
 *    widget - the widget that was activated
 *    data   - the main textview
 */
void update_text(GtkWidget *widget, gpointer textview) {
  static int times_pressed = 0;              // notice static, so we keep a running total
  char* text;                                // text to add to textview
  GtkTextBuffer* buf;                        // current textview buffer

  // create text buffer, +4 for some padding and '\n'
  text = calloc(strlen(message)+4, sizeof(char));  
  if(!text) {
    g_printerr("Error: Could not allocate memory for text buffer.\n");
    return;
  }

  // retrieve the buffer of the textview
  buf = gtk_text_view_get_buffer((GtkTextView*) textview);
  if(!text || !buf) {
    g_printerr("Error: Could not allocate text or retrieve buffer.\n");
    return;
  }

  // print message to console
  g_print("Button was pressed\n");

  // check for too many times pressed (prevents text from being too large)
  if(++times_pressed > MAX_COUNT) {
    times_pressed = 1;
    g_printerr("Warning: Exceeding max pressed count and wrapping.\n");
  }

  // create new message to be printed
  sprintf(text, message, times_pressed);

  // set the textview text
  gtk_text_buffer_set_text(buf, text, strlen(text));

  // free the text buffer
  free(text);
}
