#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

/*** Defines ***/

#define I_LEFT_CHILD(i) (2*(i)+1)
#define I_PARENT(i) (((i)-1)/2)

/*** Global variables ***/

static int *data = NULL;
static int num_elements = 50;
static int comparisons = 0;
static int swaps = 0;
// Global stop flag: when set, the current sort should interrupt
static gboolean stop_requested = FALSE;

// Global indices for currently compared data; -1 means none
static int current_compare1 = -1, current_compare2 = -1;

// --- Global Widget Pointers ---
static GtkWidget *drawing_area = NULL;
static GtkWidget *label_comparisons = NULL;
static GtkWidget *label_swaps = NULL;
static GtkWidget *combo_algorithm = NULL;
static GtkWidget *combo_data_length = NULL;
static GtkWidget *speed_slider = NULL;
static GtkWidget *button_randomize = NULL;  // for disabling during sort

/*** Utility functions ***/

void swap(int* x, int* y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void shuffleArr(int arr[], size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() % (n - i);
            swap(&arr[i], &arr[j]);
        }
    }
}

void update_counters() {
    char buf[64];
    sprintf(buf, "Comparisons: %d", comparisons);
    gtk_label_set_text(GTK_LABEL(label_comparisons), buf);
    sprintf(buf, "Swaps: %d", swaps);
    gtk_label_set_text(GTK_LABEL(label_swaps), buf);
}

// This function queues a redraw, processes pending events (allowing Stop button clicks),
// and then sleeps for a short delay (based on the slider).
void update_visualization() {
    gtk_widget_queue_draw(drawing_area);
    while (gtk_events_pending())
        gtk_main_iteration();
    gdouble delay = gtk_range_get_value(GTK_RANGE(speed_slider));
    g_usleep((guint)(delay * 1000));
    if (stop_requested)
        return;
}

// --- Visualization: Draw the Data as Vertical Bars ---
// Bars with indices equal to current_compare1 or current_compare2 are drawn in red
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data_unused) {
    if (!data)
        return FALSE;
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    int width = allocation.width, height = allocation.height;
    
    // Clear background
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    
    double bar_width = (double)width / num_elements;
    
    // Find maximum value for scaling
    int max_val = 0;
    for (int i = 0; i < num_elements; i++) {
        if (data[i] > max_val)
            max_val = data[i];
    }
    if (max_val == 0)
        max_val = 1;
    
    for (int i = 0; i < num_elements; i++) {
        double bar_height = ((double)data[i] / max_val) * (height - 20);
        // Highlight in red if currently compared
        if (i == current_compare1 || i == current_compare2)
            cairo_set_source_rgb(cr, 1, 0, 0);
        else
            cairo_set_source_rgb(cr, 0.2, 0.6, 0.8);
        double x = i * bar_width;
        double y = height - bar_height;
        cairo_rectangle(cr, x, y, bar_width - 2, bar_height);
        cairo_fill(cr);
    }
    return FALSE;
}

// --- Data Initialization ---
void randomize_data() {
    // Get chosen data length from dropdown
    gchar *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_data_length));
    if (text) {
        num_elements = atoi(text);
        g_free(text);
    } else {
        num_elements = 50;
    }
    
    if (data)
        free(data);
    data = malloc(num_elements * sizeof(int));
    if (!data) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    // Fill array with
    for (int i = 0; i < num_elements; i++) {
        data[i] = i + 1;
    }
    srand(time(NULL));
    shuffleArr(data, num_elements);
    
    comparisons = 0;
    swaps = 0;
    current_compare1 = -1;
    current_compare2 = -1;
    update_counters();
    gtk_widget_queue_draw(drawing_area);
}

/*** Sorting algorithms ***/

