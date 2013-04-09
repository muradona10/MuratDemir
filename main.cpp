// murat demir - 040080223

#include <iostream>
#include <GL/glew.h>
#include "Scene.h"
#include <GL\glut.h>
#include "ScreenSpaceShaderNode.h"
#include "NormalShaderNode.h"
#include "ColorFillNode.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#define WINDOW_TITLE_PREFIX "Ders 7"

void IdleFunction(void);
void ResizeFunction(int Width, int Height);
void setupScene();

unsigned FrameCount = 0;

int CurrentWidth = 800,
CurrentHeight = 600,
WindowHandle = 0, mouseX = 0, mouseY = 0;

SceneNode* pad1;
SceneNode* pad2;
SceneNode* ball;

Camera *camera;

Scene* scene;

float directionX = 1;
float directionY = 0.5;
float speed = 0.02;

static void Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    scene->draw();
    
	glLineWidth(4.5);
	glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(-1.0, 1.0, 0.0);
		glVertex3f(1.0, 1.0, 0);
	glEnd();

	glLineWidth(4.5);
	glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(-1.0, -1.0, 0.0);
		glVertex3f(1.0, -1.0, 0);
	glEnd();

	glLineWidth(4.5);
	glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(-1.0, 1.0, 0.0);
		glVertex3f(-1.0, -1.0, 0);
	glEnd();

	glLineWidth(4.5);
	glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(1.0, -1.0, 0);
	glEnd();

	glLineWidth(2.5);
	glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(0.0, -1, 0.0);
		glVertex3f(0.0, 1, 0);
	glEnd();

    glutSwapBuffers();
    FrameCount++;
}

static void Key(unsigned char key, int x, int y)
{
    printf("%d", key);
	switch (key) {
		case 27: // ESC
			exit(0);
            break;
        case 'a': // ESC
			pad1->translate(0, -4, 0);
            break;
        case 'q': // ESC
			pad1->translate(0, 4, 0);
            break;
        case 'l': // ESC
			pad2->translate(0, -4, 0);
            break;
        case 'o': // ESC
			pad2->translate(0, 4, 0);
            break;
	}
}
void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

static void timerCallback (int value)
{
    if (0 != value) {
		char* TempString = (char*)
        malloc(512 + strlen(WINDOW_TITLE_PREFIX));
        
		sprintf(
                TempString,
                "%s: %d Frames Per Second @ %d x %d",
                WINDOW_TITLE_PREFIX,
                FrameCount * 4,
                CurrentWidth,
                CurrentHeight
                );
        
		glutSetWindowTitle(TempString);
		free(TempString);
	}
	FrameCount = 0;
	glutTimerFunc(250, timerCallback, 1);
}

void IdleFunction(void)
{
    glm::mat4 ballMatrix = ball->getModelMatrix();
    float x = ball->getPosition().x;
    float y = ball->getPosition().y;

	float pad1x = pad1->getPosition().x; // pad1 in pozisyonunun x koordinatý
	float pad2x = pad2->getPosition().x; // pad2 nin pozisyonunun x koordinatý
	float pad1y = pad1->getPosition().y; // pad1 in pozisyonunun y koordinatý
    float pad2y = pad2->getPosition().y; // pad2 in pozisyonunun y koordinatý
	
    if( y > 1.15 )
    {
        directionY *= -1;
    }
    if( y < -1.25 )
    {
        directionY *= -1;
    }
    if( x > pad2x-0.085 && (y < pad2y+0.3 && y > pad2y-0.3))
    {
        directionX *= -1;
    }
	if( x > pad2x || x < pad1x)
	{
		printf("Game Over");
		exit(1);
	}

    if( x < pad1x+0.10 && (y < pad1y+0.3 && y > pad1y-0.3) )
    {
        directionX *= -1;
    }
	
	if(pad1y > 0.95)
		pad1->translate(0, -4, 0);
	if(pad1y < -0.95)
		pad1->translate(0, 4, 0);
	if(pad2y > 0.95)
		pad2->translate(0, -4, 0);
	if(pad2y < -0.95)
		pad2->translate(0, 4, 0);


    //pad1->rotate(1, 0, 1, 0);
    ball->translate(directionX*speed, directionY*speed, 0);
	glutPostRedisplay();
}

void setupScene(){
    scene = new Scene();
    
    pad1 = new ScreenSpaceShaderNode("C:/opengl/Pad.obj");
    
    pad2 = new ColorFillNode("C:/opengl/Pad.obj");
    
    ball = new ColorFillNode("C:/opengl/bunny.obj");
    
    pad1->scale(0.0008,0.0038,0.01);
    pad2->scale(0.0008,0.0038,0.01);
	ball->scale(0.5,0.5,0.5);

    scene->addNode(pad1);
    scene->addNode(pad2);
    scene->addNode(ball);
    
    camera = new Camera();
    
    camera->translate(0.f, 0.f, 3.f);
	
    pad1->translate(-1900.f, 0.f, 0.f);
	
    
    pad2->translate( 1900.f, 0.f, 0.f);
    
    scene->setCamera(camera);
}

int main (int argc, char ** argv)
{
    
    GLenum type;
    
	glutInit(&argc, argv);
	glutInitWindowSize(CurrentWidth,CurrentHeight);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
    type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	glutCreateWindow("");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    
    glClearColor(0.0, 1.0, 0.0, 1.0);
    // Z-Buffer i aciyoruz
    glEnable(GL_DEPTH_TEST);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE); 
    
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    printf("GLSL Version: %s\n",glslVersion);
    
    const GLubyte* glVersion = glGetString(GL_VERSION);
    
    printf("GL Version: %s\n",glVersion);
       
    setupScene();

	glLineWidth(2.5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(15, 0, 0);
	glEnd();
    
    timerCallback(0);
    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(Draw);
    glutKeyboardFunc(Key);
    glutIdleFunc(IdleFunction);
    glutMainLoop();
    
    return 0;
}


