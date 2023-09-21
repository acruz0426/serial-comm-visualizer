//#include "../include/i2c.h"
//#include "../include/spi.h"
//#include "../include/uart.h"
//#include "../include/can.h"
#include "../include/serial_comm.hpp"
#include "../include/visualizer.hpp"

#ifdef _WIN32
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <cstdio>
#include <iostream>
#include <vector>
 
#define WIDTH 800
#define HEIGHT 600
#define DELAY 4000

bool init();
bool loadMedia();
void close();
void renderI2C();
void renderUART();
void renderCAN();
void renderSPI();
void toMainMenu();
void testSend();
void testReceive();

//////////////////////
// Global Variables //
//////////////////////
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
TTF_Font *font = nullptr;
SerialComm *SC;
bool inMainMenu = true;
bool inI2C = false;
bool inSPI = false;
bool inUART = false;
bool inCAN = false;
bool I2CSet = false;
bool UARTSet = false;
bool CANSet = false;
bool SPISet = false;
bool changeSign = false;
bool send = true;
bool receive = false;

int main(int argc, char *argv[])
{
    SDL_Event e;
    bool quit = false;
    bool mouseClick = false;
    int mouseX = 0;
    int mouseY = 0;
    bool uartCollide = false;
    bool spiCollide = false;
    bool canCollide = false;
    bool i2cCollide = false;
    bool backCollide = false;
    bool sendCollide = false;
    bool receiveCollide = false;
    if (!init())
    {
        fprintf(stderr, "Failure to initialize.\n");
        return 1;
    }

    if (!loadMedia())
    {
        fprintf(stderr, "Failure to load media.\n");
        return 1;
    }
    
    Button *i2cButton = new Button(renderer, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 - BUTTON_HEIGHT/2 - 60, 100, 50, "I2C", font, renderI2C);
    Button *uartButton = new Button(renderer, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 - BUTTON_HEIGHT/2, 100, 50, "UART", font, renderUART);
    Button *canButton = new Button(renderer, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 - BUTTON_HEIGHT/2 + 60, 100, 50, "CAN", font, renderCAN);
    Button *spiButton = new Button(renderer, WIDTH/2 - BUTTON_WIDTH/2, HEIGHT/2 - BUTTON_HEIGHT/2 + 120, 100, 50, "SPI", font, renderSPI);
    Button *backButton = new Button(renderer, 20, 20, BUTTON_WIDTH, BUTTON_HEIGHT, "Back", font, toMainMenu);
    Button *sendData = new Button(renderer, WIDTH/2-BUTTON_WIDTH-20, 20, BUTTON_WIDTH, BUTTON_HEIGHT, "Send", font, testSend);
    Button *receiveData = new Button(renderer, WIDTH/2+20, 20, BUTTON_WIDTH, BUTTON_HEIGHT, "Receive", font, testReceive);
    /*
    Device *device1 = new Device(renderer, WIDTH/2 - DEVICE_WIDTH/2 - 200, HEIGHT/2 - DEVICE_HEIGHT/2, DEVICE_WIDTH, DEVICE_HEIGHT, font, 0);
    Device *device2 = new Device(renderer, WIDTH/2 - DEVICE_WIDTH/2 + 200, HEIGHT/2 - DEVICE_HEIGHT/2, DEVICE_WIDTH, DEVICE_HEIGHT, font, 0);
fprintf(stdout, "Initialization complete.\n");
    Wire *wire1 = new Wire(renderer, 0, 0, 0, 0);
    Wire *wire2 = new Wire(renderer, 0, 0, 0, 0);
    Wire *wire3 = new Wire(renderer, 0, 0, 0, 0);
    Wire *wire4 = new Wire(renderer, 0, 0, 0, 0);
    */
    SerialComm *serialComm = new SerialComm(renderer, font, 0);
    SC = serialComm;
    uint32_t currentTime;
    uint32_t elapsedTime;
    uint32_t startTime = SDL_GetTicks(); // Get starting time
    //std::vector<int> buf = {1};
    //wire2->getWave()->setBuffer(buf);
    int val = 2;
    while(!quit)
    {
        //Button *uartButton = new Button();
        //Button *spiButton = new Button();
        //Button *canButton = new Button();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Setup current time and elapsed time
        currentTime = SDL_GetTicks();
        elapsedTime = currentTime - startTime; 

        // Get User input
        while(SDL_PollEvent(&e))
        {
            // User 'X' in corner
            if (e.type == SDL_QUIT)
                quit = true;

            // User presses down on key
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
            
            else if (e.type == SDL_MOUSEMOTION)
            {
                mouseX = e.motion.x;
                mouseY = e.motion.y;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                mouseX = e.button.x;
                mouseY = e.button.y;
                mouseClick = true;
            }
        }
        if (inMainMenu)
        {
            if (i2cButton->isColliding(mouseX, mouseY))
            {
                i2cCollide = true;
                if (mouseClick)
                {
                    i2cButton->onClick();
                    mouseClick = false;
                }
            }
            else if (spiButton->isColliding(mouseX, mouseY))
            {
                spiCollide = true;
                if (mouseClick)
                {
                    spiButton->onClick();
                    mouseClick = false;
                }
            }
            else if (canButton->isColliding(mouseX, mouseY))
            {
                canCollide = true;
                if (mouseClick)
                {
                    canButton->onClick();
                    mouseClick = false;
                }
            }
            else if (uartButton->isColliding(mouseX, mouseY))
            {
                uartCollide = true;
                if (mouseClick)
                {
                    uartButton->onClick();
                    mouseClick = false;
                }
            }


            i2cButton->render(i2cCollide);
            spiButton->render(spiCollide);
            canButton->render(canCollide);
            uartButton->render(uartCollide);
            uartCollide = false;
            spiCollide = false;
            canCollide = false;
            i2cCollide = false;

        }
        else if (inI2C)
        {
            
            // Setup I2C device
            if (!I2CSet)
            {
                serialComm->setType(0);
                I2CSet = true;
            }

            // Check for mouse collisions on buttons
            if(backButton->isColliding(mouseX, mouseY))
            {
                backCollide = true;
                if (mouseClick)
                {
                    backButton->onClick();
                    mouseClick = false;
                }
            }
            else if (sendData->isColliding(mouseX, mouseY))
            {
                sendCollide = true;
                if (mouseClick)
                {
                    sendData->onClick();
                    mouseClick = false;
                }
            }
            else if (receiveData->isColliding(mouseX, mouseY))
            {
                receiveCollide = true;
                if (mouseClick)
                {
                    receiveData->onClick();
                    mouseClick = false;
                }
            }

            // render all the objects in i2c
            serialComm->render();
            backButton->render(backCollide);
            sendData->render(sendCollide);
            receiveData->render(receiveCollide);
            if (elapsedTime > 100)
            {
                if (serialComm->moveWaves(elapsedTime, changeSign) == 0)
                   changeSign = false; 
                //val = wire2->getWave()->moveWave(elapsedTime, 10.0f, true);
                /*if (val == 0)
                    std::cout << "Rising edge" << std::endl;
                else if (val == 1)
                    std::cout << "Falling edge" << std::endl;
                else
                    std::cout << "No edge detected: " << val << std::endl;
                */
                startTime = currentTime;
            }
            
            backCollide = false;
            sendCollide = false;
            receiveCollide = false;
        }
        else if (inUART)
        {
            // Setup I2C device
            if (!UARTSet)
            {
                serialComm->setType(1);
                UARTSet = true;
            }

            // Check for mouse collisions on buttons
            if(backButton->isColliding(mouseX, mouseY))
            {
                backCollide = true;
                if (mouseClick)
                {
                    backButton->onClick();
                    mouseClick = false;
                }
            }
            else if (sendData->isColliding(mouseX, mouseY))
            {
                sendCollide = true;
                if (mouseClick)
                {
                    sendData->onClick();
                    mouseClick = false;
                }
            }
            else if (receiveData->isColliding(mouseX, mouseY))
            {
                receiveCollide = true;
                if (mouseClick)
                {
                    receiveData->onClick();
                    mouseClick = false;
                }
            }

            // render all the objects in i2c
            serialComm->render();
            backButton->render(backCollide);
            sendData->render(sendCollide);
            receiveData->render(receiveCollide);
            if (elapsedTime > 100)
            {
                if(serialComm->moveWaves(elapsedTime, changeSign) == 0)
                    changeSign = false;
                //val = wire2->getWave()->moveWave(elapsedTime, 10.0f, true);
                /*if (val == 0)
                    std::cout << "Rising edge" << std::endl;
                else if (val == 1)
                    std::cout << "Falling edge" << std::endl;
                else
                    std::cout << "No edge detected: " << val << std::endl;
                */
                startTime = currentTime;
            }
            
            backCollide = false;
            sendCollide = false;
            receiveCollide = false;
        }
        else if (inSPI)
        {
            // Setup I2C device
            if (!SPISet)
            {
                serialComm->setType(2);
                SPISet = true;
            }

            // Check for mouse collisions on buttons
            if(backButton->isColliding(mouseX, mouseY))
            {
                backCollide = true;
                if (mouseClick)
                {
                    backButton->onClick();
                    mouseClick = false;
                }
            }
            else if (sendData->isColliding(mouseX, mouseY))
            {
                sendCollide = true;
                if (mouseClick)
                {
                    sendData->onClick();
                    mouseClick = false;
                }
            }
            else if (receiveData->isColliding(mouseX, mouseY))
            {
                receiveCollide = true;
                if (mouseClick)
                {
                    receiveData->onClick();
                    mouseClick = false;
                }
            }

            // render all the objects in i2c
            serialComm->render();
            backButton->render(backCollide);
            sendData->render(sendCollide);
            receiveData->render(receiveCollide);
            if (elapsedTime > 100)
            {
                if(serialComm->moveWaves(elapsedTime, changeSign) == 0)
                    changeSign = false;
                //val = wire2->getWave()->moveWave(elapsedTime, 10.0f, true);
                /*if (val == 0)
                    std::cout << "Rising edge" << std::endl;
                else if (val == 1)
                    std::cout << "Falling edge" << std::endl;
                else
                    std::cout << "No edge detected: " << val << std::endl;
                */
                startTime = currentTime;
            }
            
            backCollide = false;
            sendCollide = false;
            receiveCollide = false; 
        }
        else if (inCAN)
        {
            // Setup I2C device
            if (!CANSet)
            {
                serialComm->setType(3);
                CANSet = true;
            }

            // Check for mouse collisions on buttons
            if(backButton->isColliding(mouseX, mouseY))
            {
                backCollide = true;
                if (mouseClick)
                {
                    backButton->onClick();
                    mouseClick = false;
                }
            }
            else if (sendData->isColliding(mouseX, mouseY))
            {
                sendCollide = true;
                if (mouseClick)
                {
                    sendData->onClick();
                    mouseClick = false;
                }
            }
            else if (receiveData->isColliding(mouseX, mouseY))
            {
                receiveCollide = true;
                if (mouseClick)
                {
                    receiveData->onClick();
                    mouseClick = false;
                }
            }

            // render all the objects in i2c
            serialComm->render();
            backButton->render(backCollide);
            sendData->render(sendCollide);
            receiveData->render(receiveCollide);
            if (elapsedTime > 100)
            {
                if (serialComm->moveWaves(elapsedTime, changeSign) == 0)
                    changeSign = false;
                //val = wire2->getWave()->moveWave(elapsedTime, 10.0f, true);
                /*if (val == 0)
                    std::cout << "Rising edge" << std::endl;
                else if (val == 1)
                    std::cout << "Falling edge" << std::endl;
                else
                    std::cout << "No edge detected: " << val << std::endl;
                */
                startTime = currentTime;
            }
            
            backCollide = false;
            sendCollide = false;
            receiveCollide = false;
        }

        SDL_RenderPresent(renderer);
        mouseClick = false;
    }
    delete i2cButton;
    delete canButton;
    delete uartButton;
    delete spiButton;
    delete backButton;
    delete serialComm;
    close();

    return 0;

}

