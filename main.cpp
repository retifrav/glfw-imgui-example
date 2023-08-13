// C++
#include <string>
#include <iostream>
#include <filesystem>

// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Dear ImGui
#include <dearimgui/imgui.h>
#include <dearimgui/imgui_stdlib.h>
#include <dearimgui/imgui_impl_glfw.h>
#include <dearimgui/imgui_impl_opengl3.h>

#include "functions.h"
#include "imgui-style.h"

std::string programName = "GLFW and Dear ImGui";
int windowWidth = 1200,
    windowHeight = 800;
float highDPIscaleFactor = 1.0;
float backgroundR = 0.1f,
      backgroundG = 0.3f,
      backgroundB = 0.2f;
std::filesystem::path currentPath = ".";
std::filesystem::path basePath = ".";
std::string fontName = "JetBrainsMono-ExtraLight.ttf";

GLFWwindow *glfWindow = NULL;
bool show_demo_window = false;
bool show_another_window = false;
int counter = 0;

unsigned int shaderProgram, VBO, VAO;
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "[ERROR] GLFW error: " << error << ", " << description << std::endl;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void teardown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    if (glfWindow != NULL) { glfwDestroyWindow(glfWindow); }
    glfwTerminate();
}

bool initializeGLFW()
{
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        std::cerr << "[ERROR] Couldn't initialize GLFW" << std::endl;
        return false;
    }
    else
    {
        std::cout << "[INFO] GLFW initialized" << std::endl;
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER , 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    // adjust these values depending on the OpenGL supported by your GPU driver
    std::string glsl_version = "";
#ifdef __APPLE__
    // GLSL 150 + GL 4.1
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // required on Mac OS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#elif __linux__
    // GLSL 150 + GL 4.3
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#elif _WIN32
    // GLSL 130 + GL 4.6
    glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _WIN32
    // if it's a HighDPI monitor, try to scale everything
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    std::cout << "[INFO] Monitor scale: " << xscale << "x" << yscale << std::endl;
    if (xscale > 1 || yscale > 1)
    {
        highDPIscaleFactor = xscale;
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
#elif __APPLE__
    // to prevent 1200x800 from becoming 2400x1600
    // and some other weird resizings
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    //const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfWindow = glfwCreateWindow(
        windowWidth,//mode->width,
        windowHeight,//mode->height,
        programName.c_str(),
        NULL,//monitor
        NULL
    );
    if (!glfWindow)
    {
        std::cerr << "[ERROR] Couldn't create a GLFW window" << std::endl;
        return false;
    }

    glfwSetWindowPos(glfWindow, 100, 100);
    glfwSetWindowSizeLimits(
        glfWindow,
        static_cast<int>(900 * highDPIscaleFactor),
        static_cast<int>(500 * highDPIscaleFactor),
        GLFW_DONT_CARE,
        GLFW_DONT_CARE
    );

    // watch window resizing
    glfwSetFramebufferSizeCallback(glfWindow, framebuffer_size_callback);

    glfwMakeContextCurrent(glfWindow);
    // VSync
    glfwSwapInterval(1);

    std::cout << "[INFO] OpenGL from GLFW "
              << glfwGetWindowAttrib(glfWindow, GLFW_CONTEXT_VERSION_MAJOR)
              << "."
              << glfwGetWindowAttrib(glfWindow, GLFW_CONTEXT_VERSION_MINOR)
              << std::endl;

    return true;
}

bool initializeGLAD()
{
    // load all OpenGL function pointers with glad
    // without it not all the OpenGL functions will be available,
    // such as glGetString(GL_RENDERER), and application might just segfault
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "[ERROR] Couldn't initialize GLAD" << std::endl;
        return false;
    }
    else
    {
        std::cout << "[INFO] GLAD initialized" << std::endl;
    }

    std::cout << "[INFO] OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "[INFO] OpenGL from glad "
              << GLVersion.major << "." << GLVersion.minor
              << std::endl;

    return true;
}

bool initializeDearImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.Fonts->AddFontFromFileTTF(
        fontName.c_str(),
        24.0f * highDPIscaleFactor,
        NULL,
        NULL
    );
    setImGuiStyle(highDPIscaleFactor);

    // setup platform/renderer bindings
    if (!ImGui_ImplGlfw_InitForOpenGL(glfWindow, true)) { return false; }
    if (!ImGui_ImplOpenGL3_Init()) { return false; }

    return true;
}

