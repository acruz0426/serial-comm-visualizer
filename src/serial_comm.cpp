#include "../include/serial_comm.hpp"
#include <iostream>
#include <vector>
#include "../include/visualizer.hpp"

///////////////////////////////////////////////////
///                  Wave Class                 /// 
///////////////////////////////////////////////////


Wave::Wave(SDL_Renderer *renderer, int start_x, int start_y, int end_x, int height)
    : m_renderer(renderer), m_start_point({start_x, start_y}), m_end_x(end_x), m_height(height)
{
    m_points.push_back({start_x, start_y});
    //m_points.push_back({(start_x+end_x)/2, start_y});
    m_points.push_back({end_x, start_y});
    int counter = 1;
    for(auto it = m_points.begin(); it != m_points.end(); it++)
    {
        //std::cout << "count: " << counter << " " << it->x << " " << it->y << std::endl;
        counter += 1;

    }
}

void Wave::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
    SDL_RenderDrawLines(m_renderer, m_points.data(), m_points.size());
}

// Don't forget to set the startTime to currentTime after this function call
SDL_Point Wave::moveWave(uint32_t elapsedTime, float speed, bool isCLK)
{
    //std::cout << "moveWave called" << std::endl;
    float deltaTime = static_cast<float>(elapsedTime) / 1000.0f;
    float distance = speed * deltaTime;
    int edge = -1;
    int edge2  = -1;
    if (elapsedTime > 100)
    {
        //std::cout << " Inside if statement" << std::endl;
        //SDL_Point start = {};
        //SDL_Point end = {};
        /*for(auto it = m_points.begin(); it != m_points.end(); it++)
        {
            std::cout << it->x << " " << it->y << std::endl;
        }*/
        m_points.erase(m_points.begin());
        m_points.pop_back();
        int counter = 1;
        //if (m_points.empty())
            //std::cout << "m_points empty." << std::endl;
        for(auto it = m_points.begin(); it != m_points.end();)
        {
            it->x += static_cast<int>(distance);
            //std::cout << "counter: " << counter << " " << it->x << " " << it->y << std::endl;
            counter += 1;
            if (it->x >= m_end_x && speed > 0)
            {
                if (edge == -1)
                    edge = (it->y - (it+1)->y < 0) ? 0 : 1;
                it = m_points.erase(it);
            }
            else if (it->x <= m_start_point.x && speed < 0)
            {
                if (edge == -1)
                    edge = (it->y - (it+1)->y < 0) ? 1 : 0;
                it = m_points.erase(it);
            }
            else
                ++it;
        }
        //std::cout << m_points.begin()->x << " " << (m_points.begin()->x >= m_start_point.x) << std::endl;
        //std::cout << (speed > 0) << std::endl;
        if ((m_points.begin()->x >= m_start_point.x + 5) && (speed > 0)) // After 5 pixels more or less
        {
            if (!m_buffer->empty())
            {
                // Check if value in buffer is 1
                if (m_buffer->back() == 1)
                {
                    //std::cout << "Inside m_buffer.back()" << std::endl;
                    if (m_points.begin()->y == m_start_point.y)
                    {
                        //std::cout << "Inside right place" << std::endl;
                        auto it = m_points.insert(m_points.begin(), {m_start_point.x, m_start_point.y});
                        m_points.insert(m_points.begin(), {m_start_point.x, m_start_point.y-m_height});
                        edge2 = 0;
                    } 
                    else
                        m_points.insert(m_points.begin(), {m_start_point.x, m_start_point.y-m_height});
                }
                
                // Check if value in buffer is 0
                else 
                {
                    
                    if (m_points.begin()->y == m_start_point.y-m_height)
                    {
                        m_points.insert(m_points.begin(), {m_start_point.x, m_start_point.y-m_height});
                        m_points.insert(m_points.begin(), {m_start_point.x, m_start_point.y});
                        edge2 = 1;
                    }
                    else
                        m_points.insert(m_points.begin(), {m_start_point.x, m_start_point.y});
                } 
                if (isCLK)
                    m_buffer->insert(m_buffer->begin(), !m_buffer->back());
                m_buffer->pop_back(); //remove bit from buffer

            }

            // If there is nothing in the buffer
            else
            {
                //std::cout << "Buffer is empty" << std::endl;
                if (m_points.begin()->y == m_start_point.y-m_height)
                {
                    m_points.insert(m_points.begin(), {m_start_point.x, m_start_point.y-m_height});
                    m_points.insert(m_points.begin(), {m_start_point.x, m_start_point.y});
                    edge2 = 1;
                }

                else
                    m_points.insert(m_points.begin(), {m_start_point.x, m_start_point.y});
            }
        }
        else if (((m_points.end()-1)->x <= m_end_x - 5) && (speed < 0))
        {
            if (!m_buffer->empty())
            {    
                //std::cout << buffer.back() << std::endl;
                if (m_buffer->back() == 1)
                {
                    if ((m_points.end())->y == m_start_point.y)
                    {
                        m_points.insert(m_points.end(), {m_end_x, m_start_point.y});
                        m_points.insert(m_points.end(), {m_end_x, m_start_point.y-m_height});
                        edge2 = 0;
                    }
                    else
                    {
                        m_points.insert(m_points.end(), {m_end_x, m_start_point.y-m_height});
                    }
                }
                else
                {
                    if ((m_points.end())->y == m_start_point.y-m_height)
                    {
                        m_points.insert(m_points.end(), {m_end_x, m_start_point.y-m_height});
                        m_points.insert(m_points.end(), {m_end_x, m_start_point.y});
                        edge2 = 1;
                    }
                    else
                        m_points.insert(m_points.end(), {m_end_x, m_start_point.y});
                }
                     //points.insert(points.begin(), {0, SCREEN_HEIGHT/2 + buffer.back()*(WAVE_AMPLITUDE)});
                if (isCLK)
                    m_buffer->insert(m_buffer->begin(), !m_buffer->back());
                m_buffer->pop_back();
            }
            else
            {
// Work on transistion from 1 to zero with empty buffer. check visual.
                if (m_points.end()->y == m_start_point.y-m_height)
                {
                    m_points.insert(m_points.end(), {m_end_x, m_start_point.y-m_height});
                    m_points.insert(m_points.end(), {m_end_x, m_start_point.y});
                    edge2 = 1;
                } else {
                    m_points.insert(m_points.end(), {m_end_x, m_start_point.y});
                }
            }

        }
        m_points.insert(m_points.begin(), {m_start_point.x, m_points.begin()->y});
        m_points.push_back({m_end_x, (m_points.end()-1)->y}); // check case for this
    }
    return {edge, edge2};
}

