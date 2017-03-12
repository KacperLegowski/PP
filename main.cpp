#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;
extern "C" {
	//#ifdef BIT64
	//#include"./sdl64/include/SDL.h"
	//#include"./sdl64/include/SDL_main.h"
	//#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
	//#endif
}

#define SCREEN_WIDTH	700
#define SCREEN_HEIGHT	700

enum Klocek { I, T, O, L, J, S, Z, SCIANA, PUSTE };

struct Punkt
{
	int x;
	int y;
};



struct ObramowanieKlocka
{
	Punkt punkty[4];
	Klocek klocek;
};


// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};


// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
// b¹dŸ poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostok¹ta o d³ugoœci boków l i k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

// rysowanie planszy wraz z klockami
void RysujPlansze(SDL_Surface *screen, Klocek plansza[22][12])
{
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	int ciemnoszary = SDL_MapRGB(screen->format, 0x66, 0x66, 0x66);
	int out_ciemnoszary = SDL_MapRGB(screen->format, 0x4C, 0x4C, 0x4C);

	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int szary = SDL_MapRGB(screen->format, 0x99, 0x99, 0x99);
	int blekitny = SDL_MapRGB(screen->format, 0x00, 0xFF, 0xFF);
	int zolty = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
	int rozowy = SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);

	int out_czerwony = SDL_MapRGB(screen->format, 0x99, 0x00, 0x00);
	int out_szary = SDL_MapRGB(screen->format, 0x6B, 0x6B, 0x6B);
	int out_blekitny = SDL_MapRGB(screen->format, 0x99, 0xFF, 0xFF);
	int out_zolty = SDL_MapRGB(screen->format, 0x99, 0x99, 0x00);
	int out_rozowy = SDL_MapRGB(screen->format, 0xD8, 0x5F, 0xD8);
	int out_niebieski = SDL_MapRGB(screen->format, 0x00, 0x00, 0x99);
	int out_zielony = SDL_MapRGB(screen->format, 0x00, 0x99, 0x00);

	Uint32 outline, fill;

	for (int i = 0;i < 22;i++)
	{
		for (int j = 0;j < 12;j++)
		{
			switch (plansza[i][j])
			{
			case SCIANA:
				outline = out_ciemnoszary;
				fill = ciemnoszary;
				break;
			case I:
				outline = out_czerwony;
				fill = czerwony;
				break;
			case T:
				outline = out_szary;
				fill = szary;
				break;
			case O:
				outline = out_blekitny;
				fill = blekitny;
				break;
			case L:
				outline = out_zolty;
				fill = zolty;
				break;
			case J:
				outline = out_rozowy;
				fill = rozowy;
				break;
			case S:
				outline = out_niebieski;
				fill = niebieski;
				break;
			case Z:
				outline = out_zielony;
				fill = zielony;
				break;
			default:
				outline = czarny;
				fill = czarny;
				break;
			}
			DrawRectangle(screen, 150 + j * 30, 40 + i * 30, 30, 30, outline, fill);
		}
	}
}

