/**
 * @date 15.01.2021
 * @author Alexander Kaschta
 */

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Game.h"

class Scene {
public:
    virtual void Init() = 0;
    virtual void Update(double dt) = 0;
    virtual void HandleEvents(Game* game, double dt) = 0;
    virtual void Render() = 0;
    virtual void ImGuiRender() = 0;
    virtual void Clear() = 0;
};

#endif //GAME_SCENE_H
