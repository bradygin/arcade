#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <math.h>
#include <string>
#include <random>

int score = 0;
int lives = 3;
bool atTitlePage = true;
bool gameOver = false;
bool isGamePaused = false;
int brick_color = 0,ball_color = 2,level = 0,paddle_color = 2,text_color = 3,size = 1;;
GLfloat twoModel[]={GL_TRUE};
int game_level[] = {5};
float rate = game_level[level];

GLfloat brick_color1[][1] = {148,0,211};
GLfloat paddle_color1[][1] = {0,0,1};
GLfloat text_color1[][1] = {1,1,1,1};
GLfloat paddle_size[] = {2,4,6};
//The grid parameters for the bricks
int rows = 5;
int columns = 10;

// Structure to store the coordinates of each brick
struct brick_coords{
	
	GLfloat x;
	GLfloat y;
};

//Array to store the bricks
brick_coords brick_array[50][50];
GLfloat px,bx = 0,by = -12.94 ,speed = 0,dirx=0,diry=0,start = 0;

void initializeBricks() {
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= columns; j++) {
            brick_array[i][j].x = (GLfloat)(j * 4 * 0.84);
            brick_array[i][j].y = (GLfloat)(i * 2 * 0.6);
        }
    }
}

// Function to draw the title page
void drawTitlePage() {
    // Clear the screen
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up an orthographic projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));

    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if (gameOver) {
        // Set text color to white
        glColor3f(1.0, 1.0, 1.0); // White color

        // Game Over screen
        std::string gameOverText = "Game Over! Press 'R' to Restart";
        int textWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)gameOverText.c_str());
        int textX = (glutGet(GLUT_WINDOW_WIDTH) - textWidth) / 2;
        int textY = glutGet(GLUT_WINDOW_HEIGHT) / 2; // Center vertically
        glRasterPos2f(textX, textY);
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    } else {
        // Define title and controls text
        std::string title = "Brick Breaker Game";
        std::string controls = "Press 'S' to Start, 'A' and 'D' to move";

        // Set text color
        glColor3f(1.0, 1.0, 1.0); // White color

        // Center and render the title text
        int titleWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)title.c_str());
        int titleX = (glutGet(GLUT_WINDOW_WIDTH) - titleWidth) / 2;
        int titleY = glutGet(GLUT_WINDOW_HEIGHT) / 2 + 50; // Position slightly above center
        glRasterPos2f(titleX, titleY);
        for (char c : title) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        // Center and render the controls text
        int controlsWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)controls.c_str());
        int controlsX = (glutGet(GLUT_WINDOW_WIDTH) - controlsWidth) / 2;
        int controlsY = titleY - 30; // Position below the title
        glRasterPos2f(controlsX, controlsY);
        for (char c : controls) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    // Restore the previous matrices
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // Swap buffers to display
    glutSwapBuffers();
}



// Function to draw the paddle
void draw_paddle()
{
	glDisable(GL_LIGHTING);
	glColor3fv(paddle_color1[paddle_color]);
	glBegin(GL_POLYGON);
	glVertex3f(-paddle_size[size]+px,0-15,0);
	glVertex3f(paddle_size[size]+px,0-15,0);
	glVertex3f(paddle_size[size]+px,1-15,0);
	glVertex3f(-paddle_size[size]+px,1-15,0);
	glEnd();
	glEnable(GL_LIGHTING);
}


//Function to draw a single brick
void brick(GLfloat x,GLfloat y, GLfloat z)
{	

	glDisable(GL_LIGHTING);
	glColor3fv(brick_color1[brick_color]);
	glBegin(GL_QUADS);
	glVertex3f(x,y,z);
	glVertex3f(x+3,y,z);
	glVertex3f(x+3,y+1,z);
	glVertex3f(x,y+1,z);       
	glEnd();
	glEnable(GL_LIGHTING);
}


// Function to draw the grid of bricks
void draw_bricks()
{
	
	int i,j;
	if(start == 0)
	{
		for(i = 1;i<=rows;i++)
		{
			for(j = 1;j<=columns;j++)
			{   
				
				brick_array[i][j].x = (GLfloat)(j*4*0.84);
				brick_array[i][j].y = (GLfloat)(i*2*0.6) ;
			}
		}
	}
	
	
	glPushMatrix();
	glTranslatef(-19.5,5,0);

	for(i = 1;i<=rows;i+=1)
	{
		for(j = 1;j<=columns;j+=1)
		{
			
			if(brick_array[i][j].x==0 || brick_array[i][j].y ==0)
			{
				continue;
			}
			brick(brick_array[i][j].x,brick_array[i][j].y,0);
		}
	}
	glPopMatrix();
	
}

