uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

attribute vec3 vertexPostition;

void main()
{
  vec4 modelPos = matModel * vec4(vertexPostition, 1.0);

  modelPos.y += sin(modelPos.x * 0.1) * 3.1;

  vec4 viewPos = matView * modelPos;
  vec4 projectedPos = matProjection * viewPos;
  gl_Position = projectedPos;
  }
