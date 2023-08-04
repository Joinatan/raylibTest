#define BT_USE_DOUBLE_PRECISION 1

#include "/home/jonatan/Documents/kod/cpp/libraries/bullet3/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "raylib.h"
#include "raymath.h"
#include <btBulletDynamicsCommon.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

// bullet init
btDynamicsWorld *world; // world
btDispatcher *dispatcher;
btBroadphaseInterface *broadPhase;         // collision algorithm
btConstraintSolver *solver;                // calcuate forces
btCollisionConfiguration *collisionConfig; // use default collision
// std::shared_ptr<btCollisionConfiguration> collisionConfig =
//     std::make_shared<btDefaultCollisionConfiguration>();
std::vector<btRigidBody *> bodies;

btRigidBody *addSphere(float rad, float x, float y, float z, float mass) {
  btTransform t;
  t.setIdentity();
  t.setOrigin(btVector3(x, y, z));
  btSphereShape *sphere = new btSphereShape(rad);
  btVector3 inertia(1.0, 1.0, 1.0);
  if (mass != 0.0) {
    sphere->calculateLocalInertia(mass, inertia);
  }

  btMotionState *motion = new btDefaultMotionState(t);
  btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
  btRigidBody *body = new btRigidBody(info);
  body->setFriction(8.5f);
  // body.calculateLocalInertia(mass, inertia);
  world->addRigidBody(body);
  bodies.push_back(body);
  return body;
}

void renderSphere(btRigidBody *sphere) {
  if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)
    return;
  // set color
}