// Bubble Sort
void bubbleSortAnimated(int arr[], size_t n) {
    size_t i, j;
    int swapped;
    for (i = 0; i < n - 1 && !stop_requested; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1 && !stop_requested; j++) {
            // Highlight the pair being compared
            current_compare1 = j;
            current_compare2 = j + 1;
            comparisons++;
            update_counters();
            update_visualization();
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swaps++;
                update_counters();
                update_visualization();
                swapped = 1;
            }
            if (stop_requested)
                return;
        }
    }
    current_compare1 = current_compare2 = -1;
}

// Selection Sort
void selectionSortAnimated(int arr[], size_t n) {
    size_t i, j, min;
    for (i = 0; i < n - 1 && !stop_requested; i++) {
        min = i;
        for (j = i + 1; j < n && !stop_requested; j++) {
            current_compare1 = j;
            current_compare2 = min;
            comparisons++;
            update_counters();
            update_visualization();
            if (arr[j] < arr[min])
                min = j;
            if (stop_requested)
                return;
        }
        if (min != i) {
            swap(&arr[min], &arr[i]);
            swaps++;
            update_counters();
            update_visualization();
        }
    }
    current_compare1 = current_compare2 = -1;
}

// Insertion Sort
void insertionSortAnimated(int arr[], size_t n) {
    size_t i, j;
    int x;
    for (i = 1; i < n && !stop_requested; i++) {
        x = arr[i];
        j = i;
        while (j > 0 && !stop_requested) {
            current_compare1 = j - 1;
            current_compare2 = j;
            comparisons++;
            update_counters();
            update_visualization();
            if (arr[j - 1] > x) {
                arr[j] = arr[j - 1];
                swaps++;
                update_counters();
                update_visualization();
                j--;
            } else {
                break;
            }
            if (stop_requested)
                return;
        }
        arr[j] = x;
        update_visualization();
    }
    current_compare1 = current_compare2 = -1;
}

// Quick Sort
void quickSortAnimated(int arr[], int low, int high) {
    if (stop_requested)
        return;
    if (low >= high || low < 0)
        return;
    int pivot = arr[high];
    int i = low;
    for (int j = low; j < high && !stop_requested; j++) {
        current_compare1 = j;
        current_compare2 = high; // comparing with pivot
        comparisons++;
        update_counters();
        update_visualization();
        if (arr[j] <= pivot) {
            swap(&arr[i], &arr[j]);
            swaps++;
            update_counters();
            update_visualization();
            i++;
        }
        if (stop_requested)
            return;
    }
    swap(&arr[i], &arr[high]);
    swaps++;
    update_counters();
    update_visualization();
    quickSortAnimated(arr, low, i - 1);
    quickSortAnimated(arr, i + 1, high);
    current_compare1 = current_compare2 = -1;
}

// Merge Sort
void mergeAnimated(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2 && !stop_requested) {
        current_compare1 = left + i;
        current_compare2 = mid + 1 + j;
        comparisons++;
        update_counters();
        update_visualization();
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        swaps++;
        update_counters();
        update_visualization();
        k++;
    }
    while (i < n1 && !stop_requested) {
        arr[k] = L[i];
        i++;
        k++;
        swaps++;
        update_counters();
        update_visualization();
    }
    while (j < n2 && !stop_requested) {
        arr[k] = R[j];
        j++;
        k++;
        swaps++;
        update_counters();
        update_visualization();
    }
    free(L);
    free(R);
    current_compare1 = current_compare2 = -1;
}

void mergeSortAnimated(int arr[], int left, int right) {
    if (stop_requested)
        return;
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortAnimated(arr, left, mid);
        mergeSortAnimated(arr, mid + 1, right);
        mergeAnimated(arr, left, mid, right);
    }
    current_compare1 = current_compare2 = -1;
}

