#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include "stb_easy_font.h"

typedef struct {
    int         width, height;
    GLFWwindow  *main;
    void        (*draw)();
} WindowInstance;

typedef WindowInstance *wi_t;

wi_t init_window_instance(int w, int h, char *title)
{
    if(!glfwInit())
        return NULL;

    wi_t window = (wi_t)malloc(sizeof(WindowInstance));
    window->width = w, window->height = h;
    window->main = glfwCreateWindow(w, h, title, NULL, NULL);
    if(!window->main)
        return NULL;

    glfwMakeContextCurrent(window->main);

    return window;
}

void set_resize_handler(wi_t wi, void (*handle)())
{
    glfwSetFramebufferSizeCallback(wi->main, handle);
}

void display_window(wi_t wi, int thread)
{
    while(!glfwWindowShouldClose(wi->main))
    {
        glClear(GL_COLOR_BUFFER_BIT);


        wi->draw(wi);

        glfwSwapBuffers(wi->main);
        glfwPollEvents();
        usleep(16000);
    }
}

void window_destruct(wi_t wi)
{
    glfwDestroyWindow(wi->main);
    glfwTerminate();
}

void draw_box(float cx, float cy, float w, float h, float rgb[3])
{
    glColor3f(rgb[0], rgb[1], rgb[2]);

    glBegin(GL_QUADS);
        glVertex2f(cx - w/2, cy - h/2);
        glVertex2f(cx + w/2, cy - h/2);
        glVertex2f(cx + w/2, cy + h/2);
        glVertex2f(cx - w/2, cy + h/2);
    glEnd();
}

void draw_rounded_box(float cx, float cy, float w, float h, float r, int segments, float rgb[3])
{
    glColor3f(rgb[0], rgb[1], rgb[2]);

    glBegin(GL_QUADS);
        glVertex2f(cx - w/2 + r, cy - h/2);
        glVertex2f(cx + w/2 - r, cy - h/2);
        glVertex2f(cx + w/2 - r, cy + h/2);
        glVertex2f(cx - w/2 + r, cy + h/2);

        glVertex2f(cx - w/2, cy - h/2 + r);
        glVertex2f(cx - w/2 + r, cy - h/2 + r);
        glVertex2f(cx - w/2 + r, cy + h/2 - r);
        glVertex2f(cx - w/2, cy + h/2 - r);

        glVertex2f(cx + w/2 - r, cy - h/2 + r);
        glVertex2f(cx + w/2, cy - h/2 + r);
        glVertex2f(cx + w/2, cy + h/2 - r);
        glVertex2f(cx + w/2 - r, cy + h/2 - r);
    glEnd();

    float startAngles[4] = {3.14159265f, 1.5f*3.14159265f, 0.5f*3.14159265f, 0.0f};

    for (int i = 0; i < 4; i++)
    {
        float centerX = (i%2==0) ? cx - w/2 + r : cx + w/2 - r;
        float centerY = (i<2) ? cy - h/2 + r : cy + h/2 - r;

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(centerX, centerY);

        for (int j = 0; j <= segments; j++)
        {
            float theta = startAngles[i] + (3.14159265f/2.0f) * j / (float)segments;
            float x = centerX + r * cosf(theta);
            float y = centerY + r * sinf(theta);
            glVertex2f(x, y);
        }
        glEnd();
    }
}

void draw_text(wi_t wi, int pos_col, int pos_height, char *title, float scale, float r, float b, float g)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, wi->width, wi->height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if(!scale)
        scale = 2.0f;

    glPushMatrix();
    glScalef(scale, scale, 1);
    glTranslatef(pos_col, pos_height, 0);
    glColor3f(r, g, b);

    char buffer[99999];
    int num_quads = stb_easy_font_print(0, 0, title, NULL, buffer, sizeof(buffer));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads*4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void draw_window(wi_t wi)
{
    glColor3f(0.2f, 0.6f, 0.9f);
    draw_rounded_box(
        -0.6,
        -0.1,
        0.6f,
        1.5f,
        0.1f,
        18,
        (float [3]){1.0f, 1.0f, 1.0f}
    );
    
    draw_rounded_box(
        -0.6,
        0.5,
        0.5f,
        0.2f,
        0.1f,
        18,
        (float [3]){1.0f, 0.0f, 0.0f}
    );
    draw_text(wi, 62, 95, "Disassembled View", 1.5f, 1.0f, 1.0f, 1.0f);
    
    draw_rounded_box(
        0,
        0.85,
        0.6f,
        0.2f,
        0.1f,
        10,
        (float [3]){1.0f, 0.0f, 0.0f}
    );

    draw_text(wi, 155, 20, "CoreSTD Debugger", 2.0f, 1.0f, 1.0f, 1.0f);
}

int main(void)
{
    wi_t wi = init_window_instance(800, 600, "CoreSTD Compiler - Debugger");
    wi->draw = draw_window;
    display_window(wi, 0);
    window_destruct(wi);
    return 0;
}