void Wave::setBuffer(std::vector<int> *buf)
{
    m_buffer = buf;
    //std::cout << "setBuffer called" << std::endl;
}

void Wave::setWave(int x1, int y1, int x2)
{
    m_start_point.x = x1;
    m_start_point.y = y1;
    m_end_x = x2;
    m_points.clear();
    m_points.push_back(m_start_point);
    //m_points.push_back({(m_start_point.x+m_end_x)/2, m_start_point.y});
    m_points.push_back({m_end_x, m_start_point.y});
}

std::vector<SDL_Point>* Wave::getPoints()
{
    return &m_points;
}

int Wave::getY()
{
    return m_start_point.y;
}
///////////////////////////////////////////////////
///             Serial Comm Class               /// 
///////////////////////////////////////////////////


SerialComm::SerialComm(SDL_Renderer *renderer, TTF_Font *font, int type)
    : m_renderer(renderer), m_type(type), m_font(font)
{
    m_device1 = new Device(renderer, WIDTH/2 - DEVICE_WIDTH/2 - 200, HEIGHT/2 - DEVICE_HEIGHT/2, DEVICE_WIDTH, DEVICE_HEIGHT, font, 0);
    m_device2 = new Device(renderer, WIDTH/2 - DEVICE_WIDTH/2 + 200, HEIGHT/2 - DEVICE_HEIGHT/2, DEVICE_WIDTH, DEVICE_HEIGHT, font, 0);
    m_wire1 = new Wire(renderer, 0, 0, 0, 0);
    m_wire2 = new Wire(renderer, 0, 0, 0, 0);
    m_wire3 = new Wire(renderer, 0, 0, 0, 0);
    m_wire4 = new Wire(renderer, 0, 0, 0, 0);
    m_wire1->getWave()->setBuffer(&m_buf1);
    m_wire2->getWave()->setBuffer(&m_buf2);
    m_wire3->getWave()->setBuffer(&m_buf3);
    m_wire4->getWave()->setBuffer(&m_buf4);
    m_sign = 1;
}

