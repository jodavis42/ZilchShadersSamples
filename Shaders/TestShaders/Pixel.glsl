#version 430

//uniform mat4 LocalToWorld;
//uniform mat4 WorldToView;
//uniform mat4 ViewToProjection;
//
//uniform vec3 ViewLightPosition;
//uniform vec4 LightAmbient;
//uniform vec4 LightDiffuse;
//uniform vec4 LightSpecular;
//
//struct LightData
//{
//  vec4 ViewPosition;
//};
//
////layout (location = 0)
//buffer LightBuffer
//{
//  LightData Lights[];
//};
//
//in block
//{
//  vec3 ViewPosition;
//  vec3 ViewNormal;
//  vec3 Position;
//} In;
//
//vec4 ComputeLighting(LightData light)
//{
//  vec3 l = light.ViewPosition.xyz - In.ViewPosition;
//  l = normalize(l);
//  float diffuse = dot(In.ViewNormal, l);
//
//  return vec4(vec3(diffuse), 1);
//}

out vec4 colorOut;
void main()
{
  colorOut = vec4(1, 0, 1, 1);
  //LightData mainLight;
  //mainLight.ViewPosition = vec4(ViewLightPosition, 1);
  //
  //colorOut = ComputeLighting(mainLight);
  //for(int i = 0; i < Lights.length(); ++i)
  //  colorOut += ComputeLighting(Lights[i]);
  //vec3 l = ViewLightPosition - In.ViewPosition;
  //l = normalize(l);
  //float diffuse = dot(In.ViewNormal, l);
  //
  //colorOut = vec4(vec3(diffuse), 1);
  //colorOut = vec4(In.ViewNormal, 1);
}