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

// Checks if two objects overlap each other
bool ObjectsOverlap(PMovObject_t obj1, PMovObject_t obj2);
// Checks if an object is on the edge of the screen
bool ObjectOutOfBounds(PMovObject_t obj);

geometry_t victimSprite = {
	12,
	4,4,
	{{0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {1,3}, {2,0}, {2,1}, {2,2}, {2,3}, {3,1}, {3,2}}
};

movObject_t victim ={
	&victimSprite,
	0,0,
	80,20,
	DrawObject,
	ClearObject,
	MoveObject,
	SetObjectSpeed
};

geometry_t creatureSprite ={
	22,
	6,8,
	{
		{2,0}, {3,0}, {1,1}, {4,1}, {0,2}, 
		{1,2}, {2,2}, {3,2}, {4,2}, {5,2},
		{0,3}, {2,3}, {3,3}, {5,3}, {1,4},
		{4,4}, {2,5}, {3,5}, {1,6}, {4,6},
		{0,7}, {5,7}
	}
};

movObject_t creature ={
	&creatureSprite,
	0,0,
	10,10,
	DrawObject,
	ClearObject,
	MoveObject,
	SetObjectSpeed
};



void main(void){
	GraphicInitialize();
	GraphicClearScreen();
	KeypadInit(false);
	
	unsigned char c;
	PMovObject_t p = &creature,
	pv = &victim;
	pv->setSpeed(pv, 4,1);
	
	
	while(true){
		p->move(p);
		pv->move(pv);
		c = KeypadGetKeycode();
		
		switch(c){
			// Key 4
			case 0x21: p->setSpeed(p, -2, 0); break;
			// Key 6
			case 0x23: p->setSpeed(p, 2, 0); break;
			// Key 2
			case 0x12: p->setSpeed(p, 0, -2); break;
			// Key 5
			case 0x22: p->setSpeed(p, 0, 2); break;
			// No  allowed key pressed
			default: p->setSpeed(p, 0, 0); break;
		}
		if(ObjectsOverlap(p, pv) || ObjectOutOfBounds(p)){
			// End game
			break;
		}
		
		SleepMilliSeconds(20);
	}
}

bool ObjectsOverlap(PMovObject_t obj1, PMovObject_t obj2){
	int posx1 = obj1->posx, 
	posy1 = obj1->posy,
	sizex1 = obj1->geo->sizex, 
	sizey1 = obj1->geo->sizey,
	posx2 = obj2->posx, 
	posy2 = obj2->posy,
	sizex2 = obj2->geo->sizex, 
	sizey2 = obj2->geo->sizey;
	// saves the corners of obj2
	point_t corners[] = {
		{posx2,posy2}, {posx2 + sizex2,posy2},
		{posx2,posy2 + sizey2}, {posx2 + sizex2,posy2 + sizey2}
		};
	
	// checks if any corner is whithing the confines of obj1
	for(int i = 0; i < 4; i++){
		if(posx1 <= corners[i].x && posx1 + sizex1 >= corners[i].x &&
		posy1 <= corners[i].y && posy1 + sizey1 >= corners[i].y){
			return true;
		}
	}
	return false;
}

bool ObjectOutOfBounds(PMovObject_t obj){
	int posx = obj->posx,
	posy = obj->posy,
	*dirx = &(obj->dirx),
	*diry = &(obj->diry),
	*sizex = &(obj->geo->sizex),
	*sizey = &(obj->geo->sizey);
	
	posx += *dirx;
	posy += *diry;
	
	if(posx < 1 || posx + *sizex > 128 ||
	posy < 1 || posy + *sizey > 64){
		return true;
	}
	return false;
}