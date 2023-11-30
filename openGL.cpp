#define GL_SILENCE_DEPRECATION
#include <string>
#include <vector>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <unistd.h>
#include <vector>

bool running = true;
void DrawLives();
std::vector<bool> hearts(3, true);

float playerX = 400.0;  // Initial X position of the player
float playerY = 300.0;  // Initial Y position of the player
float playerSize = 50.0;  // Size of the player

// NPC class definition
class NPC {
public:
    float x, y;     // Position
    float size;     // Size of the NPC
    float dx, dy;   // Movement direction and speed

    NPC(float startX, float startY, float npcSize) : x(startX), y(startY), size(npcSize) {
        // Initialize movement direction and speed randomly
        dx = (rand() % 20 - 10) / 10.0f;  // Random speed between -1 and 1
        dy = (rand() % 20 - 10) / 10.0f;  // Random speed between -1 and 1
    }

    void move() {
        // Update position
        x += dx;
        y += dy;

        // Ensure NPC stays within window bounds
        if (x < 0 || x > 800 - size) dx = -dx;
        if (y < 0 || y > 600 - size) dy = -dy;
    }

    void draw() const {
        glColor3f(0.0, 1.0, 0.0);  // Green color for NPCs
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + size, y);
        glVertex2f(x + size, y + size);
        glVertex2f(x, y + size);
        glEnd();
    }

};

std::vector<NPC> npcs;

const float screenWidth = 800.0;
const float screenHeight = 600.0;

float jumpSpeed = 0.0;
float gravity = -0.5;  // Gravity strength

bool isJumping = false;

void init() {
    glClearColor(0.529, 0.808, 0.922, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 600);

    // Create NPCs
    for (int i = 0; i < 3; i++) {
        npcs.emplace_back(rand() % 800, rand() % 600, 30.0f); // Random position, size 30
    }
}

void update() {
    // Update game logic 
    for (auto& npc : npcs) {
        npc.move();
    }
        jumpSpeed += gravity;
    playerY += jumpSpeed;

    // Check if the player has landed
    if (playerY <= 0) {
        playerY = 0;
        jumpSpeed = 0.0;  // Reset jump speed when landed
        isJumping = false;
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

    // Draw NPCs
    for (const auto& npc : npcs) {
        npc.draw();
    }

    // Draw lives
    DrawLives();

    glFlush();
}

void DrawLives() {
    glColor3f(1.0, 1.0, 1.0);

    // Adjust the position based on the screen dimensions
    float xPos = screenWidth - 100.0; // Adjust this value as needed
    float yPos = screenHeight - 20.0; // Adjust this value as needed

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
            if (playerY == 0 && !isJumping) {
                // Only jump if the player is on the ground and not already jumping
                jumpSpeed = 10.0;
                isJumping = true;
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

// Function to handle space bar key press for jumping
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 32: // ASCII code for space bar
            if (playerY == 0 && !isJumping) {
                // Only jump if the player is on the ground and not already jumping
                jumpSpeed = 10.0;
                isJumping = true;
            }
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
    glutSpecialFunc(specialKeys);

    // Register the keyboard function for space bar input
    glutKeyboardFunc(keyboard);

    glutMainLoop();  // Start the GLUT main loop

    return 0;
}
