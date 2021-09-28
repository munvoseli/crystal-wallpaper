

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
	char offset = rand() & 3;
	(*p_pixels)[3 * (x + y * w) + 0] = (*p_pixels)[3 * (xo + yo * w) + 0] + offset;
	(*p_pixels)[3 * (x + y * w) + 1] = (*p_pixels)[3 * (xo + yo * w) + 1] + offset;
	(*p_pixels)[3 * (x + y * w) + 2] = (*p_pixels)[3 * (xo + yo * w) + 2] + offset;
}

int main(void)
{
	srand( (unsigned int) time(NULL) ); // should use ms in future
	FILE* fp;
	char* pixels;
	int width;
	int height;

	fp = fopen( "bg.ppm", "r" );
	if( fp == NULL )
	{
		width = 1920;
		height = 1080;
		pixels = get_rand_data( width, height );
	}
	else
	{
		pixels = load_image_from_ppm( fp, &width, &height );
		fclose( fp );
	}
	int i;
	for( i = 0; i < 100000; ++i )
		change_pixel( width, height, &pixels );
	for( i = 0; i < width * height * 3; ++i )
		pixels[i] += 1;
	write_file( width, height, pixels );
	return 0;
}


// while loop
