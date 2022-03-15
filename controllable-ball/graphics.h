#ifndef GRAPHICS_DOT_H
#define GRAPHICS_DOT_H

#include "delay.h"
#include <stdbool.h>

// A struct representing a single point in a 2D space
typedef struct{
	int x,y;
} point_t, *PPoint_t;

// A struct representing a line in a 2D space
typedef struct{
	point_t p0, p1;
} Line_t, *PLine_t;

// A struct representing a rectangle in a 2D space
typedef struct {
	point_t origo;
	int x,y;
} rect_t, *PRect_t;

typedef struct polygonpoint{
	unsigned char x,y;
	struct polygonpoint *next;
} polyPoint_t, *PPolyPoint_t;

// Draws a line on the display
extern void DrawLine(const PLine_t cordinates);

// Removes the pixels that matches the position of the line
extern void ClearLine(const PLine_t cordinates);

// Draws a rectangle according to the points in the struct
extern void DrawRect(const PRect_t rectangle);

// Draws a polygon in a 2D space. The polypoints are linked together where the 
//last one has the value 0. Returns true when drawing the poylgon is a success
extern bool DrawPolygon(const PPolyPoint_t startPoint);

// Initializes the display for displaying graphics
extern void GraphicInitialize();

// Clears the display of content
extern void GraphicClearScreen();

// Creates a pixel on the specified cordinates
extern void GraphicPixelSet(int x, int y);

// Removes a pixel on the specified cordinates
extern void GraphicPixelClear(int x, int y);

#endif /* GRAPHICS_DOT_H */