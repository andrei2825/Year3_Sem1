#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"


namespace m1
{
    class Lab5 : public gfxc::SimpleScene
    {
     public:
        Lab5();
        ~Lab5();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        int colisionDetector(glm::vec3 target);
        int Lab5::colisionDetectorEnemy(glm::vec3 p1, glm::vec3 p2);

     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        glm::mat3 modelMatrix;
        int fov;
        int x1, x2, y1, y2;
        int cameraMode;
        float rx, ry, rz;
        float playerRotation;
        int** maze;
        int mazeSize;
        glm::vec3 projectilePos;
        int shoot;
        float projectileDistance;
        float projectileRotation;
        int movementDir;
        glm::vec3 enemyPos;
        int enemyDist;
        int r;
        float hp;
        int timer;
        float time;
    };
}   // namespace m1
