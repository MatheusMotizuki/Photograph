#include "Application.hpp"
#include <iostream>

#include <GLES3/gl3.h>

Application::Application(const std::string& title, int width, int height)
    : m_title(title)
    , m_width(width)
    , m_height(height)
    , m_running(false)
    , m_window(nullptr)
    , m_gl_context(nullptr)
{
}

Application::~Application()
{
    shutdown();
}

bool Application::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Setup SDL with OpenGL ES 3.0 for web, OpenGL 3.3 for desktop
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_width,
        m_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (!m_window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    m_gl_context = SDL_GL_CreateContext(m_window);
    if (!m_gl_context)
    {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_MakeCurrent(m_window, m_gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize GUI
    m_gui = std::make_unique<GUI>(m_window, m_gl_context);
    if (!m_gui->initialize())
    {
        std::cerr << "Failed to initialize GUI" << std::endl;
        return false;
    }

    // initialize GUI
    m_gui = std::make_unique<GUI>(m_window, m_gl_context);
    if (!m_gui->initialize())
    {
        std::cerr << "Error initializing GUI" << std::endl;
        return false;
    }

    m_running = true;
    std::cout << "Application initialized successfully!" << std::endl;
    return true;
}

void Application::run()
{
    // Set up the main loop FIRST, then it will handle timing automatically
    emscripten_set_main_loop_arg(emscripten_loop, this, 0, 1);
}

void Application::emscripten_loop(void* arg)
{
    Application* app = static_cast<Application*>(arg);
    app->m_running = app->processEvents();
    if (!app->m_running)
    {
        emscripten_cancel_main_loop();
        return;
    }
    app->update();
    app->render();
}

bool Application::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        m_gui->processEvent(event);
        
        if (event.type == SDL_QUIT)
            return false;
        
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
            return false;
    }
    
    return true;
}

void Application::update()
{
    m_gui->newFrame();
}

void Application::render()
{
    // Clear screen
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render GUI
    m_gui->render();
    
    SDL_GL_SwapWindow(m_window);
}

void Application::shutdown()
{
    if (m_gui)
    {
        m_gui.reset();
    }
    
    if (m_gl_context)
    {
        SDL_GL_DeleteContext(m_gl_context);
        m_gl_context = nullptr;
    }
    
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    
    SDL_Quit();
}
