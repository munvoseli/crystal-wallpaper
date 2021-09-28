#include <X11/Xlib.h>
#include <Imlib2.h>

void render( int w, int h, char* pixels )
{
	Display* disp = XOpenDisplay(NULL);
	Window win = DefaultRootWindow(disp);
	int    depth = DefaultDepth( disp, DefaultScreen(disp) ); // usually 24
	Pixmap drawable = XCreatePixmap( disp, win, w, h, depth );
	imlib_set_cache_size(256 * 256 * 1024);

	Visual*  vis   = DefaultVisual(disp, DefaultScreen(disp));
	Colormap cm    = DefaultColormap(disp, DefaultScreen(disp));
	imlib_context_set_display(disp);
	imlib_context_set_visual(vis);
	imlib_context_set_colormap(cm);
	imlib_context_set_drawable(win);

	Imlib_Image im = imlib_load_image( "bg.ppm" );
	imlib_context_set_image( im );
	imlib_context_set_drawable( win );
	imlib_context_set_anti_alias( 0 ); // char
	imlib_context_set_dither( 0 ); // char
	imlib_context_set_blend( 0 ); // char
	imlib_context_set_angle(0);
	imlib_render_image_on_drawable_at_size(0, 0, w, h);
}

int main()
{
	render( 1920, 1080, "hi" );
	return 0;
}
