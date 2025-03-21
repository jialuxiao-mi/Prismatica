#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/lvgl.h"
#include <math.h>
#include <string.h>

#define CANVAS_WIDTH 480
#define CANVAS_HEIGHT 480

static void prismatica_setup(lv_draw_buf_t* draw_buf)
{
    lv_draw_buf_clear(draw_buf, NULL);
}

static void prismatica_loop(lv_draw_buf_t* draw_buf)
{
    lv_color_t* buf = (lv_color_t*)lv_draw_buf_goto_xy(draw_buf, 0, 0);
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            *buf = lv_color_make(lv_rand(0, 255), lv_rand(0, 255), lv_rand(0, 255));
            buf++;
        }
    }
}

static void canvas_loop(lv_timer_t* timer)
{
    lv_obj_t* canvas = (lv_obj_t*)lv_timer_get_user_data(timer);

    prismatica_loop(lv_canvas_get_draw_buf(canvas));

    lv_obj_invalidate(canvas);
}

int app_entry(int argc, char** argv)
{
    lv_obj_t* canvas = lv_canvas_create(lv_screen_active());

    lv_draw_buf_t* draw_buf = lv_draw_buf_create(
        CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_RGB888, LV_STRIDE_AUTO);
    lv_canvas_set_draw_buf(canvas, draw_buf);
    lv_obj_center(canvas);

    prismatica_loop(draw_buf);

    lv_timer_create(canvas_loop, 16, canvas);
    return 0;
}