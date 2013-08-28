#include <stdio.h>
int width = 128;
int height = 8;

void WriteFrame(FILE *file, int r, int g, int b)
{
	int i;
	for (i=0; i<width*height; i++)
	{
		fputc(r, file);
		fputc(g, file);
		fputc(b, file);
	}
}

int main()
{
FILE *file;
file = fopen("test.ledbar","w+");
fputc(width, file);
fputc(height, file);
int intensity;
for (intensity = 0 ; intensity < 255; intensity++)
	WriteFrame(file, intensity, 0, 0);

for (intensity = 255 ; intensity > 0; intensity--)
	WriteFrame(file, intensity, 0, 0);

for (intensity = 0 ; intensity < 255; intensity++)
	WriteFrame(file,0 ,intensity, 0);

for (intensity = 255 ; intensity > 0; intensity--)
	WriteFrame(file,0 ,intensity, 0);

for (intensity = 0 ; intensity < 255; intensity++)
	WriteFrame(file,0 ,0, intensity);

for (intensity = 255 ; intensity > 0; intensity--)
	WriteFrame(file,0 ,0, intensity);

fclose(file);
return 0;
}
