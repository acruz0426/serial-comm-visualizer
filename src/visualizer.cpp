#include "../include/visualizer.hpp"
#include "../include/serial_comm.hpp"
#include <iostream>

///////////////////////////////////////////////////
///                Button Class                 /// 
///////////////////////////////////////////////////

Button::Button(SDL_Renderer *renderer, int x, int y, int w, int h, const std::string &name, TTF_Font *font, std::function<void()> func)
    : m_x1(x), m_y1(y), m_w(w), m_h(h), m_name(name), m_function(func)
{
    m_renderer = renderer;
    m_font = font;
    m_x2 = m_x1 + m_w;
    m_y2 = m_y1+ m_h;
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w;
    m_rect.h = h;
    m_color_background.r = 255;
    m_color_background.g = 255;
    m_color_background.b = 255;
    m_color_background.a = 255;
    m_color_text.r = 0;
    m_color_text.g = 0;
    m_color_text.b = 0;
    m_color_text.a = 0;
    m_text_rect.w = m_rect.w / 2;
    m_text_rect.h = m_rect.h / 2;
    m_text_rect.x = m_rect.x + (m_rect.w - m_text_rect.w) / 2;
    m_text_rect.y = m_rect.y + (m_rect.h - m_text_rect.h) / 2;
    
}

bool Button::isColliding(int mouseX, int mouseY)
{
    if ((mouseX >= this->m_x1 && mouseX <= this->m_x2) && (mouseY >= this->m_y1 && mouseY <= this->m_y2))
        return true;
    return false;
}

void Button::render(bool isCollide=false)
{
    //const SDL_Rect temp = this->m_rect;
    if (isCollide)
    {
        m_color_background.r = 200;
        m_color_background.g = 200;
        m_color_background.b = 200;
    }
    else 
    {
        m_color_background.r = 255;
        m_color_background.g = 255;
        m_color_background.b = 255;
    }
    SDL_SetRenderDrawColor(m_renderer, m_color_background.r, m_color_background.g, m_color_background.b, m_color_background.a);
    if (this->m_renderer == nullptr)
        fprintf(stderr, "Is nullptr\n");
    else if (SDL_RenderFillRect(this->m_renderer, &m_rect) != 0)
    {
        fprintf(stderr, "Failure to render Rect: %s\n", SDL_GetError());
    }

    // Draw text onto rect
    SDL_Surface *text = TTF_RenderText_Solid(m_font, m_name.c_str(), m_color_text);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, text);
    SDL_FreeSurface(text);
    text = nullptr;
    SDL_RenderCopy(m_renderer, texture, NULL, &m_text_rect);
    SDL_DestroyTexture(texture);
    texture = nullptr;

}

SDL_Rect Button::getRect()
{
    return m_rect;
}


/////// Update text rectangle too ///////////
void Button::setRect(int x, int y, int w, int h)
{
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w;
    m_rect.h = h;
    m_text_rect.w = m_rect.w / 2;
    m_text_rect.h = m_rect.h / 2;
    m_text_rect.x = m_rect.x + (m_rect.w - m_text_rect.w) / 2;
    m_text_rect.y = m_rect.y + (m_rect.h - m_text_rect.h) / 2;
    m_x1 = x;
    m_y1 = y;
    m_x2 = m_x1+w;
    m_y2 = m_y1+h;

}

void Button::onClick()
{
    if (m_function)
        m_function();
}

void Button::setFunction(std::function<void()> func)
{
    m_function = func;
}


///////////////////////////////////////////////////
///                Device Class                 /// 
///////////////////////////////////////////////////


Device::Device(SDL_Renderer *renderer, int x, int y, int w, int h, TTF_Font *font, int type)
    : m_x1(x), m_y1(y), m_w(w), m_h(h), m_comm_type(type)
{
    m_renderer = renderer;
    m_font = font;
    m_x2 = m_x1 + m_w;
    m_y2 = m_y1+ m_h;
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w;
    m_rect.h = h;
    m_color_background.r = 255;
    m_color_background.g = 255;
    m_color_background.b = 255;
    m_color_background.a = 255;
    m_color_text.r = 255;
    m_color_text.g = 255;
    m_color_text.b = 255;
    m_color_text.a = 255;
    m_text_rect.w = m_rect.w / 2;
    m_text_rect.h = m_rect.h / 2;
    m_text_rect.x = m_rect.x + (m_rect.w - m_text_rect.w) / 2;
    m_text_rect.y = m_rect.y + (m_rect.h - m_text_rect.h) / 2;
    if (type == 0)
        m_name = "I2C";
    else if (type == 1)
        m_name = "UART";
    else if (type == 2)
        m_name = "SPI";
    else if (type == 3)
        m_name = "CAN";
    else
        m_name = "NaN";
    // Load font for port
    m_portFont = TTF_OpenFont("./fonts/DejaVuSans.ttf", 12);
    if(font == nullptr)
    {
        fprintf(stderr, "Couldn't load font: %s\n", TTF_GetError());
    }
    // Create ports
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
            m_ports[i] = new Port(renderer, m_x1, m_y1+35+(35*i), true, m_portFont);
        else
            m_ports[i] = new Port(renderer, m_x1+DEVICE_WIDTH, m_y1+35+(35*(i%4)), false, m_portFont);
    }
}

