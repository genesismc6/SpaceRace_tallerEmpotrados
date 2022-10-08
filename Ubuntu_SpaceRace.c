//gcc Proyecto2.c `sdl2-config --cflags --libs` -o Proyecto2

#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>


typedef struct asteroids{
	signed short x; //loc. en x
	signed short y; //loc. en y
	signed short x2; //Registro para guardar ubicación en x anterior 

}asteroid;
//Se define la estructura para la bala

typedef struct player{
	unsigned short x; //loc. en x
	unsigned short y; //loc. en y
	unsigned short w; //Ancho
	unsigned short h; //Largo
}ship;

typedef struct win_border{
	chtype leftside,rightside,topside,bottomside,topleft,topright,bottomlet,bottomright;

}WIN_BORDER; //Estructura para limitar los border de las pantalla

typedef struct win_struct{
	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;
