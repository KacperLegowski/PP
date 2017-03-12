#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "conio2.h"
#include <stdio.h>
#include <ctime>

using namespace std;

enum kierunek { GORA, PRAWO, DOL, LEWO };
enum sciana { PODST = 1, PUSTA = 0, EXTRA1 = 3, EXTRA2 = 4, DRZWI_OTWARTE = 5, DRZWI_ZAMKNIETE = 2, POCZATEK = 8, KONIEC = 9 };

char pobierz_znak_sciany(sciana s);
void rysuj_labirynt(int x, int y, sciana** labirynt, kierunek kier, int n, int m);
sciana** wczytaj(char* nazwaPliku, int &n, int &m, int &startx, int &starty, kierunek &kier);
int sprawdz_ruch(int y, int x, sciana s);
int sprawdz_drzwi(int x, int y, sciana s);
void rysuj_czlowieka(int x, int y, kierunek kier);
void rysuj_ruchy(int ruchyx, int ruchyy);
void rysuj_czas(int ruchyx, int ruchyy);
void rysuj_wartosc_ruchu(int ruchyx, int ruchyy, int ruchy);
void rysuj_wartosc_czasu(int ruchyx, int ruchyy, clock_t czas);
void rysuj_pomoc(int ruchyx, int ruchyy);
void ustaw_wartosci_poczatkowe(int &x, int &y, int m, int &nast_x, int &nast_y, int startx, int starty, int &ruchy, int &ruchyx, int &ruchyy, kierunek kier);
bool sprawdz_labirynt(sciana** lab_ed, int h, int w);
sciana** wyswietl_edytor(int &startx, int &starty, int &n, int &m, kierunek &kier);
void wygrana(int ruchy, clock_t czas);
void otworz_drzwi(int m);
void zamknij_drzwi(int m);
void graj(int &startx, int &starty, sciana** labirynt, int n, int m, kierunek kier);
void wyswietl_pomoc();
void wyswietl_menu();
void rysuj_perspektywe(int x, int y, sciana** labirynt, kierunek kier, int n, int m);

int main()
{
	textmode(FULLSCREEN);
	_setcursortype(0);
	wyswietl_menu();

	return 0;
}

char pobierz_znak_sciany(sciana s)
{
	switch (s)
	{
	case PODST:
		return '#';
	case PUSTA:
		return ' ';
	case EXTRA1:
		return '%';
	case EXTRA2:
		return '&';
	case DRZWI_OTWARTE:
		return '/';
	case DRZWI_ZAMKNIETE:
		return '?';
	case POCZATEK:
		return 'P';
	case KONIEC:
		return 'K';
	default: 
		return '#';
	}
}

void rysuj_labirynt(int x, int y, sciana** labirynt, kierunek kier, int n, int m)
{
	clrscr();
	textcolor(WHITE);
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{

			gotoxy(j, i);
			cout << pobierz_znak_sciany(labirynt[i - 1][j - 1]);
		}
	}
	rysuj_perspektywe(x, y, labirynt, kier, n, m);
}

sciana** wczytaj(char* nazwaPliku, int &n, int &m, int &startx, int &starty, kierunek &kier)
{
	FILE * plik;
	plik = fopen(nazwaPliku, "r");
	if (plik == NULL)
		return NULL;

	fscanf(plik, "%d", &n);
	fscanf(plik, "%d", &m);
	fscanf(plik, "%d", &kier);
	int a;
	sciana s;
	sciana** labirynt = (sciana**)malloc(n * sizeof(sciana*));
	for (int i = 0;i < n;i++)
	{
		labirynt[i] = (sciana*)malloc(m * sizeof(sciana));
		for (int j = 0;j < m;j++)
		{
			fscanf(plik, "%d", &a);
			switch (a)
			{
			case 1:
				s = PODST;
				break;

			case 2:
				s = DRZWI_ZAMKNIETE;
				break;

			case 3:
				s = EXTRA1;
				break;

			case 4:
				s = EXTRA2;
				break;

			case 5:
				s = DRZWI_OTWARTE;
				break;

			case 8:
				s = POCZATEK;
				startx = i + 1;
				starty = j + 1;
				break;

			case 9:
				s = KONIEC;
				break;

			case 0:
				s = PUSTA;
				break;
			}
			labirynt[i][j] = s;
		}
	}
	fclose(plik);
	return labirynt;
}

