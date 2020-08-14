#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;


in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float u_gamma;
uniform float u_shine;

void main()
{

      // diffuse
      //vec3 norm = normalize(Normal);
      vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
      normal = normalize(normal * 2.0 - 1.0);

      vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;

      vec3 ambient = 0.1 * color;

        vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

//       vec3 lightDir = normalize(light.position - FragPos);
//       float diff = max(dot(norm, lightDir), 0.0);
//       vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_shine);
    vec3 specular = spec * texture(texture_specular1, fs_in.TexCoords).rgb;

      // specular
//       vec3 viewDir = normalize(viewPos - FragPos);
//       vec3 reflectDir = reflect(-lightDir, norm);
//       float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shine);


    //vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
    vec3 height = texture(texture_height1, fs_in.TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(pow(result, vec3(1.0 / u_gamma)),1.0);
}