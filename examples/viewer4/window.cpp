#include "window.hpp"

#include "imfilebrowser.h"

void Window::onEvent(SDL_Event const &event) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (event.type == SDL_MOUSEMOTION) {
    m_trackBallModel.mouseMove(mousePosition);
    m_trackBallLight.mouseMove(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      m_trackBallModel.mousePress(mousePosition);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mousePress(mousePosition);
    }
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      m_trackBallModel.mouseRelease(mousePosition);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mouseRelease(mousePosition);
    }
  }				
  if (event.type == SDL_MOUSEWHEEL) {
    m_zoom += (event.wheel.y > 0 ? -1.0f : 1.0f) / 5.0f;
    m_zoom = glm::clamp(m_zoom, -1.5f, 1.0f);
  }


  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE)
      banana1.travado = false;
}
  //if (event.type == SDL_KEYUP) {
    // if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE))
    //  m_dollySpeed = 0.0f;
  //}
}

void Window::onCreate() {

// inicia a posicao da camera e da banana na posicao inicial:
  glm::vec3 posicaoInicial(0.0f, 0.0f, 0.0f);
  glm::vec3 posicaoAlvo(0.0f, 6.0f, -10.0f);
  banana1.m_position = posicaoInicial;
  alvo1.m_position = posicaoAlvo;
  
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  // Create programs
  for (auto const &name : m_shaderNames) {
    auto const path{assetsPath + "shaders/" + name};
    auto const program{abcg::createOpenGLProgram(
        {{.source = path + ".vert", .stage = abcg::ShaderStage::Vertex},
         {.source = path + ".frag", .stage = abcg::ShaderStage::Fragment}})};
    m_programs.push_back(program);
  }

  // Carrega os modelos
  loadModel(assetsPath + "cannon.obj");
  loadModel2(assetsPath + "banana.obj");
  loadModel3(assetsPath + "alvo.obj");

  m_mappingMode = 3; // "From mesh" option
  m_mappingMode2 = 3;
  // Initial trackball spin
  m_trackBallModel.setAxis(glm::normalize(glm::vec3(1, 1, 1)));
  m_trackBallModel.setVelocity(0.1f);
}

void Window::loadModel(std::string_view path) {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  m_model.destroy();

  m_model.loadDiffuseTexture(assetsPath + "maps/cannon.jpg");
  m_model.loadObj(assetsPath + "cannon.obj");
  m_model.setupVAO(m_programs.at(m_currentProgramIndex));
  m_trianglesToDraw = m_model.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}

void Window::loadModel2(std::string_view path) {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  modelo_banana.destroy();

  modelo_banana.loadDiffuseTexture(assetsPath + "maps/textura_banana.jpg");
  modelo_banana.loadObj(assetsPath + "banana.obj");
  modelo_banana.setupVAO(m_programs.at(m_currentProgramIndex));
  m_trianglesToDraw2 = modelo_banana.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka2 = modelo_banana.getKa();
  m_Kd2 = modelo_banana.getKd();
  m_Ks2 = modelo_banana.getKs();
  m_shininess2 = modelo_banana.getShininess();
}

