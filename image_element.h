
#ifndef IMG_ELEM_H
#define IMG_ELEM_H

#include "PiLED.h"

enum Direction
{
	UP,
	DOWN,
	RIGHT,
	LEFT
};

struct image_element
{
	short ID;
	short speed;
	struct sPixel *colors;
	enum Direction direction;
	int lastFrame;
	struct sPixel *PixelMatrix;
	void *data;
	void (*init)(struct image_element *img);
	void (*update)(struct image_element *img, int frame);
};

// forward declares
// void init_fullColor(struct image_element *img);
// void update_fullColor(struct image_element *img, int frame);
// void init_rain(struct image_element *img);
// void update_rain(struct image_element *img, int frame);
void init_test(struct image_element *img);
void update_test(struct image_element *img, int frame);
// void init_fullPan(struct image_element *img);
// void update_fullPan(struct image_element *img, int frame);
void init_artnet(struct image_element *img);
void update_artnet(struct image_element *img, int frame);

#endif
