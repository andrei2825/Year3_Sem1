#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize character initial position
    
    positionBodyX = resolution.x/2-50;
    positionBodyY = resolution.y/2-50;
    positionLeftEyeX = positionBodyX*5 +50;
    positionLeftEyeY = positionBodyY*5 + 400;
    positionRightEyeX = positionBodyX * 5 + 350;
    positionRightEyeY = positionBodyY * 5 + 400;
    positionBackgroundX = 0;
    positionBackgroundY = 0;



    posTempX1 = 589;
    posTempX2 = -1308;
    posTempY1 = 310;
    posTempY2 = -1589;


    // character rotation angle
    radB = 0;
    radLE = 0;
    radRE = 0;

    // Initialize angularStep
    angularStep = 0;

    camX = 1;
    camY = 1;
    camZ = 1;
    
    mousePosition = resolution;

    //Obstacle coordinates 
    posObs1X = 1665;
    posObs1Y = 1448;
    posObs2X = 824;
    posObs2Y = 1292;
    posObs3X = 1584;
    posObs3Y = 265;
    posObs4X = 220;
    posObs4Y = 204;
    posObs5X = 1138;
    posObs5Y = 464;
    posObs6X = 523;
    posObs6Y = 1901;
    posObs7X = 962;
    posObs7Y = 1181;
    posObs8X = 910;
    posObs8Y = 274;
    posObs9X = 424;
    posObs9Y = 548;
    posObs10X = 1457;
    posObs10Y = 983;
    posObs11X = 167;
    posObs11Y = 1170;

    //UI
    hp = 3;
    points = 0;

    //Projectile
    projectileX = positionBodyX*5 + 200;
    projectileY = positionBodyY*5 + 200;
    targetX = projectileX;
    targetY = projectileY;
    spawn = 0;
    pSpeed = 2;
    distance = 4000;

    // enemy
    distanceFromPlayer = 250;
    enemyX = rand() % (int)mousePosition.x;
    enemyY = rand() % (int)mousePosition.y;
    while (enemyX < mousePosition.x / 2 + distanceFromPlayer && enemyX > mousePosition.x / 2 - distanceFromPlayer && enemyY < mousePosition.y / 2 + distanceFromPlayer && enemyY < mousePosition.y / 2 + distanceFromPlayer) {
        enemyX = rand() % (int)mousePosition.x;
        enemyY = rand() % (int)mousePosition.y;
    }
    enemyLeftEyeX = enemyX + 10;
    enemyLeftEyeY = enemyY + 80;
    enemyRightEyeX = enemyX + 70;
    enemyRightEyeY = enemyY + 80;
    enemyAngle = 0;
    enemySpawn = 1;
    enemySpeed = 250;
    

    // Character

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square3);

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0.5, 0), true);
    AddMeshToList(square1);

    // Map

    Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(square4);

    //obstacles

    Mesh* obstacle1 = object2D::CreateSquare("obstacle1", corner, squareSide, glm::vec3(0.2, 1, 0.5), true);
    AddMeshToList(obstacle1);
    


    //UI
    Mesh* hpBar = object2D::CreateSquare("hpBar", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(hpBar);
    Mesh* hpOutline = object2D::CreateSquare("hpOutline", corner, squareSide, glm::vec3(1, 0, 0), false);
    AddMeshToList(hpOutline);
    Mesh* hpBck = object2D::CreateSquare("hpBck", corner, squareSide, glm::vec3(0, 0, 0), true);
    AddMeshToList(hpBck);

    Mesh* scoreBar = object2D::CreateSquare("scoreBar", corner, squareSide, glm::vec3(1, 1, 0), true);
    AddMeshToList(scoreBar);
    Mesh* scoreOutline = object2D::CreateSquare("scoreOutline", corner, squareSide, glm::vec3(1, 1, 0), false);
    AddMeshToList(scoreOutline);



    //Projectile 
    Mesh* projectile = object2D::CreateSquare("projectile", corner, squareSide, glm::vec3(0, 0, 0), true);
    AddMeshToList(projectile);

    //enemy

    Mesh* enemyBody = object2D::CreateSquare("enemyBody", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(enemyBody);
    Mesh* enemyLeft = object2D::CreateSquare("enemyLeft", corner, squareSide, glm::vec3(0, 0, 1), true);
    AddMeshToList(enemyLeft);
    Mesh* enemyRight = object2D::CreateSquare("enemyRight", corner, squareSide, glm::vec3(0, 0, 1), true);
    AddMeshToList(enemyRight);

}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    
}


