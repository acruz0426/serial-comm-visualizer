#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <functional>
#include "./serial_comm.hpp"

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50
#define DEVICE_WIDTH 120
#define DEVICE_HEIGHT 180
#define PORT_LINE_DIST 5
#define PORT_WIDTH 20
#define PORT_HEIGHT 17

class SDL_Renderer;
class SDL_Rect;
class Wave;

class Button {
    public:
        Button(SDL_Renderer *renderer, int x, int y, int w, int h, const std::string &name, TTF_Font *font, std::function<void()> func);
        bool isColliding(int mouseX, int mouseY);
        void render(bool isCollide);
        SDL_Rect getRect();
        void setRect(int x, int y, int w, int h);
        void onClick();
        void setFunction(std::function<void()> func);
    private:
        SDL_Renderer *m_renderer;
        SDL_Rect m_rect;
        SDL_Rect m_text_rect;
        SDL_Color m_color_background;
        SDL_Color m_color_text;
        TTF_Font *m_font;
        int m_x1;
        int m_y1;
        int m_x2;
        int m_y2;
        int m_w;
        int m_h;
        std::string m_name;
        std::function<void()> m_function;
};

class Wire {
    public:
        Wire(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);
        ~Wire();
        void render();
        void setWire(SDL_Point port1, SDL_Point port2);
        Wave* getWave();
    private:
        SDL_Renderer *m_renderer;
        SDL_Color m_color;
        Wave *m_signal;
        int m_x1;
        int m_y1;
        int m_x2;
        int m_y2;

};

class Port {
    public:
        Port(SDL_Renderer *renderer, int x, int y, bool left, TTF_Font *font);
        void render();
        SDL_Point getPort();
        void setName(const std::string name);
    private:
        SDL_Color m_color;
        SDL_Renderer *m_renderer;
        TTF_Font *m_font;
        SDL_Rect m_rect;
        int m_in_x;
        int m_in_y;
        int m_out_x;
        int m_out_y;
        bool m_isLeft;
        std::string m_name;

};

class Device {
    public:
      Device(SDL_Renderer *renderer, int x, int y, int w, int h, TTF_Font *font, int type);
      ~Device();
      void render();
      void setType(int type);
      Port** getPorts();
    private:
        SDL_Renderer *m_renderer;
        SDL_Rect m_rect;
        SDL_Rect m_text_rect;
        SDL_Color m_color_background;
        SDL_Color m_color_text;
        TTF_Font *m_font;
        TTF_Font *m_portFont;
        int m_x1;
        int m_y1;
        int m_x2;
        int m_y2;
        int m_w;
        int m_h;
        std::string m_name;
        int m_comm_type;
        Port *m_ports[8];
};

#endif
