#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <unistd.h>

bool running = true;

float playerX = 400.0;  // Initial X position of the player
float playerY = 300.0;  // Initial Y position of the player
float playerSize = 50.0;  // Size of the player

const float screenWidth = 800.0;
const float screenHeight = 600.0;

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 600);
}

void update() {
    // Update game logic here
}

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

    glFlush();
}

void timer(int value) {
    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(33, timer, 0); // Set the next timer
}

void idle() {
    update();
    render();
}

// Function to handle arrow key input for player movement
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (playerY + playerSize < screenHeight)
                playerY += 10.0;  // Move up
            break;
        case GLUT_KEY_DOWN:
            if (playerY > 0)
                playerY -= 10.0;  // Move down
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