void Lab3::Update(float deltaTimeSeconds)
{   
    //UI
    if (points < 3) {
        points += 0.001;
    }
    if (points == 3) {
        cout << "You won!";
        points = 3.001;
        enemyX = 100000000;
        enemyY = 100000000;
        enemyLeftEyeX = 100000000;
        enemyLeftEyeY = 100000000;
        enemyRightEyeX = 100000000;
        enemyRightEyeY = 100000000;
        enemySpawn = 0;
    }

    if (hp == 0) {
        cout << "Game over\n";
        hp = -0.001;
        enemyX = 100000000;
        enemyY = 100000000;
        enemyLeftEyeX = 100000000;
        enemyLeftEyeY = 100000000;
        enemyRightEyeX = 100000000;
        enemyRightEyeY = 100000000;
        enemySpawn = 0;
    }
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(mousePosition.x - 60, 10);
    modelMatrix *= transform2D::Scale(0.2f, 3);
    RenderMesh2D(meshes["scoreOutline"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(mousePosition.x - 60, 10);
    modelMatrix *= transform2D::Scale(0.2f, points);
    RenderMesh2D(meshes["scoreBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(mousePosition.x - 60, 10);
    modelMatrix *= transform2D::Scale(0.2f, 3);
    RenderMesh2D(meshes["hpBck"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(mousePosition.x - 30,  10);
    modelMatrix *= transform2D::Scale(0.2f, 3);
    RenderMesh2D(meshes["hpOutline"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(mousePosition.x - 30, 10);
    modelMatrix *= transform2D::Scale(0.2f, hp);
    RenderMesh2D(meshes["hpBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(mousePosition.x - 30, 10);
    modelMatrix *= transform2D::Scale(0.2f, 3);
    RenderMesh2D(meshes["hpBck"], shaders["VertexColor"], modelMatrix);

    //Enemy
    if (enemySpawn) {
        if (enemyX > mousePosition.x / 2 - 50) {
            enemyX -= deltaTimeSeconds * enemySpeed;
            enemyLeftEyeX -= deltaTimeSeconds * enemySpeed;
            enemyRightEyeX -= deltaTimeSeconds * enemySpeed;
        }
        else if (enemyX < mousePosition.x / 2 - 50) {
            enemyX += deltaTimeSeconds * enemySpeed;
            enemyLeftEyeX += deltaTimeSeconds * enemySpeed;
            enemyRightEyeX += deltaTimeSeconds * enemySpeed;
        }
        if (enemyY > mousePosition.y / 2 - 50) {
            enemyY -= deltaTimeSeconds * enemySpeed;
            enemyLeftEyeY -= deltaTimeSeconds * enemySpeed;
            enemyRightEyeY -= deltaTimeSeconds * enemySpeed;
        }
        else if (enemyY < mousePosition.y / 2 - 50) {
            enemyY += deltaTimeSeconds * enemySpeed;
            enemyLeftEyeY += deltaTimeSeconds * enemySpeed;
            enemyRightEyeY += deltaTimeSeconds * enemySpeed;
        }
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemyLeftEyeX, enemyLeftEyeY);
        modelMatrix *= transform2D::Scale(0.2f, 0.2f);
        RenderMesh2D(meshes["enemyLeft"], shaders["VertexColor"], modelMatrix);


        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemyRightEyeX, enemyRightEyeY);
        modelMatrix *= transform2D::Scale(0.2f, 0.2f);
        RenderMesh2D(meshes["enemyRight"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemyX, enemyY);
        RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], modelMatrix);
    }
    

    // player
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Scale(0.2f, 0.2f);
    modelMatrix *= transform2D::Translate(positionBodyX*5 + 250, positionBodyY*5 + 250);
    modelMatrix *= transform2D::Rotate(radB);
    modelMatrix *= transform2D::Translate(-positionBodyX*5 - 250, -positionBodyY*5 - 250);
    modelMatrix *= transform2D::Translate(positionLeftEyeX, positionLeftEyeY);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Scale(0.2f, 0.2f);
    modelMatrix *= transform2D::Translate(positionBodyX * 5 + 250, positionBodyY * 5 + 250);
    modelMatrix *= transform2D::Rotate(radB);
    modelMatrix *= transform2D::Translate(-positionBodyX * 5 - 250, -positionBodyY * 5 - 250);
    modelMatrix *= transform2D::Translate(positionRightEyeX, positionRightEyeY);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(positionBodyX, positionBodyY);
    modelMatrix *= transform2D::Translate(50, 50);
    modelMatrix *= transform2D::Rotate(radB);
    modelMatrix *= transform2D::Translate(- 50, - 50);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    // Obstacles

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs1X, posObs1Y, modelMatrix);
    modelMatrix *= transform2D::Scale(2,2); 
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs2X, posObs2Y, modelMatrix);
    modelMatrix *= transform2D::Scale(4,4);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs3X, posObs3Y, modelMatrix);
    modelMatrix *= transform2D::Scale(4,1);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs4X, posObs4Y, modelMatrix);
    modelMatrix *= transform2D::Scale(2,4);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs5X, posObs5Y, modelMatrix);
    modelMatrix *= transform2D::Scale(3,1);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs6X, posObs6Y, modelMatrix);
    modelMatrix *= transform2D::Scale(4,1);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs7X, posObs7Y, modelMatrix);
    modelMatrix *= transform2D::Scale(3,2);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs8X, posObs8Y, modelMatrix);
    modelMatrix *= transform2D::Scale(2,1);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs9X, posObs9Y, modelMatrix);
    modelMatrix *= transform2D::Scale(1,2);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs10X, posObs10Y, modelMatrix);
    modelMatrix *= transform2D::Scale(4,3);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix = modelMatrix = moveObs(posObs11X, posObs11Y, modelMatrix);
    modelMatrix *= transform2D::Scale(1,4);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);
    
    //projectile
    if (spawn) {
        modelMatrix = glm::mat3(1);
        if (distance <= 0) {
            spawn = 0;
            projectileX = positionBodyX * 5 + 200;
            projectileY = positionBodyY * 5 + 200;
            distance = 4000;
        }
        modelMatrix *= transform2D::Scale(0.2f, 0.2f);
        projectileY += 10000 * deltaTimeSeconds;
        distance -= 10000 * deltaTimeSeconds;
        modelMatrix *= transform2D::Translate(positionBodyX * 5 + 250, positionBodyY * 5 + 250);
        modelMatrix *= transform2D::Rotate(projectileAngle);
        modelMatrix *= transform2D::Translate(-positionBodyX * 5 - 250, -positionBodyY * 5 - 250);
        modelMatrix *= transform2D::Translate(projectileX, projectileY);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
    }

    // Map

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(positionBackgroundX, positionBackgroundY);
    modelMatrix *= transform2D::Scale(20, 20);
    RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);

}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


