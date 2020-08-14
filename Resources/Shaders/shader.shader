#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main()
{
    gl_Position = projection * view * model * position;
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

uniform vec3 u_color;
void main()
{
    color = vec4(u_color,1.0);
}