int sprawdz_ruch(int y, int x, sciana s)
{
	if (s == PUSTA)
		return 1;
	else if (s == DRZWI_OTWARTE)
		return 2;
	else if (s == KONIEC)
		return -1;
	else
		return 0;
}

int sprawdz_drzwi(int x, int y, sciana s)
{
	if (s == DRZWI_ZAMKNIETE)
		return 0;
	else if (s == DRZWI_OTWARTE)
		return 1;
	else
		return -1;
}

void rysuj_czlowieka(int x, int y, kierunek kier)
{
	gotoxy(y, x);
	textcolor(YELLOW);
	switch (kier)
	{
	case GORA:
		cout << "^";
		break;

	case DOL:
		cout << "v";
		break;

	case LEWO:
		cout << "<";
		break;

	case PRAWO:
		cout << ">";
		break;

	}
}

void rysuj_ruchy(int ruchyx, int ruchyy)
{
	gotoxy(ruchyx, ruchyy);
	textcolor(YELLOW);
	clreol();
	cout << "Ruchy: ";
}

void rysuj_czas(int ruchyx, int ruchyy)
{
	gotoxy(ruchyx, ruchyy + 1);
	textcolor(YELLOW);
	clreol();
	cout << "Czas: ";
}

void rysuj_wartosc_ruchu(int ruchyx, int ruchyy, int ruchy)
{
	gotoxy(ruchyx + 7, ruchyy);
	clreol();
	cout << ruchy;
}

void rysuj_wartosc_czasu(int ruchyx, int ruchyy, clock_t czas)
{
	gotoxy(ruchyx + 7, ruchyy + 1);
	clreol();
	double aktualnyCzas = czas / (double)CLOCKS_PER_SEC;
	cout << aktualnyCzas << " s";
}

void rysuj_pomoc(int ruchyx, int ruchyy)
{
	gotoxy(ruchyx, ruchyy + 2);
	cout << "Pomoc: ";
	wyswietl_pomoc();
}

void ustaw_wartosci_poczatkowe(int &x, int &y, int m, int &nast_x, int &nast_y, int startx, int starty, int &ruchy, int &ruchyx, int &ruchyy, kierunek kier)
{
	cout << " ";
	x = startx;
	y = starty;
	ruchyx = 1;
	ruchyy = (m > 20 ? m : 20) + 5;
	ruchy = 0;
	rysuj_ruchy(ruchyx, ruchyy);
	rysuj_czas(ruchyx, ruchyy);
	rysuj_czlowieka(x, y, kier);
	nast_x = x;
	nast_y = y;
}

bool sprawdz_labirynt(sciana** lab_ed, int h, int w)
{
	bool ok = true;
	for (int i = 0;i < h;i++)
	{
		for (int j = 0;j < w;j++)
		{
			if (i < h - 1 || j < w - 1)
			{
				if (lab_ed[i][j] == PUSTA && lab_ed[i + 1][j] == PUSTA && lab_ed[i][j + 1] == PUSTA && lab_ed[i + 1][j + 1] == PUSTA)
					return false;
			}
			if (i == 0 || j == 0 || i == h - 1 || j == w - 1)
			{
				if (lab_ed[i][j] == PUSTA)
					return false;
			}
		}
	}
	return ok;
}