ObramowanieKlocka RysujKlocek(SDL_Surface *screen, int x, int y, Klocek klocek)
{
	struct ObramowanieKlocka obramowanie;

	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int szary = SDL_MapRGB(screen->format, 0x99, 0x99, 0x99);
	int blekitny = SDL_MapRGB(screen->format, 0x00, 0xFF, 0xFF);
	int zolty = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
	int rozowy = SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);

	int out_czerwony = SDL_MapRGB(screen->format, 0x99, 0x00, 0x00);
	int out_szary = SDL_MapRGB(screen->format, 0x6B, 0x6B, 0x6B);
	int out_blekitny = SDL_MapRGB(screen->format, 0x99, 0xFF, 0xFF);
	int out_zolty = SDL_MapRGB(screen->format, 0x99, 0x99, 0x00);
	int out_rozowy = SDL_MapRGB(screen->format, 0xD8, 0x5F, 0xD8);
	int out_niebieski = SDL_MapRGB(screen->format, 0x00, 0x00, 0x99);
	int out_zielony = SDL_MapRGB(screen->format, 0x00, 0x99, 0x00);

	obramowanie.klocek = klocek;

	struct Punkt punkt;

	switch (klocek)
	{
	case I:
		DrawRectangle(screen, x, y, 30, 30, out_czerwony, czerwony);
		DrawRectangle(screen, x, y - 30, 30, 30, out_czerwony, czerwony);
		DrawRectangle(screen, x, y + 30, 30, 30, out_czerwony, czerwony);
		DrawRectangle(screen, x, y + 60, 30, 30, out_czerwony, czerwony);
		punkt.x = x;
		punkt.y = y;
		obramowanie.punkty[0] = punkt;
		punkt.x = x;
		punkt.y = y - 30;
		obramowanie.punkty[1] = punkt;
		punkt.x = x;
		punkt.y = y + 30;
		obramowanie.punkty[0] = punkt;
		punkt.x = x;
		punkt.y = y + 60;
		obramowanie.punkty[3] = punkt;
		return obramowanie;
	case T:
		DrawRectangle(screen, x, y, 30, 30, out_szary, szary);
		DrawRectangle(screen, x - 30, y, 30, 30, out_szary, szary);
		DrawRectangle(screen, x + 30, y, 30, 30, out_szary, szary);
		DrawRectangle(screen, x, y + 30, 30, 30, out_szary, szary);
		punkt.x = x;
		punkt.y = y;
		obramowanie.punkty[0] = punkt;
		punkt.x = x - 30;
		punkt.y = y;
		obramowanie.punkty[1] = punkt;
		punkt.x = x + 30;
		punkt.y = y;
		obramowanie.punkty[2] = punkt;
		punkt.x = x;
		punkt.y = y + 30;
		obramowanie.punkty[3] = punkt;
		return obramowanie;
	case O:
		DrawRectangle(screen, x, y, 30, 30, out_blekitny, blekitny);
		DrawRectangle(screen, x + 30, y, 30, 30, out_blekitny, blekitny);
		DrawRectangle(screen, x, y + 30, 30, 30, out_blekitny, blekitny);
		DrawRectangle(screen, x + 30, y + 30, 30, 30, out_blekitny, blekitny);
		punkt.x = x;
		punkt.y = y;
		obramowanie.punkty[0] = punkt;
		punkt.x = x + 30;
		punkt.y = y;
		obramowanie.punkty[1] = punkt;
		punkt.x = x;
		punkt.y = y + 30;
		obramowanie.punkty[2] = punkt;
		punkt.x = x + 30;
		punkt.y = y + 30;
		obramowanie.punkty[3] = punkt;
		return obramowanie;
	case L:
		DrawRectangle(screen, x, y, 30, 30, out_zolty, zolty);
		DrawRectangle(screen, x, y - 30, 30, 30, out_zolty, zolty);
		DrawRectangle(screen, x, y + 30, 30, 30, out_zolty, zolty);
		DrawRectangle(screen, x + 30, y + 30, 30, 30, out_zolty, zolty);
		punkt.x = x;
		punkt.y = y;
		obramowanie.punkty[0] = punkt;
		punkt.x = x;
		punkt.y = y - 30;
		obramowanie.punkty[1] = punkt;
		punkt.x = x;
		punkt.y = y + 30;
		obramowanie.punkty[2] = punkt;
		punkt.x = x + 30;
		punkt.y = y + 30;
		obramowanie.punkty[3] = punkt;
		return obramowanie;
	case J:
		DrawRectangle(screen, x, y, 30, 30, out_rozowy, rozowy);
		DrawRectangle(screen, x, y - 30, 30, 30, out_rozowy, rozowy);
		DrawRectangle(screen, x, y + 30, 30, 30, out_rozowy, rozowy);
		DrawRectangle(screen, x + 30, y + 30, 30, 30, out_rozowy, rozowy);
		punkt.x = x;
		punkt.y = y;
		obramowanie.punkty[0] = punkt;
		punkt.x = x;
		punkt.y = y - 30;
		obramowanie.punkty[1] = punkt;
		punkt.x = x;
		punkt.y = y + 30;
		obramowanie.punkty[2] = punkt;
		punkt.x = x + 30;
		punkt.y = y + 30;
		obramowanie.punkty[3] = punkt;
		return obramowanie;
	case S:
		DrawRectangle(screen, x, y, 30, 30, out_niebieski, niebieski);
		DrawRectangle(screen, x - 30, y, 30, 30, out_niebieski, niebieski);
		DrawRectangle(screen, x, y - 30, 30, 30, out_niebieski, niebieski);
		DrawRectangle(screen, x + 30, y - 30, 30, 30, out_niebieski, niebieski);
		punkt.x = x;
		punkt.y = y;
		obramowanie.punkty[0] = punkt;
		punkt.x = x - 30;
		punkt.y = y;
		obramowanie.punkty[1] = punkt;
		punkt.x = x;
		punkt.y = y - 30;
		obramowanie.punkty[2] = punkt;
		punkt.x = x + 30;
		punkt.y = y - 30;
		obramowanie.punkty[3] = punkt;
		return obramowanie;
	case Z:
		DrawRectangle(screen, x, y, 30, 30, out_zielony, zielony);
		DrawRectangle(screen, x - 30, y, 30, 30, out_zielony, zielony);
		DrawRectangle(screen, x, y + 30, 30, 30, out_zielony, zielony);
		DrawRectangle(screen, x + 30, y + 30, 30, 30, out_zielony, zielony);
		punkt.x = x;
		punkt.y = y;
		obramowanie.punkty[0] = punkt;
		punkt.x = x - 30;
		punkt.y = y;
		obramowanie.punkty[1] = punkt;
		punkt.x = x;
		punkt.y = y + 30;
		obramowanie.punkty[2] = punkt;
		punkt.x = x + 30;
		punkt.y = y + 30;
		obramowanie.punkty[3] = punkt;
		return obramowanie;
	default:
		DrawRectangle(screen, x, y, 30, 30, out_blekitny, blekitny);
		DrawRectangle(screen, x + 30, y, 30, 30, out_blekitny, blekitny);
		DrawRectangle(screen, x, y + 30, 30, 30, out_blekitny, blekitny);
		DrawRectangle(screen, x + 30, y + 30, 30, 30, out_blekitny, blekitny);
		punkt.x = x;
		punkt.y = y;
		obramowanie.punkty[0] = punkt;
		punkt.x = x + 30;
		punkt.y = y;
		obramowanie.punkty[1] = punkt;
		punkt.x = x;
		punkt.y = y + 30;
		obramowanie.punkty[2] = punkt;
		punkt.x = x + 30;
		punkt.y = y + 30;
		obramowanie.punkty[3] = punkt;
		return obramowanie;
	}
}

