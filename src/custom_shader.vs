uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

uniform vec2 u_time;

attribute vec3 vertexPostition;
attribute vec2 fragTexCoord;
attribute float a_randomPos;


varying vec2 vUv;
varying float vElevation;

void main()
{
  vec4 modelPos = matModel * vec4(vertexPostition, 1.0);

float elevation = sin(modelPos.x - u_time.x) * 1.1;
elevation += sin(modelPos.y - u_time.x) * 1.1;
modelPos.z += elevation;
  //modelPos.z += a_randomPos;

  vec4 viewPos = matView * modelPos;
  vec4 projectedPos = matProjection * viewPos;
  gl_Position = projectedPos;

  vUv = fragTexCoord;
  vElevation = elevation;
  }
