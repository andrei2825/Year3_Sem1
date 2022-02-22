#include "lab_m1/lab5/lab5.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"


using namespace std;
using namespace m1;


Lab5::Lab5()
{
}


Lab5::~Lab5()
{
}





void Lab5::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    mazeSize = 42;
    maze = new int*[mazeSize];
    for (int i = 0; i < mazeSize; i++) {
        maze[i] = new int[mazeSize];
    }
    string inFileName;
    int map;
    map = rand() % 10;
    switch (map)
    {
    case 0: inFileName = "maze1.txt";
        break;
    case 1: inFileName = "maze2.txt";
        break;
    case 2: inFileName = "maze3.txt";
        break;
    case 3: inFileName = "maze4.txt";
        break;
    case 4: inFileName = "maze5.txt";
        break;
    case 5: inFileName = "maze6.txt";
        break;
    case 6: inFileName = "maze7.txt";
        break;
    case 7: inFileName = "maze8.txt";
        break;
    case 8: inFileName = "maze9.txt";
        break;
    case 9: inFileName = "maze10.txt";
        break;
    default:
        break;
    }
    ifstream inFile;
    inFile.open(inFileName.c_str());

    if (inFile.is_open()) {
        for (int i = 0; i < mazeSize; i++) {
            for (int j = 0; j < mazeSize; j++) {
                inFile >> maze[i][j];
                cout << maze[i][j] << " ";
            }
            cout << endl;
        }
        inFile.close();
    }
    else { //Error message
        cerr << "Can't find input file " << inFileName << endl;
    }
    cout << inFileName;
    projectilePos = glm::vec3(camera->GetTargetPosition().x, camera->GetTargetPosition().y, camera->GetTargetPosition().z);
    shoot = 0;
    r = 0;
    projectileDistance = 0;
    projectileRotation = 0;
    movementDir = 0;
    x1 = -1;
    x2 = 1;
    y1 = -1;
    y2 = 1;
    rx = 0;
    ry = 0;
    rz = 0;
    fov = 60;
    playerRotation = 0;
    cameraMode = 0;
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
    enemyDist = 0;
    enemyPos = glm::vec3(999, -3, 999);
    hp = 100;
    time = 10000;

}