void WstawKlocekNaPlansze(Klocek plansza[22][12], Klocek klocek, int i, int j)
{
	switch (klocek)
	{
	case I:
		plansza[i - 1][j] = klocek;
		plansza[i][j] = klocek;
		plansza[i + 1][j] = klocek;
		plansza[i + 2][j] = klocek;
		break;
	case T:
		plansza[i][j - 1] = klocek;
		plansza[i][j] = klocek;
		plansza[i][j + 1] = klocek;
		plansza[i + 1][j] = klocek;
		break;
	case O:
		plansza[i][j] = klocek;
		plansza[i][j + 1] = klocek;
		plansza[i + 1][j + 1] = klocek;
		plansza[i + 1][j] = klocek;
		break;
	case L:
		plansza[i - 1][j] = klocek;
		plansza[i][j] = klocek;
		plansza[i + 1][j] = klocek;
		plansza[i + 1][j + 1] = klocek;
		break;
	case J:
		plansza[i - 1][j] = klocek;
		plansza[i][j] = klocek;
		plansza[i + 1][j] = klocek;
		plansza[i + 1][j - 1] = klocek;
		break;
	case S:
		plansza[i][j] = klocek;
		plansza[i][j + 1] = klocek;
		plansza[i + 1][j] = klocek;
		plansza[i + 1][j - 1] = klocek;
		break;
	case Z:
		plansza[i +1][j + 1] = klocek;
		plansza[i + 1][j] = klocek;
		plansza[i][j] = klocek;
		plansza[i][j - 1] = klocek;
		break;
	}
}

