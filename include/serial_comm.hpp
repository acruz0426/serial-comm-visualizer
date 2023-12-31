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
        SDL_Point moveWave(uint32_t elapsedTime, float speed, bool isCLK, bool isHigh);
        void setBuffer(std::vector<int> *buf);
        void setWave(int x1, int y1, int x2, bool isHigh);
        std::vector<SDL_Point>* getPoints();
        int getY();
        int getHigh();
        int getLow();
        bool isHigh();
    private:
        SDL_Renderer *m_renderer;
        std::vector<int> *m_buffer;
        std::vector<SDL_Point> m_points;
        SDL_Point m_start_point;
        int m_end_x;
        int m_height;
        bool m_isHigh;
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
        void beginTransmissionWrite();
        void beginTransmissionRead();
        void sendData();
        void receiveData();
        SDL_Point getEdge(int edge);
        void pushBack(int buf, int val);
        void pushBackTx(int buf, int val);
        std::vector<int>* getRxBuf(int buf);
        std::vector<int>* getTxBuf(int buf);
        void setLow(int buf);
        int* clkCounter(int counter);
        SDL_Point readData(int wire);
        void sendACK(int device);
        void stopTransmission();
    private:
        SDL_Renderer *m_renderer;
        TTF_Font *m_font;
        Device *m_device1;
        Device *m_device2;
        int m_clkCounter1 = 0;
        int m_clkCounter2 = 0;
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
        std::vector<int> m_rx_buf1;
        std::vector<int> m_rx_buf2;
        std::vector<int> m_rx_buf3;
        std::vector<int> m_rx_buf4;
};
#endif