SerialComm::~SerialComm()
{
    delete m_device1;
    delete m_device2;
    delete m_wire1;
    delete m_wire2;
    delete m_wire3;
    delete m_wire4;
}

void SerialComm::setType(int type)
{
    m_type = type;
    switch (type)
    {
        case 0: // I2C
            m_device1->setType(type);
            m_device2->setType(type);
            m_device1->getPorts()[4]->setName("");
            m_device2->getPorts()[0]->setName("");
            m_device1->getPorts()[5]->setName("SDA");
            m_device2->getPorts()[1]->setName("SDA");
            m_device1->getPorts()[6]->setName("SCL");
            m_device2->getPorts()[2]->setName("SCL"); 
            m_device1->getPorts()[7]->setName("");
            m_device2->getPorts()[3]->setName("");
            m_wire1->setWire(m_device1->getPorts()[5]->getPort(), m_device2->getPorts()[1]->getPort());
            m_wire2->setWire(m_device1->getPorts()[6]->getPort(), m_device2->getPorts()[2]->getPort());
            m_sign = 1;
            m_buf1.clear();
            m_buf2 = {1};
            m_buf3.clear();
            m_buf4.clear();
            break;
        case 1: // UART
            m_device1->setType(type);
            m_device2->setType(type);
            m_device1->getPorts()[4]->setName("");
            m_device2->getPorts()[0]->setName("");
            m_device1->getPorts()[5]->setName("TX");
            m_device2->getPorts()[1]->setName("RX");
            m_device1->getPorts()[6]->setName("RX");
            m_device2->getPorts()[2]->setName("TX"); 
            m_device1->getPorts()[7]->setName("");
            m_device2->getPorts()[3]->setName("");
            m_wire1->setWire(m_device1->getPorts()[5]->getPort(), m_device2->getPorts()[1]->getPort());
            m_wire2->setWire(m_device1->getPorts()[6]->getPort(), m_device2->getPorts()[2]->getPort());
            m_sign = 1;
            m_buf1.clear();
            m_buf2.clear();
            m_buf3.clear();
            m_buf4.clear();
            break;
        case 2: // SPI
            m_device1->setType(type);
            m_device2->setType(type);
            m_device1->getPorts()[4]->setName("SCLK");
            m_device2->getPorts()[0]->setName("SCLK");
            m_device1->getPorts()[5]->setName("MOSI");
            m_device2->getPorts()[1]->setName("MOSI");
            m_device1->getPorts()[6]->setName("MISO");
            m_device2->getPorts()[2]->setName("MISO");
            m_device1->getPorts()[7]->setName("CS");
            m_device2->getPorts()[3]->setName("CS");
            m_wire1->setWire(m_device1->getPorts()[4]->getPort(), m_device2->getPorts()[0]->getPort());
            m_wire2->setWire(m_device1->getPorts()[5]->getPort(), m_device2->getPorts()[1]->getPort()); 
            m_wire3->setWire(m_device1->getPorts()[6]->getPort(), m_device2->getPorts()[2]->getPort());
            m_wire4->setWire(m_device1->getPorts()[7]->getPort(), m_device2->getPorts()[3]->getPort());
            m_sign = 1;
            m_buf1 = {1};
            m_buf2.clear();
            m_buf3.clear();
            m_buf4.clear();
            break;
        case 3: // CAN
            m_device1->setType(type);
            m_device2->setType(type);
            m_device1->getPorts()[4]->setName("");
            m_device2->getPorts()[0]->setName("");
            m_device1->getPorts()[5]->setName("CH");
            m_device2->getPorts()[1]->setName("CH");
            m_device1->getPorts()[6]->setName("CL");
            m_device2->getPorts()[2]->setName("CL"); 
            m_device1->getPorts()[7]->setName("");
            m_device2->getPorts()[3]->setName("");
            m_wire1->setWire(m_device1->getPorts()[5]->getPort(), m_device2->getPorts()[1]->getPort());
            m_wire2->setWire(m_device1->getPorts()[6]->getPort(), m_device2->getPorts()[2]->getPort());
            m_sign = 1;
            m_buf1.clear();
            m_buf2.clear();
            m_buf3.clear();
            m_buf4.clear();
            break;
    }
}