sciana** wyswietl_edytor(int &startx, int &starty, int &n, int &m, kierunek &kier)
{
	clrscr();
	textcolor(WHITE);
	int n1, m1;
	sciana** lab_ed;
	int kier1;
	bool czy_start = false, czy_koniec = false;
	gotoxy(1, 1);
	system("cls");
	cout << "Podaj szerokosc labiryntu: ";
	cin >> n1;
	cout << "Podaj wysokosc labiryntu: ";
	cin >> m1;

	char a;
	sciana s;
	lab_ed = (sciana**)malloc(n1 * sizeof(sciana*));
	do
	{
		clrscr();
		gotoxy(n1 + 5, 1);
		cout << "| 0 - pusta sciana";
		gotoxy(n1 + 5, 2);
		cout << "| 1 - sciana podstawowa";
		gotoxy(n1 + 5, 3);
		cout << "| 2 - drzwi zamkniete";
		gotoxy(n1 + 5, 4);
		cout << "| 3 - sciana extra1";
		gotoxy(n1 + 5, 5);
		cout << "| 4 - sciana extra2";
		gotoxy(n1 + 5, 6);
		cout << "| 5 - drzwi otwarte";
		gotoxy(n1 + 5, 7);
		cout << "| 8 - poczatek";
		gotoxy(n1 + 5, 8);
		cout << "| 9 - koniec";
		gotoxy(n1 + 5, 9);
		cout << "| q - wyjscie z edytora";
		gotoxy(1, 1);
		for (int i = 0;i < n1;i++)
		{
			lab_ed[i] = (sciana*)malloc(m1 * sizeof(sciana));
			for (int j = 0;j < m1;j++)
			{
				do
				{
					a = getch();
					if (a == 'q')
						return NULL;
				} while (!isdigit(a));
				cout << a;
				s = (sciana)(a - '0');
				if (s == POCZATEK)
				{
					czy_start = true;
					startx = i + 1;
					starty = j + 1;
				}
				if (s == KONIEC)
					czy_koniec = true;
				lab_ed[i][j] = s;
			}
			cout << endl;
		}
	} while (!czy_koniec || !czy_start || !sprawdz_labirynt(lab_ed, n1, m1));
	gotoxy(1, m1 > 9 ? m1 + 2 : 10);
	cout << "Aby zapisac labirynt nacisnij s, aby wyjsc dowolny przycisk.";
	a = getch();
	if (a == 's')
	{
		cout << "Podaj poczatkowy kierunek 0-Gora, 1-Prawo, 2-Dol, 3-Lewo: ";
		cin >> kier1;
		cout << "Aby zapisac labirynt podaj nazwe: ";
		char nazwaPliku[20];
		cin >> nazwaPliku;
		FILE * pFile;

		pFile = fopen(nazwaPliku, "w");
		if (pFile != NULL)
		{
			fprintf(pFile, "%d\n", n1);
			fprintf(pFile, "%d\n", m1);
			fprintf(pFile, "%d\n", kier1);
			for (int i = 0;i < n1;i++)
			{
				for (int j = 0;j < m1;j++)
				{
					fprintf(pFile, "%d ", lab_ed[i][j]);
				}
				fprintf(pFile, "\n");
			}
		}

		fclose(pFile);

		cout << "Aby zagrac w stworzonym labiryncie wcisnij w, aby wyjsc dowolny przycisk.";
		a = getch();
		if (a == 'w')
		{
			n = n1;
			m = m1;
			kier = (kierunek)kier1;
			return lab_ed;
		}
	}
	return NULL;
}

void wygrana(int ruchy, clock_t czas)
{
	clrscr();
	gotoxy(25, 5);
	cout << "Wygrales !" << endl;
	cout << "Przeszedles labirynt w " << ruchy << " ruchach!" << endl;
	double aktualnyCzas = czas / (double)CLOCKS_PER_SEC;
	cout << "Zajelo to Tobie " << aktualnyCzas << " s!";
	_sleep(5000);
}

void otworz_drzwi(int m)
{
	textcolor(WHITE);
	int przesuniecie = m + 10;
	for (int i = 2;i <= 45;i++)
	{
		for (int h = 1;h <= 16;h++)
		{
			gotoxy(przesuniecie + i, 1 + h);
			cout << " ";
		_sleep(1);
		}
	}
}

void zamknij_drzwi(int m)
{
	textcolor(WHITE);
	int przesuniecie = m + 10;
	for (int i = 2;i <= 45;i++)
	{
		for (int h = 1;h <= 16;h++)
		{
			gotoxy(przesuniecie + i, 1 + h);
			cout << "?";
			_sleep(1);
		}
	}
}

