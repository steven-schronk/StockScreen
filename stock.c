#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include <GL/glut.h>

#include "random.c"

struct dowpoint {
	double open;
	double close;
	double min;
	double max;
	int time;
};

typedef struct movinglistdata {
	char *pString; 		/* chars printed on screen */
	float value;		/* value of data - also printed on screen */
	int diff;			/* difference from last value >0 is positive, 0 is no_change, <0 is negative*/
} MovingListData;

void DrawString(int x, int y, char *string)
{
  int len, i;
  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
  }
}

/*
	draw arrow up or down, up arrows are green, down are red, neutral are white dashes
	updown = positive is up and a negative is down zero draws neutral
*/
void IconArrow(int updwn, int x, int y)
{
	glLineWidth(1.0);
	if(updwn > 0)
	{
		glColor3f(0.0, 0.8, 0.0);
		glBegin(GL_POLYGON);
			glVertex2i(x, y);
			glVertex2i(x+12, y);
			glVertex2i(x+6, y+12);
			glVertex2i(x, y);
		glEnd();
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINES);
			glVertex2i(x+12, y);
			glVertex2i(x+6, y+12);
		glEnd();
	} else if (updwn == 0) {
		glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_POLYGON);
			glVertex2i(x, y+3);
			glVertex2i(x+12, y+3);
			glVertex2i(x+12, y+6+3);
			glVertex2i(x, y+6+3);
			glVertex2i(x, y+3);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
			glVertex2i(x, y+9);
			glVertex2i(x+12, y+9);
		glEnd();
	} else {
		glColor3f(0.7, 0.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex2i(x, y+12);
			glVertex2i(x+12, y+12);
			glVertex2i(x+6, y);
			glVertex2i(x, y+12);
		glEnd();
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);
			glVertex2i(x+12, y+12);
			glVertex2i(x+6, y);
		glEnd();
	}
}

void DrawMovingList(int tx, int ty, int bx, int by, MovingListData *employees, int count)
{
	int offset = by - 40;
	char buf[12];

	/* draw outline of main window */
	glColor3f(0.929, 0.615, 0.309);
	glLineWidth(1.0);

	glBegin(GL_LINE_STRIP);
		glVertex2i(tx, ty);
		glVertex2i(tx, by);
		glVertex2i(bx,by);
		glVertex2i(bx,ty);
		glVertex2i(tx, ty);
	glEnd();

	/* draw title bar */
	glBegin(GL_LINES);
		glVertex2i(tx, by-25);
		glVertex2i(bx, by-25);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	DrawString(tx+10, by-18, "Best Performance");

	/* draw each of the data elements with icons */
	count--;
	while(count >= 0)
	{
		IconArrow(employees[count].diff, tx + 10, offset);
		glColor3f(1.0, 1.0, 1.0);
		DrawString(tx + 30, offset, employees[count].pString);

		snprintf(buf, 12, "%.2f", employees[count].value);
		DrawString(tx + 200, offset, buf);
		offset -= 20;
		count--;
	}
}

