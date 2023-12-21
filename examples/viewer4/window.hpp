#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include "model.hpp"
#include "trackball.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:

// ========================================

  GLint m_modelMatrixLocation{};
  GLint m_modelMatrixLocation2{};
  GLint m_modelMatrixLocation3{};


    struct Banana {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
    glm::vec3 orientacao{};
    glm::vec3 orientacaoFinal{};
    bool voltouOrigem = true;

    bool travado = true;
  };

  Banana banana1;

  struct Alvo {
    glm::vec3 m_position{};
  };

  Alvo alvo1;
  // ========================================

  glm::ivec2 m_viewportSize{};

  Model m_model, modelo_banana, modeloAlvo;
  int m_trianglesToDraw{};
  int m_trianglesToDraw2{};
  int m_trianglesToDraw3{};

  TrackBall m_trackBallModel;
  TrackBall m_trackBallLight;
  float m_zoom{};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  // Shaders
  std::vector<char const *> m_shaderNames{"texture", "blinnphong", "phong",
                                          "gouraud", "normal",     "depth"};
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};
  int m_mappingMode2{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  glm::vec4 m_lightDir2{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia2{1.0f};
  glm::vec4 m_Id2{1.0f};
  glm::vec4 m_Is2{1.0f};
  glm::vec4 m_Ka2{};
  glm::vec4 m_Kd2{};
  glm::vec4 m_Ks2{};
  float m_shininess2{};

  glm::vec4 m_lightDir3{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia3{1.0f};
  glm::vec4 m_Id3{1.0f};
  glm::vec4 m_Is3{1.0f};
  glm::vec4 m_Ka3{};
  glm::vec4 m_Kd3{};
  glm::vec4 m_Ks3{};
  float m_shininess3{};

  void loadModel(std::string_view pathObj);
  void loadModel2(std::string_view pathObj);
  void loadModel3(std::string_view pathObj);
};

#endif