void graj(int &startx, int &starty, sciana** labirynt, int n, int m, kierunek kier)
{
	int ruch, drzwi;
	int x, y;//aktualna pozycja
	int nast_x, nast_y;//nastepna pozycja wzgledem kierunku
	int ruchy, ruchyx, ruchyy;//suma ruchow,pozycja do wyswietlenia infrmacji na temat ruchu
	ustaw_wartosci_poczatkowe(x, y, m, nast_x, nast_y, startx, starty, ruchy, ruchyx, ruchyy, kier);
	int a;
	clock_t graj = clock();
	clock_t czas;
	bool czy_pomoc = false;
	do
	{

		a = getch();
		czas = clock() - graj;
		switch (a)
		{

		case 80: // w dol - do tylu

			switch (kier)
			{
			case GORA:
				nast_x += 1;
				break;

			case PRAWO:
				nast_y -= 1;
				break;

			case DOL:
				nast_x -= 1;
				break;

			case LEWO:
				nast_y += 1;
				break;
			}
			if (nast_x - 1 < 0 || nast_y - 1 < 0)
				ruch = 0;
			else
				ruch = sprawdz_ruch(nast_y, nast_x, labirynt[nast_x - 1][nast_y - 1]);
			if (ruch >= 1)
			{
				cout << " ";
				if (ruch == 2)
				{
					switch (kier)
					{
					case GORA:
						nast_x += 1;
						break;

					case PRAWO:
						nast_y -= 1;
						break;

					case DOL:
						nast_x -= 1;
						break;

					case LEWO:
						nast_y += 1;
						break;
					}
				}
				x = nast_x;
				y = nast_y;
				rysuj_czlowieka(x, y, kier);
				rysuj_perspektywe(x, y, labirynt, kier, n, m);
				ruchy += 1;
				rysuj_wartosc_ruchu(ruchyx, ruchyy, ruchy);
				rysuj_wartosc_czasu(ruchyx, ruchyy, czas);
			}
			else if (ruch == -1)
			{
				wygrana(ruchy, czas);
				return;
			}
			else
			{
				nast_x = x;
				nast_y = y;
			}
			break;

		case 72: // w gore - naprzod
			switch (kier)
			{
			case GORA:
				nast_x -= 1;
				break;

			case PRAWO:
				nast_y += 1;
				break;

			case DOL:
				nast_x += 1;
				break;

			case LEWO:
				nast_y -= 1;
				break;
			}
			if (nast_x - 1 < 0 || nast_y - 1 < 0)
				ruch = 0;
			else
				ruch = sprawdz_ruch(nast_y, nast_x, labirynt[nast_x - 1][nast_y - 1]);
			if (ruch >= 1)
			{
				cout << " ";
				if (ruch == 2)
				{
					switch (kier)
					{
					case GORA:
						nast_x -= 1;
						break;

					case PRAWO:
						nast_y += 1;
						break;

					case DOL:
						nast_x += 1;
						break;

					case LEWO:
						nast_y -= 1;
						break;
					}
				}
				x = nast_x;
				y = nast_y;
				rysuj_czlowieka(x, y, kier);
				rysuj_perspektywe(x, y, labirynt, kier, n, m);
				ruchy += 1;
				rysuj_wartosc_ruchu(ruchyx, ruchyy, ruchy);
				rysuj_wartosc_czasu(ruchyx, ruchyy, czas);
			}
			else if (ruch == -1)
			{
				wygrana(ruchy, czas);
				return;
			}
			else
			{
				nast_x = x;
				nast_y = y;
			}
			break;

		case 75: // w lewo - obrot w lewo
			if (kier == GORA)
				kier = LEWO;
			else
				kier = (kierunek)(kier - 1);
			rysuj_czlowieka(x, y, kier);
			rysuj_perspektywe(x, y, labirynt, kier, n, m);
			break;

		case 77: // w prawo - obrot w prawo
			if (kier == LEWO)
				kier = GORA;
			else
				kier = (kierunek)(kier + 1);
			rysuj_czlowieka(x, y, kier);
			rysuj_perspektywe(x, y, labirynt, kier, n, m);
			break;

		case 'd': //otwieranie i zamykanie drzwi
			switch (kier)
			{
			case GORA:
				nast_x -= 1;
				break;

			case PRAWO:
				nast_y += 1;
				break;

			case DOL:
				nast_x += 1;
				break;

			case LEWO:
				nast_y -= 1;
				break;
			}
			if (nast_x - 1 < 0 || nast_y - 1 < 0)
				drzwi = -1;
			else
				drzwi = sprawdz_drzwi(nast_x, nast_y, labirynt[nast_x - 1][nast_y - 1]);
			if (drzwi == 0)//zamkniete - otworz
			{
				labirynt[nast_x - 1][nast_y - 1] = DRZWI_OTWARTE;
				gotoxy(nast_y, nast_x);
				textcolor(WHITE);
				cout << "/";
				textcolor(YELLOW);
				gotoxy(y, x);
				otworz_drzwi(m);
				rysuj_perspektywe(x, y, labirynt, kier, n, m);
			}
			else if (drzwi == 1)//otwarte - zamknij
			{
				labirynt[nast_x - 1][nast_y - 1] = DRZWI_ZAMKNIETE;
				gotoxy(nast_y, nast_x);
				textcolor(WHITE);
				cout << "?";
				textcolor(YELLOW);
				gotoxy(y, x);
				zamknij_drzwi(m);
			}
			nast_x = x;
			nast_y = y;
			break;

		case 'r': //reset
			graj = clock();
			ustaw_wartosci_poczatkowe(x, y, m, nast_x, nast_y, startx, starty, ruchy, ruchyx, ruchyy, kier);
			break;

		case 'h': //pomoc
			textcolor(RED);
			if (!czy_pomoc)
			{
				czy_pomoc = true;
				rysuj_pomoc(ruchyx, ruchyy);
			}
			else
			{
				czy_pomoc = false;
				for (int i = 1;i <= 7;i++)
				{
					gotoxy(ruchyx, ruchyy + 1 + i);
					clreol();
				}
			}
			break;
		}
		gotoxy(y, x);
	} while (a != 'q');
}