// build and compile our shader program
void buildShaderProgram()
{
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO
    // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // you can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other VAOs requires a call
    // to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs)
    // when it's not directly necessary
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void composeDearImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    // standard demo window
    if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }

    // a window is defined by Begin/End pair
    {
        int glfw_width = 0, glfw_height = 0, controls_width = 0;
        // get the window size as a base for calculating widgets geometry
        glfwGetFramebufferSize(glfWindow, &glfw_width, &glfw_height);
        controls_width = glfw_width;
        // make controls widget width to be 1/3 of the main window width
        if ((controls_width /= 3) < 300) { controls_width = 300; }

        // position the controls widget in the top-right corner with some margin
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        // here we set the calculated width and also make the height to be
        // be the height of the main window also with some margin
        ImGui::SetNextWindowSize(
            ImVec2(static_cast<float>(controls_width), static_cast<float>(glfw_height - 20)),
            ImGuiCond_Always
            );

        ImGui::SetNextWindowBgAlpha(0.7f);
        // create a window and append into it
        ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize);

        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Time");
        ImGui::Text("%s", currentTime(std::chrono::system_clock::now()).c_str());

        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Application");
        ImGui::Text("Main window width: %d", glfw_width);
        ImGui::Text("Main window height: %d", glfw_height);

        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "GLFW");
        ImGui::Text("%s", glfwGetVersionString());

        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Dear ImGui");
        ImGui::Text("%s", IMGUI_VERSION);

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        // buttons and most other widgets return true when clicked/edited/activated
        if (ImGui::Button("Counter button"))
        {
            std::cout << "counter button clicked" << std::endl;
            counter++;
            if (counter == 9) { ImGui::OpenPopup("Easter egg"); }
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        if (ImGui::BeginPopupModal("Easter egg", NULL))
        {
            ImGui::Text("Ho-ho, you found me!");
            if (ImGui::Button("Buy Ultimate Orb")) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        if (!show_demo_window)
        {
            if (ImGui::Button("Open standard demo"))
            {
                show_demo_window = true;
            }
        }

        ImGui::Checkbox("show a custom window", &show_another_window);
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(
                ImVec2(250.0f, 150.0f),
                ImGuiCond_FirstUseEver // after first launch it will use values from imgui.ini
                );
            // the window will have a closing button that will clear the bool variable
            ImGui::Begin("A custom window", &show_another_window);

            ImGui::Dummy(ImVec2(0.0f, 1.0f));
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Some label");

            ImGui::TextColored(ImVec4(128 / 255.0f, 128 / 255.0f, 128 / 255.0f, 1.0f), "%s", "another label");
            ImGui::Dummy(ImVec2(0.0f, 0.5f));

            ImGui::Dummy(ImVec2(0.0f, 1.0f));
            if (ImGui::Button("Close"))
            {
                std::cout << "close button clicked" << std::endl;
                show_another_window = false;
            }

            ImGui::End();
        }

        ImGui::End();
    }
}

int main(int argc, char *argv[])
{
    std::cout << "["
              << currentTime(std::chrono::system_clock::now())
              << "] "
              << "Start\n- - -\n\n";

    // setting paths to resources
    currentPath = std::filesystem::current_path();
    //std::cout << "[DEBUG] Current working directory: " << currentPath << std::endl;
    //basePath = std::filesystem::path(argv[0]).parent_path();
    basePath = std::filesystem::path(argv[0]).remove_filename();
#ifndef _WIN32 // on Windows argv[0] is absolute path
    basePath = currentPath / basePath;
#endif
    //std::cout << "[DEBUG] Executable name/path: " << argv[0] << std::endl
    //          << "parent path: " << basePath << std::endl << std::endl;
    fontName = (basePath / fontName).string();

    if (!initializeGLFW())
    {
        std::cerr << "[ERROR] GLFW initialization failed" << std::endl;
        return EXIT_FAILURE;
    }

    if (!initializeGLAD())
    {
        std::cerr << "[ERROR] glad initialization failed" << std::endl;
        return EXIT_FAILURE;
    }

    if (!initializeDearImGui())
    {
        std::cerr << "[ERROR] Dear ImGui initialization failed" << std::endl;
        return EXIT_FAILURE;
    }

    // build and compile our shader program
    buildShaderProgram();

    // rendering loop
    while (!glfwWindowShouldClose(glfWindow))
    {
        // the frame starts with a clean scene
        glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // draw our triangle
        glUseProgram(shaderProgram);
        // seeing as we only have a single VAO there's no need to bind it every time,
        // but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glBindVertexArray(0); // no need to unbind it every time

        // Dear ImGui frame
        composeDearImGuiFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glfWindow);

        // continuous rendering, even if window is not visible or minimized
        glfwPollEvents();
        // or you can sleep the thread until there are some events
        // in case of running animations (glTF, for example), also call glfwPostEmptyEvent() in render()
        //glfwWaitEvents();
    }

    teardown();

    std::cout << "\n- - -\n"
              << "["
              << currentTime(std::chrono::system_clock::now())
              << "] "
              << "Quit\n";
    
    return 0;
}
