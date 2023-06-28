/*
work06a

author: Davide Gadia

Real-Time Graphics Programming - a.a. 2022/2023
Master degree in Computer Science
Universita' degli Studi di Milano
*/

/*
OpenGL coordinate system (right-handed)
positive X axis points right
positive Y axis points up
positive Z axis points "outside" the screen


                              Y
                              |
                              |
                              |________X
                             /
                            /
                           /
                          Z
*/


// Std. Includes
#include <string>

// Loader for OpenGL extensions
// http://glad.dav1d.de/
// THIS IS OPTIONAL AND NOT REQUIRED, ONLY USE THIS IF YOU DON'T WANT GLAD TO INCLUDE windows.h
// GLAD will include windows.h for APIENTRY if it was not previously defined.
// Make sure you have the correct definition for APIENTRY for platforms which define _WIN32 but don't use __stdcall
#ifdef _WIN32
    #define APIENTRY __stdcall
#endif

#include "glad/glad.h"

// GLFW library to create window and to manage I/O
#include <GLFW/glfw3.h>

// another check related to OpenGL loader
// confirm that GLAD didn't include windows.h
#ifdef _WINDOWS_
    #error windows.h was included!
#endif

// classes developed during lab lectures to manage shaders, to load models, and for FPS camera
#include <utils/shader.h>
#include <utils/model.h>
#include <utils/camera.h>

// we load the GLM classes used in the application
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

// we include the library for images loading
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

// dimensions of application's window
GLuint screenWidth = 1280, screenHeight = 720;

// the rendering steps used in the application
enum render_passes{ SHADOWMAP, RENDER};

// callback functions for keyboard and mouse events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// if one of the WASD keys is pressed, we call the corresponding method of the Camera class
void apply_camera_movements();

// in this application, we have isolated the models rendering using a function, which will be called in each rendering step
void RenderObjects(Shader &shader, Model &planeModel, Model &cubeModel, Model &sphereModel, Model &bunnyModel, GLint render_pass, GLuint depthMap, GLuint colorMap);

// load image from disk and create an OpenGL texture
GLint LoadTexture(const char* path);

// we initialize an array of booleans for each keyboard key
bool keys[1024];

// we need to store the previous mouse position to calculate the offset with the current frame
GLfloat lastX, lastY;
// when rendering the first frame, we do not have a "previous state" for the mouse, so we need to manage this situation
bool firstMouse = true;

// parameters for time calculation (for animations)
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// rotation angle on Y axis
GLfloat orientationY = 0.0f;
// rotation speed on Y axis
GLfloat spin_speed = 30.0f;
// boolean to start/stop animated rotation on Y angle
GLboolean spinning = GL_FALSE;

// boolean to activate/deactivate wireframe rendering
GLboolean wireframe = GL_FALSE;

// View matrix: the camera moves, so we just set to indentity now
glm::mat4 view = glm::mat4(1.0f);

// Model and Normal transformation matrices for the objects in the scene: we set to identity
glm::mat4 sphereModelMatrix = glm::mat4(1.0f);
glm::mat3 sphereNormalMatrix = glm::mat3(1.0f);
glm::mat4 cubeModelMatrix = glm::mat4(1.0f);
glm::mat3 cubeNormalMatrix = glm::mat3(1.0f);
glm::mat4 bunnyModelMatrix = glm::mat4(1.0f);
glm::mat3 bunnyNormalMatrix = glm::mat3(1.0f);
glm::mat4 planeModelMatrix = glm::mat4(1.0f);
glm::mat3 planeNormalMatrix = glm::mat3(1.0f);

// we create a camera. We pass the initial position as a paramenter to the constructor. The last boolean tells if we want a camera "anchored" to the ground
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f), GL_FALSE);

// in this example, we consider a directional light. We pass the direction of incoming light as an uniform to the shaders
glm::vec3 lightDir0 = glm::vec3(1.0f, 1.0f, 1.0f);

