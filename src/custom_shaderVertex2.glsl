uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;


attribute vec3 vertexPostition;
attribute vec2 fragTexCoord;
attribute float a_randomPos;


varying vec2 vUv;
varying float vElevation;

void main()
{
  vec4 modelPos = matModel * vec4(vertexPostition, 1.0);

  vec4 viewPos = matView * modelPos;
  vec4 projectedPos = matProjection * viewPos;
  gl_Position = projectedPos;

  vUv = fragTexCoord;
  }
