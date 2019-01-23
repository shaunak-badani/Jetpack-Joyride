#include "main.h"
#include "timer.h"
#include "ball.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
int noOfFrames = 50;

int score = 0;

Ball ball1,platform[16*50],top[16*50],wall[2*50],coins[16*50],firelines[50];

float camera_rotation_angle = 90.0f;

// Eye - Location of camera. Don't change unless you are sure!!
glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
// Target - Where is the camera looking at.  Don't change unless you are sure!!
glm::vec3 target (0, 0, 0);
// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
glm::vec3 up (0, 1, 0);


void moveWindow(float offset){
    if(eye.x + offset < 0 || target.x + offset < 0)
        return;
    if(eye.x + offset >= (noOfFrames-1) * 8 + 1)
        return;
    eye.x += offset;
    target.x += offset;
}

void movePlayerForward() {
    if(ball1.position.x >= (noOfFrames-1) * 8 - 2)
    {
        moveWindow(0.5);
        return;
    }
    if(ball1.position.x + 0.05 > 1.0 + eye.x) {
        moveWindow(0.05);    
        // Move window faster to test edge cases
        // moveWindow(0.5);    
        // return;
    }
    ball1.position.x += 0.05;
    // ball1.position.x += 0.5;
}

void movePlayerBackward() {
    if(ball1.position.x - 0.05 < -3.0 + eye.x) {
        moveWindow(-0.05);
        return;
    }
    ball1.position.x -= 0.05;
}


void makePolygon(GLfloat g_vertex_buffer_data[], float centerX, float centerY, float sideLength, int n) {
    float angle = 2 * M_PI / n;
    float start_coordX = sideLength / 2;
    float start_coordY = -start_coordX / tan(angle / 2);
    float tempX, tempY;
    for(int i = 0 ; i < 3*n ; i += 3)
	{
		// Initializing all 3rd co-ordinates to 0
		g_vertex_buffer_data[3*i] = centerX;
		g_vertex_buffer_data[3*i + 1] = centerY;
		g_vertex_buffer_data[3*i + 2] = 0.0f;
	
		g_vertex_buffer_data[3*(i+1)] = centerX + start_coordX;
		g_vertex_buffer_data[3*(i+1) + 1] = centerY + start_coordY;
		g_vertex_buffer_data[3*(i+1) + 2] = 0.0f;
	
		g_vertex_buffer_data[3*(i+2)] = centerX + start_coordX * cos(angle) - start_coordY * sin(angle);
		g_vertex_buffer_data[3*(i+2) + 1] = centerY + start_coordX * sin(angle) + start_coordY*cos(angle) ;
		g_vertex_buffer_data[3*(i+2) + 2] = 0.0f;

		tempX = start_coordX;
		tempY = start_coordY;	

		start_coordX = tempX * cos(angle) - tempY * sin(angle);
		start_coordY = tempX * sin(angle) + tempY * cos(angle); 
	}
}

void makeFireLine(GLfloat fireLine[], float centerX, float centerY, float width, float height, float angle) {

    float start_coordX = width * cos(angle)/2;
    float start_coordY = width * sin(angle)/2;

    fireLine[0] = centerX + start_coordX + height*sin(angle)/2;
    fireLine[1] = centerY + start_coordY - height*cos(angle) /2; 
    fireLine[2] = 0.0f;
    fireLine[3] = centerX + start_coordX - height*sin(angle)/2;
    fireLine[4] = centerY + start_coordY + height*cos(angle)/2;
    fireLine[5] = 0.0f;
    fireLine[6] = 0.0f;
    fireLine[7] = 0.0f; 
    fireLine[8] = 0.0f;     
    fireLine[9] = centerX + start_coordX - height*sin(angle)/2;
    fireLine[10] = centerY + start_coordY + height*cos(angle)/2;
    fireLine[11] = 0.0f;
    fireLine[12] = centerX -start_coordX - height*sin(angle)/2; 
    fireLine[13] = centerY - start_coordY + height*cos(angle) /2;
    fireLine[14] = 0.0f;
    fireLine[15] = 0.0f;
    fireLine[16] = 0.0f;
    fireLine[17] = 0.0f;
    fireLine[18] = centerX - start_coordX - height*sin(angle)/2;
    fireLine[19] = centerY - start_coordY + height*cos(angle) /2;
    fireLine[20] = 0.0f;
    fireLine[21] = centerX - start_coordX + height*sin(angle)/2;
    fireLine[22] = centerY - start_coordY - height*cos(angle) /2;
    fireLine[23] = 0.0f;
    fireLine[24] = 0.0f, 
    fireLine[25] = 0.0f;
    fireLine[26] = 0.0f;
    fireLine[27] = centerX - start_coordX + height*sin(angle)/2;
    fireLine[28] = centerY - start_coordY - height*cos(angle) /2;
    fireLine[29] = 0.0f;
    fireLine[30] = centerX + start_coordX + height*sin(angle)/2;
    fireLine[31] = centerY + start_coordY - height*cos(angle) /2;
    fireLine[32] = 0.0f;     
    fireLine[33] = 0.0f;
    fireLine[34] = 0.0f;
    fireLine[35] = 0.0f;
}


float lengthToRadius(float sideLength,int n) {
    return 2*sideLength*sin(M_PI/n);
}

float rDG(int range) {
    return ((float)(rand()%500))/1000 + rand() % (range + 1);
}


void initializeObjects() {
    GLfloat vertex_data[10000][100];
    makePolygon(vertex_data[0], 0.0f, 0.0f, lengthToRadius(0.5,60), 60); 
    ball1       = Ball(-2.5, -2.75, 0,vertex_data[0],60,COLOR_RED);

    makePolygon(vertex_data[1], 0.0f, 0.0f, lengthToRadius(0.1,60),60);

    GLfloat vertex_data_x[] = {
        -1.0f, -2.75f, 0.0f,
        -0.75f, -3.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        -0.75f,-3.0f,0.0f,
        0.75f,-3.0f,0.0f,
        0.0f,0.0f,0.0f,
        0.75f, -3.0f, 0.0f,
        1.0f, -2.75f, 0.0f,
        0.0f,0.0f,0.0f,
        1.0f, -2.75f, 0.0f,
        1.0f, 2.75f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 2.75f, 0.0f,
        0.75f, 3.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.75f, 3.0f, 0.0f,
        -0.75f, 3.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        -0.75f, 3.0f, 0.0f,
        -1.0f, 2.75f, 0.0f,
        0.0f, 0.0f, 0.0f,
        -1.0f, 2.75f, 0.0f,
        -1.0f, -2.75f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    GLfloat vertex_data_y[] = {
        -2.0f, -2.75f, 0.0f,
        -1.75f, -3.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        -1.75f,-3.0f,0.0f,
        1.75f,-3.0f,0.0f,
        0.0f,0.0f,0.0f,
        1.75f, -3.0f, 0.0f,
        2.0f, -2.75f, 0.0f,
        0.0f,0.0f,0.0f,
        2.0f, -2.75f, 0.0f,
        2.0f, 2.75f, 0.0f,
        0.0f, 0.0f, 0.0f,
        2.0f, 2.75f, 0.0f,
        1.75f, 3.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.75f, 3.0f, 0.0f,
        -1.75f, 3.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        -1.75f, 3.0f, 0.0f,
        -2.0f, 2.75f, 0.0f,
        0.0f, 0.0f, 0.0f,
        -2.0f, 2.75f, 0.0f,
        -2.0f, -2.75f, 0.0f,
        0.0f, 0.0f, 0.0f
    };
    

    GLfloat fireLineVertices[50][36];
    float widthLine, heightLine, centerY, angle;
    for(int i = 0; i < noOfFrames ; i++) {
        widthLine = 1 + rDG(2);
        heightLine = rDG(0);
        angle = 2 * M_PI * rDG(0);
        makeFireLine(fireLineVertices[i],0,0,widthLine,heightLine,angle );
        centerY = -2.5 + widthLine * sin(angle) / 2 + heightLine*cos(angle) / 2 + rDG((int)(6 - widthLine*sin(angle) - heightLine*cos(angle)));
        firelines[i] = Ball(-4.0 + i*8 + rDG(8),centerY,0,fireLineVertices[i],12,COLOR_ORANGE);
        firelines[i].width = widthLine;
        firelines[i].height = heightLine; 
    }

    for(int i = 0; i < 2*noOfFrames ; i+= 2) {
        wall[i] = Ball(-2.5 + i*4,0,0,vertex_data_x,8,COLOR_BLACK);
        wall[i+1] = Ball(1.5 + i*4,0,0,vertex_data_y,8,COLOR_GREEN);
    }

    for(int i = 0 ; i < 15*noOfFrames ; i++) {
        float randY = ((float)(rand()%1000))/1000 + ((rand() % 7) - 3.5);
        float randX = ((float)(rand()%1000))/1000 + (rand() % 396);
        coins[i] = Ball(randX,randY,0,vertex_data[1],60,COLOR_BLUE);
    }

    int objectNo = 2;
    for(int i = 0 ; i < 16*noOfFrames; i++,objectNo++) {
        makePolygon(vertex_data[objectNo], 0.0f, 0.0f, 1.0f, 4);
        platform[i] = Ball(-3.75f + i*0.5,-3.75f, 0.0,vertex_data[objectNo],4,COLOR_GREY);
    }

    for(int i = 0 ; i < 16*noOfFrames; i++,objectNo++) {
        makePolygon(vertex_data[objectNo], 0.0f, 0.0f, 1.0f, 4);
        top[i] = Ball(-3.75f + i*0.5,3.75f, 0.0,vertex_data[objectNo],4,COLOR_GREY);
    }


}

void tick_all() {
    ball1.tick();
}

void draw_all(glm::mat4 VP){
    for(int i = 0;i < 2*noOfFrames; i++) {
        wall[i].draw(VP);
    }
    for(int i = 0;i < 15*noOfFrames ; i++) {
        coins[i].draw(VP);
    }
    ball1.draw(VP);
    for(int i = 0;i < 16*noOfFrames; i++) {
        platform[i].draw(VP);
    }
    for(int i = 0;i < 16*noOfFrames; i++) {
        top[i].draw(VP);
    }
    for(int i = 0; i < noOfFrames ; i++) {
        firelines[i].draw(VP);
    }
}

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model
    // Scene render
    draw_all(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int key_d = glfwGetKey(window, GLFW_KEY_D);
    int key_a = glfwGetKey(window, GLFW_KEY_A);
    int key_space = glfwGetKey(window, GLFW_KEY_SPACE);
    if (left) {
        // ball1.speed -= 1;
    }
    if(key_d) {
        movePlayerForward();
    }
    if(key_a) {
        movePlayerBackward();
    }
    if(key_space) {
        ball1.speedY += 0.02;
    }
}

void ballCollision() {
    bounding_box_t temp1,temp2;
    temp1.x = ball1.position.x;
    temp1.y = ball1.position.y;
    temp1.width = 0.5;
    temp1.height = 0.5;

    if(ball1.position.y < -2.75) {
        ball1.position.y = -2.75;
        ball1.speedY = 0;
    }
    else{
        ball1.speedY -= 0.01;
    }
    if(ball1.position.y > 2.75) {
        ball1.position.y = 2.75;
        ball1.speedY = 0;
    }
    temp2.width = 0.1f;
    for(int i = 0; i < 15*noOfFrames ; i++) {
        temp2.x = coins[i].position.x;
        temp2.y = coins[i].position.y;
        if(detect_collision(temp1,temp2)) {
            score+=10;
            cout << "Score : " << score << endl;
            coins[i].position.y = 8;
        }
    }
}

// Function which executes after every ticking of clock

void tick_elements() {
    ballCollision();
    tick_all();
    // camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    initializeObjects();

    // coin1       = Coin(-1, -1, 0, COLOR_GREEN);
    // ball2       = Ball(3, 0, 0.025, COLOR_GREEN);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    //   Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    // switch(type) {
        // case "coin" : 
            return (sqrt((a.x - b.x)*(a.x - b.x) + (a.y-b.y)*(a.y - b.y)) < a.width + b.width);
        // case "fireline" :
            // return (abs(a.x - b.x) < b.width / 2 + a.width) && (abs(a.y - b.y) < b.height / 2 + a.width);
        // default:
            // return false;
    // }   
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
