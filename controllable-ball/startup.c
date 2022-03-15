// Imports previously made code
#include <stdbool.h>
#include "delay.h"
#include "graphics.h"
#include "keypad.h"
/*
 * 	startup.c
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define MAX_POINTS 30

typedef struct {
	int numpoints, sizex, sizey;
	point_t px[MAX_POINTS];
} geometry_t, *PGeometry_t;

typedef struct tMovObj{
	PGeometry_t geo;
	int dirx, diry,
	posx, posy;
	void(* draw) (struct tMovObj *);
	void(* clear) (struct tMovObj *);
	void(* move) (struct tMovObj *);
	void(* setSpeed) (struct tMovObj *, int, int);
} movObject_t, *PMovObject_t;

// Draws a pixel art according the geometry cordinates
void DrawBallObject(const PMovObject_t obj);
// Clears an area according to the geometry cordinates
void ClearBallObject(const PMovObject_t obj);
// Moves an object n amount of pixels according to it's speed
void MoveBallObject(PMovObject_t obj);
// Sets the movements speed and direction
void SetBallobjectSpeed(PMovObject_t obj, int speedx, int speedy);

geometry_t ball_geometry = {
	12,
	4,4,
	{{0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {1,3}, {2,0}, {2,1}, {2,2}, {2,3}, {3,1}, {3,2}}
};

movObject_t ball ={
	&ball_geometry,
	0,0,
	1,1,
	DrawBallObject,
	ClearBallObject,
	MoveBallObject,
	SetBallobjectSpeed
};

void main(void){
	GraphicInitialize();
	GraphicClearScreen();
	KeypadInit(false);
	
	unsigned char c;
	PMovObject_t p = &ball;
	
	while(true){
		p->move(p);
		SleepMilliSeconds(20);
		c = KeypadGetKeycode();
		
		switch(c){
			// Key 4
			case 0x21: p->setSpeed(p, -3, 0); break;
			// Key 6
			case 0x23: p->setSpeed(p, 3, 0); break;
			// Key 2
			case 0x12: p->setSpeed(p, 0, -3); break;
			// Key 5
			case 0x22: p->setSpeed(p, 0, 3); break;
			// No  allowed key pressed
			default: p->setSpeed(p, 0, 0); break;
		}
	}
	
}

void DrawBallObject(const PMovObject_t obj){
	
	PPoint_t ptr = obj->geo->px;
	int length = (obj->geo->numpoints),
	posx = obj->posx,
	posy = obj->posy;
	
	for(int i=0; i<length; i++){
	GraphicPixelSet(ptr[i].x + posx,  ptr[i].y + posy);
	}
}

void ClearBallObject(const PMovObject_t obj){
	
	PPoint_t ptr = obj->geo->px;
	int length = (obj->geo->numpoints),
	posx = obj->posx,
	posy = obj->posy;
	
	for(int i=0; i<length; i++){
	GraphicPixelClear(ptr[i].x + posx,  ptr[i].y + posy);
	}
}

void MoveBallObject(PMovObject_t obj){
	obj->clear(obj);
	int *posx = &(obj->posx),
	*posy = &(obj->posy),
	*dirx = &(obj->dirx),
	*diry = &(obj->diry),
	*sizex = &(obj->geo->sizex),
	*sizey = &(obj->geo->sizey);
	
	*posx += *dirx;
	*posy += *diry;
	
	if(*posx < 1 || *posx + *sizex > 128){
		*dirx *= -1;
		*posx += *dirx;
	}
	if(*posy < 1 || *posy + *sizey > 64){
		*diry *= -1;
		*posy += *diry;
	}
	
	obj->draw(obj);
}

void SetBallobjectSpeed(PMovObject_t obj, int speedx, int speedy){
	obj->dirx = speedx;
	obj->diry = speedy;
}

