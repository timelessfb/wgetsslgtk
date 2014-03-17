#include <gtk/gtk.h>
#include "wgetssl.h"
#include <stdlib.h>
#include <unistd.h>

#define URLMINSIZE 3

struct widgetpntrs {
    GtkWidget *widgetEntryUrl;
    GtkWidget *widgetEntryFileName;
    GtkWidget *widgetLabelStatus;
    GtkWidget *d;
};

static void button_clicked(GtkWidget *widget, gpointer data)
{
    struct widgetpntrs *tempStructForFunction = malloc(sizeof(struct widgetpntrs));
    tempStructForFunction = data;
    int a = (int)gtk_entry_get_text_length(
                GTK_ENTRY(tempStructForFunction->widgetEntryUrl));

    //Insane part of Errorchecking
    if(a > URLMINSIZE) {
        if(getFileFromSSLUrl(
                    (char *)gtk_entry_get_text(
                        GTK_ENTRY(tempStructForFunction->widgetEntryUrl)),
                    (char *)gtk_entry_get_text(
                        GTK_ENTRY(tempStructForFunction->widgetEntryFileName))
                ) == 0) {
            gtk_label_set_text(
                GTK_LABEL(tempStructForFunction->widgetLabelStatus),
                "Status: file written");

        } else {
            gtk_label_set_text(
                GTK_LABEL(tempStructForFunction->widgetLabelStatus),
                "Status: cannot get file");
        }

    } else {
        gtk_label_set_text(
            GTK_LABEL(tempStructForFunction->widgetLabelStatus),
            "Status: please enter URL and Filename to write");
    }

    sleep(1);
}

int main(int argc, char *argv[])
{
    GtkWidget *window,
              *label,
              *labelURL,
              *labelFilename,
              *button,
              *hbox,
              *urlEntryField,
              *entryfile,
              *statustext;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    urlEntryField = gtk_entry_new();
    entryfile = gtk_entry_new();
    label = gtk_label_new("wgetssl");
    labelURL = gtk_label_new("URL:");
    labelFilename = gtk_label_new("Filename to write:");
    statustext = gtk_label_new("Status:");
    button = gtk_button_new_with_mnemonic("get!");
    hbox = gtk_vbox_new(0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), label, 1, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button, 1, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), labelURL, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), urlEntryField, 1, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), labelFilename, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), entryfile, 1, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), statustext, 1, 0, 0);
    gtk_container_add(GTK_CONTAINER(window), hbox);
    gtk_widget_set_size_request(window, 400, 400);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    //set up widgetEntryUrl struct for GtkWidgetPointers to submit to button function
    struct  widgetpntrs *newWidgetPntrsStruct;
    newWidgetPntrsStruct = malloc(sizeof(struct widgetpntrs));
    newWidgetPntrsStruct->widgetEntryUrl = urlEntryField;
    newWidgetPntrsStruct->widgetEntryFileName = entryfile;
    newWidgetPntrsStruct->widgetLabelStatus = statustext;
    newWidgetPntrsStruct->d = NULL;
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked),
                     (gpointer)newWidgetPntrsStruct);
    gtk_widget_show_all(window);
    gtk_window_move(GTK_WINDOW(window), 200, 200);
    gtk_main();
    return 0;
}