bool SprawdzKlocekDol(Klocek plansza[22][12], Klocek klocek, int i, int j, int pozycja)
{
	switch (klocek)
	{
	case I:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i + 3][j] == PUSTE)
			{
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 2][j] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i + 2][j] = klocek;
				plansza[i + 3][j] = klocek;

				return true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i+1][j-2] == PUSTE && plansza[i+1][j - 1] == PUSTE && plansza[i+1][j + 1] == PUSTE && plansza[i+1][j] == PUSTE)
			{
				plansza[i][j - 2] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i][j] = PUSTE;

				plansza[i+1][j - 2] = klocek;
				plansza[i+1][j - 1] = klocek;
				plansza[i + 1][j + 1] = klocek;
				plansza[i + 1][j] = klocek;

				return true;
			}
		}
		break;
	case T:
		if (pozycja == 1) {
			if (plansza[i + 1][j - 1] == PUSTE && plansza[i + 2][j] == PUSTE && plansza[i + 1][j + 1] == PUSTE)
			{
				plansza[i][j - 1] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i + 1][j - 1] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i + 1][j + 1] = klocek;
				plansza[i + 2][j] = klocek;

				return true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i+1][j-1] == PUSTE && plansza[i + 2][j] == PUSTE) {
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j] = PUSTE;

				plansza[i+1][j - 1] = klocek;
				plansza[i][j] = klocek;
				plansza[i + 2][j] = klocek;
				plansza[i+1][j] = klocek;

				return true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i + 1][j] == PUSTE && plansza[i + 1][j - 1] == PUSTE && plansza[i + 1][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i+1][j - 1] = klocek;
				plansza[i+1][j + 1] = klocek;
				plansza[i][j] = klocek;

				return true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i + 2][j] == PUSTE && plansza[i + 1][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i][j] = klocek;
				plansza[i + 2][j] = klocek;
				plansza[i+1][j + 1] = klocek;
				return true;
			}
		}

		break;
	case O:
		if (plansza[i + 2][j] == PUSTE && plansza[i + 2][j + 1] == PUSTE)
		{
			plansza[i][j] = PUSTE;
			plansza[i][j + 1] = PUSTE;
			plansza[i + 1][j + 1] = PUSTE;
			plansza[i + 1][j] = PUSTE;

			plansza[i + 1][j] = klocek;
			plansza[i + 1][j + 1] = klocek;
			plansza[i + 2][j + 1] = klocek;
			plansza[i + 2][j] = klocek;

			return true;
		}
		break;
	case L:
		if (pozycja == 1) {
			if (plansza[i + 2][j] == PUSTE && plansza[i + 2][j + 1] == PUSTE)
			{
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i + 2][j] = klocek;
				plansza[i + 2][j + 1] = klocek;

				return true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i + 2][j - 1] == PUSTE && plansza[i + 1][j] == PUSTE && plansza[i + 1][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i+1][j - 1] = klocek;
				plansza[i+1][j + 1] = klocek;
				plansza[i + 2][j - 1] = klocek;

				return true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i + 2][j] == PUSTE && plansza[i][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i][j] = klocek;
				plansza[i + 2][j] = klocek;

				return true;
			}
		}

		else if (pozycja == 4) {
			if (plansza[i + 1][j] == PUSTE && plansza[i + 1][j - 1] == PUSTE && plansza[i + 1][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i - 1][j + 1] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i+1][j - 1] = klocek;
				plansza[i+1][j + 1] = klocek;
				plansza[i][j + 1] = klocek;

				return true;
			}
		}
		break;
	case J:
		if(pozycja==1){
			if (plansza[i + 2][j] == PUSTE && plansza[i + 2][j - 1] == PUSTE)
			{
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i + 2][j] = klocek;
				plansza[i + 2][j - 1] = klocek;

				return true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i+1][j - 1] == PUSTE && plansza[i + 1][j] == PUSTE && plansza[i+1][j+1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j-1] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i+1][j - 1] = klocek;
				plansza[i+1][j + 1] = klocek;
				return true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i][j+1] == PUSTE && plansza[i + 2][j] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i - 1][j + 1] = PUSTE;
			
				plansza[i+1][j] = klocek;
				plansza[i + 2][j] = klocek;
				plansza[i][j] = klocek;
				plansza[i ][j + 1] = klocek;
				return true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i + 1][j - 1] == PUSTE && plansza[i + 1][j] == PUSTE && plansza[i + 2][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i+1][j - 1] = klocek;
				plansza[i+1][j + 1] = klocek;
				plansza[i + 2][j + 1] = klocek;
				return true;
			}
		}
		break;
	case S:
		if (pozycja == 1 || pozycja ==3) {
			if (plansza[i + 2][j - 1] == PUSTE && plansza[i + 2][j] == PUSTE && plansza[i+1][j + 1] == PUSTE)
			{
				plansza[i][j + 1] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i + 1][j] = klocek;
				plansza[i + 1][j+1] = klocek;
				plansza[i+2][j] = klocek;
				plansza[i+2][j - 1] = klocek;

				return true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i + 2][j] == PUSTE && plansza[i + 1][j-1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i + 2][j] = klocek;
				plansza[i+1][j - 1] = klocek;
				plansza[i][j - 1] = klocek;
				return true;
			}
		}
		break;
	case Z:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i+1][j - 1] == PUSTE && plansza[i + 2][j] == PUSTE && plansza[i + 2][j + 1] == PUSTE)
			{
				plansza[i][j - 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i+1][j + 1] = PUSTE;

				plansza[i+1][j - 1] = klocek;
				plansza[i+1][j] = klocek;
				plansza[i + 2][j] = klocek;
				plansza[i + 2][j + 1] = klocek;

				return true;
			}
		}
		if (pozycja == 2 || pozycja == 4) {
			if (plansza[i + 1][j] == PUSTE && plansza[i + 2][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i+1][j - 1] = klocek;
				plansza[i][j] = klocek;
				plansza[i + 2][j - 1] = klocek;
				return true;
			}
		}
		break;
	}
	return false;
}