void wyswietl_pomoc()
{
	cout << "strzalki lewo/prawo -> zmiana kierunku patrzenia o 90 stopni w wybranym kierunku" << endl;
	cout << "strzalka w gore -> poruszanie sie do przodu" << endl;
	cout << "strzalka w dol -> poruszanie sie do tylu" << endl;
	cout << "d -> otwieranie i zamykanie drzwi" << endl;
	cout << "h -> wyswietlenie pomocy" << endl;
	cout << "r -> restart \"gry\"" << endl;
	cout << "q -> opuszczenie programu" << endl;
}

void wyswietl_menu()
{
	clrscr();
	textcolor(WHITE);
	gotoxy(1, 1);
	settitle("Kacper Legowski 160407");
	int a;//aktualnie przetwarzany znak
	int n, m;//wysokosc i szerokosc labiryntu
	kierunek kier;//kierunek patrzenia
	sciana** labirynt;//labirynt
	int startx, starty;//poczatkowa pozycja
	do
	{
		clrscr();
		gotoxy(1, 1);
		textcolor(WHITE);
		cout << "s -> start" << endl;
		cout << "i -> wczytanie domyslnego labiryntu" << endl;
		cout << "h -> wyswietlenie pomocy (w grze)" << endl;
		cout << "e -> wejscie do edytora" << endl;
		cout << "o -> odczytanie labiryntu z pliku" << endl;
		cout << "r -> restart \"gry\"" << endl;
		cout << "q -> opuszczenie programu" << endl;
		a = getch();
		switch (a)
		{
		case 's': //rozpoczecie gry
			labirynt = wczytaj("test.txt", n, m, startx, starty, kier);
			if (labirynt != NULL)
			{
				rysuj_labirynt(startx, starty, labirynt, kier, n, m);
				graj(startx, starty, labirynt, n, m, kier);
			}
			break;

		case 'i': //wczytanie domyslnego labiryntu
			labirynt = wczytaj("domyslny.txt", n, m, startx, starty, kier);
			if (labirynt != NULL)
			{
				rysuj_labirynt(startx, starty, labirynt, kier, n, m);
				graj(startx, starty, labirynt, n, m, kier);
			}
			break;

		case 'o': //otwarcie labiryntu z pliku
			system("cls");
			cout << "Podaj nazwe pliku: ";
			char nazwaPliku[20];
			cin >> nazwaPliku;
			labirynt = wczytaj(nazwaPliku, n, m, startx, starty, kier);
			if (labirynt != NULL)
			{
				rysuj_labirynt(startx, starty, labirynt, kier, n, m);
				graj(startx, starty, labirynt, n, m, kier);
			}
			break;

		case 'e': //edytor
			labirynt = wyswietl_edytor(startx, starty, n, m, kier);
			if (labirynt != NULL)
			{
				rysuj_labirynt(startx, starty, labirynt, kier, n, m);
				graj(startx, starty, labirynt, n, m, kier);
			}
			break;

		}

	} while (a != 'q');
}

