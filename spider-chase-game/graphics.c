#include "graphics.h"

int abs(int value){
	if(value < 0){
		value *= -1;
	}
	return value;
}

void swapInt(int *var1, int *var2){
	int temp;
	temp = *var2;
	*var2 = *var1;
	*var1 = temp;
}

void DrawLine(const PLine_t cordinates){
	int x0 = cordinates->p0.x,
	y0 = cordinates->p0.y,
	x1 = cordinates->p1.x,
	y1 = cordinates->p1.y;
	int deltaX, deltaY;
	bool steep = false;
	
	if(abs(y1-y0) > abs(x1-x0)){
		steep = true;
	}
	if(steep){
		swapInt(&x0, &y0);
		swapInt(&x1, &y1);
	}
	
	if(x0 > x1){
		swapInt(&x0, &x1);
		swapInt(&y0, &y1);
	}
	
	deltaX = x1 - x0;
	deltaY = abs(y1 - y0);
	int error = 0,
	y = y0,
	ystep = -1;
	
	if(y0 < y1){
		ystep = 1;
	}
	
	for(int x = x0; x <= x1; x++){
		if(steep){
			GraphicPixelSet(y, x);
		}
		else{
			GraphicPixelSet(x, y);
		}
		error += deltaY;
		
		if(2*error >= deltaX){
			y += ystep;
			error -= deltaX;
		}
	}
}

void ClearLine(const PLine_t cordinates){
	int x0 = cordinates->p0.x,
	y0 = cordinates->p0.y,
	x1 = cordinates->p1.x,
	y1 = cordinates->p1.y;
	int deltaX, deltaY;
	bool steep = false;
	
	if(abs(y1-y0) > abs(x1-x0)){
		steep = true;
	}
	if(steep){
		swapInt(&x0, &y0);
		swapInt(&x1, &y1);
	}
	
	if(x0 > x1){
		swapInt(&x0, &x1);
		swapInt(&y0, &y1);
	}
	
	deltaX = x1 - x0;
	deltaY = abs(y1 - y0);
	int error = 0,
	y = y0,
	ystep = -1;
	
	if(y0 < y1){
		ystep = 1;
	}
	
	for(int x = x0; x <= x1; x++){
		if(steep){
			GraphicPixelClear(y, x);
		}
		else{
			GraphicPixelClear(x, y);
		}
		error += deltaY;
		
		if(2*error >= deltaX){
			y += ystep;
			error -= deltaX;
		}
	}
}

void DrawRect(const PRect_t rect){
	Line_t tempLine1 = {
		rect->origo.x,
		rect->origo.y,
		rect->origo.x + rect->x,
		rect->origo.y
		};
	DrawLine(&tempLine1);
	
	Line_t tempLine2 = {
		rect->origo.x,
		rect->origo.y,
		rect->origo.x,
		rect->origo.y + rect->y
		};
	DrawLine(&tempLine2);
	
	Line_t tempLine3 = {
		rect->origo.x + rect->x,
		rect->origo.y,
		rect->origo.x + rect->x,
		rect->origo.y + rect->y
		};
	DrawLine(&tempLine3);
	
	Line_t tempLine4 = {
		rect->origo.x,
		rect->origo.y + rect->y,
		rect->origo.x + rect->x,
		rect->origo.y + rect->y
		};
	DrawLine(&tempLine4);
	
}

bool DrawPolygon(const PPolyPoint_t startPoint){
	bool success = false;
	point_t p0 = {startPoint->x, startPoint->y};
	PPolyPoint_t ptr = startPoint->next;
	
	while(ptr != 0){
		point_t p1 = {ptr->x, ptr->y};
		Line_t line = {p0.x, p0.y, p1.x, p1.y};
		
		DrawLine(&line);
		p0.x = p1.x; // Did it this way since I got a runtime crash while change the actual struct
		p0.y = p1.y;
		ptr = ptr->next;
	}
	if(ptr->x == 0 && ptr->y == 0){
		success = true;
	}
	return success;
}

void DrawObject(const PMovObject_t obj){
	
	PPoint_t ptr = obj->geo->px;
	int length = (obj->geo->numpoints),
	posx = obj->posx,
	posy = obj->posy;
	
	for(int i=0; i<length; i++){
	GraphicPixelSet(ptr[i].x + posx,  ptr[i].y + posy);
	}
}

void ClearObject(const PMovObject_t obj){
	
	PPoint_t ptr = obj->geo->px;
	int length = (obj->geo->numpoints),
	posx = obj->posx,
	posy = obj->posy;
	
	for(int i=0; i<length; i++){
	GraphicPixelClear(ptr[i].x + posx,  ptr[i].y + posy);
	}
}

void MoveObject(PMovObject_t obj){
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

void SetObjectSpeed(PMovObject_t obj, int speedx, int speedy){
	obj->dirx = speedx;
	obj->diry = speedy;
}

__attribute__((naked))
void GraphicInitialize(){
	__asm volatile (".HWORD 0xDFF0 \n");
	__asm volatile (" BX LR \n");
}

__attribute__((naked))
void GraphicClearScreen(){
	__asm volatile (".HWORD 0xDFF1 \n");
	__asm volatile (" BX LR \n");
}

__attribute__((naked))
void GraphicPixelSet(int x, int y){
	__asm volatile (".HWORD 0xDFF2 \n");
	__asm volatile (" BX LR \n");
}

__attribute__((naked))
void GraphicPixelClear(int x, int y){
	__asm volatile (".HWORD 0xDFF3 \n");
	__asm volatile (" BX LR \n");
}