bool SprawdzKlocekPrawo(Klocek plansza[22][12], Klocek klocek, int i, int j, int pozycja)
{
	switch (klocek)
	{
	case I:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i - 1][j + 1] == PUSTE && plansza[i][j + 1] == PUSTE && plansza[i + 1][j + 1] == PUSTE && plansza[i + 2][j + 1] == PUSTE)
			{
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 2][j] = PUSTE;

				plansza[i - 1][j + 1] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i + 1][j + 1] = klocek;
				plansza[i + 2][j + 1] = klocek;

				return true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i][j+2] == PUSTE) {
				plansza[i][j-2] = PUSTE;
				plansza[i][j-1] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i][j+1] = PUSTE;

				plansza[i][j-1] = klocek;
				plansza[i][j] = klocek;
				plansza[i][j+1] = klocek;
				plansza[i][j+2] = klocek;

				return true;
			}
		}
		break;
	case T:
		if (pozycja == 1) {
			if (plansza[i][j + 2] == PUSTE && plansza[i + 1][j + 1] == PUSTE)
			{
				plansza[i][j - 1] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i][j + 2] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				return true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i - 1][j + 1] == PUSTE && plansza[i][j + 1] == PUSTE && plansza[i + 1][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j-1] = PUSTE;
				plansza[i-1][j] = PUSTE;
				plansza[i+1][j] = PUSTE;


			plansza[i][j+1] = klocek;
			plansza[i][j] = klocek;
			plansza[i - 1][j+1] = klocek;
			plansza[i + 1][j+1] = klocek;

			return true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i][j + 2] == PUSTE && plansza[i - 1][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;

				plansza[i][j+1] = klocek;
				plansza[i - 1][j+1] = klocek;
				plansza[i][j] = klocek;
				plansza[i][j + 2] = klocek;
				return true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i][j + 2] == PUSTE && plansza[i +1][j + 1] == PUSTE && plansza[i-1][j+1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i-1][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i-1][j+1] = klocek;
				plansza[i][j+1] = klocek;
				plansza[i][j + 2] = klocek;
				plansza[i + 1][j+1] = klocek;
				return true;
			}
		}
		break;
	case O:
		if (plansza[i][j + 2] == PUSTE && plansza[i + 1][j + 2] == PUSTE)
		{
			plansza[i][j] = PUSTE;
			plansza[i][j + 1] = PUSTE;
			plansza[i + 1][j + 1] = PUSTE;
			plansza[i + 1][j] = PUSTE;

			plansza[i][j + 1] = klocek;
			plansza[i][j + 2] = klocek;
			plansza[i + 1][j + 2] = klocek;
			plansza[i + 1][j + 1] = klocek;

			return true;
		}
		break;
	case L:
		if(pozycja==1){
			if (plansza[i - 1][j + 1] == PUSTE && plansza[i][j + 1] == PUSTE && plansza[i + 1][j + 2] == PUSTE)
			{
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				plansza[i - 1][j + 1] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i + 1][j + 1] = klocek;
				plansza[i + 1][j + 2] = klocek;

				return true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i][j + 2] == PUSTE && plansza[i + 1][j] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i][j+1] = klocek;
				plansza[i][j] = klocek;
				plansza[i][j + 2] = klocek;
				plansza[i + 1][j] = klocek;
				return true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i][j + 1] == PUSTE && plansza[i - 1][j + 1] == PUSTE && plansza[i + 1][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i][j+1] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i - 1][j+1] = klocek;
				plansza[i + 1][j + 1] = klocek;

					return true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i][j + 2] == PUSTE && plansza[i - 1][j + 2] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i - 1][j + 1] = PUSTE;

				plansza[i][j+1] = klocek;
				plansza[i][j] = klocek;
				plansza[i][j + 2] = klocek;
				plansza[i - 1][j + 2] = klocek;
				return true;
			}
		}
		break;
	case J:
		if (pozycja == 1) {
			if (plansza[i - 1][j + 1] == PUSTE && plansza[i][j + 1] == PUSTE && plansza[i + 1][j + 1] == PUSTE)
			{
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i+1][j] = PUSTE;
				plansza[i+1][j - 1] = PUSTE;

				plansza[i - 1][j + 1] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i + 1][j + 1] = klocek;
				plansza[i + 1][j] = klocek;

				return true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i][j + 2] == PUSTE && plansza[i - 1][j] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j+1] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;

				plansza[i][j+2] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i][j+1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				return true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i - 1][j + 2] == PUSTE && plansza[i][j + 1] == PUSTE && plansza[i + 1][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i - 1][j + 1] = PUSTE;

				plansza[i][j+1] = PUSTE;
				plansza[i + 1][j+1] = PUSTE;
				plansza[i - 1][j+1] = PUSTE;
				plansza[i - 1][j + 2] = PUSTE;
				return true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i][j + 2] == PUSTE && plansza[i + 1][j + 2] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				plansza[i][j+1] = PUSTE;
				plansza[i][j + 2] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j + 2] = PUSTE;
				return true;
			}
		}
		break;
	case S:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i + 1][j + 1] == PUSTE && plansza[i][j + 2] == PUSTE)
			{
				plansza[i][j + 1] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i][j+1] = klocek;
				plansza[i][j + 2] = klocek;
				plansza[i + 1][j + 1] = klocek;
				plansza[i + 1][j] = klocek;

				return true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i][j + 1] == PUSTE && plansza[i - 1][j] == PUSTE && plansza[i + 1][j + 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;

				plansza[i][j+1] = klocek;
				plansza[i + 1][j+1] = klocek;
				plansza[i][j] = klocek;
				plansza[i - 1][j] = klocek;
				return true;
			}
		}
		break;
	case Z:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i][j + 1] == PUSTE && plansza[i+1][j + 2] == PUSTE)
			{
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j+1] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i + 1][j + 1] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i + 1][j + 2] = klocek;

				return true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i][j + 1] == PUSTE && plansza[i - 1][j + 1] == PUSTE && plansza[i + 1][j] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i][j+1] = klocek;
				plansza[i][j] = klocek;
				plansza[i - 1][j+1] = klocek;
				plansza[i + 1][j] = klocek;

				return true;
			}
		}
		break;
	}
	return false;
}