void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab5::Update(float deltaTimeSeconds)
{
    if (camera->position.x < 0) {
        camera->position.x = 0;
    }
    if (camera->position.z < 0) {
        camera->position.z = 0;
    }
    //ui
    time -= 0.1;
    if (time <= 0) {
        cout << "Game over" << endl;
        this->Exit();
    }
    cout << time << endl;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 1.4, camera->GetTargetPosition().z));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(hp/100, 0.1, 0.001));
        RenderMesh(meshes["box"], shaders["Red"], modelMatrix);
    }
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, camera->GetTargetPosition().y+1.4, camera->GetTargetPosition().z));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.1, 0.001));
        RenderMesh(meshes["box"], shaders["Red"], modelMatrix);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 1.3, camera->GetTargetPosition().z));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(time / 10000, 0.1, 0.001));
        RenderMesh(meshes["box"], shaders["Gold"], modelMatrix);
    }
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 1.3, camera->GetTargetPosition().z));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.1, 0.001));
        RenderMesh(meshes["box"], shaders["Gold"], modelMatrix);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




    if (camera->position.x >= 160 && camera->position.z >= 160) {
        this->Exit();
    }
    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; j++) {
            if (maze[i][j] == 0) {
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(j*4, 1.5, i*4));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
                    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
                }
            }
            else if (maze[i][j] == 2) {
                if (colisionDetectorEnemy(camera->GetTargetPosition(), enemyPos)) {
                    hp -= 1;
                    cout << hp << endl;
                    if (hp <= 0) {
                        cout << "Game over!" << endl;
                        this->Exit();

                    }
                }
                if (!colisionDetectorEnemy(projectilePos, glm::vec3(j * 4, 1.5, i * 4))) {
                    if (enemyDist * 0.001 >= 1) {
                        if (movementDir == 1) {
                            movementDir = 0;
                            r = rand() % 4;
                        }
                        else {
                            movementDir = 1;
                            r = rand() % 4;
                        }
                        enemyDist = 0;
                    }
                    if (movementDir == 0) {
                        enemyDist++;
                        glm::mat4 modelMatrix = glm::mat4(1);
                        if (r == 0) {
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * 4 + enemyDist * 0.001, 1.5, i * 4 + enemyDist * 0.001));
                            enemyPos = glm::vec3(j * 4 + enemyDist * 0.001, 1.5, i * 4 + enemyDist * 0.001);
                        }
                        else if (r == 1) {
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * 4 - enemyDist * 0.001, 1.5, i * 4 - enemyDist * 0.001));
                            enemyPos = glm::vec3(j * 4 - enemyDist * 0.001, 1.5, i * 4 - enemyDist * 0.001);
                        }
                        else if (r == 2) {
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * 4 - enemyDist * 0.001, 1.5, i * 4 + enemyDist * 0.001));
                            enemyPos = glm::vec3(j * 4 - enemyDist * 0.001, 1.5, i * 4 + enemyDist * 0.001);
                        }
                        else if (r == 3) {
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * 4 + enemyDist * 0.001, 1.5, i * 4 - enemyDist * 0.001));
                            enemyPos = glm::vec3(j * 4 + enemyDist * 0.001, 1.5, i * 4 - enemyDist * 0.001);
                        }
                        modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
                        RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
                    }
                    else {
                        enemyDist++;
                        glm::mat4 modelMatrix = glm::mat4(1);
                        if (r == 0) {
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * 4 + enemyDist * 0.001, 1.5, i * 4 + enemyDist * 0.001));
                            enemyPos = glm::vec3(j * 4 + enemyDist * 0.001, 1.5, i * 4 + enemyDist * 0.001);
                        }
                        else if (r == 1) {
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * 4 - enemyDist * 0.001, 1.5, i * 4 - enemyDist * 0.001));
                            enemyPos = glm::vec3(j * 4 - enemyDist * 0.001, 1.5, i * 4 - enemyDist * 0.001);
                        }
                        else if (r == 2) {
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * 4 - enemyDist * 0.001, 1.5, i * 4 + enemyDist * 0.001));
                            enemyPos = glm::vec3(j * 4 - enemyDist * 0.001, 1.5, i * 4 + enemyDist * 0.001);
                        }
                        else if (r == 3) {
                            modelMatrix = glm::translate(modelMatrix, glm::vec3(j * 4 + enemyDist * 0.001, 1.5, i * 4 - enemyDist * 0.001));
                            enemyPos = glm::vec3(j * 4 + enemyDist * 0.001, 1.5, i * 4 - enemyDist * 0.001);
                        }
                        modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
                        RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
                    }
                }
                else {
                    maze[i][j] = 1;
                    shoot = 0;
                    projectileDistance = 0;
                }
            }

        }
    }


    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    
    //map
    {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1000, 0.4, 1000));
            RenderMesh(meshes["box"], shaders["Floor"], modelMatrix);
        }
    }

    //character
    if (renderCameraTarget)
    {
        //head
        {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                RenderMesh(meshes["box"], shaders["Head"], modelMatrix);
            }

            // left eye
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.07, 0.08, -0.13));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.07, -0.08, 0.13));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.07, 0.08, -0.13));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05));
                RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
            }

            // right eye
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.07, 0.08, -0.13));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.07, -0.08, 0.13));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.07, 0.08, -0.13));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05));
                RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
            }
            //mouth
            {
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.06, -0.14));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.06, 0.14));
                    modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.06, -0.14));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.03));
                    RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
                }
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.03, -0.06, -0.14));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.03, 0.06, 0.14));
                    modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.03, -0.06, -0.14));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.03));
                    RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
                }
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.03, -0.06, -0.14));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.03, 0.06, 0.14));
                    modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.03, -0.06, -0.14));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.03));
                    RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
                }
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.06, -0.06, -0.14));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.06, 0.06, 0.14));
                    modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.06, -0.06, -0.14));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.03));
                    RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
                }
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.06, -0.06, -0.14));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.06, 0.06, 0.14));
                    modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.06, -0.06, -0.14));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.03));
                    RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
                }
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.06, -0.03, -0.14));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.06, 0.03, 0.14));
                    modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.06, -0.03, -0.14));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.03));
                    RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
                }
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.06, -0.03, -0.14));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.06, 0.03, 0.14));
                    modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.06, -0.03, -0.14));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.03));
                    RenderMesh(meshes["box"], shaders["Black"], modelMatrix);
                }
            }
        }
        //body
        {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6, 0.7, 0.3));
                RenderMesh(meshes["box"], shaders["Body"], modelMatrix);
            }
        }
        // hands
        {
            // left
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.46, -0.5, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.46, 0.5, 0));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.46, -0.5, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.7, 0.3));
                RenderMesh(meshes["box"], shaders["Hand"], modelMatrix);
            }
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.46, -0.7, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.46, 0.7, 0));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.46, -0.7, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.302));
                RenderMesh(meshes["box"], shaders["Head"], modelMatrix);
            }
            // right
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.46, -0.5, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.46, 0.5, 0));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.46, -0.5, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.7, 0.3));
                RenderMesh(meshes["box"], shaders["Hand"], modelMatrix);
            }
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.46, -0.7, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.46, 0.7, 0));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.46, -0.7, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.302));
                RenderMesh(meshes["box"], shaders["Head"], modelMatrix);
            }
        }
        //legs
        {
            // left
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.155, -1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.155, 1, 0));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.155, -1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.289, 0.6, 0.295));
                RenderMesh(meshes["box"], shaders["Leg"], modelMatrix);
            }
            // right
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.155, -1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.155, 1, 0));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(playerRotation), glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.155, -1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.289, 0.6, 0.295));
                RenderMesh(meshes["box"], shaders["Leg"], modelMatrix);
            }
        }
    }
    else {
        {
            if (shoot && projectileDistance <= 0) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, projectilePos);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(projectileRotation), glm::vec3(0, 1, 0));
                RenderMesh(meshes["sphere"], shaders["Body"], modelMatrix);
                projectileDistance = 10;
                projectilePos = camera->GetTargetPosition();
            }
        }
    }
    if (shoot && projectileDistance > 0 && !colisionDetector(projectilePos)) {
        glm::mat4 modelMatrix = glm::mat4(1);
        projectilePos[0] += cos(projectileRotation) * 10 *deltaTimeSeconds;
        projectilePos[2] += sin(projectileRotation) * 10 *deltaTimeSeconds;
        modelMatrix = glm::translate(modelMatrix, projectilePos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3));
        RenderMesh(meshes["sphere"], shaders["Body"], modelMatrix);

        projectileDistance -= 10*deltaTimeSeconds;
        //cout << projectilePos << endl;
    }
    else if (shoot && projectileDistance > 0 && colisionDetector(projectilePos)) {
        projectileDistance = 0;
    }
    if (projectileDistance <= 0) {
        shoot = 0;
    }
}


