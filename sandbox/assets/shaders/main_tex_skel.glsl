#shader vertex
#version 430

const int NUM_BONES = 64;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 color;
layout(location = 4) in vec3 tangent;
layout(location = 5) in ivec4 jointIds;
layout(location = 6) in vec4 jointWeights;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    vec3 Color;
    mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 mvp;

uniform mat4 jointTransforms[NUM_BONES];

void main()
{
  mat4 boneTransform = jointTransforms[jointIds[0]] * jointWeights[0];
  boneTransform += jointTransforms[jointIds[1]] * jointWeights[1];
  boneTransform += jointTransforms[jointIds[2]] * jointWeights[2];
  boneTransform += jointTransforms[jointIds[3]] * jointWeights[3];

  // Calculate TBN-space transform matric for normal mapping
  vec3 T = normalize(vec3(model * boneTransform * vec4(tangent,   0.0)));
  vec3 N = normalize(vec3(model * boneTransform * vec4(normal,    0.0)));
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(T, N);

  vs_out.FragPos = vec3(model * boneTransform * vec4(position, 1.0));
  vs_out.TexCoords = texcoord;
  vs_out.Color = color;
  vs_out.Normal = vec3(transpose(inverse(model)) * boneTransform * vec4(normal, 0.0));
  vs_out.TBN = mat3(T, B, N);

  gl_Position = mvp * boneTransform * vec4(position, 1.0);
  
  gl_PointSize = 5;
}
#endshader





#shader fragment
#version 430

layout(location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    vec3 Color;
    mat3 TBN;
} fs_in;  

struct Material {
  sampler2D diffuse1;
  sampler2D specular1;
  sampler2D normal1;
  float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform bool isSpecular;
uniform bool useNormal;

void main()
{
    // Calculate normal vector based on either vertex normal or normal map
    vec3 normal;
    if (useNormal) {
      normal = texture(material.normal1, fs_in.TexCoords).rgb;
      normal = normalize(normal * 2.0 - 1.0);
      normal = normalize(fs_in.TBN * normal);
    }
    else {
      normal = normalize(fs_in.Normal);
    }

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse1, fs_in.TexCoords).rgb;
  	
    // diffuse 
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse1, fs_in.TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular1, fs_in.TexCoords).rgb;
    if (!isSpecular)
      specular = vec3(0, 0, 0);

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);

    // Debug code to display normal map
    //if (useNormal) {
    //  FragColor = vec4(texture(material.normal1, fs_in.TexCoords).rgb, 1.0);
    //}
}
#endshader