bool SprawdzKlocekLewo(Klocek plansza[22][12], Klocek klocek, int i, int j, int pozycja)
{
	switch (klocek)
	{
	case I:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i - 1][j -1 ] == PUSTE && plansza[i][j - 1] == PUSTE && plansza[i + 1][j -1] == PUSTE && plansza[i + 2][j - 1] == PUSTE)
			{
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 2][j] = PUSTE;

				plansza[i - 1][j - 1] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i + 1][j - 1] = klocek;
				plansza[i + 2][j - 1] = klocek;

				return true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i][j-3] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j-2] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j+1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j-3] = klocek;
				plansza[i][j-2] = klocek;
				plansza[i][j -1] = klocek;
				return true;
			}
		}
		break;
	case T:
		if (pozycja == 1) {
			if (plansza[i][j - 2] == PUSTE && plansza[i + 1][j - 1] == PUSTE)
			{
				plansza[i][j - 1] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i][j - 2] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i][j] = klocek;
				plansza[i + 1][j - 1] = klocek;

				return true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i][j - 2] == PUSTE && plansza[i - 1][j-1] == PUSTE && plansza[i + 1][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i][j-1] = klocek;
				plansza[i][j-2] = klocek;
				plansza[i - 1][j-1] = klocek;
				plansza[i + 1][j-1] = klocek;
				return true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i-1][j - 1] == PUSTE && plansza[i][j - 2] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j - 2] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i-1][j-1] = klocek;
				return true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i][j - 1] == PUSTE && plansza[i - 1][j - 1] == PUSTE && plansza[i + 1][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;

				plansza[i][j-1] = klocek;
				plansza[i - 1][j-1] = klocek;
				plansza[i + 1][j-1] = klocek;
				plansza[i][j] = klocek;
				return true;
			}
		}
		break;
	case O:
		if (plansza[i][j - 1] == PUSTE && plansza[i + 1][j - 1] == PUSTE)
		{
			plansza[i][j] = PUSTE;
			plansza[i][j + 1] = PUSTE;
			plansza[i + 1][j + 1] = PUSTE;
			plansza[i + 1][j] = PUSTE;

			plansza[i][j - 1] = klocek;
			plansza[i][j] = klocek;
			plansza[i + 1][j] = klocek;
			plansza[i + 1][j - 1] = klocek;

			return true;
		}
		break;
	case L:
		if (pozycja == 1) {
			if (plansza[i][j - 1] == PUSTE && plansza[i-1][j - 1] == PUSTE && plansza[i + 1][j - 1] == PUSTE)
			{
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				plansza[i - 1][j - 1] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i + 1][j - 1] = klocek;
				plansza[i + 1][j] = klocek;

				return true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i][j - 2] == PUSTE && plansza[i + 1][j - 2] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;

				plansza[i][j-1] = klocek;
				plansza[i][j - 2] = klocek;
				plansza[i + 1][j - 2] = klocek;
				plansza[i][j] = klocek;
				return true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i - 1][j - 2] == PUSTE && plansza[i][j - 1] == PUSTE && plansza[i + 1][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i][j-1] = klocek;
				plansza[i - 1][j - 2] = klocek;
				plansza[i - 1][j-1] = klocek;
				plansza[i + 1][j-1] = klocek;
				return true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i][j - 2] == PUSTE && plansza[i - 1][j] == PUSTE){
			plansza[i][j] = PUSTE;
			plansza[i][j - 1] = PUSTE;
			plansza[i][j + 1] = PUSTE;
			plansza[i - 1][j + 1] = PUSTE;

			plansza[i][j - 1] = klocek;
			plansza[i][j - 2] = klocek;
			plansza[i][j] = klocek;
			plansza[i - 1][j] = klocek;
			return true;
			}
		}
		break;
	case J:
		if (pozycja == 1) {
			if (plansza[i - 1][j - 1] == PUSTE && plansza[i][j - 1] == PUSTE && plansza[i + 1][j - 2] == PUSTE)
			{
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i - 1][j - 1] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i + 1][j - 1] = klocek;
				plansza[i + 1][j - 2] = klocek;

				return true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i][j - 2] == PUSTE && plansza[i - 1][j - 2] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;

				plansza[i][j-2]= klocek;
				plansza[i][j-1] = klocek;
				plansza[i][j] = klocek;
				plansza[i - 1][j-2] = klocek;
				return true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i][j - 1] == PUSTE && plansza[i - 1][j - 1] == PUSTE && plansza[i + 1][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i - 1][j + 1] = PUSTE;

				plansza[i][j-1] = klocek;
				plansza[i - 1][j-1] = klocek;
				plansza[i + 1][j-1] = klocek;
				plansza[i - 1][j] = klocek;
				return true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i][j - 2] == PUSTE && plansza[i + 1][j] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				plansza[i][j - 1] = klocek;
				plansza[i][j - 2] = klocek;
				plansza[i][j] = klocek;
				plansza[i + 1][j] = klocek;

				return true;
			}
		}
		break;
	case S:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i + 1][j - 2] == PUSTE && plansza[i][j - 1] == PUSTE)
			{
				plansza[i][j + 1] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i+ 1][j] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i + 1][j - 2] = klocek;
				plansza[i + 1][j-1] = klocek;

				return true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i][j - 2] == PUSTE && plansza[i - 1][j - 2] == PUSTE && plansza[i+1][j-1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i][j-1] = klocek;
				plansza[i][j - 2] = klocek;
				plansza[i - 1][j - 2] = klocek;
				plansza[i + 1][j-1] = klocek;

				return true;
			}
		}
			break;
	case Z:
		if(pozycja == 1 || pozycja == 3){
			if (plansza[i + 1][j - 2] == PUSTE && plansza[i][j - 2] == PUSTE && plansza[i+1][j-1] == PUSTE)
			{
				plansza[i][j - 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i+1][j + 1] = PUSTE;

				plansza[i][j - 2] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i+1][j] = klocek;
				plansza[i+1][j-1] = klocek;

				return true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i][j - 2] == PUSTE && plansza[i + 1][j - 2] == PUSTE && plansza[i - 1][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j-1] = PUSTE;

				plansza[i][j-1] = klocek;
				plansza[i-1][j - 1] = klocek;
				plansza[i][j-2] = klocek;
				plansza[i+1][j - 2] = klocek;
				return true;
			}
		}
		break;
	}
	return false;
}

