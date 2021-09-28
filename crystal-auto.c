

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> /* sleep */
#include <X11/Xlib.h>
#include <Imlib2.h>


char* load_image_from_ppm( FILE* fp, int* p_width, int* p_height )
{
	int c;
	char* part = NULL;
	size_t part_size = 0;
	// skip the magic number, "P6\n"
	getdelim( &part, &part_size, '\n', fp );

	// get width and height
	getdelim( &part, &part_size, ' ', fp );
	*p_width = strtol( part, NULL, 10 );
	getline( &part, &part_size, fp );
	*p_height = strtol( part, NULL, 10 );

	// and max color
	getline( &part, &part_size, fp );

	unsigned int bytes = *p_width * *p_height * 3;
	char* pixels = malloc( bytes );
	fread( pixels, 1, bytes, fp );
	return pixels;
}

void write_file( int width, int height, char* pixels )
{
	FILE* fp = fopen( "bg.ppm", "w" );
	fprintf( fp, "P6\n%d %d\n255\n", width, height );
	fwrite( pixels, 1, width * height * 3, fp );
	fclose( fp );
}

char* get_rand_data( int w, int h )
{
	char* pixels = malloc( w * h * 3 );
	int i;
	for( i = 0; i < w * h * 3;)
	{
		pixels[i] = rand() & 255;
		pixels[++i] = pixels[i];
		pixels[++i] = pixels[i];
		++i;
	}
	return pixels;
}

void change_pixel( int w, int h, char** p_pixels )
{
	int x = rand() % (w - 2);
	int y = rand() % (h - 2);
	int xo = rand() % 3 + x;
	int yo = rand() % 3 + y;
	x += 1;
	y += 1;
	int sum = (*p_pixels)[3 * (x + y * w) + 0] + (*p_pixels)[3 * (x + y * w) + 1] + (*p_pixels)[3 * (x + y * w) + 2];
	if (sum < 192)
	{
		(*p_pixels)[3 * (x + y * w) + 0] = (*p_pixels)[3 * (xo + yo * w) + 0] + rand() % 4 - 0;
		(*p_pixels)[3 * (x + y * w) + 1] = (*p_pixels)[3 * (xo + yo * w) + 1] + rand() % 4 - 0;
		(*p_pixels)[3 * (x + y * w) + 2] = (*p_pixels)[3 * (xo + yo * w) + 2] + rand() % 4 - 0;
	}
	else
	{
		(*p_pixels)[3 * (x + y * w) + 0] = (*p_pixels)[3 * (xo + yo * w) + 0] + rand() % 4 - 3;
		(*p_pixels)[3 * (x + y * w) + 1] = (*p_pixels)[3 * (xo + yo * w) + 1] + rand() % 4 - 3;
		(*p_pixels)[3 * (x + y * w) + 2] = (*p_pixels)[3 * (xo + yo * w) + 2] + rand() % 4 - 3;
	}
}


void render( int w, int h, char* pixels, Display* disp, Window win,
	     int depth)
{
	//XSetCloseDownMode( disp, RetainPermanent );
}

int main(void)
{
	srand( (unsigned int) time(NULL) ); // should use ms in future
	Display* disp = XOpenDisplay(NULL);
	Window win    = RootWindow( disp, DefaultScreen(disp) );
	int    depth  = DefaultDepth( disp, DefaultScreen(disp) ); // usually 24
	Visual*  vis  = DefaultVisual(disp, DefaultScreen(disp));
	Colormap cm   = DefaultColormap(disp, DefaultScreen(disp));
	FILE* fp;
	char* pixels;
	int width;
	int height;
	int i;
	int j = 0;

	Pixmap pm_drawable;
	Imlib_Image im;

	XGCValues gcvalues;
	GC gc;

	imlib_set_cache_size(256 * 256 * 1024);
	imlib_context_set_display(disp);
	imlib_context_set_visual(vis);
	imlib_context_set_colormap(cm);
	
	imlib_context_set_anti_alias( 0 ); // char
	imlib_context_set_dither( 0 ); // char
	imlib_context_set_blend( 0 ); // char
	imlib_context_set_angle(0);

	
	for(;;)
	{
		printf("%d  ", j);
		fp = fopen( "bg.ppm", "r" );
		if( fp == NULL )
		{
			width = 192;
			height = 108;
			pixels = get_rand_data( width, height );
		}
		else
		{
			pixels = load_image_from_ppm( fp, &width, &height );
			fclose( fp );
		}
		for( i = 0; i < (width * height) >> 3; ++i )
			change_pixel( width, height, &pixels );
		write_file( width, height, pixels );
		free( pixels );
		printf("%d  ", j);
		//render( 1920, 1080, "hi", disp, win, depth );

		//XFlush( disp );
		pm_drawable = XCreatePixmap( disp, win, 1920, 1080, depth );
		imlib_context_set_drawable( win );
		im = imlib_load_image_without_cache( "bg.ppm" );
		imlib_context_set_image( im );
		imlib_render_image_on_drawable_at_size( 0, 0, 1920, 1080 );

		gcvalues.tile = pm_drawable;
		gc = XCreateGC( disp, pm_drawable, 0, &gcvalues );
		//XSetClipMask( disp, gc, pm_drawable );
		//XFillRectangle( disp, pm_drawable, gc, 0, 0, 1920, 1080 );

		//XSetWindowBackgroundPixmap( disp, win, pm_drawable );
		//XCopyArea( disp, pm_drawable, win, gc, 0, 0, 1920, 1080, 0, 0 );
		//XClearWindow( disp, win );
		XFlush( disp );
		XFreeGC( disp, gc );
		XFreePixmap( disp, pm_drawable );

		usleep( 50000 );
		printf("%d\n", j);
		++j;
	}
	XCloseDisplay( disp );
	return 0;
}


// while loop