//Function to draw the spherical ball
void draw_ball()
{
//	glDisable(GL_LIGHTING);

	GLfloat ambient1[] = {1,1,1};
	GLfloat diffuse1[] = {0.4,0.4,0.4};
	GLfloat specular1[] = {1,1,1};
	
	GLfloat position[] = {0,0,-50,1};
	GLfloat ambient2[] = {0,0,0};
	GLfloat diffuse2[] = {1,1,1};
	GLfloat specular2[] = {0,1,1};
	
	float materialColours[][3]={{1,0,0},{0,0,1},{0,1,1}};
	GLfloat matAmbient1[] = {1,1,1};
	GLfloat matDiffuse1[] = {1,1,1};
	GLfloat matSpecular1[] = {1,1,1};
	GLfloat shininess[] = {1000};
	
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular1);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient1);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse1);
	
	glLightfv(GL_LIGHT1,GL_POSITION,position);
	glLightfv(GL_LIGHT1,GL_SPECULAR,specular2);
	glLightfv(GL_LIGHT1,GL_AMBIENT,ambient2);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse2);
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialColours[ball_color]);
	
	
	
	glPushMatrix();
	glTranslatef(bx,by,0);
	glScalef(1.0, 1.0, 0.5);
	//glScalef(size[i], size[], size[]);
	glutSolidSphere(1.0, 52, 52);
	   
	glPopMatrix();
	
}


//mouse function
void mousemotion(int x,int y)
{
  
  if(start == 1)
  {
    px=(x-glutGet(GLUT_WINDOW_WIDTH)/2)/20;
    if(px>15)
	  {
		  px=15;
	  }
	  if(px<-15)
	  {
		  px=-15;
	  }
    }
    
    else glutSetCursor(GLUT_CURSOR_INHERIT);
}

//handle brick color
void change_brick_color(int action)
{
	
	brick_color=action-1;
}

//handle ball color
void change_ball_color(int action)
{
	
	ball_color=action-1;
}

//handle level
void change_difficulty(int action)
{
	
	level=action-1;
}

//handle menu
void handle_menu(int action)
{
	
}

//handle paddle color
void change_paddle_color(int action)
{
	paddle_color = action -1;
}

//handle paddle color
void change_text_color(int action)
{
	text_color = action -1;
}

//handle paddle size
void change_paddle_size(int action)
{
	size = action -1;
}