void sprawdzIobroc(Klocek plansza[22][12], Klocek klocek, int &pozycja, int i, int j) {
	bool czyObrot = false;
	switch (klocek) {
	case I:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i][j + 1] == PUSTE && plansza[i][j - 1] == PUSTE && plansza[i][j - 2] == PUSTE) {
				plansza[i + 2][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i][j - 2] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i + 1][j] == PUSTE && plansza[i + 2][j] == PUSTE && plansza[i - 1][j] == PUSTE) {
				plansza[i + 1][j] = klocek;
				plansza[i + 2][j] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i][j] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j - 2] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j+1] = PUSTE;

				czyObrot = true;
			}
		}
		break;

	case T:
		if (pozycja == 1) {
			if (plansza[i - 1][j] == PUSTE) {
				plansza[i][j + 1] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j] = PUSTE;

				plansza[i+1][j] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i][j -1] = klocek;
				plansza[i][j] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i][j + 1] == PUSTE) {
				plansza[i - 1][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i+1][j] = PUSTE;
				plansza[i][j] = PUSTE;

				plansza[i][j + 1] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i][j] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i+1][j] == PUSTE) {
				plansza[i][j+1] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i][j] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i+1][j] = klocek;
				plansza[i][j + 1] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;

				plansza[i][j + 1] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i][j] = klocek;
				czyObrot = true;
			}
		}

	case L: 
		if (pozycja == 1) {
			if (plansza[i][j + 1] == PUSTE && plansza[i][j - 1] == PUSTE && plansza[i + 1][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i + 1][j - 1] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i + 1][j] == PUSTE && plansza[i-1][j-1] ==  PUSTE && plansza[i-1][j] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i - 1][j - 1] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i + 1][j] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i][j + 1] == PUSTE && plansza[i][j-1] == PUSTE && plansza[i - 1][j+1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i - 1][j + 1] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i + 1][j] == PUSTE && plansza[i + 1][j + 1] == PUSTE && plansza[i-1][j] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i - 1][j + 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i + 1][j + 1] = klocek;
				czyObrot = true;
			}
		}
		break;
	case J:
		if (pozycja == 1) {
			if (plansza[i][j + 1] == PUSTE && plansza[i][j - 1] == PUSTE && plansza[i - 1][j - 1]) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i - 1][j - 1] = klocek;
				
				czyObrot = true;
			}
		}
		else if (pozycja == 2) {
			if (plansza[i + 1][j] == PUSTE && plansza[i - 1][j + 1]) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i - 1][j + 1] = klocek;
				plansza[i + 1][j] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 3) {
			if (plansza[i][j - 1] == PUSTE && plansza[i + 1][j + 1] == PUSTE && plansza[i][j+1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i - 1][j + 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i + 1][j + 1] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 4) {
			if (plansza[i - 1][j] == PUSTE && plansza[i + 1][j] == PUSTE && plansza[i+1][j-1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i + 1][j-1] = klocek;
				czyObrot = true;
			}
		}
		break;
	case S:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i][j - 1] == PUSTE && plansza[i - 1][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j + 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i - 1][j - 1] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i][j + 1] == PUSTE && plansza[i + 1][j - 1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i - 1][j - 1] = PUSTE;
			
				plansza[i][j] = klocek;
				plansza[i][j + 1] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i + 1][j - 1] = klocek;
				czyObrot = true;
			}
		}
		break;
	case Z:
		if (pozycja == 1 || pozycja == 3) {
			if (plansza[i + 1][j-1] == PUSTE && plansza[i-1][j] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i + 1][j] = PUSTE;
				plansza[i + 1][j + 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i - 1][j] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i + 1][j - 1] = klocek;
				czyObrot = true;
			}
		}
		else if (pozycja == 2 || pozycja == 4) {
			if (plansza[i][j + 1] == PUSTE && plansza[i - 1][j-1] == PUSTE) {
				plansza[i][j] = PUSTE;
				plansza[i - 1][j] = PUSTE;
				plansza[i][j - 1] = PUSTE;
				plansza[i + 1][j - 1] = PUSTE;

				plansza[i][j] = klocek;
				plansza[i][j - 1] = klocek;
				plansza[i + 1][j] = klocek;
				plansza[i + 1][j + 1] = klocek;
				czyObrot = true;
			}
		}
		break;
	}

	if (czyObrot) {
		if (pozycja == 4) {
			pozycja = 1;
		}
		else {
			pozycja++;
		}
	}

}


void przesunWdol(Klocek plansza[22][12], int wiersz) {
	for (int i = wiersz; i > 0; i--) {
		for (int j = 1; j < 11; j++) {
			plansza[i][j] = plansza[i-1][j];
		}
	}
	for (int j = 1; j < 11; j++)
		plansza[1][j] = PUSTE;
}

