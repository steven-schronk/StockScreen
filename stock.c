#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>


struct dowpoint {
	double open;
	double close;
	double min;
	double max;
	int time;
};

int GenerateRandomInt(int min, int max)
{
	if(min > max)
	{
		return 0;
	}
	else
	{
		return  min + rand() % ((max+1) - min);
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
	int width, height, i;

	int candle_tx, candle_ty, candle_bx, candle_by;
	int vol_tx, vol_ty, vol_bx, vol_by;
	int candle_width = 10;

	int open, close, min, max;

	glClear(GL_COLOR_BUFFER_BIT);

	/* find width and height of window */
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	candle_tx = width - 30;  candle_ty = height - 30;
	candle_bx =  30; candle_by = 330;

	vol_tx = width - 30;  vol_ty = height = 300;
	vol_bx = 30; vol_by = 30;

	/*
	glBegin(GL_LINES);
		glVertex2i(vol_tx, vol_ty);
		glVertex2i(vol_ty, vol_bx);
		glVertex2i(vol_bx, vol_by);
	glEnd();


	glPointSize(10);
	glBegin(GL_POINTS);
		glVertex2i(vol_tx, vol_ty);
	glEnd();


	 draw line from corner to corner of candle
	glBegin(GL_LINES);
		glVertex2i(candle_tx, candle_ty);
		glVertex2i(candle_bx, candle_by);
	glEnd();
	*/

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
		min = GenerateRandomInt(candle_by, candle_ty);
		max = GenerateRandomInt(min, candle_ty);
		/* get direction */
		open = GenerateRandomInt(0, 1);

		if(open > 0)
		{
			open = GenerateRandomInt(min, max);
			close = GenerateRandomInt(min, open);
		} else {
			close = GenerateRandomInt(min, max);
			open = GenerateRandomInt(min, close);
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
	/* draw volume chart data points*/
	for( i = vol_bx; i < vol_tx; i += candle_width)
	{
		/* generate volume ammount */
		open = GenerateRandomInt(vol_by, vol_ty);

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
	glutInitWindowSize(350, 150);
	glutCreateWindow(argv[0]);
	glutFullScreen();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
