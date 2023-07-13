// precision mediump float;

uniform vec2 u_time;
uniform sampler2D uTexture;

//varyings from VS
varying vec2 vUv;
varying float vElevation;

void main()
{
  vec4 textureColor = texture2D(uTexture, vUv);
  textureColor.rgb *= (vElevation*0.1+0.5);
  gl_FragColor = textureColor;
   //gl_FragColor = vec4(abs(sin(u_time.x)), 0.0, 1.0, 1.0);

// layout(location = 0) out vec4 diffuseColor;
  }