void rysuj_perspektywe(int x, int y, sciana** labirynt, kierunek kier, int n, int m)
{
	sciana widok[7][3];
	switch (kier)
	{
	case GORA:
		for (int i = 0;i < 7;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				if (x - i - 2 < 0)
				{
					widok[i][j] = PODST;
					continue;
				}
				if (j == 0)
				{
					if (y - 2 < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x - i - 2][y - 2];
				}
				else if (j == 1)
				{
					if (y - 1 < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x - i - 2][y - 1];
				}
				else
				{
					if (y < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x - i - 2][y];
				}
			}
		}
		break;
	case DOL:
		for (int i = 0;i < 7;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				if (x + i >= n)
				{
					widok[i][j] = PODST;
					continue;
				}
				if (j == 0)
				{
					if (y < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x + i][y];
				}
				else if (j == 1)
				{
					if (y - 1 < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x + i][y - 1];
				}
				else
				{
					if (y - 2 < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x + i][y - 2];
				}
			}
		}
		break;
	case PRAWO:
		for (int i = 0;i < 7;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				if (y + i >= m)
				{
					widok[i][j] = PODST;
					continue;
				}
				if (j == 0)
				{
					if (x - 2 < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x - 2][y + i];
				}
				else if (j == 1)
				{
					if (x - 1 < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x - 1][y + i];
				}
				else
				{
					if (x < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x][y + i];
				}
			}
		}
		break;
	case LEWO:
		for (int i = 0;i < 7;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				if (y - i - 2 < 0)
				{
					widok[i][j] = PODST;
					continue;
				}
				if (j == 0)
				{
					if (x  < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x][y - i - 2];
				}
				else if (j == 1)
				{
					if (x - 1 < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x - 1][y - i - 2];
				}
				else
				{
					if (x - 2 < 0)
						widok[i][j] = PODST;
					else
						widok[i][j] = labirynt[x - 2][y - i - 2];
				}
			}
		}
		break;
	}

	for (int i = 1;i <= 18;i++)
	{
		gotoxy(m + 5, i);
		clreol();
	}

	textcolor(WHITE);
	int ilosc_scian = 16;
	int k = 1;//poziom
	bool czy_sciana = false;
	char znak_sciany;
	int przesuniecie = m + 10;
	for (int j = 1;j <= 7;j++)//rysowanie lewej sciany
	{
		if (widok[k - 1][1] != PUSTA)
		{
			if (widok[k - 1][1] == DRZWI_OTWARTE)
			{
				int poczatek_prawej_sciany = (3 * (k + (8 - k) * 2));
				for (int i = (3 * k) - 1;i <= poczatek_prawej_sciany;i++)
				{
					gotoxy(przesuniecie + i, k);
					cout << "_";
					gotoxy(przesuniecie + i, ilosc_scian + (k * 2) - k);
					cout << "_";
				}
			}
			else
			{
				czy_sciana = true;
				znak_sciany = pobierz_znak_sciany(widok[k - 1][1]);
				break;
			}
		}

		if (widok[k - 1][0] != PUSTA)
		{
			for (int i = 1;i <= ilosc_scian;i++)
			{
				gotoxy(przesuniecie + (3 * k) - 2, k + i);
				cout << "|";
			}
			int i = (3 * k) - 1;
			int l = 0;
			znak_sciany = pobierz_znak_sciany(widok[k - 1][0]);
			for (;l < 2;l++)
			{
				gotoxy(przesuniecie + i + l, k);
				cout << "_";
				for (int h = 1;h <= ilosc_scian;h++)
				{
					gotoxy(przesuniecie + i + l, k + h);
					cout << znak_sciany;
				}
			}
			gotoxy(przesuniecie + i + l, k + 1);
			cout << "\\";
			for (int h = 1;h <= ilosc_scian - 1;h++)
			{
				gotoxy(przesuniecie + i + l, k + h + 1);
				cout << znak_sciany;
			}
			gotoxy(przesuniecie + i + l, ilosc_scian + k);
			cout << "/";
		}
		else
		{
			if (k - 2 >= 0 && widok[k - 2][0] != PUSTA)
			{
				for (int i = 1;i <= ilosc_scian;i++)
				{
					gotoxy(przesuniecie + (3 * k) - 2, k + i);
					cout << "|";
				}
			}
		}
		k++;
		ilosc_scian -= 2;

	}

	int j = 7;

	if (!czy_sciana)
	{
		for (int i = 1;i <= ilosc_scian;i++)
		{
			gotoxy(przesuniecie + (3 * k) - 2, k + i);
			cout << "|";
		}
		for (int l = 1;l <= 2;l++)
		{
			gotoxy(przesuniecie + (3 * k) - 2 + l, k);
			cout << "_";
			gotoxy(przesuniecie + (3 * k) - 2 + l, ilosc_scian + k);
			cout << "_";
		}
		for (int i = 1;i <= ilosc_scian;i++)
		{
			gotoxy(przesuniecie + (3 * k) + 1, k + i);
			cout << "|";
		}
	}
	else
	{
		for (int i = 1;i <= ilosc_scian;i++)
		{
			gotoxy(przesuniecie + (3 * k) - 2, k + i);
			cout << "|";
		}
		int poczatek_prawej_sciany = (3 * (k + (8 - k) * 2));
		for (int i = (3 * k) - 1;i <= poczatek_prawej_sciany;i++)
		{
			gotoxy(przesuniecie + i, k);
			cout << "_";
			for (int h = 1;h <= ilosc_scian;h++)
			{
				gotoxy(przesuniecie + i, k + h);
				cout << znak_sciany;
			}
		}
		for (int i = 1;i <= ilosc_scian;i++)
		{
			gotoxy(przesuniecie + poczatek_prawej_sciany + 1, k + i);
			cout << "|";
		}
		j = k - 1;
		k += (8 - k) * 2;
	}
	for (;j >= 1;j--)//rysowanie prawej sciany
	{
		if (widok[j - 1][2] != PUSTA)
		{
			int l = 1;
			int i = (3 * k) + 1;

			gotoxy(przesuniecie + i, j + 1);
			cout << "/";

			gotoxy(przesuniecie + i, j + ilosc_scian + 2);
			cout << "\\";
			znak_sciany = pobierz_znak_sciany(widok[j - 1][2]);
			for (;l <= 2;l++)
			{
				gotoxy(przesuniecie + i + l, j);
				cout << "_";
				for (int h = 1;h <= ilosc_scian + 2;h++)
				{
					gotoxy(przesuniecie + i + l, j + h);
					cout << znak_sciany;
				}
			}
			ilosc_scian += 2;
			for (int i = 1;i <= ilosc_scian;i++)
			{
				gotoxy(przesuniecie + (3 * k) + 1 + l, j + i);
				cout << "|";
			}
		}
		else
		{
			if (j - 2 >= 0 && widok[j - 2][2] != PUSTA)
			{
				ilosc_scian += 2;
				for (int i = 1;i <= ilosc_scian;i++)
				{
					gotoxy(przesuniecie + (3 * k) + 4, j + i);
					cout << "|";
				}
			}
		}
		k++;
	}
}

