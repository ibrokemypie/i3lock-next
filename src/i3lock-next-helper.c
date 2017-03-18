#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <Imlib2.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fputs("Error: this program takes exactly two arguments.\n",stderr);
        return 1;
    }

    const char *display_name = getenv("DISPLAY");
    if (!display_name)
        display_name = ":0";
    Display *disp = XOpenDisplay(display_name);
    if (!disp)
    {
        fprintf(stderr, "Can't open display %s!\n", display_name);
        return 1;
    }

    imlib_context_set_display(disp);
    imlib_context_set_visual(DefaultVisual(disp, DefaultScreen(disp)));
    imlib_context_set_colormap(DefaultColormap(disp, DefaultScreen(disp)));
    imlib_context_set_drawable(DefaultRootWindow(disp));

    unsigned int width = DefaultScreenOfDisplay(disp)->width;
    unsigned int height = DefaultScreenOfDisplay(disp)->height;

    Imlib_Image *im = imlib_create_image_from_drawable(1, 0, 0, width, height, 1);
    if (!im)
    {
        fputs("Error: cannot grab image!\n", stderr);
        return 1;
    }

    imlib_context_set_image(imlib_create_image(width, height));
    imlib_context_set_font(imlib_load_font(argv[2]));
    if (!imlib_context_get_font())
    {
        fprintf(stderr, "Error: could not find font %s!\n", argv[2]);
        return 1;
    }
    int offset_w, offset_h, ignore_me;
    imlib_text_draw_with_return_metrics(0, 0, "Type password to unlock.", &offset_w, &offset_h, &ignore_me, &ignore_me);

    imlib_context_set_image(im);

    imlib_context_set_color_modifier(imlib_create_color_modifier());
    imlib_modify_color_modifier_gamma(0.6);
    imlib_apply_color_modifier();
    imlib_context_set_image(imlib_create_cropped_scaled_image(0, 0, width, height, width/5, height/5));
    imlib_image_blur(1);
    imlib_image_blur(1);
    imlib_image_blur(1);
    imlib_context_set_image(imlib_create_cropped_scaled_image(0, 0, width/5, height/5, width, height));

    imlib_text_draw(width/2-offset_w/2, height/1.5, "Type password to unlock.");

    Imlib_Image *lock = imlib_load_image("/usr/share/i3lock-next/lock.png");
    if (!lock)
    {
        fputs("Error: could not find lock.png!\n", stderr);
        return 1;
    }
    imlib_blend_image_onto_image(lock, 0, 0, 0, 80, 80, width/2-40, height/2-40, 80, 80);

    imlib_save_image(argv[1]);
}
// vim: set tabstop=4 softtabstop=4 shiftwidth=4 expandtab:
