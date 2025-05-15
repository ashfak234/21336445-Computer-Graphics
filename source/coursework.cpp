#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>

// Function prototypes
void keyboardInput(GLFWwindow* window);
void mouseInput(GLFWwindow* window);

// Frame timers
float previousTime = 0.0f;  // time of previous iteration of the loop
float deltaTime = 0.0f;  // time elapsed since the previous frame

// Create camera object
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

// Object struct
struct Object
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    float angle = 0.0f;
    std::string name;
};



int main(void)
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Coursework", NULL, NULL);

    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // DISABLED CULLING
    glDisable(GL_CULL_FACE);

    // Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Capture mouse inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Compile shader program
    unsigned int shaderID, lightShaderID;
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    lightShaderID = LoadShaders("lightVertexShader.glsl", "lightFragmentShader.glsl");

    // Activate shader
    glUseProgram(shaderID);

    // Load models
    Model plane("../assets/plane.obj");
    Model oak_wood("../assets/cube.obj"); 
    Model oak_plank("../assets/cube.obj");
    Model glass("../assets/cube.obj");
    Model door_top("../assets/cube.obj");
    Model door_bottom("../assets/cube.obj"); 

    
    // Load the textures
    plane.addTexture("../assets/grass.jpg", "diffuse");
    oak_wood.addTexture("../assets/oak_wood.jpg", "diffuse");
    oak_plank.addTexture("../assets/oak_plank.jpg", "diffuse");
    glass.addTexture("../assets/glass.png", "diffuse");  
    door_top.addTexture("../assets/door_top.png", "diffuse");
    door_bottom.addTexture("../assets/door_bottom.png", "diffuse");


    // this will store different types of objects
    std::vector<Object> objects;
    Object object;

    // i will manually adjust the block postion and scale

    //plain (grass field)
    object.position = glm::vec3(-2.0f, -1.0f, 0.0f);
    object.scale = glm::vec3(20.0f, 1.0f, 20.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "plane";
    objects.push_back(object);

    //front part of the house
    object.position = glm::vec3(0.0f, 2.0f, 0.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "door_top";
    objects.push_back(object);

    object.position = glm::vec3(0.0f, 0.0f, 0.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "door_bottom";
    objects.push_back(object);

 
    object.position = glm::vec3(-2.0f, 0.0f, 0.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "oak_plank";
    objects.push_back(object);

    object.position = glm::vec3(2.0f, 0.0f, 0.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "oak_plank";
    objects.push_back(object);

    object.position = glm::vec3(2.0f, 4.0f, 0.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "oak_plank";
    objects.push_back(object);

    object.position = glm::vec3(0.0f, 4.0f, 0.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "oak_plank";
    objects.push_back(object);

    object.position = glm::vec3(-2.0f, 4.0f, 0.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "oak_plank";
    objects.push_back(object);

    object.position = glm::vec3(-2.0f, 2.0f, 0.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "glass";
    objects.push_back(object);

    object.position = glm::vec3(2.0f, 2.0f, 0.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "glass";
    objects.push_back(object);

    
    object.position = glm::vec3(0.0f, 8.0f, -4.0f);
    object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    object.angle = 0.0f;
    object.name = "oak_wood";
    objects.push_back(object);

    float x = 0.0f;

    int count = 0;

    while (count < 3) {

        // four oak wood postions (pillars)
        object.position = glm::vec3(-4.0f, x, 0.0f); // front left
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_wood";
        objects.push_back(object);

        object.position = glm::vec3(4.0f, x, 0.0f); // front right
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_wood";
        objects.push_back(object);

        object.position = glm::vec3(4.0f, x, -8.0f); // back right
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_wood";
        objects.push_back(object);

        object.position = glm::vec3(-4.0f, x, -8.0f); // back left
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_wood";
        objects.push_back(object);
        
        //left side of house
        object.position = glm::vec3(-4.0f, 0.0f, -2.0f-x);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_plank";
        objects.push_back(object);

        object.position = glm::vec3(-4.0f, 2.0f, -2.0f - x);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "glass";
        objects.push_back(object);

        object.position = glm::vec3(-4.0f, 4.0f, -2.0f - x);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_plank";
        objects.push_back(object);

        // right side of house

        object.position = glm::vec3(4.0f, 0.0f, -2.0f - x);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_plank";
        objects.push_back(object);

        object.position = glm::vec3(4.0f, 2.0f, -2.0f - x);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "glass";
        objects.push_back(object);

        object.position = glm::vec3(4.0f, 4.0f, -2.0f - x);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_plank";
        objects.push_back(object);


        // back side of house

        object.position = glm::vec3(-2.0f + x, 0.0f, -8.0f);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_plank";
        objects.push_back(object);

        object.position = glm::vec3(-2.0f + x, 2.0f, -8.0f);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "glass";
        objects.push_back(object);

        object.position = glm::vec3(-2.0f + x, 4.0f, -8.0f);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_plank";
        objects.push_back(object);

        //top 


        object.position = glm::vec3(-2.0f + x, 6.0f, -6.0f);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_wood";
        objects.push_back(object);

        object.position = glm::vec3(-2.0f + x, 6.0f, -4.0f);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_wood";
        objects.push_back(object);

        object.position = glm::vec3(-2.0f + x, 6.0f, -2.0f);
        object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
        object.angle = 0.0f;
        object.name = "oak_wood";
        objects.push_back(object);

        // adds a block
        x += 2.0f;
        count++; 
    }



    //object.position = glm::vec3(-4.0f, 0.0f, -0.0f);
    //object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    //object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    //object.angle = 0.0f;
    //object.name = "oak_wood";
    //objects.push_back(object);

    //object.position = glm::vec3(4.0f, 0.0f, -0.0f);
    //object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    //object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    //object.angle = 0.0f;
    //object.name = "oak_wood";
    //objects.push_back(object);

    //object.position = glm::vec3(4.0f, 0.0f, -8.0f);
    //object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    //object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    //object.angle = 0.0f;
    //object.name = "oak_wood";
    //objects.push_back(object);

    //object.position = glm::vec3(4.0f, 0.0f, -8.0f);
    //object.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    //object.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    //object.angle = 0.0f;
    //object.name = "oak_wood";
    //objects.push_back(object);




    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Update timer
        float time = glfwGetTime();
        deltaTime = time - previousTime;
        previousTime = time;

        // Get inputs
        keyboardInput(window);
        mouseInput(window);

        // Clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate view and projection matrices
        camera.target = camera.eye + camera.front;
        camera.calculateMatrices();

        // Activate shader
        glUseProgram(shaderID);

 

        // Loop through objects
        for (unsigned int i = 0; i < static_cast<unsigned int>(objects.size()); i++)
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(objects[i].position);
            glm::mat4 scale = Maths::scale(objects[i].scale);
            glm::mat4 rotate = Maths::rotate(objects[i].angle, objects[i].rotation);
            glm::mat4 model = translate * rotate * scale;

            // Send the MVP and MV matrices to the vertex shader
            glm::mat4 MV = camera.view * model;
            glm::mat4 MVP = camera.projection * MV;
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MV"), 1, GL_FALSE, &MV[0][0]);


            if (objects[i].name == "plane")
                plane.draw(shaderID);

            if (objects[i].name == "oak_wood")
                oak_wood.draw(shaderID);

            if (objects[i].name == "oak_plank")
                oak_plank.draw(shaderID);

            if (objects[i].name == "glass")
                glass.draw(shaderID);

            if (objects[i].name == "door_top")
                door_top.draw(shaderID);

            if (objects[i].name == "door_bottom")
                door_bottom.draw(shaderID);
        }



        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    
    plane.deleteBuffers();
    oak_wood.deleteBuffers();
    oak_plank.deleteBuffers();
    glass.deleteBuffers();
    glDeleteProgram(shaderID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}






void keyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Move the camera using WASD keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.eye += 15.0f * deltaTime * camera.front;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.eye -= 15.0f * deltaTime * camera.front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.eye -= 15.0f * deltaTime * camera.right;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.eye += 15.0f * deltaTime * camera.right;


    // left shift key only will go down in the y axis
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
        camera.eye -= 15.0f * deltaTime * camera.up;


    // go up in the y axis
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.eye += 15.0f * deltaTime * camera.up;  
}

void mouseInput(GLFWwindow* window)
{
    // Get mouse cursor position and reset to centre
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Update yaw and pitch angles
    camera.yaw += 0.005f * float(xPos - 1024 / 2);
    camera.pitch += 0.005f * float(768 / 2 - yPos);

    // Calculate camera vectors from the yaw and pitch angles
    camera.calculateCameraVectors();
}

