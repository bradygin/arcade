#define GL_SILENCE_DEPRECATION
#include <string>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <unistd.h>

bool running = true;
void DrawLives();

float playerX = 10.0;  // Initial X position of the player
float playerY = 100.0;  // Initial Y position of the player
float playerSize = 25.0;  // Size of the player

const float screenWidth = 800.0;
const float screenHeight = 600.0;

float jumpSpeed = 0.0;
float gravity = -0.5;  // Gravity strength

void init() {
    glClearColor(0.529, 0.808, 0.922, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 600);
}

void update() {
    // Apply gravity
    jumpSpeed += gravity;
    playerY += jumpSpeed;

    // Check if the player has landed
    if (playerY <= 0) {
        playerY = 0;
        jumpSpeed = 0.0;  // Reset jump speed when landed
    }
}

int lives = 3;

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the player as a red box
    glColor3f(1.0, 0.0, 0.0);  // Red color
    glBegin(GL_QUADS);
    glVertex2f(playerX, playerY);
    glVertex2f(playerX + playerSize, playerY);
    glVertex2f(playerX + playerSize, playerY + playerSize);
    glVertex2f(playerX, playerY + playerSize);
    glEnd();

    // Draw lives
    DrawLives();

    glFlush();
}

void DrawLives() {
    glColor3f(1.0, 1.0, 1.0);

    // Adjust the position based on the screen dimensions
    float xPos = screenWidth - 100.0; 
    float yPos = screenHeight - 20.0; 

    glRasterPos2f(xPos, yPos);

    std::string livesText = "Lives: " + std::to_string(lives);

    for (char const &c : livesText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}


void timer(int value) {
    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(33, timer, 0); // Set the next timer
}

void idle() {
    update();
    render();
}

// Function to handle arrow key input for player movement and jumping
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (playerY == 0) {
                // Only jump if the player is on the ground
                jumpSpeed = 10.0;
            }
            break;
        case GLUT_KEY_DOWN:
            // You can add specific functionality for the down arrow if needed
            break;
        case GLUT_KEY_LEFT:
            if (playerX > 0)
                playerX -= 10.0;  // Move left
            break;
        case GLUT_KEY_RIGHT:
            if (playerX + playerSize < screenWidth)
                playerX += 10.0;  // Move right
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Window");

    init();

    glutDisplayFunc(render);
    glutTimerFunc(0, timer, 0);
    glutIdleFunc(idle);

    // Register the specialKeys function for arrow key input
    glutSpecialFunc(specialKeys);

    glutMainLoop();  // Start the GLUT main loop

    return 0;
}
