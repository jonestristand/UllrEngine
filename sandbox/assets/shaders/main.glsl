#shader vertex
#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
  v_TexCoord = texCoord;
}
#endshader

#shader fragment
#version 430

layout(location = 0) out vec4 outColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform vec4 u_Color;

void main()
{
  vec4 texColor = texture(u_Texture1, v_TexCoord); //mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
	outColor = texColor * u_Color;
}
#endshader