// Heap Sort
void siftDownAnimated(int arr[], size_t root, size_t end) {
    while (I_LEFT_CHILD(root) < end && !stop_requested) {
        size_t child = I_LEFT_CHILD(root);
        if (child + 1 < end && arr[child] < arr[child + 1])
            child++;
        current_compare1 = root;
        current_compare2 = child;
        comparisons++;
        update_counters();
        update_visualization();
        if (arr[root] < arr[child]) {
            swap(&arr[root], &arr[child]);
            swaps++;
            update_counters();
            update_visualization();
            root = child;
        } else {
            return;
        }
    }
    current_compare1 = current_compare2 = -1;
}

void heapifyAnimated(int arr[], size_t count) {
    size_t start = I_PARENT(count - 1) + 1;
    while (start > 0 && !stop_requested) {
        start--;
        siftDownAnimated(arr, start, count);
    }
}

void heapSortAnimated(int arr[], size_t count) {
    heapifyAnimated(arr, count);
    size_t end = count;
    while (end > 0 && !stop_requested) {
        end--;
        swap(&arr[end], &arr[0]);
        swaps++;
        update_counters();
        update_visualization();
        siftDownAnimated(arr, 0, end);
    }
    current_compare1 = current_compare2 = -1;
}

// Stalin Sort
int stalinSortAnimated(int arr[], size_t n) {
    if (n == 0)
        return 0;
    size_t j = 1;
    for (size_t i = 1; i < n && !stop_requested; i++) {
        current_compare1 = j - 1;
        current_compare2 = i;
        comparisons++;
        update_counters();
        update_visualization();
        if (arr[i] >= arr[j - 1]) {
            arr[j] = arr[i];
            j++;
        } else {
            arr[i] = 0;
        }
        swaps++;
        update_counters();
        update_visualization();
    }
    current_compare1 = current_compare2 = -1;
    return j;
}

// To check if bogo is sorted
int isSorted(int arr[], size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (arr[i - 1] >= arr[i])
            return 0;
    }
    return 1;
}

// Bogo Sort
void bogoSortAnimated(int arr[], size_t n) {
    while (!isSorted(arr, n) && !stop_requested) {
        current_compare1 = current_compare2 = -1;
        shuffleArr(arr, n);
        swaps++;
        update_counters();
        update_visualization();
        if (stop_requested)
            return;
    }
    current_compare1 = current_compare2 = -1;
}

/*** Callbacks ***/

// Play: Reset the stop flag and controls, then run the selected algorithm
static void on_play_clicked(GtkButton *button, gpointer user_data) {
    // Disable controls while sorting
    gtk_widget_set_sensitive(combo_algorithm, FALSE);
    gtk_widget_set_sensitive(combo_data_length, FALSE);
    gtk_widget_set_sensitive(button_randomize, FALSE);
    
    // Reset stop flag and counters
    stop_requested = FALSE;
    comparisons = 0;
    swaps = 0;
    update_counters();
    
    // Reinitialize data so the algorithm starts from scratch
    randomize_data();
    
    gchar *algo = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_algorithm));
    if (!algo)
        return;
    
    if (strcmp(algo, "Bubble Sort") == 0) {
        bubbleSortAnimated(data, num_elements);
    } else if (strcmp(algo, "Selection Sort") == 0) {
        selectionSortAnimated(data, num_elements);
    } else if (strcmp(algo, "Insertion Sort") == 0) {
        insertionSortAnimated(data, num_elements);
    } else if (strcmp(algo, "Quick Sort") == 0) {
        quickSortAnimated(data, 0, num_elements - 1);
    } else if (strcmp(algo, "Merge Sort") == 0) {
        mergeSortAnimated(data, 0, num_elements - 1);
    } else if (strcmp(algo, "Heap Sort") == 0) {
        heapSortAnimated(data, num_elements);
    } else if (strcmp(algo, "Stalin Sort") == 0) {
        stalinSortAnimated(data, num_elements);
    } else if (strcmp(algo, "Bogo Sort") == 0) {
        bogoSortAnimated(data, num_elements);
    } else {
        g_print("Algorithm %s not implemented.\n", algo);
    }
    g_free(algo);
    
    // Final update before re-enabling controls
    gtk_widget_queue_draw(drawing_area);
    
    // Re-enable controls
    gtk_widget_set_sensitive(combo_algorithm, TRUE);
    gtk_widget_set_sensitive(combo_data_length, TRUE);
    gtk_widget_set_sensitive(button_randomize, TRUE);
}