// weight for the diffusive component
GLfloat Kd = 3.0f;
// roughness index for GGX shader
GLfloat alpha = 0.2f;
// Fresnel reflectance at 0 degree (Schlik's approximation)
GLfloat F0 = 0.9f;

// vector for the textures IDs
vector<GLint> textureID;

// UV repetitions
GLfloat repeat = 1.0;
GLuint VBO, VAO, EBO;

/////////////////// MAIN function ///////////////////////
int main()
{
    // Initialization of OpenGL context using GLFW
    glfwInit();
    // We set OpenGL specifications required for this application
    // In this case: 4.1 Core
    // If not supported by your graphics HW, the context will not be created and the application will close
    // N.B.) creating GLAD code to load extensions, try to take into account the specifications and any extensions you want to use,
    // in relation also to the values indicated in these GLFW commands
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // we set if the window is resizable
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // we create the application's window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "MagicEye", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // we put in relation the window and the callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // we disable the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD tries to load the context set by GLFW
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // we define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // we enable Z test
    glEnable(GL_DEPTH_TEST);

    //the "clear" color for the frame buffer
    glClearColor(0.26f, 0.46f, 0.98f, 1.0f);

    // we create the Shader Program used for objects
    Shader stereogram_shader = Shader("assets/shaders/stereogram.vert", "assets/shaders/stereogram.frag");
    Shader illumin_shader = Shader("assets/shaders/illumin.vert", "assets/shaders/illumin.frag");

    // we load the images and store them in a vector
    textureID.push_back(LoadTexture("assets/textures/UV_Grid_Sm.png"));
    textureID.push_back(LoadTexture("assets/textures/SoilCracked.png"));
    textureID.push_back(LoadTexture("assets/textures/rand.png"));

    // we load the model(s) (code of Model class is in include/utils/model.h)
    Model cubeModel("assets/models/cube.obj");
    Model sphereModel("assets/models/sphere.obj");
    Model bunnyModel("assets/models/bunny_lp.obj");
    Model planeModel("assets/models/plane.obj");

    GLfloat vertices[] = {
        1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint depthMapFBO, depthMap, colorMap;
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    glGenTextures(1, &colorMap);
    glBindTexture(GL_TEXTURE_2D, colorMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorMap, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ///////////////////////////////////////////////////////////////////

    // Projection matrix of the camera: FOV angle, aspect ratio, near and far planes
    glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 50.0f);

    GLuint colorSSBO, uvSSBO;
    glGenBuffers(1, &colorSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSBO); 
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec4) * width * height, nullptr, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, colorSSBO);

    glGenBuffers(1, &uvSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2) * width * height, nullptr, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, uvSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Rendering loop: this code is executed at each frame
    while(!glfwWindowShouldClose(window))
    {
        // we determine the time passed from the beginning
        // and we calculate time difference between current frame rendering and the previous one
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check is an I/O event is happening
        glfwPollEvents();
        // we apply FPS camera movements
        apply_camera_movements();

        view = camera.GetViewMatrix();
        illumin_shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(illumin_shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(illumin_shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(glGetUniformLocation(illumin_shader.Program, "lightVector"), 1, glm::value_ptr(lightDir0));
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderObjects(illumin_shader, planeModel, cubeModel, sphereModel, bunnyModel, SHADOWMAP, depthMap, colorMap);
        ///////////////////////////////////////////////////////////////////

        stereogram_shader.Use();
        GLuint index = glGetSubroutineIndex(stereogram_shader.Program, GL_FRAGMENT_SHADER, "firstPass");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSBO); 
        GLint zero = 0;
        glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32I, GL_RED_INTEGER, GL_INT, &zero);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        GLint textureLocation = glGetUniformLocation(stereogram_shader.Program, "depthMap");
        glUniform1i(textureLocation, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, colorMap);
        textureLocation = glGetUniformLocation(stereogram_shader.Program, "colorMap");
        glUniform1i(textureLocation, 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureID[2]);
        textureLocation = glGetUniformLocation(stereogram_shader.Program, "random");
        glUniform1i(textureLocation, 2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        index = glGetSubroutineIndex(stereogram_shader.Program, GL_FRAGMENT_SHADER, "secondPass");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
    }

    // when I exit from the graphics loop, it is because the application is closing
    // we delete the Shader Programs
    stereogram_shader.Delete();
    illumin_shader.Delete();

    // chiudo e cancello il contesto creato
    glfwTerminate();
    return 0;
}