bool init()
{
    // Get SDL running
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL failed to initialize: %s\n", SDL_GetError());
        return false;
    }

    // Create Window
    window = SDL_CreateWindow("Serial Communication Visualizer",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WIDTH,
                            HEIGHT,
                            0);
    if (window == nullptr)
    {
        fprintf(stderr, "SDL window failed to initialize: %s\n", SDL_GetError());
        return false;
    }

    // Create Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        fprintf(stderr, "SDL renderer failed to initialize: %s\n", SDL_GetError());
        return false;
    }
    // Initialize SDL TTF
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF init failed to initialize: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

bool loadMedia()
{
    font = TTF_OpenFont("./fonts/DejaVuSans.ttf", 72);
    if(font == nullptr) {
        fprintf(stderr, "Couldn't load font: %s\n", TTF_GetError());
        return false;
    }
    
    return true; 
}

void close()
{
    // Free up stuff and Quit
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void renderI2C()
{
    fprintf(stdout, "Render I2C contents and go to I2C logic.\n");
    inMainMenu = false;
    inI2C = true;
    inSPI = false;
    inCAN = false;
    inUART = false;
    I2CSet = false;
}

void renderUART()
{
    fprintf(stdout, "Render UART contents and go to UART logic.\n");
    inMainMenu = false;
    inI2C = false;
    inSPI = false;
    inCAN = false;
    inUART = true;
    UARTSet = false;
}

void renderCAN()
{
    fprintf(stdout, "Render CAN contents and go to CAN logic.\n");
    inMainMenu = false;
    inI2C = false;
    inSPI = false;
    inCAN = true;
    inUART = false;
    CANSet = false;

}

void renderSPI()
{
    fprintf(stdout, "Render SPI contents and go to SPI logic.\n");
    inMainMenu = false;
    inI2C = false;
    inSPI = true;
    inCAN = false;
    inUART = false;
    SPISet = false;
}

void toMainMenu()
{
    fprintf(stdout, "Going back to main menu.\n");
    inMainMenu = true;
    inI2C = false;
    inSPI = false;
    inCAN = false;
    inUART = false;
}

void testSend()
{
    send = true;
    if (receive && (SC->getType() == 0 || SC->getType() == 3))
        changeSign = true;
    receive = false;
}

void testReceive()
{
    receive = true;
    if (send && (SC->getType() == 0 || SC->getType() == 3))
        changeSign = false;
    send = false;
}
