#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>
#include <unistd.h>
#include <vector>
#include <png.h>
#include <iostream>

bool running = true;

float playerX = 50.0;
float playerY = 50.0;
float playerSize = 50.0;

GLuint backgroundTexture;  // Variable to store the background texture ID
float backgroundSizeX = 800.0;
float backgroundSizeY = 600.0;

// NPC class definition
class NPC {
public:
    float x, y;
    float size;
    float dx, dy;

    NPC(float startX, float startY, float npcSize) : x(startX), y(startY), size(npcSize) {
        dx = (rand() % 20 - 10) / 10.0f;
        dy = (rand() % 20 - 10) / 10.0f;
    }

    void move() {
        x += dx;
        y += dy;

        if (x < 0 || x > 800 - size) dx = -dx;
        if (y < 0 || y > 600 - size) dy = -dy;
    }

    void draw() const {
        glColor3f(0.0, 1.0, 0.0);
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

//background rendering
GLuint loadPNG(const char* filename, float* width, float* height) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 0;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(file);
        std::cerr << "Error: png_create_read_struct failed" << std::endl;
        return 0;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        fclose(file);
        png_destroy_read_struct(&png, NULL, NULL);
        std::cerr << "Error: png_create_info_struct failed" << std::endl;
        return 0;
    }

    if (setjmp(png_jmpbuf(png))) {
        fclose(file);
        png_destroy_read_struct(&png, &info, NULL);
        std::cerr << "Error: Error during png_setjmp" << std::endl;
        return 0;
    }

    png_init_io(png, file);
    png_read_info(png, info);

    // Ensure RGBA format
    if (png_get_color_type(png, info) != PNG_COLOR_TYPE_RGBA) {
        std::cerr << "Error: Unsupported color type (required RGBA)" << std::endl;
        fclose(file);
        png_destroy_read_struct(&png, &info, NULL);
        return 0;
    }

    *width = static_cast<float>(png_get_image_width(png, info));
    *height = static_cast<float>(png_get_image_height(png, info));

    // Read image data
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * static_cast<size_t>(*height));
    for (size_t y = 0; y < static_cast<size_t>(*height); y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    fclose(file);

    // Create OpenGL texture and upload image data
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(*width), static_cast<GLsizei>(*height), 0, GL_RGBA, GL_UNSIGNED_BYTE, row_pointers[0]);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clean up
    for (size_t y = 0; y < static_cast<size_t>(*height); y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_read_struct(&png, &info, NULL);

    return textureID;
}


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 600);

    // Load the background texture using libpng
    backgroundTexture = loadPNG("background.png", &backgroundSizeX, &backgroundSizeY);

    // Create NPCs
    for (int i = 0; i < 3; i++) {
        npcs.emplace_back(rand() % 800, rand() % 600, 30.0f);
    }
}

void update() {
    // Update NPCs
    for (auto& npc : npcs) {
        npc.move();
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the background
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
    glTexCoord2f(1.0, 0.0); glVertex2f(backgroundSizeX, 0);
    glTexCoord2f(1.0, 1.0); glVertex2f(backgroundSizeX, backgroundSizeY);
    glTexCoord2f(0.0, 1.0); glVertex2f(0, backgroundSizeY);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Draw the player as a red box
    glColor3f(1.0, 0.0, 0.0);
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

    glFlush();
}

void timer(int value) {
    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(33, timer, 0); // Set the next timer
}

void idle() {
    update();
}

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
    glutSpecialFunc(specialKeys);

    glutMainLoop();

    return 0;
}
