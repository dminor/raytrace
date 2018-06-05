#include <GL/glew.h>

#include <SDL/SDL.h>

typedef struct {
    float x, y, z, r, g, b;
} Point;

Point *pts;
GLuint *pt_indices;
GLint pt_count;

GLuint pt_buffer_id[2];

void init()
{
    //init gl state
    glClearColor(0.2f, 0.2f, 0.5f, 0.0f);

    glViewport(0, 0, 640, 480);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glPointSize(2);
    gluPerspective(30, 1.5, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, -5.0, -25.0f);

    //set up vertex and index buffer
    glGenBuffers(2, pt_buffer_id);

    glBindBuffer(GL_ARRAY_BUFFER, pt_buffer_id[0]);
    glBufferData(GL_ARRAY_BUFFER, pt_count*sizeof(Point), pts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pt_buffer_id[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pt_count*sizeof(GLuint),
        pt_indices, GL_STATIC_DRAW);
}

void render()
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, pt_buffer_id[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pt_buffer_id[1]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Point), 0);
    glColorPointer(3, GL_FLOAT, sizeof(Point), (void *)(3*sizeof(float)));

    glDrawElements(GL_POINTS, pt_count, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapBuffers();
}

int main(int argc, char **argv)
{
    int done = 0;
    SDL_Event ev;
    int err, i;

    //check arguments
    if (argc != 2) {
        fprintf(stderr, "usage: view-photon-map <filename>\n");
        return 1;
    }

    //read points
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "error: could not open %s\n", argv[1]);
        return 1;
    }

    err = fscanf(f, "%d", &pt_count);
    if (err != 1 || pt_count <= 0) {
        fprintf(stderr, "error: invalid point count %d\n", pt_count);
        return 1;
    }

    pts = (Point *)malloc(pt_count*sizeof(Point));
    pt_indices = (GLuint*)malloc(pt_count*sizeof(GLuint));
    if (!pts || !pt_indices) {
        fprintf(stderr, "error: could not allocate memory for points\n");
        return 1;
    }

    i = 0;
    while (!feof(f) && i < pt_count) {
        err = fscanf(f, "%f %f %f %f %f %f", &pts[i].x, &pts[i].y, &pts[i].z,
            &pts[i].r, &pts[i].g, &pts[i].b);

        if (err != 6) {
            fprintf(stderr, "warning: invalid point %d ignored\n", i);
            continue;
        }

        //clamp colours to viewable range
        if (pts[i].r > 1.0f) pts[i].r = 1.0f;
        if (pts[i].g > 1.0f) pts[i].g = 1.0f;
        if (pts[i].b > 1.0f) pts[i].b = 1.0f;

        pt_indices[i] = i;

        ++i;
    }

    fclose(f);

    //set up SDL for OpenGL
    if (SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        fprintf(stderr, "error: unable to initialize SDL: %s\n",
            SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16,
        SDL_OPENGL);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "error: unable to initialize GLEW\n");
        return 1;
    }

    init();

    while (!done) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_KEYDOWN) {
                switch(ev.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        done = 1;
                        break;
                }
            }
        }

        render();
    }

    free(pts);

    return 0;
}