void DrawOther(void)
{
	int width, height, i;

	int candle_tx, candle_ty, candle_bx, candle_by;
	int vol_tx, vol_ty, vol_bx, vol_by;
	int candle_width = 10;

	int open, close, min, max;
	/* find width and height of window */
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	candle_tx = width - 30;  candle_ty = height - 30;
	candle_bx =  30; candle_by = 330;

	vol_tx = width - 30;  vol_ty = height - 700;
	vol_bx = 30; vol_by = 30;

	/* draw text name of graph */
	glColor3f(1.0, 1.0, 1.0);
	DrawString(width/2 - 150, height-20, "Dow Jones Industrial Average");

	/* Draw American Flag */
	/* Other Flags Will Need To Be Options Later */


	/* draw grid lines for candlestick chart */

	/* horizontal lines */
	glColor3f(.1, .1, .1); /* very dark gray */
	glLineWidth(1.0);
	for(i = candle_by; i < candle_ty; i += 75)
	{
		glBegin(GL_LINES);
			glVertex2i(candle_tx, i);
			glVertex2i(candle_bx, i);
		glEnd();
	}
	/* draw last horizontal line */
	glBegin(GL_LINES);
		glVertex2i(candle_tx, candle_ty);
		glVertex2i(candle_bx, candle_ty);
	glEnd();

	/* vertical lines */
	glLineWidth(1.0);
	for(i = candle_bx; i < candle_tx; i += 145)
	{
		glBegin(GL_LINES);
			glVertex2i(i, candle_ty);
			glVertex2i(i, candle_by);
		glEnd();
	}
	/* draw last vertical line */
	glBegin(GL_LINES);
		glVertex2i(candle_tx, candle_ty);
		glVertex2i(candle_tx, candle_by);
	glEnd();


	/* draw gridlines for volume chart */

	/* horizontal lines */
	glColor3f(.1, .1, .1); /* very dark gray */
	glLineWidth(1.0);
	for(i = vol_by; i < vol_ty; i += 75)
	{
		glBegin(GL_LINES);
			glVertex2i(vol_tx, i);
			glVertex2i(vol_bx, i);
		glEnd();
	}
	/* draw last horizontal line */
	glBegin(GL_LINES);
		glVertex2i(vol_tx, vol_ty);
		glVertex2i(vol_bx, vol_ty);
	glEnd();

	/* vertical lines */
	glLineWidth(1.0);
	for(i = vol_bx; i < vol_tx; i += 145)
	{
		glBegin(GL_LINES);
			glVertex2i(i, vol_ty);
			glVertex2i(i, vol_by);
		glEnd();
	}
	/* draw last vertical line */
	glBegin(GL_LINES);
		glVertex2i(vol_tx, vol_ty);
		glVertex2i(vol_tx, vol_by);
	glEnd();

	srand((unsigned)time(0));

	/* draw candlestick data points */
	for(i = candle_bx; i < candle_tx; i += candle_width)
	{
		/* generate random values for this point */
		min = RandomInt(candle_by, candle_ty);
		max = RandomInt(min, candle_ty);
		/* get direction */
		open = RandomInt(0, 1);

		if(open > 0)
		{
			open = RandomInt(min, max);
			close = RandomInt(min, open);
		} else {
			close = RandomInt(min, max);
			open = RandomInt(min, close);
		}

		/* color set by diff from start to stop */
		if(open == close) glColor3f(0.75, 0.75, 0.75);
		else if (close > open) { glColor3f(0.0, 1.0, 0.0); }
		else { glColor3f(1.0, 0.0, 0.0); }

		glLineWidth(1.0);
		/* staff line */
		glBegin(GL_LINES);
			glVertex2i(i, min);
			glVertex2i(i, max);
		glEnd();

		glLineWidth(2.0);
		/* open */
		glBegin(GL_LINES);
			glVertex2i(i-(candle_width/2), open);
			glVertex2i(i, open);
		glEnd();

		/* close */
		glBegin(GL_LINES);
			glVertex2i(i+(candle_width/2), close);
			glVertex2i(i, close);
		glEnd();

	}

	close = 0;


	/* draw volume chart data points */
	for( i = vol_bx; i < vol_tx; i += candle_width)
	{
		/* generate volume ammount */
		open = RandomInt(vol_by, vol_ty);

		if(open == close) glColor3f(0.75, 0.75, 0.75);
		else if (open > close) { glColor3f(0.0, 0.50, 0.0); }
		else { glColor3f(0.50, 0.0, 0.0); }

		close = open;


		glBegin(GL_POLYGON);
			glVertex2i(i, vol_by);
			glVertex2i(i, open);
			glVertex2i(i+candle_width, open);
			glVertex2i(i+candle_width, vol_by);
			glVertex2i(i, vol_by);
		glEnd();

		glColor3f(0.0, 0.0, 0.0);
		glLineWidth(0.5);
		glBegin(GL_LINE_STRIP);
			glVertex2i(i, vol_by);
			glVertex2i(i, open);
			glVertex2i(i+candle_width, open);
			glVertex2i(i+candle_width, vol_by);
			glVertex2i(i, vol_by);
		glEnd();
	}
}


/*

Create hash of dow points with time as the key.
Points can be added and removed at any time and will be stored as long as the program is open.
These can be verified to be present when each frame needs to be re-drawn.  When points are missing, 
they can be pulled from the random number generator (later database) and then displayed.

This should be done without the user having to perform any action.

Functions needed:

Find absoute max data point
Find absolute min data point
	Used to set scale of graph

Add point to graph.

Find difference between current point and previous point.
	Could be positive or negative.

*/

void display(void)
{

	MovingListData employees[10];

	employees[0].pString = "Kurt Godel";
	employees[0].value = 2343.23;
	employees[0].diff = -1;

	employees[1].pString = "Douglas Hosftadter";
	employees[1].value = 2142.78;
	employees[1].diff = 1;

	employees[2].pString = "Stephen Hawking";
	employees[2].value = 1835.97;
	employees[2].diff = -1;

	employees[3].pString = "Isaac Newton";
	employees[3].value = 1752.98;
	employees[3].diff = 1;

	employees[4].pString = "Nikola Tesla";
	employees[4].value = 1658.54;
	employees[4].diff = 0;

	employees[5].pString = "Albert Einstein";
	employees[5].value = 985.45;
	employees[5].diff = 1;

	employees[6].pString = "Bryan Kernignan";
	employees[6].value = 685.53;
	employees[6].diff = -1;

	employees[7].pString = "Dennis Ritchie";
	employees[7].value = 125.29;
	employees[7].diff = 1;

	employees[8].pString = "Ron Rivest";
	employees[8].value = 98.78;
	employees[8].diff = 1;

	employees[9].pString = "Alan Watt";
	employees[9].value = 78.33;
	employees[9].diff = 1;

	glClear(GL_COLOR_BUFFER_BIT);

	DrawMovingList(750, 400, 1020, 760, employees, 10);


	glFlush();
	glutSwapBuffers();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

/*void keyboard(unsigned char key, int x, int y)*/
void keyboard(unsigned char key)
{
	switch (key) {
	case 27:
	exit(0);
	break;
	}
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1024, 768);
	glutReshapeFunc(NULL);
	glutCreateWindow("Steven Schronk's Data Viewer");
	/*glutFullScreen();*/

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
