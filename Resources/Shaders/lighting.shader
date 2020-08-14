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
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0); // set alle 4 vector values to 1.0
}