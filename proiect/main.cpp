#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp" //core glm functionality
#include "glm/gtc/matrix_transform.hpp" //glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp" //glm extension for computing inverse matrices
#include "glm/gtc/type_ptr.hpp" //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"

#include <iostream>

// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 model2;

glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;
glm::vec3 lightDir1;
glm::vec3 lightColor1;
// shader uniform locations
GLuint modelLoc;
GLuint viewLoc;
GLuint projectionLoc;
GLuint normalMatrixLoc;
GLuint lightDirLoc;
GLuint lightColorLoc;
GLuint lightDirLoc1;
GLuint lightColorLoc1;
// camera
gps::Camera myCamera(
    glm::vec3(0.7f, 30.0f, -9.0f),
    glm::vec3(0.0f, 0.0f, -10.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

GLfloat cameraSpeed = 0.1f;

GLboolean pressedKeys[1024];

// models
gps::Model3D teapot;
gps::Model3D scena;
gps::Model3D leu;
gps::Model3D soare;

GLfloat angle;

// shaders
gps::Shader myBasicShader;

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
	//TODO
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

	if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        } else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
}

const int glWindowWidth = 1920;
const int glWindowHeight = 1080;
float  mouseSpeed;
float horizontalAngle;
float verticalAngle;
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    //TODO
    mouseSpeed = 0.0005f;
    horizontalAngle += mouseSpeed * float(glWindowWidth / 2 - xpos);
    verticalAngle += mouseSpeed * float(glWindowHeight / 2 - ypos);

    myCamera.rotate(verticalAngle, horizontalAngle);
    glfwSetCursorPos(window, glWindowWidth / 2, glWindowHeight / 2);
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
}
int shadeMode = 0;
int sare = 0;
int merge = 0;
bool leuSare = false;
bool leuFata = false;
bool leuSpate = false;
bool soareFata = false;
bool soareSpate = false;
bool prezentare = false;
int contor = 0;
GLuint fogDensityLoc;
float density = 0.0f;
void processMovement() {
	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
		//update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

    if (pressedKeys[GLFW_KEY_L]) {
       
        if (prezentare)
            prezentare = false;
        else { prezentare = true; 
            contor++;
        }
        if (prezentare && contor == 1) {
            myCamera.pozitie(0.7f, 30.0f, -9.0f);
            myCamera.rotate(0,-0.25);
            //update view matrix
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }

     }

    if (pressedKeys[GLFW_KEY_Q]) {
        angle -= 1.0f;
        
        
    }

    if (pressedKeys[GLFW_KEY_E]) {
        angle += 1.0f;
        
        
    }
    if (pressedKeys[GLFW_KEY_K]) {
        if (leuSare)
            leuSare = false;
        else leuSare = true;


    }

    if (pressedKeys[GLFW_KEY_H]) {
        if (leuFata)
            leuFata = false;
        else leuFata = true;


    }

    if (pressedKeys[GLFW_KEY_J]) {
        if (leuSpate)
            leuSpate = false;
        else leuSpate = true;


    }

    if (pressedKeys[GLFW_KEY_T]) {
        if (soareFata)
            soareFata = false;
        else soareFata = true;


    }

    if (pressedKeys[GLFW_KEY_Y]) {
        if (soareSpate)
            soareSpate = false;
        else soareSpate = true;


    }

    if (pressedKeys[GLFW_KEY_M]) {
        shadeMode++;
        if (shadeMode == 3)
            shadeMode = 0;

    }
   
    if (pressedKeys[GLFW_KEY_V]) {
        myBasicShader.useShaderProgram();
        density += 0.0005;
        fogDensityLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fogDensity");
        glUniform1f(fogDensityLoc, density);
    }

    if (pressedKeys[GLFW_KEY_B]) {
        myBasicShader.useShaderProgram();
        density -= 0.0005;
        fogDensityLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fogDensity");
        glUniform1f(fogDensityLoc, density);
    }
}

void initOpenGLWindow() {
    myWindow.Create(1920, 1080, "OpenGL Project Core");
}

void setWindowCallbacks() {
	glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
}

void initOpenGLState() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
    glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initModels() {
    
    scena.LoadModel("models/projectscenapg5.obj");
    leu.LoadModel("models/leu.obj");
    soare.LoadModel("models/soare.obj");
}

void initShaders() {
	myBasicShader.loadShader(
        "shaders/basic.vert",
        "shaders/basic.frag");
}

void initUniforms() {
	myBasicShader.useShaderProgram();

    
    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

	// get view matrix for current camera
	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
	// send view matrix to shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

	// create projection matrix
	projection = glm::perspective(glm::radians(45.0f),
                               (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
                               0.1f, 500.0f);
	projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
	// send projection matrix to shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));	

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, -1.0f, 1.0f);
	lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
	// send light dir to shader
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
    lightDir1 = glm::vec3(23.8692f, 26.477f, -10.06f);

    lightDirLoc1 = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir1");
    // send light dir to shader
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));
    glUniform3fv(lightDirLoc1, 1, glm::value_ptr(lightDir1));

    //set light color
    lightColor1 = glm::vec3(1.0f, 0.0f, 0.0f); //white light
    lightColorLoc1= glGetUniformLocation(myBasicShader.shaderProgram, "lightColor1");

	// send light color to shader
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    glUniform3fv(lightColorLoc1, 1, glm::value_ptr(lightColor1));

}



