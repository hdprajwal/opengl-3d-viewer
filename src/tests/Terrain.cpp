#include "Terrain.h"

template<typename tVal>
tVal map_value(std::pair<tVal,tVal> a, std::pair<tVal, tVal> b, tVal inVal)
{
    tVal inValNorm = inVal - a.first;
    tVal aUpperNorm = a.second - a.first;
    tVal normPosition = inValNorm / aUpperNorm;

    tVal bUpperNorm = b.second - b.first;
    tVal bValNorm = normPosition * bUpperNorm;
    tVal outVal = b.first + bValNorm;

    return outVal;
}

glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
glm::vec4 ClearColor(0.0f, 0.4f, 1.0f,0.0f);
glm::vec3 ambientColor(0.2f, 0.5f, 0.5);
glm::vec3 diffuseColor(0.5f, 0.5f, 0.5);

namespace test{
    Terrain::Terrain():m_program("/home/prajwal/CLionProjects/openglProject/Resources/Shaders/grid.shader"),
                       m_programGrid("/home/prajwal/CLionProjects/openglProject/Resources/Shaders/lighting.shader"){

        GenerateTerrain();

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER,m_vertices.size()* sizeof(terrainVertices), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(terrainVertices), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(terrainVertices), (void*)(3 * sizeof(float)));
    }

    Terrain::~Terrain() {

    }

    void Terrain::OnUpdate(float deltatime) {

    }

    void Terrain::OnRender(glm::mat4 proj, glm::mat4 view,glm::vec3 cam_pos) {

        glClearColor(ClearColor.x,ClearColor.y,ClearColor.z,ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (m_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            ClearColor = glm::vec4 (1.0);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        }



        glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER,0,m_vertices.size()* sizeof(terrainVertices),&m_vertices[0]);
        m_vertices.clear();
        GenerateTerrain();
        glm::mat4 projection = proj;
        glm::mat4 mview = view;
        glm::mat4 model = glm::mat4(1.0f);
        m_program.Bind();

        m_program.SetUniformVec3("light.position", lightPos);
        m_program.SetUniformVec3("viewPos", cam_pos);
        m_program.SetUniformVec3("light.ambient", ambientColor);
        m_program.SetUniformVec3("light.diffuse", diffuseColor);
        m_program.SetUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        m_program.SetUniformVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        m_program.SetUniformVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        m_program.SetUniformVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        m_program.SetUniform1f("material.shininess", 32.0f);

        m_program.SetUniformMat4("projection", projection);
        m_program.SetUniformMat4("view", mview);

        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
        model = glm::translate(model, glm::vec3(-50.0f, -70.0f, 5.0f));
//        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
        m_program.SetUniformMat4("model", model);
        for (int i = 0; i < 100; i++) {
            glDrawArrays(GL_TRIANGLE_STRIP,i*200,200);
        }
        glBindBuffer(GL_ARRAY_BUFFER,0);

    }

    void Terrain::OnImGuiRender() {
        if (ImGui::Button(m_wireframe ? "Disable WireFrame" : "Enable Wireframe")) {
            m_wireframe = !m_wireframe;
        }
        ImGui::SliderFloat("X Seed",&xo,0.0,1.0);
        ImGui::SliderFloat("y Seed",&yo,0.0,1.0);
        ImGui::SliderFloat2("Terrain Height",&height.x,-1.0,1.0);
        ImGui::SliderFloat3("Background Color",&ClearColor.x,0.0,1.0);
        ImGui::SliderFloat3("Light Position",&lightPos.x,-20.0,20.0);
        ImGui::SliderFloat3("Ambient Color",&ambientColor.x,0.0,1.0);
        ImGui::SliderFloat3("Diffuse Color",&diffuseColor.x,0.0,1.0);
    }

    void Terrain::GenerateTerrain() {
        m_fly -= 0.1;
        float yoff = m_fly;
        for (int y = 0; y < 80; y++) {
            float xoff = 0.0;
            for (int x = 0; x <100; x++) {
                std::pair<float,float> a(0,1), b(height.x,height.y);
                glm::vec2 p(xoff,yoff);
                float val =  map_value(a,b,glm::perlin(p));
                xoff +=xo;
                m_zCoord[x][y] = val;
            }
            yoff += yo;
        }

        for (int y = 0; y < 80-1; y++) {
            for (int x = 0; x < 100; x++) {
                std::pair<float, float> a(0, 2000), b(0, 100);
                glm::vec3 pos1;
                glm::vec3 pos2;
                terrainVertices pos;
                pos1.x = map_value(a, b, (float) (x * 20));;
                pos1.y = map_value(a, b, (float) (y * 20));
                pos1.z = m_zCoord[x][y];
                pos2.x = map_value(a, b, (float) (x * 20));;
                pos2.y = map_value(a, b, (float) ((y + 1) * 20));
                pos2.z = m_zCoord[x][y + 1];
                pos.position = pos1;
                pos.normals = glm::vec3(0.0,0.0,-1.0);
                m_vertices.push_back(pos);
                pos.position = pos2;
                pos.normals = glm::vec3(0.0,0.0,-1.0);
                m_vertices.push_back(pos);
            }
        }
    }

}

