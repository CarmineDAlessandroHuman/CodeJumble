/*Exercise of Computer Graphics*/
/*Let's draw a clock using OpenGL!*/


#include <windows.h> /*Necessary to run the program*/
#include <GL/glut.h> /*The graphic library*/
#include <stdio.h> /*Used for debugging. Deletable*/
#include <stdlib.h>
#include <time.h>

#define DEGREE_PER_SECOND 6
#define DEGREE_PER_MINUTE 6
#define DEGREE_PER_HOUR 30

#define CLOCK_TIMER 1

/*ASCII codes of the main hours of the day.
    "one" and "two" are used for the twelve
*/
enum hoursOfTheDay {one = 49, two = 50 , three = 51, six = 54, nine = 57};

/*managing the time*/
struct tm* currentTime; /*current time*/
int hour, minute, second;

void initializeTheFrame () {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600,550);
    glutInitWindowPosition(200,100);
    glutCreateWindow("The Time");
}

void initializeTheTimer () {
    /*initialization of the timer*/
    time_t timer = time (NULL);
    currentTime = localtime (&timer);

    /*getting the current time*/
    hour = currentTime -> tm_hour;
    minute = currentTime -> tm_min;
    second = currentTime -> tm_sec;
}

void initializeTheColor () {
    glClearColor(0,0,0,1);
    gluOrtho2D(-50, 50, -50, 50);
    glShadeModel(GL_SMOOTH);
}

/*draw the background*/
void drawBackground () {
    glColor3f(1.0f,1.0f,1.0f);

    glBegin(GL_POLYGON);
		glVertex2f(-0.2f, -25.0f);
	    glVertex2f(-0.2f,  25.0f);
        glVertex2f( 0.2f,  25.0f);
		glVertex2f( 0.2f, -25.0f);
    glEnd();
}

/*draw second hand*/
void drawSecond () {

    //  DEBUG: is the color correctly set?
    glColor3f(1.0f,0.0f,0.0f);
    //  SUCCESS: true

    glBegin(GL_POLYGON);
		glVertex2f(-1.5f, -1.5f);
	    glVertex2f(-0.5f,  20.0f);
        glVertex2f( 0.5f,  20.0f);
		glVertex2f( 1.5f, -1.5f);
    glEnd();

}

/*draw minute hand*/
void drawMinute() {
    glColor3f(0.0f,1.0f,0.0f);

    glBegin(GL_POLYGON);
		glVertex2f(-2.0f, -2.0f);
	    glVertex2f(-2.0f,  20.0f);
        glVertex2f( 2.0f,  20.0f);
		glVertex2f( 2.0f, -2.0f);
    glEnd();

}

/*draw hour hand*/
void drawHour() {
    glColor3f(0.0f,0.0f,1.0f);

    glBegin(GL_POLYGON);
		glVertex2f(-2.0f, -2.0f);
	    glVertex2f(-2.0f,  15.0f);
        glVertex2f( 2.0f,  15.0f);
		glVertex2f( 2.0f, -2.0f);
    glEnd();
}

/*drawing title*/
void drawTitle() {
    char* title = "The Time"; /*deprecated*/
    int c;

    glColor3f(1.0f,1.0f,1.0f); /*the color*/
    glRasterPos2i(-9, 35); /*the position*/

    c = 0;

    while (title[c] != '\0') {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title[c]);
        c++;
    }

}

/*drawing the main hour numbers (12 - 3 - 6 - 9*/
void drawHourNumbers () {
    glColor3f(1.0f, 1.0f, 1.0f);

    glRasterPos2i(-2, 27);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, one);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, two);

    glRasterPos2i(27, -1);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, three);

    glRasterPos2d(-1, -29);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, six);

    glRasterPos2i(-28, -1);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, nine);
}

/*drawing the whole clock*/
void drawClock () {
    glClear(GL_COLOR_BUFFER_BIT); /*clear the screen*/

    float secondDegree, minuteDegree, hourDegree;

    /*calculating the rotation of each hand of the clock*/
    secondDegree = second * DEGREE_PER_SECOND;
    minuteDegree = minute * DEGREE_PER_MINUTE;
    hourDegree = hour * DEGREE_PER_HOUR;

    for (int i = 0; i<6; i++) {
        glPushMatrix();
            glRotatef(i * DEGREE_PER_HOUR, 0, 0, -1);
            drawBackground();
        glPopMatrix();
    }

    glPushMatrix();
        glRotatef(hourDegree,0,0,-1);
        drawHour();
    glPopMatrix();

    glPushMatrix();
        glRotatef(minuteDegree,0,0,-1);
        drawMinute();
    glPopMatrix();

    glPushMatrix();
        glRotatef(secondDegree,0,0,-1);
        drawSecond();
    glPopMatrix();

    glPushMatrix();
        drawTitle();
    glPopMatrix();

    glPushMatrix();
        drawHourNumbers();
    glPopMatrix();

    glutSwapBuffers();  /*draw on screen*/
}

/*function to call every second*/
void nextSecond (int value) {
    /*increasing the current time*/
    second++;
    if (second == 60) {
        minute++;
        second = 0;
    }
    if (minute == 60) {
        hour++;
        minute = 0;
    }
    if (hour == 24) {
        hour = 0;
    }
    /*
    //  DEBUG: is the time synchronized with the real time?
    printf("DEBUG: hour:   %d\n",hour);
    printf("       minute: %d\n",minute);
    printf("       second: %d\n",second);
    //  SUCCESS: true
    */

    /*the rotation of the hands of the clock is calculated
     inside the display function "draw clock"*/

    /*redraw*/
    glutPostRedisplay();
    glutTimerFunc(1000, nextSecond, value);
}

void keyboard(unsigned char key, int x, int y) {

    switch (key){
        case 27:
            exit(0);
            break;
    }
}

int main (int argc, char** argv) {
    /*initializing the environment*/
    glutInit(&argc, argv);

    initializeTheFrame();
    initializeTheTimer();
    initializeTheColor();

    /*
    //  DEBUG: is the time correct?
    printf("%d : %d : %d\n",hour, minute, second);
    //  SUCCESS: true
    */

    glutDisplayFunc(drawClock);
    /*
    //  DEBUG: is the hand of the seconds correctly printed on screen?
    //  SUCCESS: true
    */
    glutKeyboardFunc(keyboard);
    glutTimerFunc(1000, nextSecond, CLOCK_TIMER);

    glutMainLoop();

    return 1;
}