void Lab5::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Lab5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

int Lab5::colisionDetector(glm::vec3 target) {
    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; j++) {
            if (maze[i][j] == 0) {
                if (target[0] > j * 4 - 2.3 && target[0] < j * 4 + 2.3 && target[2] > i * 4 - 2.3 && target[2] < i * 4 + 2.3)
                    return 1;
            }
        }
    }
    return 0;
}
int Lab5::colisionDetectorEnemy(glm::vec3 p1, glm::vec3 p2) {
    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; j++) {
            if (maze[i][j] == 2) {
                if (p1[0] > p2[0]-1 && p1[0] < p2[0] + 1 && p1[2] > p2[2] - 1 && p1[2] < p2[2] + 1)
                    return 1;
            }
        }
    }
    return 0;
}

void Lab5::OnInputUpdate(float deltaTime, int mods)
{
    {
        float cameraSpeed = 2.0f;
        if (cameraMode == 0 && !colisionDetector(camera->GetTargetPosition())) {
            if (window->KeyHold(GLFW_KEY_W)) {
                camera->MoveForward(deltaTime*2);
                //printf("%f, %f \n", camera->GetTargetPosition().x, camera->GetTargetPosition().z);
            }

            if (window->KeyHold(GLFW_KEY_A)) {
                camera->TranslateRight(-deltaTime*2);
                //printf("%f, %f \n", camera->position.x, camera->position.z);
            }

            if (window->KeyHold(GLFW_KEY_S)) {
                camera->MoveForward(-deltaTime*2);
                //printf("%f, %f \n", camera->position.x, camera->position.z);
            }

            if (window->KeyHold(GLFW_KEY_D)) {
                camera->TranslateRight(deltaTime*2);
                //printf("%f, %f \n", camera->position.x, camera->position.z);

            }

            // test commands 
            if (window->KeyHold(GLFW_KEY_Q)) {
                camera->TranslateUpward(-deltaTime);
            }

            if (window->KeyHold(GLFW_KEY_E)) {
                camera->TranslateUpward(deltaTime);
            }
        }
        else if (cameraMode == 0 && colisionDetector(camera->GetTargetPosition())) {
            if (window->KeyHold(GLFW_KEY_W)) {
                camera->MoveForward(-0.3);
                printf("%f, %f \n", camera->GetTargetPosition().x, camera->GetTargetPosition().z);
            }

            if (window->KeyHold(GLFW_KEY_A)) {
                camera->TranslateRight(0.3);
                printf("%f, %f \n", camera->position.x, camera->position.z);
            }

            if (window->KeyHold(GLFW_KEY_S)) {
                camera->MoveForward(0.3);
                printf("%f, %f \n", camera->position.x, camera->position.z);
            }

            if (window->KeyHold(GLFW_KEY_D)) {
                camera->TranslateRight(-0.3);
                printf("%f, %f \n", camera->position.x, camera->position.z);

            }
        }
    }
}


void Lab5::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
}


void Lab5::OnKeyRelease(int key, int mods)
{

}


void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        playerRotation = -mouseX/17.45 + 36.618912;
        if (cameraMode == 1) {
            renderCameraTarget = false;
            camera->TranslateForward(camera->distanceToTarget);
            //camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
            camera->TranslateForward(-(camera->distanceToTarget));
        }

        else
        {
            renderCameraTarget = true;
            //camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
        }
    }
}


void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_3 && cameraMode == 0) {
        cameraMode = 1;
    }
    else if (button == GLFW_MOUSE_BUTTON_3 && cameraMode == 1) {
        cameraMode = 0;
    }

    if (button == GLFW_MOUSE_BUTTON_2 && cameraMode == 1 && shoot == 0) {
        shoot = 1;
        projectileRotation = mouseX/999.9705 - 2.218;
    }
}


void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab5::OnWindowResize(int width, int height)
{
}
