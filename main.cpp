#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "utils/shader.h"
#include "utils/model.h"
#include "utils/camera.h"
#include "utils/image.h"

// dimensions of application's window
GLuint screenWidth = 1280, screenHeight = 720;

// the rendering steps used in the application
enum render_passes{ SHADOWMAP, RENDER};

// callback functions for keyboard and mouse events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int w, int h);
// if one of the WASD keys is pressed, we call the corresponding method of the Camera class
void apply_camera_movements();

// in this application, we have isolated the models rendering using a function, which will be called in each rendering step
void RenderObjects(Shader &shader, Model &planeModel, Model &cubeModel, Model &sphereModel, Model &bunnyModel, GLint render_pass, GLuint depthMap, GLuint colorMap);

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
std::vector<GLint> textureID;

// UV repetitions
GLfloat repeat = 1.0;
GLuint VBO, VAO, EBO;

bool showMenu = false;

float depthStrength = 5;
int width, height;
/////////////////// MAIN function ///////////////////////
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "MagicEye", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(0);

    // we put in relation the window and the callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // we disable the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD tries to load the context set by GLFW
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // we define the viewport dimensions
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorMap, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ///////////////////////////////////////////////////////////////////

    // Projection matrix of the camera: FOV angle, aspect ratio, near and far planes
    glm::mat4 projection = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 50.0f);

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

    auto deltas = std::vector<float>(256);
    int deltaIndex = 0;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    float oldW = width;
    float oldH = height;
    // Rendering loop: this code is executed at each frame
    while (!glfwWindowShouldClose(window)) {
        // we determine the time passed from the beginning
        // and we calculate time difference between current frame rendering and the previous one
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        deltas[deltaIndex = (deltaIndex+1) % 256] = deltaTime;
        float acc = 0.0;
        for (int i = 0; i < 256; i++) {
            acc += deltas[i];
        }
        float FPS = 256/acc;

        if (oldH != height || oldW != width) {
            projection = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 50.0f);

            glBindTexture(GL_TEXTURE_2D, depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glBindTexture(GL_TEXTURE_2D, colorMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSBO); 
            glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec4) * width * height, nullptr, GL_DYNAMIC_COPY);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvSSBO);
            glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2) * width * height, nullptr, GL_DYNAMIC_COPY);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            oldH = height;
            oldW = width;
        }

        glfwPollEvents();
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

        stereogram_shader.Use();

        glUniform1f(glGetUniformLocation(stereogram_shader.Program, "time"), currentFrame);
        glUniform1fv(glGetUniformLocation(stereogram_shader.Program, "depthStrength"), 1, &depthStrength);
        glUniform1i(glGetUniformLocation(stereogram_shader.Program, "bufferWidth"), width);
        glUniform1i(glGetUniformLocation(stereogram_shader.Program, "bufferHeight"), height);

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

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        index = glGetSubroutineIndex(stereogram_shader.Program, GL_FRAGMENT_SHADER, "secondPass");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        if (showMenu) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Settings");
            ImGui::Text("FPS: %.1f", FPS);
            ImGui::SliderFloat("Depth strength", &depthStrength, 0.5f, 20.0f, "%.1f");
            
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(window);
    }

    stereogram_shader.Delete();
    illumin_shader.Delete();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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

void apply_camera_movements() {
    GLboolean diagonal_movement = (keys[GLFW_KEY_W] ^ keys[GLFW_KEY_S]) && (keys[GLFW_KEY_A] ^ keys[GLFW_KEY_D]); 
    camera.SetMovementCompensation(diagonal_movement);
    
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        showMenu = !showMenu;
        glfwSetInputMode(window, GLFW_CURSOR, showMenu ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if(action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (!showMenu) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    width = w;
    height = h;
}