int SerialComm::getType()
{
    return m_type;
}

void SerialComm::render()
{
    m_device1->render();
    m_device2->render();
    m_wire1->render();
    m_wire2->render();
    if (m_type == 2)
    {
        m_wire3->render();
        m_wire4->render();
    }
    
}

int SerialComm::moveWaves(uint32_t elapsedTime, bool changeSign)
{
    m_edge1 = {-1, -1};
    m_edge2 = {-1, -1};
    m_edge3 = {-1, -1};
    m_edge4 = {-1, -1};
    int code = 0;
    switch (m_type)
    {
        case 0: // I2C
            if (changeSign)   
            {
                for (auto it = m_wire1->getWave()->getPoints()->begin(); it != m_wire1->getWave()->getPoints()->end(); it++)
                {
                    if (it->y != m_wire1->getWave()->getY())
                        code = -1;
                }
                if (code == 0)
                    m_sign *= -1;
            }
            
            m_edge2 = m_wire2->getWave()->moveWave(elapsedTime, SPEED, true); // is clk signal
            m_edge1 = m_wire1->getWave()->moveWave(elapsedTime, m_sign*SPEED, false);
            break;
        case 1: // UART
            m_edge1 = m_wire1->getWave()->moveWave(elapsedTime, SPEED, false);
            m_edge2 = m_wire2->getWave()->moveWave(elapsedTime, -SPEED, false);
            break;
        case 2: // SPI
            m_edge1 = m_wire1->getWave()->moveWave(elapsedTime, SPEED, true); // is clk signal
            m_edge2 = m_wire2->getWave()->moveWave(elapsedTime, SPEED, false);
            m_edge3 = m_wire3->getWave()->moveWave(elapsedTime, -SPEED, false);
            m_edge4 = m_wire4->getWave()->moveWave(elapsedTime, SPEED, false);
            break;
        case 3: // CAN
            if (changeSign)   
            {
                for (auto it = m_wire2->getWave()->getPoints()->begin(); it != m_wire2->getWave()->getPoints()->end(); it++)
                {
                    if (it->y != m_wire2->getWave()->getY())
                        code = -1;
                }
                if (code == 0)
                    m_sign *= -1;
            }
            m_edge1 = m_wire1->getWave()->moveWave(elapsedTime, m_sign*SPEED, false);
            m_edge2 = m_wire2->getWave()->moveWave(elapsedTime, m_sign*SPEED, false);
            break;
    }
    std::cout << "{" << m_edge1.x << ", " << m_edge1.y << "} {" << m_edge2.x << ", " << m_edge2.y << "}" << std::endl;//<< m_edge2 << " " << m_edge3 << " " << m_edge4 << std::endl;
    return code;
}

Wire* SerialComm::getWire(int wire)
{
    switch (wire)
    {
        case 1:
            return m_wire1;
        case 2:
            return m_wire2;
        case 3:
            return m_wire3;
        case 4:
            return m_wire4;
        default:
            return nullptr;
    }
}

void SerialComm::sendData()
{
    switch(m_type)
    {
        case 0:
            m_buf1 = {1};
            break;
        case 1:
            m_buf1 = {1};
            break;
        case 2:
            m_buf2 = {1};
            break;
        case 3:
            m_buf1 = {0};
            m_buf2 = {1};
            break;
    }
}

void SerialComm::receiveData()
{
    switch(m_type)
    {
        case 0:
            m_buf1 = {1};
            break;
        case 1:
            m_buf2 = {1};
            break;
        case 2:
            m_buf3 = {1};
            break;
        case 3:
            m_buf1 = {0};
            m_buf2 = {1};
            break;
    }
}

SDL_Point SerialComm::getEdge(int edge)
{
    switch (edge)
    {
        case 1:
            return m_edge1;
        case 2:
            return m_edge2;
        case 3:
            return m_edge3;
        case 4:
            return m_edge4;
    }
}
