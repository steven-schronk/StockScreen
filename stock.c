#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#include <GL/glut.h>

#include "random.c"

#define PI 3.1415926535897932384626433832795029L
#define WINDOW_BORDER 15 /* offset of graphics inside window - in pixels */
#define WINDOW_TOP_PAD 25 /* offset of top bar for window - in pixels */
#define OHLC_COUNT 90

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

typedef struct ohlcdata {
	int time;		/* time stamp stored in epoc time */
	int open;			/* value at start of time period et. al.  */
	int high;
	int low;
	int close;
} OHLCata;

typedef struct news {
	int time;
	char *headline;
} News;

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
	draw box made of lines at at start and stop locations.
	TODO: Add color as a parameter.

void DrawSquareBox(int tx, int ty, int bx, int by, int width)
{

}
 */

void DrawCircle(float cx, float cy, float r, int segments)
{
	float theta, x, y, ii;
	glBegin(GL_LINE_LOOP);
	for(ii = 0; ii < segments; ii++)
	{
		theta = 2.0f * PI * (float)ii / (float)segments; /* get current angle */
		x = r * cos(theta);/* calculate the x component */
		y = r * sin(theta);/* calculate the y component */
		glVertex2f(x + cx, y + cy);/* output vertex */
	}
	glEnd();
}


/*
	draw a refresh button - used at top of most windows.
	x and y is the position of the button.
 */
void ButtonRefresh(int bx, int by)
{
	int tx = bx + 16;
	int ty = by + 16;
	glLineWidth(1.0);
	glColor3f(1.0, 1.0, 1.0);

	/* draw square box of button */
	glBegin(GL_LINE_STRIP);
		glVertex2i(tx, ty);
		glVertex2i(tx, by);
		glVertex2i(bx,by);
		glVertex2i(bx,ty);
		glVertex2i(tx, ty);
	glEnd();

	/* draw symbol inside of button */
	DrawCircle(bx + 8, by + 9, 5.0, 8);
}

/*
	button on left of window provides the user with a place to drag window from one location to another.
	initial version uses this as a decoration for the window. makes windows look more conventional.
 */
