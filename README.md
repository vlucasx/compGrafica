


# Canhão de Banana

- Nome: Lucas Guerra Silvestre 
- RA: 11201721549


## Arquivo window.cpp

### método onEvent():

Foi adicionando o trecho

```
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE)
      banana1.travado = false;
}
```

que é responsável por controlar o disparo da banana.

### método onCreate():

// inicia a posição da câmera e da banana na posição inicial:

```
  glm::vec3 posicaoInicial(0.0f, 0.0f, 0.0f);
  banana1.m_position = posicaoInicial;
```


Carrega os modelos do canhão e da banana e seus respectivos modos de mapeamento:

```
  loadModel(assetsPath + "cannon.obj");
  loadModel2(assetsPath + "banana.obj");
  m_mappingMode = 3;
  m_mappingMode2 = 3;
  ```

### metodo onPaint():
Renderiza a banana dentro do trackball, acompanhando a orientacao do canhão, caso a variável "travada" = true.
Renderiza também o canhão.

### metodo onUpdate():
Se a banana sair do raio de 6.0, volta para a origem para ser atirada novamente:
```
  if (banana1.m_position.z <= -6.0 || banana1.m_position.z >= 6.0 || banana1.m_position.x <= -6.0 || banana1.m_position.x >= 6.0 || banana1.m_position.y <= -6.0 || banana1.m_position.y >= 6.0 ) {
      banana1.m_position.z = 0.0;
      banana1.m_position.y = 0.0;
      banana1.m_position.x = 0.0;
      banana1.travado = true;
  }
```
Caso a banana não esteja travada, ou seja, esteja fora da origem, a mesma terá sua posição atualizada em função do tempo de forma proporcional ás componentes vetoriais que descrevem sua orientação no momento do disparo, que coincidem com a orientação do canhão naquele momento. O resultado é um deslocamento com velocidade constante na direção e sentido apontado pelo vetor de orientacao do canhão no momento do disparo.

O método adiciona também um componente exponencial no decremento da posição em y da banana, para simular uma forca gravitacional:
```
  banana1.m_position.y = banana1.m_position.y - 3*deltaTime*deltaTime;

```

Por fim, quando a banana volta para a origem, sua posição é realocada para a ponta do canhão para o próximo disparo.