// Stop: Set the stop flag so that the running algorithm aborts
static void on_stop_clicked(GtkButton *button, gpointer user_data) {
    stop_requested = TRUE;
}

// Randomize: Create a new random dataset
static void on_randomize_clicked(GtkButton *button, gpointer user_data) {
    randomize_data();
}

// --- Main ---
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sorting Visualization");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Main vertical box
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // --- Top Controls ---
    GtkWidget *hbox_top = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_top, FALSE, FALSE, 5);

    // Algorithm dropdown
    GtkWidget *label_algo = gtk_label_new("Algorithm:");
    gtk_box_pack_start(GTK_BOX(hbox_top), label_algo, FALSE, FALSE, 5);
    combo_algorithm = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algorithm), "Bubble Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algorithm), "Selection Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algorithm), "Insertion Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algorithm), "Quick Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algorithm), "Merge Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algorithm), "Heap Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algorithm), "Stalin Sort");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_algorithm), "Bogo Sort");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_algorithm), 0);
    gtk_box_pack_start(GTK_BOX(hbox_top), combo_algorithm, FALSE, FALSE, 5);

    // Speed slider
    GtkWidget *label_speed = gtk_label_new("Speed (ms):");
    gtk_box_pack_start(GTK_BOX(hbox_top), label_speed, FALSE, FALSE, 5);
    speed_slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 10, 500, 10);
    gtk_range_set_value(GTK_RANGE(speed_slider), 100);
    gtk_widget_set_size_request(speed_slider, 150, -1);
    gtk_box_pack_start(GTK_BOX(hbox_top), speed_slider, FALSE, FALSE, 5);

    // Data length dropdown
    GtkWidget *label_length = gtk_label_new("Data Length:");
    gtk_box_pack_start(GTK_BOX(hbox_top), label_length, FALSE, FALSE, 5);
    combo_data_length = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_data_length), "10");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_data_length), "50");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_data_length), "100");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_data_length), "200");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_data_length), 1);  // Default 50.
    gtk_box_pack_start(GTK_BOX(hbox_top), combo_data_length, FALSE, FALSE, 5);

    // Play button
    GtkWidget *button_play = gtk_button_new_with_label("Play");
    g_signal_connect(button_play, "clicked", G_CALLBACK(on_play_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_top), button_play, FALSE, FALSE, 5);

    // Stop button
    GtkWidget *button_stop = gtk_button_new_with_label("Stop");
    g_signal_connect(button_stop, "clicked", G_CALLBACK(on_stop_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_top), button_stop, FALSE, FALSE, 5);

    // Randomize button
    button_randomize = gtk_button_new_with_label("Randomize");
    g_signal_connect(button_randomize, "clicked", G_CALLBACK(on_randomize_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_top), button_randomize, FALSE, FALSE, 5);

    // --- Drawing Area ---
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 800, 400);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);

    // --- Bottom Status Bar ---
    GtkWidget *hbox_bottom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_bottom, FALSE, FALSE, 5);
    label_comparisons = gtk_label_new("Comparisons: 0");
    gtk_box_pack_start(GTK_BOX(hbox_bottom), label_comparisons, FALSE, FALSE, 5);
    label_swaps = gtk_label_new("Swaps: 0");
    gtk_box_pack_start(GTK_BOX(hbox_bottom), label_swaps, FALSE, FALSE, 5);

    // Initialize random data
    randomize_data();

    gtk_widget_show_all(window);
    gtk_main();

    if (data)
        free(data);
    return 0;
}
