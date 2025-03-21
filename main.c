/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/lvgl.h"
#include <stdlib.h>
#include <unistd.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t* hal_init(int32_t w, int32_t h);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char** argv)
{
    (void)argc; /*Unused*/
    (void)argv; /*Unused*/

    /*Initialize LVGL*/
    lv_init();

    /*Initialize the display, and the input devices*/
    hal_init(800, 600);

    extern int app_entry(int argc, char** argv);
    int retval = app_entry(argc, argv);
    if (retval != 0) {
        return retval;
    }

    while (1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        uint32_t idle = lv_timer_handler();
        usleep(idle * 1000);
    }

    lv_deinit();
    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void indev_set_cursor(lv_indev_t* indev, int32_t size)
{
    lv_obj_t* cursor_obj = lv_indev_get_cursor(indev);
    if (size <= 0) {
        if (cursor_obj) {
            lv_obj_delete(cursor_obj);
            lv_indev_set_cursor(indev, NULL);
        }
    } else {
        if (cursor_obj == NULL) {
            cursor_obj = lv_obj_create(lv_layer_sys());
            lv_obj_remove_style_all(cursor_obj);
            lv_obj_set_style_radius(cursor_obj, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_bg_opa(cursor_obj, LV_OPA_50, 0);
            lv_obj_set_style_bg_color(cursor_obj, lv_color_black(), 0);
            lv_obj_set_style_border_width(cursor_obj, 2, 0);
            lv_obj_set_style_border_color(cursor_obj, lv_palette_main(LV_PALETTE_GREY), 0);
        }
        lv_obj_set_size(cursor_obj, size, size);
        lv_obj_set_style_translate_x(cursor_obj, -size / 2, 0);
        lv_obj_set_style_translate_y(cursor_obj, -size / 2, 0);
        lv_indev_set_cursor(indev, cursor_obj);
    }
}

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t* hal_init(int32_t w, int32_t h)
{
    lv_group_set_default(lv_group_create());

    lv_display_t* disp = lv_sdl_window_create(w, h);

    lv_indev_t* mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_display(mouse, disp);
    lv_display_set_default(disp);

    indev_set_cursor(mouse, 0);

    lv_indev_t* mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_display(mousewheel, disp);

    lv_indev_t* keyboard = lv_sdl_keyboard_create();
    lv_indev_set_display(keyboard, disp);
    lv_indev_set_group(keyboard, lv_group_get_default());

    return disp;
}