void ButtonDrag(int bx, int by)
{
	int tx = bx + 16;
	int ty = by + 16;
	int i = 0;
	glLineWidth(1.0);
	glColor3f(1.0, 1.0, 1.0);

	/* draw square box of button */
	glBegin(GL_LINE_STRIP);
		glVertex2i(tx, ty);
		glVertex2i(tx, by);
		glVertex2i(bx,by);
		glVertex2i(bx,ty);
		glVertex2i(tx, ty);
	glEnd();

	/* draw lines inside box */
	by += 4;
	for (i = 1; i < 5; i++)
	{
		glBegin(GL_LINES);
			glVertex2i(bx + 2, by);
			glVertex2i(bx + 15, by);
		glEnd();
		by += 3;
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

	ButtonDrag(tx + 5, by - 20);
	ButtonRefresh(tx + 250, by - 20);
	/* draw outline of main window */
	glColor3f(0.929, 0.615, 0.309);
	glLineWidth(1.0);

	/* draw outer box */
	glBegin(GL_LINE_STRIP);
		glVertex2i(tx, ty);
		glVertex2i(tx, by);
		glVertex2i(bx,by);
		glVertex2i(bx,ty);
		glVertex2i(tx, ty);
	glEnd();

	/* draw title bar */
	glBegin(GL_LINES);
		glVertex2i(tx, by-WINDOW_TOP_PAD);
		glVertex2i(bx, by-WINDOW_TOP_PAD);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	DrawString(tx+30, by-18, "Best Performance");

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

/*
	chart contains open, high, low and close for each data point.
	chart automatically scales based on min and max data points.
 */

/* DrawOHLC(10, 400, 200, 760, "Production Count", ohlc, 20); */
void DrawOHLC(int tx, int ty, int bx, int by, char *title, OHLCata data[], int count)
{
	int i = 0;
	int candle_tx = tx + WINDOW_BORDER, candle_ty = ty + WINDOW_BORDER;
	int candle_bx = bx - WINDOW_BORDER, candle_by = by - WINDOW_BORDER - WINDOW_TOP_PAD;
	int candle_space = (candle_bx - candle_tx) / count;
	int candle_xpos = candle_tx + (candle_tx/2); /* extra offset for width of horizontal bar on left */
	int data_max_val = 0, data_min_val = 0;
	/* coefficient value of each pixel - each pixel in the data area of the graph represents this much value */
	float pixel_value;

	/* draw outline of graph */
	ButtonDrag(tx + 5, by - 20);
	ButtonRefresh(bx - 20, by - 20);
	/* draw outline of main window */
	glColor3f(0.929, 0.615, 0.309);
	glLineWidth(1.0);

	/* draw outer box */
	glBegin(GL_LINE_STRIP);
		glVertex2i(tx, ty);
		glVertex2i(tx, by);
		glVertex2i(bx,by);
		glVertex2i(bx,ty);
		glVertex2i(tx, ty);
	glEnd();

	/* draw title bar */
	glBegin(GL_LINES);
		glVertex2i(tx, by-WINDOW_TOP_PAD);
		glVertex2i(bx, by-WINDOW_TOP_PAD);
	glEnd();

	/* draw text name of graph */
	glColor3f(1.0, 1.0, 1.0);
	DrawString(tx+30, by-18, title);

	/* draw grid lines for candlestick chart */

	/* horizontal lines */
	glColor3f(0.2, 0.2, 0.2); /* very dark gray */
	glLineWidth(1.0);
	for(i = candle_ty; i < candle_by; i += 75)
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
	for(i = candle_tx; i < candle_bx; i += 145)
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

	srand((unsigned)time(0));

	/* draw candlestick data points */

	/* find minimum and maximum values for this dataset */
	data_max_val = data[0].high;
	data_min_val = data[0].low;
	for(i = 1; i < count; i++)
	{
		if(data[i].high > data_max_val) { data_max_val = data[i].high; }
		if(data[i].low  < data_min_val) { data_min_val = data[i].low;  }
	}

	pixel_value =  (float)(candle_by - candle_ty) / (data_max_val - data_min_val);
	printf("graph area %d, pixel value %f\t data max %d\tdata_min %d\n", candle_by - candle_ty, pixel_value, data_max_val, data_min_val);

	for(i = 0; i < count; i++) /* for each point in dataset */
	{
		/* color set by diff from open to close */
		if(data[i].open == data[i].close) glColor3f(0.75, 0.75, 0.75);
		else if (data[i].close > data[i].open) { glColor3f(0.0, 1.0, 0.0); }
		else { glColor3f(1.0, 0.0, 0.0); }

		/* staff line */
		glLineWidth(1.0);
		glBegin(GL_LINES);
			glVertex2i(candle_xpos, candle_ty + (data[i].high * pixel_value));
			glVertex2i(candle_xpos, candle_ty + (data[i].low * pixel_value));
		glEnd();

		/* open mark */
		glLineWidth(2.0);
		glBegin(GL_LINES);
			glVertex2i(candle_xpos - (candle_space / 2), candle_ty + (data[i].open * pixel_value));
			glVertex2i(candle_xpos, candle_ty + (data[i].open * pixel_value));
		glEnd();

		/* close mark */
		glBegin(GL_LINES);
			glVertex2i(candle_xpos + (candle_space / 2), candle_ty + (data[i].close * pixel_value));
			glVertex2i(candle_xpos, candle_ty + (data[i].close * pixel_value));
		glEnd();
		candle_xpos += candle_space; /* move next candle position over */
	}

}

/*
	Displays current news items from news feeds.
	News updates automatically.
 */
void DrawNews(int tx, int ty, int bx, int by, char *title, News news[], int count)
{
	int i = 0, offset = 0;
	/* draw outline of graph */
	ButtonDrag(tx + 5, by - 20);
	ButtonRefresh(bx - 20, by - 20);
	/* draw outline of main window */
	glColor3f(0.929, 0.615, 0.309);
	glLineWidth(1.0);

	/* draw outer box */
	glBegin(GL_LINE_STRIP);
		glVertex2i(tx, ty);
		glVertex2i(tx, by);
		glVertex2i(bx,by);
		glVertex2i(bx,ty);
		glVertex2i(tx, ty);
	glEnd();

	/* draw title bar */
	glBegin(GL_LINES);
		glVertex2i(tx, by-WINDOW_TOP_PAD);
		glVertex2i(bx, by-WINDOW_TOP_PAD);
	glEnd();

	/* draw text name of graph */
	glColor3f(1.0, 1.0, 1.0);
	DrawString(tx+30, by-18, title);

	/* print out each news item in news */
	offset = by - WINDOW_TOP_PAD - 25;
	for (i = 0; i < count; i++)
	{
		glColor3f(0.2, 0.2, 0.4);
		glBegin(GL_LINES);
			glVertex2i(tx, offset-6);
			glVertex2i(bx, offset-6);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
		DrawString(tx+30, offset, news[i].headline);
		offset -= 30; /* move down to next item */
	}
}

/*
	Displays speedomoeter with data values.
	TODO: Has levels drawn on background. Level green and two yellows and reds.
	These are used to display a tolerance to the user.
	Units value is printed in the dial.
 */
void DrawSpeedometer(int tx, int ty, int bx, int by, char *title, char *units, int value)
{
	int i = 0, offset = 0;
	char buf[12];
	float cx = ((bx-tx)/2)+tx;  /* center of x */
	float cy = (by-ty)/2;		/* center of y */
	/* draw outline of graph */
	ButtonDrag(tx + 5, by - 20);
	ButtonRefresh(bx - 20, by - 20);
	/* draw outline of main window */
	glColor3f(0.929, 0.615, 0.309);
	glLineWidth(1.0);

	/* draw outer box */
	glBegin(GL_LINE_STRIP);
		glVertex2i(tx, ty);
		glVertex2i(tx, by);
		glVertex2i(bx,by);
		glVertex2i(bx,ty);
		glVertex2i(tx, ty);
	glEnd();

	/* draw title bar */
	glBegin(GL_LINES);
		glVertex2i(tx, by-WINDOW_TOP_PAD);
		glVertex2i(bx, by-WINDOW_TOP_PAD);
	glEnd();

	/* draw text name of graph */
	glColor3f(1.0, 1.0, 1.0);
	DrawString(tx+30, by-18, title);

	/* draw outer dial of speedometer */
	DrawCircle(cx, cy, ((bx-tx)/2)-9, 30);
	DrawCircle(cx, cy, ((bx-tx)/2)-14, 30);

	/* draw speed ranges of speedometer */

	glColor3f(1.0, 0.0, 0.0); /* red */

	float angle;
	glBegin(GL_POLYGON);
	    for(i = 0; i < 15; i++)
	    {
	    	angle = i*2*3.14159/15;
	    	glVertex2f(cos(angle),sin(angle));
	    	printf("x%f\ty%f\n",cos(angle), sin(angle));
	    }
	glEnd();





	glColor3f(0.0, 1.0, 0.0); /* green */

	/* draw units string under current value */
	glColor3f(1.0, 1.0, 1.0);
	snprintf(buf, 12, "%.2f", value);
	DrawString(tx+30, by - 185, buf);
	DrawString(tx+30, by - 200, units);

	/* draw pointer */

}


/*

Create hash of dow points with time as the key.
Points can be added and removed at any time and will be stored as long as the program is open.
These can be verified to be present when each frame needs to be re-drawn.  When points are missing, 
they can be pulled from the random number generator (later database) and then displayed.

This should be done without the user having to perform any action.

Functions needed:

Find absolute max data point
Find absolute min data point
	Used to set scale of graph

Add point to graph.

Find difference between current point and previous point.
	Could be positive or negative.

*/

void display(void)
{
	int i = 0;
	int epoc = 1284493172;

	MovingListData employees[10];
	OHLCata ohlc[OHLC_COUNT];
	News news[10];

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

	/* load epoc time and data into ohlc */
	for(i = 0; i < OHLC_COUNT; i++)
	{
		RandomOHLC(0, 100, &ohlc[i].open, &ohlc[i].high, &ohlc[i].low, &ohlc[i].close);
		ohlc[i].time = epoc;
		epoc += 60; 			/* load time data evenly spaced */
	}

	news[0].time = epoc;
	news[0].headline = "New Product Announcement";

	news[1].time = epoc;
	news[1].headline = "PROMOTION: John Cage Promoted to District Manager";

	news[2].time = epoc;
	news[2].headline = "Stock Buy Open";

	news[3].time = epoc;
	news[3].headline = "Merger with BigCo Completed";

	news[4].time = epoc;
	news[4].headline = "X25 Product in Production";

	glClear(GL_COLOR_BUFFER_BIT);

	/* void DrawNews(int tx, int ty, int bx, int by, char *title, News data[], int count) */
	DrawNews(10, 10, 740, 390, "General News", news, 5);
	DrawMovingList(750, 400, 1020, 760, employees, 10);
	DrawOHLC(10, 400, 740, 760, "Production Count", ohlc, OHLC_COUNT);
	DrawSpeedometer(750, 10, 1020, 390, "Overtime", "HRS/Employee", 57);

	glFlush();
	glutSwapBuffers();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	/* glEnable(GL_DEPTH_TEST); */
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

/*void keyboard(unsigned char key, int x, int y)*/
void keyboard(unsigned char key, int x, int y)
{
	x = 0;
	y = 0;
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
