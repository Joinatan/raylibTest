#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <iostream>
#include <vector>

int main() {

  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 1366;
  const int screenHeight = 768;
  // std::cout << "hej";

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
  Material planeMaterial = LoadMaterialDefault();

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

  //--------------------PLANE SHADER TEST
  Mesh shaderPlaneMesh = GenMeshPlane(10, 10, 200, 200);
  Model shaderPlaneModel = LoadModelFromMesh(shaderPlaneMesh);
  shaderPlaneModel.transform = MatrixRotateXYZ((Vector3){90.0f, 00.0f, 0.0f});

  //-----------Load shaders
  Shader planeShader = LoadShader(
      TextFormat(
          "/home/jonatan/Documents/kod/cpp/raylibtest/src/custom_shader.vs",
          330),
      TextFormat(
          "/home/jonatan/Documents/kod/cpp/raylibtest/src/custom_shader.fs",
          330));
  shaderPlaneModel.materials[0].shader = planeShader;
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

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //=================CONTROLS
    // if (IsKeyDown(KEY_UP)) {
    //   camera.position.x -= 0.1;
    // }
    // if (IsKeyDown(KEY_DOWN)) {
    //   camera.position.x += 0.1;
    // }
    // if (IsKeyDown(KEY_LEFT)) {
    //   camera.position.z += 0.1;
    // }
    // if (IsKeyDown(KEY_RIGHT)) {
    //   camera.position.z -= 0.1;
    // }
    if (IsKeyDown(KEY_CAPS_LOCK)) {
      CloseWindow();
    }

    // mousePos = GetMousePosition();
    // camera.target = (Vector3){mousePos.x, mousePos.y, 1.0f};
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

    BeginMode3D(camera);
    DrawModel(cubeModelMirris, cubePositionMirris, 1.0, WHITE);
    DrawModel(cubeModelPurre, cubePositionPurre, 1.0, WHITE);
    DrawModel(planeModel, planePosition, 1.0, WHITE);
    DrawModel(shaderPlaneModel, Vector3{0.0, 2.0, -10.0}, 1.0, WHITE);
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
