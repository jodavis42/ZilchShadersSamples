#version 430

in vec3 LocalPosition;
in vec3 LocalNormal;
//out vec3 vPosition;
uniform mat4 LocalToWorld;
uniform mat4 WorldToView;
uniform mat4 ViewToPerspective;

//out vec3 vViewPosition;
//out vec3 vViewNormal;

out block
{
  vec3 ViewPosition;
  vec3 ViewNormal;
  vec3 Position;
} Out;

void main()
{
  Out.Position = LocalPosition;
  Out.ViewPosition = (vec4(LocalPosition, 1) * LocalToWorld * WorldToView).xyz;
  Out.ViewNormal = (vec4(LocalNormal, 0) * LocalToWorld * WorldToView).xyz;
  
  gl_Position = vec4(LocalPosition, 1) * LocalToWorld * WorldToView * ViewToPerspective;
}