Device::~Device()
{
    for (int i = 0; i < 8; i++)
    {
        delete m_ports[i];
    }
}


void Device::render()
{
    
    SDL_SetRenderDrawColor(m_renderer, m_color_background.r, m_color_background.g, m_color_background.b, m_color_background.a);
    if (this->m_renderer == nullptr)
        fprintf(stderr, "Is nullptr\n");
    else if (SDL_RenderDrawRect(this->m_renderer, &m_rect) != 0)
    {
        fprintf(stderr, "Failure to render Rect: %s\n", SDL_GetError());
    }

    // Draw text onto rect
    SDL_Surface *text = TTF_RenderText_Solid(m_font, m_name.c_str(), m_color_text);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, text);
    SDL_FreeSurface(text);
    text = nullptr;
    SDL_RenderCopy(m_renderer, texture, NULL, &m_text_rect);
    SDL_DestroyTexture(texture);
    texture = nullptr;
    
    // Render Ports on Device
    for (int i = 0; i < 8; i++)
    {
        m_ports[i]->render();
    }

}


void Device::setType(int type)
{
    m_comm_type = type;
    if (type == 0)
        m_name = "I2C";
    else if (type == 1)
        m_name = "UART";
    else if (type == 2)
        m_name = "SPI";
    else if (type == 3)
        m_name = "CAN";
    else
        m_name = "NaN";
}

Port** Device::getPorts()
{
    return m_ports;
}

///////////////////////////////////////////////////
///                  Port Class                 /// 
///////////////////////////////////////////////////

Port::Port(SDL_Renderer *renderer, int x, int y, bool left, TTF_Font *font)
    : m_in_x(x), m_in_y(y), m_out_y(y), m_font(font), m_isLeft(left)
{
    m_renderer = renderer;
    if (left == true)
    {
        m_out_x = m_in_x - PORT_LINE_DIST;
        m_rect = {m_in_x+5, m_in_y-PORT_HEIGHT/2, PORT_WIDTH, PORT_HEIGHT};
    }
    else
    {
        m_out_x = m_in_x + PORT_LINE_DIST;
        m_rect = {m_in_x-PORT_WIDTH-5, m_in_y-PORT_HEIGHT/2, PORT_WIDTH, PORT_HEIGHT};
    }
    m_color = {255, 255, 255, 255};
}


void Port::render()
{
    SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    if (this->m_renderer == nullptr)
        fprintf(stderr, "Is nullptr\n");
    else if (SDL_RenderDrawLine(this->m_renderer, m_in_x, m_in_y, m_out_x, m_out_y) != 0)
    {
        fprintf(stderr, "Failure to render Rect: %s\n", SDL_GetError());
    }
    SDL_Surface *text = TTF_RenderText_Solid(m_font, m_name.c_str(), m_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, text);
    SDL_FreeSurface(text);
    text = nullptr;
    SDL_RenderCopy(m_renderer, texture, NULL, &m_rect);
    SDL_DestroyTexture(texture);
    texture = nullptr;

}

SDL_Point Port::getPort()
{
    return SDL_Point{m_out_x, m_out_y};
}

void Port::setName(const std::string name)
{
    m_name = name;
}
/*
void Port::setSignal(Wave* signal)
{
    m_signal = signal;
}

int Port::readPort()
{
    // check clk low -> high transition
    // Read height of line at that position
    // insert into buffer on device
    if (m_isLeft)
    {
        if (
    }
}

void Port::sendPort(bool isRight)
{

}*/


///////////////////////////////////////////////////
///                  Wire Class                 /// 
///////////////////////////////////////////////////

Wire::Wire(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
    : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
{
    m_renderer = renderer;
    m_color = {255, 255, 255, 255};
    m_signal = new Wave(renderer, x1, y1-5, x2, 10);
}

Wire::~Wire()
{
    delete m_signal;
}

void Wire::render()
{
    SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    if (this->m_renderer == nullptr)
        fprintf(stderr, "Is nullptr\n");
    else if (SDL_RenderDrawLine(this->m_renderer, m_x1, m_y1, m_x2, m_y2) != 0)
    {
        fprintf(stderr, "Failure to render Rect: %s\n", SDL_GetError());
    }
    //m_signal->moveWave(40.0f);
    m_signal->render(); // Set speed variable
}

void Wire::setWire(SDL_Point port1, SDL_Point port2)
{
    m_x1 = port1.x;
    m_y1 = port1.y;
    m_x2 = port2.x;
    m_y2 = port2.y;
    m_signal->setWave(port1.x, port1.y-5, port2.x);
    //std::cout << port1.x << " " << port1.y << std::endl;
}

Wave* Wire::getWave()
{
    return m_signal;
}