//////////////////////////////////////////
// we render the objects. We pass also the current rendering step, and the depth map generated in the first step, which is used by the shaders of the second step
void RenderObjects(Shader &shader, Model &planeModel, Model &cubeModel, Model &sphereModel, Model &bunnyModel, GLint render_pass, GLuint depthMap, GLuint colorMap)
{
    // we pass the needed uniforms
    GLint textureLocation = glGetUniformLocation(shader.Program, "tex");
    GLint repeatLocation = glGetUniformLocation(shader.Program, "repeat");

    // PLANE
    // we activate the texture of the plane
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    glUniform1i(textureLocation, 1);
    glUniform1f(repeatLocation, 80.0);

    /*
      we create the transformation matrix

      N.B.) the last defined is the first applied

      We need also the matrix for normals transformation, which is the inverse of the transpose of the 3x3 submatrix (upper left) of the modelview. We do not consider the 4th column because we do not need translations for normals.
      An explanation (where XT means the transpose of X, etc):
        "Two column vectors X and Y are perpendicular if and only if XT.Y=0. If We're going to transform X by a matrix M, we need to transform Y by some matrix N so that (M.X)T.(N.Y)=0. Using the identity (A.B)T=BT.AT, this becomes (XT.MT).(N.Y)=0 => XT.(MT.N).Y=0. If MT.N is the identity matrix then this reduces to XT.Y=0. And MT.N is the identity matrix if and only if N=(MT)-1, i.e. N is the inverse of the transpose of M.
    */
    // we reset to identity at each frame
    planeModelMatrix = glm::mat4(1.0f);
    planeNormalMatrix = glm::mat3(1.0f);
    planeModelMatrix = glm::translate(planeModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    planeModelMatrix = glm::scale(planeModelMatrix, glm::vec3(10.0f, 1.0f, 10.0f));
    planeNormalMatrix = glm::inverseTranspose(glm::mat3(view*planeModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(planeModelMatrix));
    glUniformMatrix3fv(glGetUniformLocation(shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(planeNormalMatrix));
    // we render the plane
    planeModel.Draw();

    // SPHERE
    // we activate the texture of the object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glUniform1i(textureLocation, 0);
    glUniform1f(repeatLocation, repeat);

    // we reset to identity at each frame
    sphereModelMatrix = glm::mat4(1.0f);
    sphereNormalMatrix = glm::mat3(1.0f);
    sphereModelMatrix = glm::translate(sphereModelMatrix, glm::vec3(-3.0f, 1.0f, 0.0f));
    sphereModelMatrix = glm::rotate(sphereModelMatrix, glm::radians(orientationY), glm::vec3(0.0f, 1.0f, 0.0f));
    sphereModelMatrix = glm::scale(sphereModelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
    sphereNormalMatrix = glm::inverseTranspose(glm::mat3(view*sphereModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(sphereModelMatrix));
    glUniformMatrix3fv(glGetUniformLocation(shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(sphereNormalMatrix));

    // we render the sphere
    sphereModel.Draw();

    // CUBE
    // we reset to identity at each frame
    cubeModelMatrix = glm::mat4(1.0f);
    cubeNormalMatrix = glm::mat3(1.0f);
    cubeModelMatrix = glm::translate(cubeModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    cubeModelMatrix = glm::rotate(cubeModelMatrix, glm::radians(orientationY), glm::vec3(0.0f, 1.0f, 0.0f));
    cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
    cubeNormalMatrix = glm::inverseTranspose(glm::mat3(view*cubeModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(cubeModelMatrix));
    glUniformMatrix3fv(glGetUniformLocation(shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(cubeNormalMatrix));

    // we render the cube
    cubeModel.Draw();

    // BUNNY
    // we reset to identity at each frame
    bunnyModelMatrix = glm::mat4(1.0f);
    bunnyNormalMatrix = glm::mat3(1.0f);
    bunnyModelMatrix = glm::translate(bunnyModelMatrix, glm::vec3(3.0f, 1.0f, 0.0f));
    bunnyModelMatrix = glm::rotate(bunnyModelMatrix, glm::radians(orientationY), glm::vec3(0.0f, 1.0f, 0.0f));
    bunnyModelMatrix = glm::scale(bunnyModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
    bunnyNormalMatrix = glm::inverseTranspose(glm::mat3(view*bunnyModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(bunnyModelMatrix));
    glUniformMatrix3fv(glGetUniformLocation(shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(bunnyNormalMatrix));

    // we render the bunny
    bunnyModel.Draw();
}

//////////////////////////////////////////
// we load the image from disk and we create an OpenGL texture
GLint LoadTexture(const char* path)
{
    GLuint textureImage;
    int w, h, channels;
    unsigned char* image;
    image = stbi_load(path, &w, &h, &channels, STBI_rgb);

    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;

    glGenTextures(1, &textureImage);
    glBindTexture(GL_TEXTURE_2D, textureImage);
    // 3 channels = RGB ; 4 channel = RGBA
    if (channels==3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    else if (channels==4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    // we set how to consider UVs outside [0,1] range
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // we set the filtering for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    // we free the memory once we have created an OpenGL texture
    stbi_image_free(image);

    // we set the binding to 0 once we have finished
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureImage;
}

//////////////////////////////////////////
// If one of the WASD keys is pressed, the camera is moved accordingly (the code is in utils/camera.h)
void apply_camera_movements()
{
    // if a single WASD key is pressed, then we will apply the full value of velocity v in the corresponding direction.
    // However, if two keys are pressed together in order to move diagonally (W+D, W+A, S+D, S+A), 
    // then the camera will apply a compensation factor to the velocities applied in the single directions, 
    // in order to have the full v applied in the diagonal direction  
    // the XOR on A and D is to avoid the application of a wrong attenuation in the case W+A+D or S+A+D are pressed together.  
    GLboolean diagonal_movement = (keys[GLFW_KEY_W] ^ keys[GLFW_KEY_S]) && (keys[GLFW_KEY_A] ^ keys[GLFW_KEY_D]); 
    camera.SetMovementCompensation(diagonal_movement);
    
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

//////////////////////////////////////////
// callback for keyboard events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // if ESC is pressed, we close the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // if P is pressed, we start/stop the animated rotation of models
    if(key == GLFW_KEY_P && action == GLFW_PRESS)
        spinning=!spinning;

    // if L is pressed, we activate/deactivate wireframe rendering of models
    if(key == GLFW_KEY_L && action == GLFW_PRESS)
        wireframe=!wireframe;

    // we keep trace of the pressed keys
    // with this method, we can manage 2 keys pressed at the same time:
    // many I/O managers often consider only 1 key pressed at the time (the first pressed, until it is released)
    // using a boolean array, we can then check and manage all the keys pressed at the same time
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

//////////////////////////////////////////
// callback for mouse events
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
      // we move the camera view following the mouse cursor
      // we calculate the offset of the mouse cursor from the position in the last frame
      // when rendering the first frame, we do not have a "previous state" for the mouse, so we set the previous state equal to the initial values (thus, the offset will be = 0)
      if(firstMouse)
      {
          lastX = xpos;
          lastY = ypos;
          firstMouse = false;
      }

      // offset of mouse cursor position
      GLfloat xoffset = xpos - lastX;
      GLfloat yoffset = lastY - ypos;

      // the new position will be the previous one for the next frame
      lastX = xpos;
      lastY = ypos;

      // we pass the offset to the Camera class instance in order to update the rendering
      camera.ProcessMouseMovement(xoffset, yoffset);
}
