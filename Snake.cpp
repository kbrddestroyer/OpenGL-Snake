#include <vector>
#include <Windows.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#include "glut.h"
#include "gamecore.h"

int main(int argc, char** argv)
{
	//FreeConsole();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 100);
	glutInitWindowSize(WINDOWSIZE_W, WINDOWSIZE_H);
	glutCreateWindow("Snake Game");

	startgame();
}