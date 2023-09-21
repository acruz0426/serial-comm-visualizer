#ifndef SERIAL_COMM
#define SERIAL_COMM
#include "./visualizer.hpp"
#include <SDL2/SDL.h>
#include <vector>

#define WIDTH 800
#define HEIGHT 600
#define SPEED 20.0f
class Device;
class Wire;

class Wave {
    public:
        Wave(SDL_Renderer *renderer, int start_x, int start_y, int end_x, int height);
        void render();
        SDL_Point moveWave(uint32_t elapsedTime, float speed, bool isCLK);
        void setBuffer(std::vector<int> *buf);
        void setWave(int x1, int y1, int x2);
        std::vector<SDL_Point>* getPoints();
        int getY();
    private:
        SDL_Renderer *m_renderer;
        std::vector<int> *m_buffer;
        std::vector<SDL_Point> m_points;
        SDL_Point m_start_point;
        int m_end_x;
        int m_height;
};

class SerialComm {
    public:
        SerialComm(SDL_Renderer *renderer, TTF_Font *font, int type);
        ~SerialComm();
        void setType(int type);
        int getType();
        void render();
        int moveWaves(uint32_t elapsedTime, bool right);
        Wire* getWire(int wire);
        void sendData();
        void receiveData();
        SDL_Point getEdge(int edge);
    private:
        SDL_Renderer *m_renderer;
        TTF_Font *m_font;
        Device *m_device1;
        Device *m_device2;
        int m_type;
        int m_sign;
        SDL_Point m_edge1;
        SDL_Point m_edge2;
        SDL_Point m_edge3;
        SDL_Point m_edge4;
        Wire *m_wire1;
        Wire *m_wire2;
        Wire *m_wire3;
        Wire *m_wire4;
        std::vector<int> m_buf1;
        std::vector<int> m_buf2;
        std::vector<int> m_buf3;
        std::vector<int> m_buf4;
};
#endif
