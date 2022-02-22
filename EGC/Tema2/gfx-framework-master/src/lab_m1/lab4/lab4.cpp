#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;
    c1X = -2.5f;
    c1Y = 0.5f;
    c1Z = -1.5f;

    c2X = 0.0f;
    c2Y = 0.5f;
    c2Z = -1.5f;

    c3X = 2.5f;
    c3Y = 0.5f;
    c3Z = -1.5f;

    cx3 = 0.0f;
    cz3 = 0.0f;

}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(c1X, c1Y, c1Z);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(c2X, c2Y, c2Z);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    //modelMatrix *= transform3D::Translate(c3X, c3Y, c3Z);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    modelMatrix *= transform3D::Translate(c3X, c3Y, c3Z-2.5f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}


void Lab4::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    // Task 3.a
    if (window->KeyHold(GLFW_KEY_W)) {
        c1Z -= 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        c1Z += 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        c1X -= 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        c1X += 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        c1X -= 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        c1X += 0.1f;
    }

    // Task 3.b
    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += 0.1f;
        scaleY += 0.1f;
        scaleZ += 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= 0.1f;
        scaleY -= 0.1f;
        scaleZ -= 0.1f;
    }

    // Task 3.c
    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX += 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX -= 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY += 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY -= 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ += 0.1f;
    }
    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ -= 0.1f;
    }

    // Bonus
    if (window->KeyHold(GLFW_KEY_9)) {
        tempX = c2X;
        tempZ = c2Z;
        c3X -= c2X;
        c3Z -= c2Z;
        c2X -= c2X;
        c2Z -= c2Z;
        angularStepOY += 0.1f;
        c2X += tempX;
        c2Z += tempZ;
        c3X += tempX;
        c3Z += tempZ;
    }
    if (window->KeyHold(GLFW_KEY_0)) {
        tempX = c2X;
        tempZ = c2Z;
        c3X -= c2X;
        c3Z -= c2Z;
        c2X -= c2X;
        c2Z -= c2Z;
        angularStepOY -= 0.1f;
        c3X += tempX;
        c3Z += tempZ;
        c2X += tempX;
        c2Z += tempZ;
    }

}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
