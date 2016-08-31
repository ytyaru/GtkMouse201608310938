#include <gtk-2.0/gtk/gtk.h>
#include <cairo.h>
#include <glib.h>

gint ConvertFromCp932ToUtf8(gchar* cp932, gchar** utf8)
{
	GError* error = NULL;
	*utf8 = g_convert(cp932, -1, "utf-8", "cp932", NULL, NULL, &error);
	if (utf8 == NULL) {
		g_print("%s\n", error->message);
		gint retval = error->code;
		g_error_free(error);
		g_free(utf8);
		return retval;
	}
	return 0;
}
gboolean Draw(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t *cr;
	cr = gdk_cairo_create(widget->window);
	
	const int kFontHeight = 18;
	const int kLineMargine = 8;
	cairo_select_font_face (cr, "Serif", CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, kFontHeight);
	
	gchar* utf8_1 = NULL;
	gchar* utf8_2 = NULL;
	gchar* utf8_3 = NULL;
	ConvertFromCp932ToUtf8("1.ウインドウでマウスを動かす", &utf8_1);
	ConvertFromCp932ToUtf8("2.コンソールを見る", &utf8_2);
	ConvertFromCp932ToUtf8("3.マウス操作に応じたイベントログが出ている", &utf8_3);

	cairo_move_to(cr, 0, 30+((kFontHeight + kLineMargine)*0));
	cairo_show_text(cr, utf8_1);
	cairo_move_to(cr, 0, 30+((kFontHeight + kLineMargine)*1));
	cairo_show_text(cr, utf8_2);
	cairo_move_to(cr, 0, 30+((kFontHeight + kLineMargine)*2));
	cairo_show_text(cr, utf8_3);

	g_free(utf8_1);
	g_free(utf8_2);
	g_free(utf8_3);

	cairo_destroy(cr);
	return FALSE;
}
gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	g_print ("delete_event\n");
	return FALSE; // windowが"delete_event"により破棄される
	//return TRUE; // windowが"delete_event"により破棄されない
}
void destroy(GtkWidget *widget, gpointer data)
{
	g_print ("destroy\n");
	gtk_main_quit ();
}
gboolean button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	switch(event->button)
	{
	case 1: // 左ボタン
		g_print("button_press_event left (%f,%f)\n", event->x, event->y);
		break;
	case 2: // 中央ボタン
		g_print("button_press_event center (%f,%f)\n", event->x, event->y);
		break;
	case 3: // 右ボタン
		g_print("button_press_event right (%f,%f)\n", event->x, event->y);
		break;
	case 4: // XButton1
		g_print("button_press_event XButton1 (%f,%f)\n", event->x, event->y);
		break;
	case 5: // XButton2
		g_print("button_press_event XButton2 (%f,%f)\n", event->x, event->y);
		break;
	}
	return TRUE;
}
gboolean button_release_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	switch(event->button)
	{
	case 1: // 左ボタン
		g_print("button_release_event left (%f,%f)\n", event->x, event->y);
		break;
	case 2: // 中央ボタン
		g_print("button_release_event center (%f,%f)\n", event->x, event->y);
		break;
	case 3: // 右ボタン
		g_print("button_release_event right (%f,%f)\n", event->x, event->y);
		break;
	case 4: // XButton1
		g_print("button_release_event XButton1 (%f,%f)\n", event->x, event->y);
		break;
	case 5: // XButton2
		g_print("button_release_event XButton2 (%f,%f)\n", event->x, event->y);
		break;
	}
	return TRUE;
}
gint motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
	g_print("motion_notify_event (%f,%f)\n", event->x, event->y);
	return TRUE;
}
gboolean enter_notify_event (GtkWidget *widget, GdkEventCrossing *event, gpointer user_data)
{
	g_print("enter_notify_event (%f,%f)\n", event->x, event->y);
	return TRUE;
}
gboolean leave_notify_event (GtkWidget *widget, GdkEventCrossing *event, gpointer user_data)
{
	g_print("leave_notify_event (%f,%f)\n", event->x, event->y);
	return TRUE;
}
gboolean scroll_event(GtkWidget *widget, GdkEventScroll *event, gpointer data)
{
	switch(event->direction){
	case GDK_SCROLL_UP:
		printf("scroll_event GDK_SCROLL_UP\n");
		break;
	case GDK_SCROLL_DOWN:
		printf("scroll_event GDK_SCROLL_DOWN\n");
		break;
	// https://bugs.launchpad.net/inkscape/+bug/1200993
	case GDK_SCROLL_LEFT: // MW_MOUSEHWHEEL WindowsVista以降から使える
		printf("scroll_event GDK_SCROLL_LEFT\n");
		break;
	case GDK_SCROLL_RIGHT: // MW_MOUSEHWHEEL WindowsVista以降から使える
		printf("scroll_event GDK_SCROLL_RIGHT\n");
		break;
	default:
		break;
	}
	return TRUE;
}
int main(int argc, char* argv[])
{
	gtk_set_locale();
	gtk_init(&argc, &argv);

	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "GTK+ Mouse Events");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_signal_connect (GTK_OBJECT (window), "delete_event",
						GTK_SIGNAL_FUNC (delete_event), NULL);
	gtk_signal_connect (GTK_OBJECT (window), "destroy",
						GTK_SIGNAL_FUNC (destroy), NULL);

	GtkWidget *drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (drawing_area, 400, 300);
	gtk_container_add(GTK_CONTAINER(window), drawing_area);
	g_signal_connect(G_OBJECT(drawing_area), "expose_event", G_CALLBACK(Draw), NULL);
	
	gtk_signal_connect (GTK_OBJECT (drawing_area), "button_press_event",
		(GtkSignalFunc) button_press_event, NULL);
	gtk_signal_connect (GTK_OBJECT (drawing_area), "button_release_event",
		(GtkSignalFunc) button_release_event, NULL);
	gtk_signal_connect (GTK_OBJECT (drawing_area), "motion_notify_event",
		(GtkSignalFunc) motion_notify_event, NULL);
	gtk_signal_connect (GTK_OBJECT (drawing_area), "enter_notify_event",
		(GtkSignalFunc) enter_notify_event, NULL);
	gtk_signal_connect (GTK_OBJECT (drawing_area), "leave_notify_event",
		(GtkSignalFunc) leave_notify_event, NULL);
	gtk_signal_connect (GTK_OBJECT (drawing_area), "scroll_event",
		(GtkSignalFunc) scroll_event, NULL);

	gtk_widget_set_events(drawing_area,
		GDK_BUTTON_PRESS_MASK |
		GDK_BUTTON_RELEASE_MASK |
		GDK_POINTER_MOTION_MASK |
		GDK_ENTER_NOTIFY_MASK |
		GDK_LEAVE_NOTIFY_MASK | 
		GDK_SCROLL_MASK
	);
	
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}