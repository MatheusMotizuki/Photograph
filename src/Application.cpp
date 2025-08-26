#include "Application.hpp"
#include <iostream>

Application::Application(const std::string& title, int width, int height)
    : m_title(title)
    , m_width(width)
    , m_height(height)
    , m_running(false)
    , m_window(nullptr)
    , m_renderer(nullptr)
    , m_gui(nullptr)
{
}

Application::~Application()
{
    shutdown();
}

bool Application::initialize()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_width,
        m_height,
        window_flags
    );

    if (!m_window)
    {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
    );

    if (!m_renderer)
    {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize GUI
    m_gui = std::make_unique<GUI>(m_window, m_renderer);
    if (!m_gui->initialize())
    {
        std::cerr << "Error initializing GUI" << std::endl;
        return false;
    }

    m_running = true;
    return true;
}

void Application::run()
{
    while (m_running)
    {
        m_running = processEvents();
        update();
        render();
    }
}

bool Application::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        m_gui->processEvent(event);
        
        if (event.type == SDL_QUIT)
            return false;
        
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(m_window))
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
    // Clear the renderer
    SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255);
    SDL_RenderClear(m_renderer);
    
    // Render GUI
    m_gui->render();
    
    // Present the renderer
    SDL_RenderPresent(m_renderer);
}

void Application::shutdown()
{
    if (m_gui)
    {
        m_gui.reset();
    }
    
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    
    SDL_Quit();
}