void sprawdzWiersze(Klocek plansza[22][12]) {
	int pelne;
	for (int i = 20; i>0; i--) {
		pelne = 0;
		for (int j = 1; j <= 10; j++) {
			if (plansza[i][j] != PUSTE) {
				pelne++;
			}
			else {
				break;
			}
		}
		if (pelne == 10) {
			przesunWdol(plansza, i);
			i++;
		}
	}
}

void tworzPlansze(Klocek plansza[22][12]) {
	for (int i = 0;i < 22;i++)//wiersze
	{
		for (int j = 0;j < 12;j++)//kolumny
		{
			if (i == 0 || i == 21 || j == 0 || j == 11)
				plansza[i][j] = SCIANA;
			else
				plansza[i][j] = PUSTE;
		}
	}
}

bool sprawdzCzyKoniec(Klocek plansza[22][12]) {
	
	for (int j = 1; j < 11; j++) {
		if (plansza[1][j] != PUSTE) {
			return true;
		}
	}

	return false;
}




// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) {
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
	bool isEnd = false;
	int pozycja = 1;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *eti;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;


	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pe³noekranowy
	//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
	//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Kacper Legowski 160407");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;

	worldTime = 0;
	distance = 0;
	etiSpeed = 1;

	double ruch = 0;
	int x = 180, y = 100;

	struct ObramowanieKlocka obramowanie;

	//obramowanie.klocek = klocek;
	ObramowanieKlocka klocki[10];
	int liczba_klockow = 0;

	double speed = 1.0;

	bool ok = true;

	srand(time(NULL));
	Klocek klocek = Klocek(rand() % 7);
	//Klocek klocek = S;

	Klocek plansza[22][12];
	tworzPlansze(plansza);

	int starti = 2, startj = 5;

	WstawKlocekNaPlansze(plansza, klocek, starti, startj);

	while (!quit) {
		t2 = SDL_GetTicks();

		// w tym momencie t2-t1 to czas w milisekundach,
		// jaki uplyna³ od ostatniego narysowania ekranu
		// delta to ten sam czas w sekundach
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		distance += etiSpeed * delta;

		//SDL_FillRect(screen, NULL, czarny);

		RysujPlansze(screen, plansza);

		ruch += delta;
		
		
		if(isEnd){
			tworzPlansze(plansza);
			speed = 100000000000;
			sprintf(text, "KONIEC GRY NACISNIJ n ABY ROZPOCZAC");
			DrawString(screen, 200, 250, text, charset);
			sprintf(text, "NOWA ROZGRYWKE LUB ESC ABY WYJSC");
			DrawString(screen, 200, 260, text, charset);
		}else
		if (ruch >= speed)//przesuniecie klocka o 1 w dol
		{
			ruch = 0;
			if (SprawdzKlocekDol(plansza, klocek, starti, startj, pozycja)) {
				starti++;
			}
			else 
			{
				if (sprawdzCzyKoniec(plansza)) {
					isEnd = true;
				}
				else {

				
				klocek = Klocek(rand() % 7);
				// klocek = S;
				pozycja = 1;
				starti = 2, startj = 5;
				sprawdzWiersze(plansza);
				
				WstawKlocekNaPlansze(plansza, klocek, starti, startj);
				}
			}
		}


		
		
		//DrawSurface(screen, eti,
		//	SCREEN_WIDTH / 2 + sin(distance) * SCREEN_HEIGHT / 3,
		//	SCREEN_HEIGHT / 2 + cos(distance) * SCREEN_HEIGHT / 3);


		//		DrawScreen(screen, plane, ship, charset, worldTime, delta, vertSpeed);

		// naniesienie wyniku rysowania na rzeczywisty ekran
		//		SDL_Flip(screen);

		fpsTimer += delta;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

		// tekst informacyjny
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		sprintf_s(text, "Tetris, czas trwania = %.1lf s  %.0lf klatek / s", worldTime, fps);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		sprintf_s(text, "Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		// obs³uga zdarzeñ (o ile jakieœ zasz³y)
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				else if (event.key.keysym.sym == SDLK_UP)
				{
					if (speed != 0)
						speed -= 0.1;
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					if (SprawdzKlocekDol(plansza, klocek, starti, startj, pozycja))
					{
						starti++;
					}
					else
					{
						break;
					}
				}
				else if (event.key.keysym.sym == SDLK_LEFT)
				{
					if (SprawdzKlocekLewo(plansza, klocek, starti, startj, pozycja))
						startj--;
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					if (SprawdzKlocekPrawo(plansza, klocek, starti, startj, pozycja))
						startj++;
				}
				else if (event.key.keysym.sym == SDLK_n) {
					isEnd = false;
					speed = 1;
					tworzPlansze(plansza);
					starti = 1, startj = 5;
					ruch = 0;
				}
				else if (event.key.keysym.sym == SDLK_SPACE) {
					sprawdzIobroc(plansza, klocek, pozycja, starti, startj);
					}
				break;
			case SDL_KEYUP:
				etiSpeed = 1.0;
				break;
			case SDL_QUIT:
				quit = 1;
				break;


			};
			frames++;
		};
	}

	// zwolnienie powierzchni
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
