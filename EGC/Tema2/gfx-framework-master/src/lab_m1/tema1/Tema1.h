#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        glm::mat3 colision(int x1, int y1, int x2, int y2, float &moveX, float &moveY, glm::mat3 modelMatrix);
        glm::mat3 Tema1::moveObs(float& moveX, float& moveY, glm::mat3 modelMatrix);

     protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float translateX2, translateY2;
        float scaleX = 0;
        float scaleY = 0;
        float angularStep = 0;
        int token = 0;
        int token2 = 0;
        int token3 = 0;
        int token4 = 0;
        int token5 = 0;
        // TODO(student): If you need any other class variables, define them here.
        int positionBodyX;
        int positionBodyY;
        int positionLeftEyeX;
        int positionLeftEyeY;
        int positionRightEyeX;
        int positionRightEyeY;
        float posObs1X;
        float posObs1Y;
        float posObs2X;
        float posObs2Y;
        float posObs3X;
        float posObs3Y;
        float posObs4X;
        float posObs4Y;
        float posObs5X;
        float posObs5Y;
        float posObs6X;
        float posObs6Y;
        float posObs7X;
        float posObs7Y;
        float posObs8X;
        float posObs8Y;
        float posObs9X;
        float posObs9Y;
        float posObs10X;
        float posObs10Y;
        float posObs11X;
        float posObs11Y;

        float positionBackgroundX;
        float positionBackgroundY;

        float radB;
        float radLE;
        float radRE;

        glm::vec2 mousePosition;
        glm::vec2 cameraPosition;
        glm::vec2 playerDirection;


        //borders

        float posTempX1, posTempX2, posTempY1, posTempY2;

        //camera coordinates 

        float camX;
        float camY;
        float camZ;

        //projectile
        int spawn;
        float projectileX;
        float projectileY;
        float targetX;
        float targetY;
        float pSpeed;
        float projectileAngle;
        float distance;


        //ui
        float hp;
        float points;


        //enemy
        float enemyX;
        float enemyY;
        float enemyAngle;
        float enemyLeftEyeX;
        float enemyLeftEyeY;
        float enemyRightEyeX;
        float enemyRightEyeY;
        int enemySpawn;
        int distanceFromPlayer;
        int enemySpeed;
    };
}   // namespace m1
