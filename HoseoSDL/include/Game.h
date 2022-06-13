#pragma once

#include "main.h"

#include <vector>

class Obstacle;
class Walker;
class Vector2D;

class Game {
private:
    Game() {}
    static Game* s_pInstance; // 정적 멤버변수 
public:
    static Game* Instance() {
   	  if (s_pInstance == 0) {
        s_pInstance = new Game();
        return s_pInstance;
    	}
    	return s_pInstance;
    }

    SDL_Renderer* getRenderer() const { return m_pRenderer; }

public:
    ~Game() {}

    bool init(const char *title, int xpos, int ypos, int width, int height, int flags);
    bool setup();
    void render();
    void update(); 
    bool running();
    void handleEvents();
    void clean();
    void quit() { m_bRunning = false; }

private:
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    bool m_bRunning;

    Vector2D* _mousePos;
    Walker* _walker;
    Walker* _hider;
    std::vector<Obstacle*> obstacles;

    SDL_Texture* mTexture;
    SDL_Rect _srcRect;
    SDL_Rect _destRect;
    float _angle;
};

typedef Game TheGame;