void Window::loadModel3(std::string_view path) {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  modeloAlvo.destroy();

  modeloAlvo.loadDiffuseTexture(assetsPath + "maps/textura_alvo.jpg");
  modeloAlvo.loadObj(assetsPath + "alvo.obj");
  modeloAlvo.setupVAO(m_programs.at(m_currentProgramIndex));
  m_trianglesToDraw3 = modeloAlvo.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka3 = modeloAlvo.getKa();
  m_Kd3 = modeloAlvo.getKd();
  m_Ks3 = modeloAlvo.getKs();
  m_shininess3 = modeloAlvo.getShininess();
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Use currently selected program
  auto const program{m_programs.at(m_currentProgramIndex)};
  abcg::glUseProgram(program);

  // Get location of uniform variables
  auto const viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  auto const modelMatrixLoc{abcg::glGetUniformLocation(program, "modelMatrix")};
  auto const normalMatrixLoc{
      abcg::glGetUniformLocation(program, "normalMatrix")};
  auto const lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")};
  auto const shininessLoc{abcg::glGetUniformLocation(program, "shininess")};
  auto const IaLoc{abcg::glGetUniformLocation(program, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(program, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(program, "Is")};
  auto const KaLoc{abcg::glGetUniformLocation(program, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(program, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(program, "Ks")};
  auto const diffuseTexLoc{abcg::glGetUniformLocation(program, "diffuseTex")};
  auto const mappingModeLoc{abcg::glGetUniformLocation(program, "mappingMode")};

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  auto const lightDirRotated{m_trackBallLight.getRotation() * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);


  auto const modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
  auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

// ======== Desloca banana ===============================================================================

  // Set uniform variables for the current model


  glm::mat4 model{1.0f};
// Altera a matriz para fora do trackball:
  glm::mat4 modelo{1.0f};  
  modelo = glm::translate(modelo, banana1.m_position); // -3.5f no z (ultimo termo do vetor)
  modelo = glm::rotate(modelo, glm::radians(45.0f), glm::vec3(0, 1, 0));
  modelo = glm::scale(modelo, glm::vec3(1.0f));

// =======================================================================================================


// Desenha deslocado: (apenas se a banana estiver na origem, "travada")
  if (banana1.travado == true) {
  m_modelMatrixLocation2 = abcg::glGetUniformLocation(program, "modelMatrix");
  abcg::glUniformMatrix4fv(m_modelMatrixLocation2, 1, GL_FALSE, &modelo[0][0]);
  }
  else {
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
  abcg::glUniformMatrix4fv(m_modelMatrixLocation2, 1, GL_FALSE, &modelo[0][0]);
  }
  modelo_banana.render(m_trianglesToDraw2);
  //m_model.render(m_trianglesToDraw);

// Desenha no trackball:
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  m_model.render(m_trianglesToDraw);


//=========== Desenha alvo ======================

  glm::mat4 modelo3{1.0f};
  modelo3 = glm::translate(modelo3, alvo1.m_position);
  modelo3 = glm::rotate(modelo3, glm::radians(30.0f), glm::vec3(0, 1, 0));
  modelo3 = glm::scale(modelo3, glm::vec3(2.0f));


  m_modelMatrixLocation3 = abcg::glGetUniformLocation(program, "modelMatrix");
  abcg::glUniformMatrix4fv(m_modelMatrixLocation3, 1, GL_FALSE, &modelo3[0][0]);

  modeloAlvo.render(m_trianglesToDraw3);
  abcg::glUseProgram(0);
}

void Window::onUpdate() {

  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
// se sair do raio de 12.0, volta para a origem:
  if (banana1.m_position.z <= -12.0 || banana1.m_position.z >= 12.0 || banana1.m_position.x <= -12.0 || banana1.m_position.x >= 12.0 || banana1.m_position.y <= -12.0 || banana1.m_position.y >= 12.0 ) {
      banana1.m_position.z = 0.0;
      banana1.m_position.y = 0.0;
      banana1.m_position.x = 0.0;
      banana1.travado = true;
  }

  if (banana1.travado == false) {
  banana1.orientacaoFinal = banana1.orientacao;

  banana1.m_position.x = banana1.m_position.x + banana1.orientacaoFinal.x *18* deltaTime;
  banana1.m_position.y = banana1.m_position.y + banana1.orientacaoFinal.y *18* deltaTime;
  banana1.m_position.z = banana1.m_position.z + banana1.orientacaoFinal.z *18* deltaTime;

// gravidade:
  banana1.m_position.y = banana1.m_position.y - deltaTime*deltaTime - 0.1;
  }
  
  else {
    if (banana1.voltouOrigem == false) {
      m_modelMatrixLocation2 = m_modelMatrixLocation;
      banana1.voltouOrigem = true;
    }

// desloca banana para a ponta do canhao
    banana1.m_position.x = banana1.orientacao.x * 1.05;
    banana1.m_position.y = banana1.orientacao.y * 1.05;
    banana1.m_position.z = banana1.orientacao.z * 1.05;
  }

  m_modelMatrix = m_trackBallModel.getRotation();
  banana1.orientacao = m_modelMatrix[2];
  m_viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f + m_zoom),
                  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  auto const scaledWidth{gsl::narrow_cast<int>(m_viewportSize.x * 0.8f)};
  auto const scaledHeight{gsl::narrow_cast<int>(m_viewportSize.y * 0.8f)};

  // File browser for models
  static ImGui::FileBrowser fileDialogModel;
  fileDialogModel.SetTitle("Load 3D Model");
  fileDialogModel.SetTypeFilters({".obj"});
  fileDialogModel.SetWindowSize(scaledWidth, scaledHeight);

  // File browser for textures
  static ImGui::FileBrowser fileDialogTex;
  fileDialogTex.SetTitle("Load Texture");
  fileDialogTex.SetTypeFilters({".jpg", ".png"});
  fileDialogTex.SetWindowSize(scaledWidth, scaledHeight);

#if defined(__EMSCRIPTEN__)
  auto const assetsPath{abcg::Application::getAssetsPath()};
  fileDialogModel.SetPwd(assetsPath);
  fileDialogTex.SetPwd(assetsPath + "/maps");
#endif

  // Create main window widget
  {
    auto widgetSize{ImVec2(222, 190)};

    if (!m_model.isUVMapped()) {
      // Add extra space for static text
      widgetSize.y += 26;
    }

    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr,
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration);

    // Menu
    {
      bool loadModel{};
      bool loadDiffTex{};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          ImGui::MenuItem("Load 3D Model...", nullptr, &loadModel);
          ImGui::MenuItem("Load Diffuse Texture...", nullptr, &loadDiffTex);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (loadModel)
        fileDialogModel.Open();
      if (loadDiffTex)
        fileDialogTex.Open();
    }

    // Slider will be stretched horizontally
    ImGui::PushItemWidth(widgetSize.x - 16);
    ImGui::SliderInt(" ", &m_trianglesToDraw, 0, m_model.getNumTriangles(),
                     "%d triangles");
    ImGui::PopItemWidth();

    static bool faceCulling{};
    ImGui::Checkbox("Back-face culling", &faceCulling);

    if (faceCulling) {
      abcg::glEnable(GL_CULL_FACE);
    } else {
      abcg::glDisable(GL_CULL_FACE);
    }

    // CW/CCW combo box
    {
      static std::size_t currentIndex{};
      std::vector<std::string> const comboItems{"CCW", "CW"};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Front face",
                            comboItems.at(currentIndex).c_str())) {
        for (auto const index : iter::range(comboItems.size())) {
          auto const isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      if (currentIndex == 0) {
        abcg::glFrontFace(GL_CCW);
      } else {
        abcg::glFrontFace(GL_CW);
      }
    }

    // Projection combo box
    {
      static std::size_t currentIndex{};
      std::vector<std::string> comboItems{"Perspective", "Orthographic"};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Projection",
                            comboItems.at(currentIndex).c_str())) {
        for (auto const index : iter::range(comboItems.size())) {
          auto const isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                        gsl::narrow<float>(m_viewportSize.y)};
      if (currentIndex == 0) {
        m_projMatrix =
            glm::perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
      } else {
        m_projMatrix =
            glm::ortho(-1.0f * aspect, 1.0f * aspect, -1.0f, 1.0f, 0.1f, 5.0f);
      }
    }

    // Shader combo box
    {
      static std::size_t currentIndex{};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Shader", m_shaderNames.at(currentIndex))) {
        for (auto const index : iter::range(m_shaderNames.size())) {
          auto const isSelected{currentIndex == index};
          if (ImGui::Selectable(m_shaderNames.at(index), isSelected))
            currentIndex = index;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      // Set up VAO if shader program has changed
      if (gsl::narrow<int>(currentIndex) != m_currentProgramIndex) {
        m_currentProgramIndex = gsl::narrow<int>(currentIndex);
        m_model.setupVAO(m_programs.at(m_currentProgramIndex));
      }
    }

    if (!m_model.isUVMapped()) {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Mesh has no UV coords.");
    }

    // UV mapping box
    {
      std::vector<std::string> comboItems{"Triplanar", "Cylindrical",
                                          "Spherical"};

      if (m_model.isUVMapped())
        comboItems.emplace_back("From mesh");

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("UV mapping",
                            comboItems.at(m_mappingMode).c_str())) {
        for (auto const index : iter::range(comboItems.size())) {
          auto const isSelected{m_mappingMode == static_cast<int>(index)};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            m_mappingMode = index;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();
    }

    ImGui::End();
  }

  // Create window for light sources
  if (m_currentProgramIndex < 4) {
    auto const widgetSize{ImVec2(222, 244)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5,
                                   m_viewportSize.y - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

    ImGui::Text("Light properties");

    // Slider to control light properties
    ImGui::PushItemWidth(widgetSize.x - 36);
    ImGui::ColorEdit3("Ia", &m_Ia.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Id", &m_Id.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Is", &m_Is.x, ImGuiColorEditFlags_Float);
    ImGui::PopItemWidth();

    ImGui::Spacing();

    ImGui::Text("Material properties");

    // Slider to control material properties
    ImGui::PushItemWidth(widgetSize.x - 36);
    ImGui::ColorEdit3("Ka", &m_Ka.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Kd", &m_Kd.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Ks", &m_Ks.x, ImGuiColorEditFlags_Float);
    ImGui::PopItemWidth();

    // Slider to control the specular shininess
    ImGui::PushItemWidth(widgetSize.x - 16);
    ImGui::SliderFloat(" ", &m_shininess, 0.0f, 500.0f, "shininess: %.1f");
    ImGui::PopItemWidth();

    ImGui::End();
  }

  fileDialogModel.Display();
  if (fileDialogModel.HasSelected()) {
    loadModel(fileDialogModel.GetSelected().string());
    fileDialogModel.ClearSelected();

    if (m_model.isUVMapped()) {
      // Use mesh texture coordinates if available...
      m_mappingMode = 3;
    } else {
      // ...or triplanar mapping otherwise
      m_mappingMode = 0;
    }
  }

  fileDialogTex.Display();
  if (fileDialogTex.HasSelected()) {
    m_model.loadDiffuseTexture(fileDialogTex.GetSelected().string());
    fileDialogTex.ClearSelected();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_trackBallModel.resizeViewport(size);
  m_trackBallLight.resizeViewport(size);
}

void Window::onDestroy() {
  m_model.destroy();
  modelo_banana.destroy();
  for (auto const &program : m_programs) {
    abcg::glDeleteProgram(program);
  }
}