//add menu
void addMenu()
{
	
	
	int submenu1 = glutCreateMenu(change_brick_color);
	glutAddMenuEntry("Red",1);
	glutAddMenuEntry("Blue",2);
	glutAddMenuEntry("Green",3);
	glutAddMenuEntry("White",4);

	int submenu2 = glutCreateMenu(change_ball_color);
	glutAddMenuEntry("Red",1);
	glutAddMenuEntry("Blue",2);
	glutAddMenuEntry("Cyan",3);


	
	int submenu4 = glutCreateMenu(change_paddle_color);
	glutAddMenuEntry("Red",1);
	glutAddMenuEntry("Blue",2);
	glutAddMenuEntry("Green",3);
	glutAddMenuEntry("Cyan",4);


	
	int submenu3 = glutCreateMenu(change_difficulty);
	glutAddMenuEntry("Easy",1);
	glutAddMenuEntry("Medium",2);
	glutAddMenuEntry("Hard",3);
	
	int submenu5 = glutCreateMenu(change_text_color);
	glutAddMenuEntry("Red",1);
	glutAddMenuEntry("Blue",2);
	glutAddMenuEntry("Green",3);
	glutAddMenuEntry("Yellow",4);

	
	int submenu6 = glutCreateMenu(change_paddle_size);
	glutAddMenuEntry("Small",1);
	glutAddMenuEntry("Medium",2);
	glutAddMenuEntry("Large",3);
	
	glutCreateMenu(handle_menu);
	glutAddSubMenu("Bricks Color",submenu1);
	glutAddSubMenu("Ball Color",submenu2);
	glutAddSubMenu("Paddle Color",submenu4);
	glutAddSubMenu("Text Color",submenu5);
	glutAddSubMenu("Difficulty",submenu3);
	glutAddSubMenu("Paddle Size",submenu6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Function to print the score and lives on the screen
void text(int sc) {
    glDisable(GL_LIGHTING);

    // Set up an orthographic projection for text rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Display score
    char score_text[40];
    snprintf(score_text, sizeof(score_text), "Score: %d", sc);

    // Display lives
    char lives_text[40];
    snprintf(lives_text, sizeof(lives_text), "Lives: %d", lives);

    // Calculate position based on window size
    int textY = glutGet(GLUT_WINDOW_HEIGHT) - 50; // Adjust position to be visible

    glColor4fv(text_color1[text_color]);

    // Render score text
    glRasterPos2f(10, textY); // Position on the left side
    for (char* c = score_text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // Render lives text
    glRasterPos2f(glutGet(GLUT_WINDOW_WIDTH) - 150, textY); // Position on the right side
    for (char* c = lives_text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // Restore the previous projection and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

//The main display function
void display (void) {
	    if (atTitlePage || gameOver) {
        drawTitlePage();
        return;
    }
	glClearColor (0.0,0.0,0.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0,0,0,0,0,-25,0,1,0);
	glTranslatef(0,0,-25);
	draw_paddle(); 
	draw_bricks();
	draw_ball();
	text(score);
	glutSwapBuffers();
}

//function to turn on lights
void lightsOn()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

void reshape (int w, int h) {
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	glMatrixMode (GL_MODELVIEW);
}

// Function to initialize the game state
void initializeGame() {
    initializeBricks();
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= columns; j++) {
            brick_array[i][j].x = (GLfloat)(j * 4 * 0.84);
            brick_array[i][j].y = (GLfloat)(i * 2 * 0.6);
        }
    }
    // Random number generation for ball direction
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(-1, 1); // Define the range

    do {
        dirx = distr(eng); // Generate a random number for x direction
    } while (dirx == 0);  // Ensure dirx is not zero

    do {
        diry = distr(eng); // Generate a random number for y direction
    } while (diry == 0);  // Ensure diry is not zero

    bx = 0;
    by = -12.94;
    px = 0;
    score = 0;
    rate = game_level[level];
}

// Function to handle keyboard inputs
void keyboard(unsigned char key, int x, int y) {
    // Check if the game is currently at the title page or paused
    if (atTitlePage || isGamePaused) {
        switch (key) {
            case 's':
            case 'S':
                if (atTitlePage) {
                    atTitlePage = false; // Exit title page
                    start = 1;           // Start the game
                    initializeGame();
                    glutSetCursor(GLUT_CURSOR_NONE); // Hide the cursor
                }
                break;
            case 'd':
            case 'D':
            case 'a':
            case 'A':
                if (isGamePaused) {
                    // Resume the game when 'D', 'A', or 'S' is pressed
                    isGamePaused = false;
                    start = 1;
                    // Reset ball and paddle positions and directions
                    bx = 0;
                    by = -12.94;
                    dirx = 1; // Adjust as needed for your game logic
                    diry = 1;
                    px = 0;
                    initializeBricks();
                }
                break;
            case 'r':
            case 'R':
                if (gameOver) {
                    // Restart the game after game over
                    gameOver = false;
                    lives = 3;
                    score = 0;
                    initializeGame();
                    atTitlePage = false;
                    start = 1;
                }
                break;
        }
        return; // Ignore other keys at the title page or when paused
    }

    // Handle other keyboard interactions when the game is running
    switch (key) {
        case 'd': 
        case 'D': 
            px += 3; 
            break;
        case 'a': 
        case 'A': 
            px -= 3; 
            break;
        case 'q': 
        case 'Q': 
            exit(0); // Quit the game
            break;
    }

    // Ensure the paddle stays within game boundaries
    if (px > 15) {
        px = 15;
    }
    if (px < -15) {
        px = -15;
    }

    // Reset paddle position if the game hasn't started
    if (start == 0) {
        px = 0;
    }

    // Request a redraw of the window
    glutPostRedisplay();
}

//Function to handle the case when the ball strikes the bricks
void hit()
{
	int i,j;
	for(i = 1;i<=rows;i++)
		for(j=1;j<=columns;j++)
		{
			if((bx>=brick_array[i][j].x-19.5-0.1 )&&( bx<=brick_array[i][j].x + 3-19.5+ 0.1))
			{
				if(by >=brick_array[i][j].y+5-0.1 && by <=brick_array[i][j].y+5 +1.2 +0.1)
				{
					brick_array[i][j].x = 0;
					brick_array[i][j].y = 0;
					diry= diry*-1;
				
					score++;
				}
			}
		}
}

//The idle function. Handles the motion of the ball along with rebounding from various surfaces
void idle()
{
	if (start == 1 && !isGamePaused) {
		hit();

		// Adjust boundary check for horizontal movement
		if (bx < -16 || bx > 16) { // Assuming -16 and 16 are the left and right bounds
			dirx = -dirx; // Invert horizontal direction
		}

		// Adjust boundary check for vertical movement
		if (by < -15 || by > 14) { // Assuming -15 and 14 are the bottom and top bounds
			diry = -diry; // Invert vertical direction
		}

		// Ball movement logic
		bx += dirx / rate;
		by += diry / rate;
		rate -= 0.001;

		float x = paddle_size[size];

		// Check for collision with paddle
		if (by <= -12.8 && bx < (px + x) && bx > (px - x) && start == 1) {
			diry = 1; // Ball moves up after hitting the paddle
		} 
		else if (by < -13)
		{
			lives--;
			if (lives > 0) {
				// Restart the game while keeping the score and lives
				isGamePaused = true;
				start = 1;
				bx = 0;
				by = -12.94;
				dirx = 1;
				diry = 1;
				px = 0;
				initializeBricks();
			} else if (lives <=0) {
				// Go to the end page
				gameOver = true;
				atTitlePage = true; // Reuse the title page flag for the end page
			}
		}
    }

    glutPostRedisplay();
}

int main (int argc,char **argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (1025, 900);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Brick Breaker");
	initializeGame();
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glEnable(GL_DEPTH_TEST);
	glutIdleFunc (idle);
	glutPassiveMotionFunc(mousemotion);
	glutKeyboardFunc(keyboard);
	lightsOn();
	glutMainLoop ();
	return 0;
}