//--------------------------------MAIN
int main() {

  collisionConfig = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfig);
  broadPhase = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver();

  // create world
  world = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver,
                                      collisionConfig);
  world->setGravity(btVector3(0, -9.0, 0));

  btTransform t;
  t.setIdentity();
  t.setOrigin(btVector3(0, 0, 0));
  // ----------------plane physics
  btStaticPlaneShape *plane =
      new btStaticPlaneShape(btVector3(0.0, 1.0, 0.0), 0.0);
  btMotionState *motion = new btDefaultMotionState(t);
  btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
  btRigidBody *planeBody = new btRigidBody(info);
  // world->addRigidBody(planeBody);
  planeBody->setRestitution(0.8f);
  planeBody->setFriction(0.5f);
  bodies.push_back(planeBody);

  float testSphereRad = 4.0;
  btRigidBody *testSphere = addSphere(testSphereRad, 0.0, 120.0, 20.0, 4.0);
  testSphere->setRestitution(0.9f);

  //  raylib Initialization
  //--------------------------------------------------------------------------------------
  //
  const int screenWidth = 1366;
  const int screenHeight = 768;

  InitWindow(screenWidth, screenHeight,
             "raylib [core] example - 3d camera mode");

  // Define the camera to look into our 3d world
  Camera3D camera = {0};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f}; // Camera looking at point
  camera.up =
      (Vector3){0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
  camera.fovy = 70.0f;             // Camera field-of-view Y
  camera.position = (Vector3){13.0f, 3.0f, 00.0f}; // Camera position
  camera.projection = CAMERA_PERSPECTIVE;          // Camera mode type
  // camera.projection = CAMERA_THIRD_PERSON;         // Camera mode type
  // camera.projection = CAMERA_FIRST_PERSON; // Camera mode type
  Vector2 mousePos;

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  float cubeX = 0.0;
  float cameraP = 0.0;
  float i = 0.0;

  //---------------------CUBEMESHMODEL MIRRIS o PURRE
  Mesh cubeMesh = GenMeshCube(2.0f, 2.0f, 2.0f);
  Model cubeModelMirris = LoadModelFromMesh(cubeMesh);
  Vector3 cubePositionMirris = {0.0f, 2.0f, 0.0f};
  Vector3 cubePositionPurre = {0.0f, 2.0f, 4.0f};

  Model cubeModelPurre = LoadModelFromMesh(cubeMesh);

  //--------------------Base Plane
  Mesh planeMesh = GenMeshPlane(100, 100, 100, 100);
  Model planeModel = LoadModelFromMesh(planeMesh);
  Vector3 planePosition = {0.0f, 0.0f, 0.0f};
  // Material planeMaterial = LoadMaterialDefault();

  //----------------------------------

  // material for
  const char *imageAddrMirris =
      "/home/jonatan/Documents/kod/cpp/raylibtest/artwork/mirris.png";
  const char *imageAddrPurre =
      "/home/jonatan/Documents/kod/cpp/raylibtest/artwork/purre.png";
  Image chessImage =
      GenImageChecked(screenWidth, screenHeight, 12, 12, BLACK, WHITE);
  Texture2D chessTexture = LoadTextureFromImage(chessImage);
  Image mirrisImage = LoadImage(imageAddrMirris);
  Image purreImage = LoadImage(imageAddrPurre);
  Texture2D mirrisTexture = LoadTextureFromImage(mirrisImage);
  Texture2D purreTexture = LoadTextureFromImage(purreImage);
  cubeModelMirris.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =
      mirrisTexture;
  cubeModelPurre.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = purreTexture;
  planeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = chessTexture;

  //--------------------PLANE SHADER TEST1
  Mesh shaderPlaneMesh = GenMeshPlane(10.f, 10.f, 200, 200);
  Model shaderPlaneModel = LoadModelFromMesh(shaderPlaneMesh);
  shaderPlaneModel.transform = MatrixRotateXYZ((Vector3){90.0f, 00.0f, 0.0f});

  //--------------------PLANE SHADER TEST1
  Mesh shaderPlaneMesh2 = GenMeshPlane(10.f, 10.f, 200, 200);
  Model shaderPlaneModel2 = LoadModelFromMesh(shaderPlaneMesh2);
  shaderPlaneModel2.transform = MatrixTranslate(2.0f, 0.0f, 0.0f);
  shaderPlaneModel2.transform = MatrixRotateXYZ((Vector3){0.0f, 0.0f, -90.0f});

  //-----------Load shaders
  Shader planeShader =
      LoadShader(TextFormat("/home/jonatan/Documents/kod/cpp/raylibtest/src/"
                            "custom_shaderVertex.glsl",
                            330),
                 TextFormat("/home/jonatan/Documents/kod/cpp/raylibtest/src/"
                            "custom_shaderFrag.glsl",
                            330));
  shaderPlaneModel.materials[0].shader = planeShader;

  Shader planeShader2 =
      LoadShader(TextFormat("/home/jonatan/Documents/kod/cpp/raylibtest/src/"
                            "custom_shaderVertex2.glsl",
                            330),
                 // 0);
                 TextFormat("/home/jonatan/Documents/kod/cpp/raylibtest/src/"
                            "custom_shaderFrag2.glsl",
                            330));
  shaderPlaneModel2.materials[0].shader = planeShader2;

  //--------------SHADER TEXTURE
  int textureLoc = GetShaderLocation(planeShader, "uTexture");
  shaderPlaneModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =
      mirrisTexture;
  //------------uniform time!
  int timeLoc = GetShaderLocation(planeShader, "u_time");
  Vector2 time = {0.0f, 0.0f};
  SetShaderValue(planeShader, timeLoc, &time, SHADER_UNIFORM_VEC2);
  //---------------uniform position
  // int posLoc = GetShaderLocation(planeShader, "a_randomPos");
  //
  // LOAD OBJ
  Model human = LoadModel(TextFormat(
      "/home/jonatan/Documents/kod/cpp/raylibtest/artwork/Human2.obj"));
  Texture2D humanTexture =
      LoadTexture("/home/jonatan/Documents/kod/cpp/raylibtest/artwork/"
                  "humanTexturePurre.png");
  // SetMaterialTexture(&human.materials[0], MATERIAL_MAP_DIFFUSE,
  // humanTexture);
  human.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = humanTexture;
  Vector3 humanPosition;
  // playerPosition
  Vector3 playerPosition = {0.0, 0.0, 0.0};

  // Texture

  std::vector<float> randomPos;
  float randomPosArr[200];
  float randomFloat;
  for (int i = 0; i < 200; i++) {
    randomFloat = (rand() % 99) * 0.11;
    // std::cout << randomFloat << std::endl;
    randomPos.push_back(randomFloat);
    randomPosArr[i] = randomFloat;
  }
  // SetShaderValueV(planeShader, posLoc, randomPosArr, SHADER_ATTRIB_FLOAT,
  // 200);
  //
  // CLOCK
  std::chrono::steady_clock::time_point timer =
      std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point oldTime =
      std::chrono::steady_clock::now();

  // ------------------------- HEIGHT MAP
  Image heightMapImage = LoadImage(
      "/home/jonatan/Documents/kod/cpp/raylibtest/artwork/heightMap.png");
  Texture heightMapTexture = LoadTextureFromImage(heightMapImage);
  Mesh heightMapMesh =
      GenMeshHeightmap(heightMapImage, (Vector3){257.0f, 16.0f, 257.0f});
  Model terrainModel = LoadModelFromMesh(heightMapMesh);
  // UnloadImage(heightMapImage);
  terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = chessTexture;

  // -----------------------heightmap physics
  // get image data
  // const void *imageData = heightMapTexture.GetData();

  // std::cout << "mipmaps: " << (int *)heightMapImage.data << std::endl;
  btHeightfieldTerrainShape *terrainShape = new btHeightfieldTerrainShape(
      heightMapTexture.height, heightMapTexture.width, heightMapImage.data,
      btScalar((1.0 / 256) * 16), btScalar(0.0f), btScalar(17.0f), 1,
      PHY_ScalarType(PHY_UCHAR), true);

  btTransform t2;
  t2.setIdentity();
  t2.setOrigin(btVector3(0, 0, 0));
  btMotionState *motion2 = new btDefaultMotionState(t2);
  btRigidBody::btRigidBodyConstructionInfo infoHeightMap(0, motion2,
                                                         terrainShape);
  btRigidBody *terrainBody = new btRigidBody(infoHeightMap);
  world->addRigidBody(terrainBody);
  terrainBody->setRestitution(0.3f);
  terrainBody->setFriction(0.8f);
  bodies.push_back(terrainBody);

  //--------------------VARIABLES FOR GETTING UPDATE FROM BULLET
  btTransform spherePosFromBullet;
  btDefaultMotionState *sphereMotionState =
      dynamic_cast<btDefaultMotionState *>(testSphere->getMotionState());

  //-------------------TIMER for bullet
  timer = std::chrono::steady_clock::now();
  auto dur = timer - oldTime;
  const std::chrono::duration<double> delta = dur;

  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    timer = std::chrono::steady_clock::now();
    const std::chrono::duration<double> delta = dur;
    // if (dur > std::chrono::milliseconds(15)) {
    //
    //--------------updating physics delta time
    world->stepSimulation(
        btScalar(std::chrono::duration<double>(delta).count()), 1,
        btScalar(1.0) / btScalar(60.0));
    // std::cout << "10\n";
    oldTime = timer;

    if (IsKeyDown(KEY_CAPS_LOCK)) {
      CloseWindow();
    }
    if (IsKeyDown(KEY_SPACE)) {
      camera.position.y += 0.1;
    }
    if (IsKeyDown(KEY_C)) {
      camera.position.y -= 0.1;
    }
    if (IsKeyDown(KEY_T)) {
      camera.position.x += 0.1;
    }
    if (IsKeyDown(KEY_Y)) {
      testSphere->activate(true);
      testSphere->applyCentralImpulse(btVector3(0.0, 12.0, 0.0));
    }
    if (IsKeyDown(KEY_U)) {
      testSphere->activate(true);
      testSphere->applyCentralImpulse(btVector3(0.5, 0.0, 0.0));
    }
    if (IsKeyDown(KEY_I)) {
      testSphere->activate(true);
      testSphere->applyCentralImpulse(btVector3(-0.5, 0.0, 0.0));
    }

    UpdateCamera(&camera, CAMERA_FIRST_PERSON);

    //----------------------------------------------------------------------------------

    // SetShaderValue(planeShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
    // Draw
    //----------------------------------------------------------------------------------
    //----------------UPDATE UNIFORMS
    time.x += 0.1f;
    time.y += 0.1f;

    // SetShaderValueTexture(planeShader, textureLoc, mirrisTexture);
    SetShaderValue(planeShader, timeLoc, &time, SHADER_UNIFORM_VEC2);
    // SetShaderValue(planeShader2, timeLoc, &time, SHADER_UNIFORM_VEC2);
    // SetShaderValue(planeShader, posLoc, randomPosArr, SHADER_ATTRIB_FLOAT);

    // SetShaderValueV(planeShader, posLoc, randomPosArr, SHADER_ATTRIB_FLOAT,
    // 200);
    BeginDrawing();

    i += 0.01;
    cubeX += std::sin(i);
    cameraP += std::cos(i);

    ClearBackground(GREEN);
    cubeModelMirris.transform = MatrixRotateXYZ((Vector3){0.0, i, i});
    cubeModelPurre.transform = MatrixTranslate(0.0f, cubeX * 0.12, 0.0f);
    // camera.position.z = (float)std::sin(i) * 10.0f;
    // camera.position.x = (float)cos(i) * 10.0f;

    //--------------get update on position from BULLET
    spherePosFromBullet = sphereMotionState->m_graphicsWorldTrans;

    BeginMode3D(camera);

    DrawSphere(Vector3{(float)spherePosFromBullet.getOrigin().getX(),
                       (float)spherePosFromBullet.getOrigin().getY(),
                       (float)spherePosFromBullet.getOrigin().getZ()},
               testSphereRad, PURPLE);
    DrawModel(human, playerPosition, 1.0, WHITE);
    DrawModel(cubeModelMirris, cubePositionMirris, 1.0, WHITE);
    DrawModel(cubeModelPurre, cubePositionPurre, 1.0, WHITE);
    // DrawModel(planeModel, planePosition, 1.0, WHITE);
    // DrawModel(terrainModel, planePosition, 1.0, WHITE);
    DrawModel(terrainModel, Vector3{-128.0f, -8.0f, -128.0f}, 1.0, WHITE);
    DrawModel(shaderPlaneModel, Vector3{0.0, 2.0, -10.0}, 1.0, WHITE);
    // DrawModel(shaderPlaneModel2,
    //           Vector3{-float(testSphereCoor.getY()), 2.0, 0.0}, 1.0, WHITE);
    EndMode3D();

    DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //
  UnloadShader(planeShader);
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------
  return 0;
}