glm::mat3 Lab3::moveObs(float& moveX, float& moveY, glm::mat3 modelMatrix) {
    modelMatrix *= transform2D::Translate(moveX, moveY);
    return modelMatrix;
}

void Lab3::OnInputUpdate(float deltaTime, int mods)
{
    float x;
    float y;
    if ((enemyY >= 225 && enemyY <= 400) && (enemyX <= 675 && enemyX >= 500) && hp > 0 && points < 3) {
        hp -= 0.5;
        //points += 0.2;
        enemyX = 100000000;
        enemyY = 100000000;
        enemyLeftEyeX = 100000000;
        enemyLeftEyeY = 100000000;
        enemyRightEyeX = 100000000;
        enemyRightEyeY = 100000000;
        enemySpawn = 0;
        enemyX = rand() % (int)mousePosition.x;
        enemyY = rand() % (int)mousePosition.y;
        while (enemyX < mousePosition.x / 2 + distanceFromPlayer && 
               enemyX > mousePosition.x / 2 - distanceFromPlayer && 
               enemyY < mousePosition.y / 2 + distanceFromPlayer && 
               enemyY < mousePosition.y / 2 + distanceFromPlayer) {
            enemyX = rand() % (int)mousePosition.x;
            enemyY = rand() % (int)mousePosition.y;
        }
        enemyLeftEyeX = enemyX + 10;
        enemyLeftEyeY = enemyY + 80;
        enemyRightEyeX = enemyX + 70;
        enemyRightEyeY = enemyY + 80;
        enemySpawn = 1;
    }
    else if (hp == 0 || points >= 3) {
        enemyX = 100000000;
        enemyY = 100000000;
        enemyLeftEyeX = 100000000;
        enemyLeftEyeY = 100000000;
        enemyRightEyeX = 100000000;
        enemyRightEyeY = 100000000;
        enemySpawn = 0;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        if (positionBackgroundX < 590 && ((positionBackgroundX < 168) || 
            positionBackgroundX > 445 || 
            ((positionBackgroundY > 190 || 
                positionBackgroundY < -284 ))) &&
                                       ((positionBackgroundX < 65) || 
                                         positionBackgroundX > 250 || 
                                      ((positionBackgroundY > -145 || 
                                       positionBackgroundY < -425))) &&
                                     ((positionBackgroundX < -525) ||
                                        positionBackgroundX > -320 || 
                                       ((positionBackgroundY > 130 || 
                                        positionBackgroundY < -55))) &&
                                     ((positionBackgroundX < -850) || 
                                        positionBackgroundX > -530 || 
                                       ((positionBackgroundY > -65 || 
                                       positionBackgroundY < -240))) &&
                                    ((positionBackgroundX < -1270) || 
                                        positionBackgroundX > -866 || 
                                       ((positionBackgroundY > -581 || 
                                            positionBackgroundY < -962))) &&
                                     ((positionBackgroundX < -1280) || 
                                         positionBackgroundX > -1075 || 
                                         ((positionBackgroundY > -1045 || 
                                             positionBackgroundY < -1330))) &&
                                     ((positionBackgroundX < -675) || 
                                         positionBackgroundX > -370 || 
                                         ((positionBackgroundY > -780 || 
                                             positionBackgroundY < -1060))) &&
                                     ((positionBackgroundX < -640) || 
                                         positionBackgroundX > -235 || 
                                         ((positionBackgroundY > -890 || 
                                             positionBackgroundY < -1369))) &&
                                     ((positionBackgroundX < -340) || 
                                         positionBackgroundX > 62 || 
                                         ((positionBackgroundY > -1500 ||
                                             positionBackgroundY < -1600))) &&
                                     ((positionBackgroundX < 318) || 
                                         positionBackgroundX > 418 || 
                                         ((positionBackgroundY > -770 || 
                                             positionBackgroundY < -1250)))) {
            positionBackgroundX += deltaTime * 400;
            posObs1X += deltaTime * 400;
            posObs2X += deltaTime * 400;
            posObs3X += deltaTime * 400;
            posObs4X += deltaTime * 400;
            posObs5X += deltaTime * 400;
            posObs6X += deltaTime * 400;
            posObs7X += deltaTime * 400;
            posObs8X += deltaTime * 400;
            posObs9X += deltaTime * 400;
            posObs10X += deltaTime * 400;
            posObs11X += deltaTime * 400;
            enemyX += deltaTime * 400;
            enemyLeftEyeX += deltaTime * 400;
            enemyRightEyeX += deltaTime * 400;
        }
        
        x = enemyX;
        y = enemyY;
        enemyAngle = -(2 * atan(x / (y + sqrt(pow(y, 2) + pow(x, 2))))) - 3.141 / 2 - (6.282 - (2 * atan(x / (y + sqrt(pow(y, 2) + pow(x, 2))))));
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        if (positionBackgroundX > -1309 && ((positionBackgroundX > 472) || positionBackgroundX < 180 || ((positionBackgroundY > 190 || positionBackgroundY < -284)))
                                        && ((positionBackgroundX > 265) || positionBackgroundX < 75  || ((positionBackgroundY > -145 || positionBackgroundY < -425)))
                                        && ((positionBackgroundX > -215) || positionBackgroundX < -420 || ((positionBackgroundY > 130 || positionBackgroundY < -55)))
                                        && ((positionBackgroundX > -445) || positionBackgroundX < -745 || ((positionBackgroundY > -65 || positionBackgroundY < -240)))
                                        && ((positionBackgroundX > -890) || positionBackgroundX < -1292 || ((positionBackgroundY > 137 || positionBackgroundY < -45)))
                                        && ((positionBackgroundX > -763) || positionBackgroundX < -1165 || ((positionBackgroundY > -581 || positionBackgroundY < -962)))
                                        && ((positionBackgroundX > -266) || positionBackgroundX < -570 || ((positionBackgroundY > -780 || positionBackgroundY < -1060)))
                                        && ((positionBackgroundX > -130) || positionBackgroundX < -535 || ((positionBackgroundY > -890 || positionBackgroundY < -1369)))
                                        && ((positionBackgroundX > -970) || positionBackgroundX < -1175 || ((positionBackgroundY > -1045 || positionBackgroundY < -1330)))
                                        && ((positionBackgroundX > 170) || positionBackgroundX < -230 || ((positionBackgroundY > -1500 || positionBackgroundY < -1600)))
                                        && ((positionBackgroundX > 525) || positionBackgroundX < 425 || ((positionBackgroundY > -770 || positionBackgroundY < -1250)))) {
            positionBackgroundX -= deltaTime * 400;
            posObs1X -= deltaTime * 400;
            posObs2X -= deltaTime * 400;
            posObs3X -= deltaTime * 400;
            posObs4X -= deltaTime * 400;
            posObs5X -= deltaTime * 400;
            posObs6X -= deltaTime * 400;
            posObs7X -= deltaTime * 400;
            posObs8X -= deltaTime * 400;
            posObs9X -= deltaTime * 400;
            posObs10X -= deltaTime * 400;
            posObs11X -= deltaTime * 400;
            enemyX -= deltaTime * 400;
            enemyLeftEyeX -= deltaTime * 400;
            enemyRightEyeX -= deltaTime * 400;
        }
        x = enemyX;
        y = enemyY;
        enemyAngle = -(2 * atan(x / (y + sqrt(pow(y, 2) + pow(x, 2))))) - 3.141 / 2 - (6.282 - (2 * atan(x / (y + sqrt(pow(y, 2) + pow(x, 2))))));
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        if (positionBackgroundY > -1590 && ((positionBackgroundY > 204) || positionBackgroundY < -284 || ((positionBackgroundX > 448 || positionBackgroundX < 180)))
                                        && ((positionBackgroundY > -135) || positionBackgroundY < -425 || ((positionBackgroundX > 253 || positionBackgroundX < 73)))
                                        && ((positionBackgroundY > 141) || positionBackgroundY < 41 || ((positionBackgroundX > -230 || positionBackgroundX < -510)))/**/
                                        && ((positionBackgroundY > -48) || positionBackgroundY < -148 || ((positionBackgroundX > -460 || positionBackgroundX < -835)))/**/
                                        && ((positionBackgroundY > 148) || positionBackgroundY < 48 || ((positionBackgroundX > -902 || positionBackgroundX < -1400)))/**/
                                        && ((positionBackgroundY > -568) || positionBackgroundY < -870 || ((positionBackgroundX > -775 || positionBackgroundX < -1255)))/**/
                                        && ((positionBackgroundY > -1032) || positionBackgroundY < -1232 || ((positionBackgroundX > -983 || positionBackgroundX < -1265)))/**/
                                        && ((positionBackgroundY > -765) || positionBackgroundY < -966 || ((positionBackgroundX > -280 || positionBackgroundX < -663)))/**/
                                        && ((positionBackgroundY > -877) || positionBackgroundY < -1280 || ((positionBackgroundX > -144 || positionBackgroundX < -624)))/**/
                                        && ((positionBackgroundY > -1486) || positionBackgroundY < -1590 || ((positionBackgroundX > 156 || positionBackgroundX < -323)))/**/
                                        && ((positionBackgroundY > -755) || positionBackgroundY < -1155 || ((positionBackgroundX > 514 || positionBackgroundX < 332)))) {
            positionBackgroundY -= deltaTime * 400;
            posObs1Y -= deltaTime * 400;
            posObs2Y -= deltaTime * 400;
            posObs3Y -= deltaTime * 400;
            posObs4Y -= deltaTime * 400;
            posObs5Y -= deltaTime * 400;
            posObs6Y -= deltaTime * 400;
            posObs7Y -= deltaTime * 400;
            posObs8Y -= deltaTime * 400;
            posObs9Y -= deltaTime * 400;
            posObs10Y -= deltaTime * 400;
            posObs11Y -= deltaTime * 400;
            enemyY -= deltaTime * 400;
            enemyLeftEyeY -= deltaTime * 400;
            enemyRightEyeY -= deltaTime * 400;
        }
        x = enemyX;
        y = enemyY;
        enemyAngle = -(2 * atan(x / (y + sqrt(pow(y, 2) + pow(x, 2))))) - 3.141 / 2 - (6.282 - (2 * atan(x / (y + sqrt(pow(y, 2) + pow(x, 2))))));
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        if (positionBackgroundY < 311 && ((positionBackgroundY < -295) || positionBackgroundY > 180 || ((positionBackgroundX > 448 || positionBackgroundX < 180)))
                                      && ((positionBackgroundY < -440) || positionBackgroundY > -155 || ((positionBackgroundX > 250 || positionBackgroundX < 80)))
                                      && ((positionBackgroundY < -66) || positionBackgroundY > 35 || ((positionBackgroundX > -230 || positionBackgroundX < -510)))
                                      && ((positionBackgroundY < -256) || positionBackgroundY > -156 || ((positionBackgroundX > -460 || positionBackgroundX < -835)))
                                      && ((positionBackgroundY < -58) || positionBackgroundY > 48 || ((positionBackgroundX > -902 || positionBackgroundX < -1400)))
                                      && ((positionBackgroundY < -976) || positionBackgroundY > -676 || ((positionBackgroundX > -775 || positionBackgroundX < -1255)))
                                      && ((positionBackgroundY < -1340) || positionBackgroundY > -1140 || ((positionBackgroundX > -983 || positionBackgroundX < -1265)))
                                      && ((positionBackgroundY < -1073) || positionBackgroundY > -873 || ((positionBackgroundX > -280 || positionBackgroundX < -663)))
                                      && ((positionBackgroundY < -1383) || positionBackgroundY > -983 || ((positionBackgroundX > -144 || positionBackgroundX < -624)))
                                      && ((positionBackgroundY < -1263) || positionBackgroundY > -860 || ((positionBackgroundX > 514 || positionBackgroundX < 332)))) {
            positionBackgroundY += deltaTime * 400;
            posObs1Y += deltaTime * 400;
            posObs2Y += deltaTime * 400;
            posObs3Y += deltaTime * 400;
            posObs4Y += deltaTime * 400;
            posObs5Y += deltaTime * 400;
            posObs6Y += deltaTime * 400;
            posObs7Y += deltaTime * 400;
            posObs8Y += deltaTime * 400;
            posObs9Y += deltaTime * 400;
            posObs10Y += deltaTime * 400;
            posObs11Y += deltaTime * 400;
            enemyY += deltaTime * 400;
            enemyLeftEyeY += deltaTime * 400;
            enemyRightEyeY += deltaTime * 400;
        }
        x = enemyX;
        y = enemyY;
        enemyAngle = -(2 * atan(x / (y + sqrt(pow(y, 2) + pow(x, 2))))) - 3.141 / 2 - (6.282 - (2 * atan(x / (y + sqrt(pow(y, 2) + pow(x, 2))))));
    }
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    
    float y = mouseY - (mousePosition.y / 2);
    float x = mouseX - (mousePosition.x / 2);
    radB = -(2 * atan(y /(x + sqrt(pow(x, 2) + pow(y, 2))))) - 3.141/2;

}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2) {
        if (spawn == 0) {
            spawn = 1;
            float y = mouseY - (mousePosition.y / 2);
            float x = mouseX - (mousePosition.x / 2);
            projectileAngle = -(2 * atan(y / (x + sqrt(pow(x, 2) + pow(y, 2))))) - 3.141 / 2;
        }

    }

}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
