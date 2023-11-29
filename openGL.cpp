#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <unistd.h>

bool running = true;

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
    // Draw objects here

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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Window");

    init();

    glutDisplayFunc(render);
    glutTimerFunc(0, timer, 0); // Start the timer with a delay of 0 milliseconds
    glutIdleFunc(idle); // Set the idle function

    // Main loop
    while (running) {
        sleep(33); // 33 ms = ~30 fps
        update();
        render();
    }

    // Note: The cleanup code is missing; you might want to add it here or in a separate function.

    return 0;
}