#include "TextureChanging.h"

namespace test{

    TextureChanging::TextureChanging() {

    }

    TextureChanging::~TextureChanging() {

    }

    void TextureChanging::OnUpdate(float deltatime) {
        Test::OnUpdate(deltatime);
    }

    void TextureChanging::OnRender(glm::mat4 proj, glm::mat4 view,glm::vec3 camPos) {

    }

    void TextureChanging::OnImGuiRender() {
        Test::OnImGuiRender();
    }
}