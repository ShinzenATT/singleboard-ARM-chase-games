#ifndef GRAPHICS_DOT_H
#define GRAPHICS_DOT_H

#include "delay.h"
#include <stdbool.h>

// Value that decides the amount of pixels a sprite may have. By default it's 30
#define MAX_POINTS 30

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

// Linked struct(s) that represents polygon vertexes in a 2D space
typedef struct polygonpoint{
	unsigned char x,y;
	struct polygonpoint *next;
} polyPoint_t, *PPolyPoint_t;

// A struct that represents pixel art sprites
typedef struct {
	int numpoints, sizex, sizey;
	point_t px[MAX_POINTS];
} geometry_t, *PGeometry_t;

// A struct that represents moving objects in a 2D space
typedef struct tMovObj{
	PGeometry_t geo;
	int dirx, diry,
	posx, posy;
	void(* draw) (struct tMovObj *);
	void(* clear) (struct tMovObj *);
	void(* move) (struct tMovObj *);
	void(* setSpeed) (struct tMovObj *, int, int);
} movObject_t, *PMovObject_t;

// Draws a line on the display
extern void DrawLine(const PLine_t cordinates);

// Removes the pixels that matches the position of the line
extern void ClearLine(const PLine_t cordinates);

// Draws a rectangle according to the points in the struct
extern void DrawRect(const PRect_t rectangle);

// Draws a polygon in a 2D space. The polypoints are linked together where the 
//last one has the value 0. Returns true when drawing the poylgon is a success
extern bool DrawPolygon(const PPolyPoint_t startPoint);

// Draws a pixel art according the geometry cordinates
extern void DrawObject(const PMovObject_t obj);
// Clears an area according to the geometry cordinates
extern void ClearObject(const PMovObject_t obj);
// Moves an object n amount of pixels according to it's speed
extern void MoveObject(PMovObject_t obj);
// Sets the movements speed and direction
extern void SetObjectSpeed(PMovObject_t obj, int speedx, int speedy);

// Initializes the display for displaying graphics
extern void GraphicInitialize();

// Clears the display of content
extern void GraphicClearScreen();

// Creates a pixel on the specified cordinates
extern void GraphicPixelSet(int x, int y);

// Removes a pixel on the specified cordinates
extern void GraphicPixelClear(int x, int y);

#endif /* GRAPHICS_DOT_H */