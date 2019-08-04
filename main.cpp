// C++
#include <string>
#include <iostream>
// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Dear ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"

#include "functions.h"
#include "imgui-style.h"

std::string programName = "GLFW and Dear ImGui";
int windowWidth = 1200,
    windowHeight = 800;
float backgroundR = 0.1f,
      backgroundG = 0.3f,
      backgroundB = 0.2f;

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "[ERROR] GLFW error: " << error << ", " << description << std::endl;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void teardown(GLFWwindow *window)
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window != NULL) { glfwDestroyWindow(window); }
    glfwTerminate();
}

// https://sourceforge.net/p/anttweakbar/code/ci/master/tree/examples/TwSimpleGLFW.c
void DrawModel(int wireframe)
{
    int pass, numPass;

    // enable OpenGL transparency and light
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);    // use default light diffuse and position
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(3.0);
    
    if(wireframe)
    {
        glDisable(GL_CULL_FACE);    
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        numPass = 1;
    }
    else
    {
        glEnable(GL_CULL_FACE); 
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        numPass = 2;
    }

    for(pass = 0; pass < numPass; ++pass)
    {
        // since the material could be transparent,
        // we draw the convex model in 2 passes
        // first its back faces, and second its front faces.
        glCullFace((pass == 0) ? GL_FRONT : GL_BACK);
        // draw the model (a cube)
        glBegin(GL_QUADS);
            // front face
            glNormal3f(0,0,-1); glVertex3f(0,0,0); glVertex3f(0,1,0); glVertex3f(1,1,0); glVertex3f(1,0,0);
            // back face
            glNormal3f(0,0,+1); glVertex3f(0,0,1); glVertex3f(1,0,1); glVertex3f(1,1,1); glVertex3f(0,1,1);
            // left face
            glNormal3f(-1,0,0); glVertex3f(0,0,0); glVertex3f(0,0,1); glVertex3f(0,1,1); glVertex3f(0,1,0);
            // right face
            glNormal3f(+1,0,0); glVertex3f(1,0,0); glVertex3f(1,1,0); glVertex3f(1,1,1); glVertex3f(1,0,1);
            // bottom face
            glNormal3f(0,-1,0); glVertex3f(0,0,0); glVertex3f(1,0,0); glVertex3f(1,0,1); glVertex3f(0,0,1); 
            // top face
            glNormal3f(0,+1,0); glVertex3f(0,1,0); glVertex3f(0,1,1); glVertex3f(1,1,1); glVertex3f(1,1,0);
        glEnd();
    }
}

int main(int argc, char *argv[])
{
    std::cout << "["
              << currentTime(std::chrono::system_clock::now())
              << "] "
              << "Start\n- - -\n\n";

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        std::cerr << "[ERROR] Couldn't initialize GLFW\n";
        return -1;
    }
    else
    {
        std::cout << "[INFO] GLFW initialized\n";
    }

    // if it's a HighDPI monitor, try to scale everything
    // but it doesn't really work that well, from what I saw in Windows
    /*
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    std::cout << xscale << ":" << yscale;
    if (xscale > 1 || yscale > 1)
    {
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
    */
#ifdef __APPLE__
    // to prevent 1200x800 from becoming 2400x1600
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    GLFWwindow *window = glfwCreateWindow(
        windowWidth,
        windowHeight,
        programName.c_str(),
        NULL,
        NULL
        );
    if (!window)
    {
        std::cerr << "[ERROR] Couldn't create a GLFW window\n";
        teardown(NULL);
        return -1;
    }
    // watch window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    // VSync
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "[ERROR] Couldn't initialize GLAD" << std::endl;
        teardown(window);
        return -1;
    }
    else
    {
        std::cout << "[INFO] GLAD initialized\n";
    }

    std::cout << "[INFO] OpenGL "
              << GLVersion.major << "." << GLVersion.minor
              << std::endl;

    // --- Dear ImGui

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    io.Fonts->AddFontFromFileTTF("verdana.ttf", 18.0f, NULL, NULL);

    setImGuiStyle();

    // setup platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    bool show_demo_window = false;
    bool show_another_window = false;
    // colors are set in RGBA, but as float
    ImVec4 background = ImVec4(35/255.0f, 35/255.0f, 35/255.0f, 1.00f);

    //glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);

    // --- model

    // current time and enlapsed time
    double time = glfwGetTime(), dt;
    // model turn counter
    double turn = 0;
    // model rotation speed
    double speed = 0.2;
    // draw model in wireframe?
    int wire = 0;
    // model color (32bits RGBA)
    unsigned char cubeColor[] = { 255, 0, 0, 128 };

    // --- rendering loop

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rotate model
        dt = glfwGetTime() - time;
        if( dt < 0 ) dt = 0;
        time += dt;
        turn += speed * dt;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(360.0*turn, 0.4, 1, 0.2);
        glTranslated(-0.5, -0.5, -0.5);
        // set color and draw model
        glColor4ubv(cubeColor);
        DrawModel(wire);

        // start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // standard demo window
        if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }
        // a window is defined by Begin/End pair
        {
            static int counter = 0;

            int glfw_width = 0, glfw_height = 0, controls_width = 0;
            // get the window size as a base for calculating widgets geometry
            glfwGetFramebufferSize(window, &glfw_width, &glfw_height);
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

            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            
            // buttons and most other widgets return true when clicked/edited/activated
            if (ImGui::Button("Counter button"))
            {
                std::cout << "counter button clicked\n";
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
                    std::cout << "close button clicked\n";
                    show_another_window = false;
                }

                ImGui::End();
            }

            ImGui::End();
        }

        // rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        glfwMakeContextCurrent(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    teardown(window);

    std::cout << "\n- - -\n"
              << "["
              << currentTime(std::chrono::system_clock::now())
              << "] "
              << "Quit\n";
    
    return 0;
}
