#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to fetch and return CPU info
char* get_cpu_info() {
    static char cpu_info[256];
    FILE *cpuinfo=fopen("/proc/cpuinfo", "r");
    if (cpuinfo != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), cpuinfo)) {
            if (strncmp(line, "model name", 10) == 0) {
                strcpy(cpu_info, line); // Save CPU info in buffer
                break;
            }
        }
        fclose(cpuinfo);
    }
    return cpu_info;
}

// Function to fetch and return Memory info
char* get_memory_info() {
    static char memory_info[512];
    FILE *meminfo=fopen("/proc/meminfo", "r");
    if (meminfo != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), meminfo)) {
            if (strncmp(line, "MemTotal", 8) == 0 || strncmp(line, "MemFree", 7) == 0) {
                strcat(memory_info, line); // Append memory info to buffer
            }
        }
        fclose(meminfo);
    }
    return memory_info;
}

// Function to fetch and return uptime
char* get_uptime_info() {
    static char uptime_info[256];
    FILE *uptimefile=fopen("/proc/uptime", "r");
    if (uptimefile != NULL) {
        double uptime_seconds;
        fscanf(uptimefile, "%lf", &uptime_seconds);
        int days=uptime_seconds / 86400;
        int hours=(uptime_seconds / 3600) - (days * 24);
        int minutes=(uptime_seconds / 60) - (days * 1440) - (hours * 60);
        sprintf(uptime_info, "Uptime: %d days, %d hours, %d minutes\n", days, hours, minutes);
        fclose(uptimefile);
    }
    return uptime_info;
}

// Function to fetch and return kernel version
char* get_kernel_version() {
    static char kernel_version[256];
    FILE *version=fopen("/proc/version", "r");
    if (version != NULL) {
        fgets(kernel_version, sizeof(kernel_version), version);
        fclose(version);
    }
    return kernel_version;
}

// Function to display system information in the GUI
void on_show_info_clicked(GtkWidget *widget, gpointer window) {
    char info[1024]="";
    strcat(info, "=== CPU Information ===\n");
    strcat(info, get_cpu_info());
    strcat(info, "\n=== Memory Information ===\n");
    strcat(info, get_memory_info());
    strcat(info, "\n=== System Uptime ===\n");
    strcat(info, get_uptime_info());
    strcat(info, "\n=== Kernel Version ===\n");
    strcat(info, get_kernel_version());

    GtkWidget *dialog=gtk_message_dialog_new(GTK_WINDOW(window),
                        GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_INFO,
                        GTK_BUTTONS_OK,
                        "%s", info);
    gtk_window_set_title(GTK_WINDOW(dialog), "System Information");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Function for the "About" button to show the author information
void on_about_clicked(GtkWidget *widget, gpointer window) {
    GtkWidget *dialog=gtk_message_dialog_new(GTK_WINDOW(window),
                        GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_INFO,
                        GTK_BUTTONS_OK,
                        "Author: Jay Mee @ J~Net 2024\n");
    gtk_window_set_title(GTK_WINDOW(dialog), "About");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button_info, *button_about;

    gtk_init(&argc, &argv);

    // Window Setup
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "System Info Fetcher");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Grid Layout
    grid=gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Buttons
    button_info=gtk_button_new_with_label("Show System Info");
    button_about=gtk_button_new_with_label("About");

    // Add buttons to the grid
    gtk_grid_attach(GTK_GRID(grid), button_info, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_about, 0, 1, 1, 1);

    // Connect button signals to their respective functions
    g_signal_connect(button_info, "clicked", G_CALLBACK(on_show_info_clicked), window);
    g_signal_connect(button_about, "clicked", G_CALLBACK(on_about_clicked), window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

