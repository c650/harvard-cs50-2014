//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
void waitForMouse();

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    // set a char array to store score
    char score[3];
    // set array for x,y velocities of ball
    double velocity[2] = {drand48() * 3,2.0 * -1};
    double lastPaddleX = getX(paddle);
    
    waitForMouse(); 
    //  ^ wait for user to start game
    // ^^ by clicking mouse

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // put ball in motion @ velocities
        move(ball, velocity[0], velocity[1]);
        // move ball at velX,    velY

        // check if ball hits any walls
        if (getX(ball) <= 0 || (getX(ball) + getWidth(ball) >= getWidth(window))) {
            velocity[0] *= -1; // reverses respective velocity if so
        } else if (getY(ball) <= 0){
            velocity[1] *= -1;
        }

        // if ball passes the paddle, take away one life and
        // reset locations of ball and paddle, 
        // wait 2 secs for user to be ready again
        if (getY(ball) + getWidth(ball) >= getHeight(window)) {
            
            lives--; // deduct a life

            // reset location of objs
            setLocation(ball,(getWidth(window) - RADIUS)/2,(getHeight(window) - RADIUS)/2);
            setLocation(paddle, (getWidth(window) - getWidth(paddle))/2, getHeight(window) - 200);
            
            // reset ball velocities
            velocity[0] = drand48() * 3;
            velocity[1] = 2.0 * -1;
            // wait for a mouse click, made a func
            waitForMouse();
        }
        // get cursor movement event
        GEvent cursor = getNextEvent(MOUSE_EVENT);

        if (cursor != NULL) { //updates paddle pos as mouse moves
            if (getEventType(cursor) == MOUSE_MOVED) {
                setLocation(paddle, getX(cursor), getY(paddle)); // move paddle if mouse moves
            }
        }
        GObject collision = detectCollision(window, ball); // var to store collided obj

        if (collision != NULL) { // checks for collision
            if (collision != paddle) {  // if the collision is not with the paddle...
            
                setVisible(collision, false); // take away the object
                setLocation(collision, WIDTH, HEIGHT); // move away the object to bottom right corner
                
                points++;
                updateScoreboard(window, label, points);
                
                velocity[1] *= -1;
            
            } else if (collision == paddle) { 
                // reverse y velocity if ball hits paddle
                velocity[1] = -velocity[1];
                // set x velocity to 1/2 velX of paddle
                velocity[0] += (lastPaddleX - getX(paddle)) / 120;

            }
        }

        if (points >= 50) { // if you win game, it waits for a click to restart
            waitForMouse(); // mouse click waiting func
            initBricks(window);
        }
        pause(10);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    char* colors[5] = {"RED","GREEN","BLUE","ORANGE","RED"};
    double lastBrickX;
    double lastBrickY = 2;
    // TODO
    for (int i = 0; i < ROWS; i++) {
        lastBrickX = 2;
        for (int j = 0; j < COLS; j++) {
            GRect brick = newGRect(lastBrickX,lastBrickY,WIDTH/COLS -2, HEIGHT/30);
            setColor(brick, colors[i]);
            setFilled(brick, true);
            add(window, brick);
            lastBrickX += getWidth(brick) + 2;
        }
        lastBrickY += HEIGHT/30 + 2;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval((getWidth(window) - RADIUS)/2,(getHeight(window) - RADIUS)/2,RADIUS * 2,RADIUS * 2);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);

    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    int width = WIDTH/COLS;
    int height = HEIGHT/50;
    GRect paddle = newGRect((getWidth(window) - width)/2, getHeight(window) - 200,width,height);
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-36");
    setColor(label, "BLACK");
    addToRegion(window, label, "SOUTH");
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

void waitForMouse() {
    while (true) {
        // declare event        
        GEvent clicked = getNextEvent(MOUSE_EVENT);
            // if there is an event
            if (clicked != NULL) {
                // see if it's a mouse click
                if (getEventType(clicked) == MOUSE_CLICKED) {
                    break; // break if mouse click, yield to main()
                }
            }
        
        pause(20);
    }
    return;
}

/************************************************************

    SOURCES: 

    http://cdn.cs50.net/2013/fall/lectures/5/m/src5m/spl/doc/gwindow.html#Type:GWindow

    https://stackoverflow.com/questions/1088622/how-do-i-create-an-array-of-strings-in-c

*************************************************************/