void renderScena(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();
    model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 1, 0));
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    scena.Draw(shader);
}
float x=0;
float y = 0;
float xsoare = 0.0f;
void renderLeu(gps::Shader shader) {
    // select active shader program
    model2 = glm::mat4(1.0);
    shader.useShaderProgram();
    model2 = glm::translate(model2, glm::vec3(0.067f, 27.01f, 9.93f));
    model2 = glm::rotate(model2, glm::radians(angle), glm::vec3(0, 1, 0));
    model2= glm::translate(model2, glm::vec3(-0.067f, -27.01f, -9.93f));
    model2 = glm::translate(model2, glm::vec3(y, x, 0.00f));
    
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
   
    // draw teapot
    leu.Draw(shader);
}

void renderSoare(gps::Shader shader) {
    model2 = glm::mat4(1.0);
    // select active shader program
    shader.useShaderProgram();
    model2 = glm::translate(model2, glm::vec3(xsoare, 0.0f, 0.0f));
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    soare.Draw(shader);
}

float axax = 0.7f;
float axay = 30.0f;
float axaz =- 9.0f;
float xrot = 0.0f;
float yrot = -0.25f;
int animatie1 = 0;
int animatie2 = 0;
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //render the scene
    switch (shadeMode)
    {
    case 0:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case 2:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    }
   
    renderScena(myBasicShader);



    if (leuSare) {
        if (sare == 1) {
            if (x <= 1.0f)
                x += 0.1;
            else {
                sare = 0;
            }
        }

        if (sare == 0 && x >= 0.0f)
            x -= 0.1;
        else sare = 1;
    }
    else {
        if (x > 0.0f)
            x -= 0.1;
        else x = x;
    
    }

    if (leuFata) {
        if (merge == 0) {
            merge = 1;
            if (y < 3.0) { y += 0.1; }
            
               
        }
        if (merge == 1) {
            if (y < 3.0) { y += 0.1; }
        
        
        }
        if (merge == -1) {
            merge = 1;
            if (y < 3.0) { y += 0.1; }


        }
    }

    if (leuSpate) {
        if (merge == 0) {
            merge = -1;
            if (y > -3.0) { y -= 0.1; }


        }
        if (merge == 1) {
            merge = -1;
            if (y > -3.0) { y -= 0.1; }


        }
        if (merge == -1) {
            if (y > -3.0) { y -= 0.1; }


        }
    }

    if (soareFata && !soareSpate && xsoare>-23.8f)
        xsoare -= 1;
    if (!soareFata && soareSpate && xsoare < 12.0f)
        xsoare += 1;

    renderLeu(myBasicShader);
    renderSoare(myBasicShader);

    if (prezentare) {
        if (axay > 16.5f && axay<= 30.0f && axax<1) {
            axay -= 0.05;
            
        }
        
        
        if (yrot<-0.2 && yrot>-1.8 && axay<=16.5f && animatie1==0) {
            axax = 42.5;
            axay = 10.0;
            yrot -= 0.005;
        }
        if (axax<42.6f && axax>10.0f && axay == 10.0f && yrot<-1.8f && animatie2==0) {
            axax -= 0.05;
            animatie1 = 1;
            
        }
        if (axax<10.0f && yrot > -11.3f && yrot < -1.8f ) {
            if (animatie1 == 1) {
                yrot = -9.7;
                xrot = -0.3;
                animatie1 = 2;
                axaz = 20;
                axax = -40;
            }
            
            yrot -= 0.005;
        
        }

        if (animatie1 == 2 && yrot < -11.3) {
            animatie2 = 1;
            yrot = 0.05;
        
        }
        if (animatie2 == 1 && yrot>-3.0) {
            axax = 30.0;
            axaz = 20.0;
            yrot -= 0.005;
        
        }

    
            myCamera.pozitie(axax, axay, axaz);
            myCamera.rotate(xrot,yrot);
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

            printf("%f", axay);
    
    
    
    }


}

void cleanup() {
    myWindow.Delete();
    //cleanup code for your own data
}

int main(int argc, const char * argv[]) {

    try {
        initOpenGLWindow();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    initOpenGLState();
	initModels();
	initShaders();
	initUniforms();
    setWindowCallbacks();

	glCheckError();
	// application loop
	while (!glfwWindowShouldClose(myWindow.getWindow())) {
        processMovement();
	    renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());

		glCheckError();
	}

	cleanup();

    return EXIT_SUCCESS;
    
   


}


