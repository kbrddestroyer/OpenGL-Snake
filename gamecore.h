/*
**************************************
*		SNAKE GAME BETA V1.1		 *
*			MAIN LIBRARY			 *
*		By Keyboard Destroyer		 *
*									 *
* |	Last review: 30.03.2021 23:19	|*
* |	By:			 Keyboard Destroyer	|*
* |	GitHub:		 NONE				|*
* |	Descrition:	 Just snake game :/	|*
**************************************
*
* 
*			TODO LIST
* Add algorythms
* Try dejikstra, a*, Lee (Wave Tracing), Floyd, etc.
* Add UI
*/

#pragma once
#include <Windows.h>
#include <vector>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#include "glut.h"
#include "botcore.h"
#include "constants.h"

using namespace std;

#ifndef GAMECORE_H
#define GAMECORE_H

struct GameInfo				// Main structure. Contains player head coordinates, apple coordinates and lentgth of the snake 
{
	int x;					// Head X position
	int y;					// Head Y position
	int apple_x;			// Apple X position
	int apple_y;			// Apple Y position
	int rating;				// Player score or snake length
};

#endif

GameInfo	player;			// Main struckture
Bot			botcore;
int		map[MAP_SIZE_X][MAP_SIZE_Y];

int		default_x = rand() % (MAP_SIZE_X - 2) + 1;
int		default_y = rand() % (MAP_SIZE_Y - 2) + 1;
int		default_apple_x = rand() % (MAP_SIZE_X - 2) + 1;
int		default_apple_y = rand() % (MAP_SIZE_Y - 2) + 1;

int		direction = 4;
bool	running = false;
bool	started = false;

vector <pair <int, int>> Vector;	// Coordinates of snake segments

void startgame		(void);			// Main function. Starts all threads (OpenGl)
void snake_update	(int);
/*			OpenGL Func			*/
void SetPixel		(int, int);
void init			(void);
void Render			(void);
/*			Core Func			*/
void drawMap		(void);
void generateApple	(void);
void bot			(int);
void keyboard		(unsigned char, int, int);

void startgame()
{
	player.rating = 0;
	for (int i = 0; i < MAP_SIZE_X; i++)
		for (int j = 0; j < MAP_SIZE_Y; j++)
		{
			if (i == 0 || j == 0 || i == MAP_SIZE_X - 1 || j == MAP_SIZE_Y - 1) map[i][j] = -1;
			else
				map[i][j] = 0;
		}
	player.x = default_x;
	player.y = default_y;
	player.apple_x = default_apple_x;
	player.apple_y = default_apple_y;
	map[player.x][player.y] = -1;
	map[player.apple_x][player.apple_y] = -2;

	init();
}

void init(void)
{
	Vector.push_back({ player.x, player.y });
	glClearColor(0.0, 0.0, 0.0, 0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-1, MAP_SIZE_X, -1, MAP_SIZE_Y);
	glPointSize(PIXEL_SIZE);
	glLineWidth(PIXEL_SIZE);
	glutDisplayFunc(Render);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(DELAY_TIME, snake_update, 5);
	glutTimerFunc(DELAY_TIME, bot, 5);
	glutMainLoop();
}

void snake_update(int value)
{
	if (running)
	{
		switch (direction)
		{
		case 1:
		{
			player.x--;
			break;
		}
		case 2:
		{
			player.x++;
			break;
		}
		case 3:
		{
			player.y++;
			break;
		}
		case 4:
		{
			player.y--;
			break;
		}
		}
		if (map[player.x][player.y] == -1)
		{
			running = false;
		}
		if (map[player.x][player.y] == 0)
		{
			map[Vector[Vector.size() - 1].first][Vector[Vector.size() - 1].second] = 0;
			for (int i = Vector.size() - 1; i > 0; i--)
				Vector[i] = Vector[i - 1];
		}
		if (map[player.x][player.y] == -2)
		{
			player.rating++;
			Vector.push_back(Vector[Vector.size() - 1]);
			for (int i = Vector.size() - 1; i > 0; i--)
				Vector[i] = Vector[i - 1];
			generateApple();
		}
		Vector[0].first = player.x;
		Vector[0].second = player.y;
		map[Vector[0].first][Vector[0].second] = -1;
		glutPostRedisplay();
		glutTimerFunc(DELAY_TIME, snake_update, value);
	}
}

void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	drawMap();
	glutSwapBuffers();
}

void SetPixel(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

void  drawMap()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(0, MAP_SIZE_Y - 1);
	glVertex2i(MAP_SIZE_X - 1, MAP_SIZE_Y - 1);
	glVertex2i(MAP_SIZE_X - 1, 0);
	glEnd();
	for (int i = 0; i < MAP_SIZE_X; i++)
	{
		for (int j = 0; j < MAP_SIZE_Y; j++)
		{
			if (map[i][j] != 0)
			{
				glColor3f(0.0, 1.0, 0.0);
				SetPixel(i, j);
			}
		}
	}

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex2i(player.apple_x, player.apple_y);
	glEnd();
	if (player.rating == 0)
	{
		glBegin(GL_POINTS);
		glColor3f(0.0, 1.0, 0.0);
		for (size_t i = 0; i < Vector.size(); i++)
		{
			glVertex2i(Vector[i].first, Vector[i].second);
		}
		glEnd();
	}
	else
	{
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 1.0, 0.0);
		for (size_t i = 0; i < Vector.size(); i++)
		{
			glVertex2i(Vector[i].first, Vector[i].second);
		}
		glEnd();
	}
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	if (!running && !started)
	{
		glutTimerFunc(DELAY_TIME, snake_update, 5);
		running = true;
		started = true;
	}
	if (!running && started)
	{
		glutDestroyWindow(glutGetWindow());
		exit(0);
	}
	switch (key)
	{
		case 'a':
		{
			if (direction != 2)
				direction = 1;
			break;
		}
		case 'd':
		{
			if (direction != 1)
				direction = 2;
			break;
		}
		case 'w':
		{
			if (direction != 4)
				direction = 3;
			break;
		}
		case 's':
		{
			if (direction != 3)
				direction = 4;
			break;
		}
	}
}

void generateApple()
{
	do
	{
		player.apple_x = rand() % (MAP_SIZE_X - 2) + 1;
		player.apple_y = rand() % (MAP_SIZE_Y - 2) + 1;
	} while (map[player.apple_x][player.apple_y] == -1);
	map[player.apple_x][player.apple_y] = -2;
}

void bot(int value)
{
	botcore.bot_logic({ player.x, player.y }, {player.apple_x, player.apple_y}, map, &direction);
	glutTimerFunc(DELAY_TIME / 4, bot, 0);
}