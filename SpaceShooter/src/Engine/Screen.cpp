#include "Glinclude.h"
#include "Screen.h"


Ptr<Screen> Screen::mInstance = nullptr;

int GLFWCALL Screen::CloseCallback()
{
    Screen::Instance()->mOpened = false;
    return GL_TRUE;
}

Ptr<Screen> Screen::Instance()
{ 
    if (mInstance == nullptr) 
        mInstance = new Screen(); 
    return mInstance;
}

Screen::Screen()
{
    glfwInit();

    mOpened = false;
}

Screen::~Screen()
{
    glfwTerminate();
}

void Screen::Open(uint16 width, uint16 height, bool fullscreen)
{
    // Open window
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    glfwOpenWindow(int(width), int(height), 8, 8, 8, 8, 0, 0, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW );
    if ( !fullscreen )
        glfwSetWindowPos((GetDesktopWidth()-width)/2, (GetDesktopHeight()-height)/2);
    glfwSetWindowCloseCallback(GLFWwindowclosefun(CloseCallback));
    glfwSwapInterval(1);
    SetTitle("");
    mOpened = true;

    // Initialize OpenGL
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    // Configure viewport
    mWidth = width;
    mHeight = height;
    glViewport(0, 0, width, height);

    // Configure Proy matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 0, 1000);

    // Configure Model matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize elapsed time
    mLastTime = (float)glfwGetTime();
    mElapsed = 0.0f;
}

void Screen::Close()
{
    glfwCloseWindow();
}

void Screen::SetTitle(const String &title)
{
    glfwSetWindowTitle(title.ToCString());
}

void Screen::Refresh()
{
    glfwSwapBuffers();
    glfwGetMousePos(&mMousex, &mMousey);
    mElapsed = ((float)glfwGetTime()) - mLastTime;
    mLastTime = (float)glfwGetTime();
}

uint16 Screen::GetDesktopWidth() const
{
    GLFWvidmode mode;
    glfwGetDesktopMode(&mode);
    return uint16(mode.Width);
}

uint16 Screen::GetDesktopHeight() const
{
    GLFWvidmode mode;
    glfwGetDesktopMode(&mode);
    return uint16(mode.Height);
}

bool Screen::MouseButtonPressed(int button) const
{
    return glfwGetMouseButton(button) == GLFW_PRESS;
}

bool Screen::KeyPressed(int key) const
{
    return glfwGetKey(key) == GLFW_PRESS;
}
