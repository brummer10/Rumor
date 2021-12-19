
#define CONTROLS 4

#define GUI_ELEMENTS 1

#define TAB_ELEMENTS 0


#define PLUGIN_UI_URI "https://github.com/brummer10/Rumor_UI"


#include "lv2_plugin.h"


#include "xresources.h"

void set_costum_theme(Xputty *main) {
    main->color_scheme->normal = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.850, 0.850, 0.850, 1.000},
        .bg =       { 0.121, 0.098, 0.141, 1.000},
        .base =     { 0.000, 0.000, 0.000, 1.000},
        .text =     { 0.900, 0.900, 0.900, 1.000},
        .shadow =   { 0.000, 0.000, 0.000, 0.200},
        .frame =    { 0.000, 0.000, 0.000, 1.000},
        .light =    { 0.100, 0.100, 0.100, 1.000}
    };

    main->color_scheme->prelight = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 1.000, 0.000, 1.000, 1.000},
        .bg =       { 0.250, 0.250, 0.250, 1.000},
        .base =     { 0.300, 0.300, 0.300, 1.000},
        .text =     { 1.000, 1.000, 1.000, 1.000},
        .shadow =   { 0.100, 0.100, 0.100, 0.400},
        .frame =    { 0.300, 0.300, 0.300, 1.000},
        .light =    { 0.300, 0.300, 0.300, 1.000}
    };

    main->color_scheme->selected = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.900, 0.900, 0.900, 1.000},
        .bg =       { 0.200, 0.200, 0.200, 1.000},
        .base =     { 0.500, 0.180, 0.180, 1.000},
        .text =     { 1.000, 1.000, 1.000, 1.000},
        .shadow =   { 0.800, 0.180, 0.180, 0.200},
        .frame =    { 0.500, 0.180, 0.180, 1.000},
        .light =    { 0.500, 0.180, 0.180, 1.000}
    };

    main->color_scheme->active = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.000, 1.000, 1.000, 1.000},
        .bg =       { 0.000, 0.000, 0.000, 1.000},
        .base =     { 0.180, 0.380, 0.380, 1.000},
        .text =     { 0.750, 0.750, 0.750, 1.000},
        .shadow =   { 0.180, 0.380, 0.380, 0.500},
        .frame =    { 0.180, 0.380, 0.380, 1.000},
        .light =    { 0.180, 0.380, 0.380, 1.000}
    };

    main->color_scheme->insensitive = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.850, 0.850, 0.850, 0.500},
        .bg =       { 0.100, 0.100, 0.100, 0.500},
        .base =     { 0.000, 0.000, 0.000, 0.500},
        .text =     { 0.900, 0.900, 0.900, 0.500},
        .shadow =   { 0.000, 0.000, 0.000, 0.100},
        .frame =    { 0.000, 0.000, 0.000, 0.500},
        .light =    { 0.100, 0.100, 0.100, 0.500}
    };
}

#include "lv2_plugin.cc"



void plugin_value_changed(X11_UI *ui, Widget_t *w, PortIndex index) {
    // do special stuff when needed
    if (index == 2) adj_set_value(ui->widget[3]->adj, adj_get_value(ui->widget[0]->adj));
}

void plugin_set_window_size(int *w,int *h,const char * plugin_uri, float scale) {
    (*w) = 300 * scale; //set initial width of main window
    (*h) = 344 * scale; //set initial height of main window
}

const char* plugin_set_name() {
    return "Rumor"; //set plugin name to display on UI
}

void plugin_create_controller_widgets(X11_UI *ui, const char * plugin_uri, float scale) {
    set_costum_theme(&ui->main);
    ui->elem[0] = add_lv2_image (ui->elem[0], ui->win, -1, "Image5", ui, 30,  30, 240 * scale, 283 * scale);
    widget_get_scaled_png(ui->elem[0], LDVAR(monster_png));
    ui->widget[0] = add_lv2_image_toggle (ui->widget[0], ui->elem[0], 2, "", ui, 90,  180, 60 * scale, 60 * scale);
    widget_get_png(ui->widget[0], LDVAR(footsw_png));

    ui->widget[1] = add_lv2_knob (ui->widget[1], ui->elem[0], 3, "intensity", ui, 0,  30, 91 * scale, 101 * scale);
    widget_get_png(ui->widget[1], LDVAR(fx_knob_png));
    set_adjustment(ui->widget[1]->adj, 0.000, 0.000, 0.000, 1.000, 0.010, CL_CONTINUOS);

    ui->widget[2] = add_lv2_knob (ui->widget[2], ui->elem[0], 4, "volume", ui, 150,  30, 91 * scale, 101 * scale);
    widget_get_png(ui->widget[2], LDVAR(fx_knob_png));
    set_adjustment(ui->widget[2]->adj, 0.000, 0.000, 0.000, 1.000, 0.010, CL_CONTINUOS);


    ui->widget[3] = add_lv2_image_toggle (ui->widget[3], ui->elem[0], 2, "", ui, 98,  135, 44 * scale, 44 * scale);
    widget_get_png(ui->widget[3], LDVAR(LED_Red_png));
}

void plugin_cleanup(X11_UI *ui) {
    // clean up used sources when needed
}


void plugin_port_event(LV2UI_Handle handle, uint32_t port_index,
                        uint32_t buffer_size, uint32_t format,
                        const void * buffer) {
    // port value change message from host
    // do special stuff when needed
}

