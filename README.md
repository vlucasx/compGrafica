


# Canhao de Banana

- Nome: Lucas Guerra Silvestre 
- RA: 11201721549


## Arquivo window.cpp

### metodo onEvent():

Foi adicionando o trecho

```
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE)
      banana1.travado = false;
}
```

que e responsavel por controlar o disparo da banana.

### metodo onCreate():

// inicia a posicao da camera e da banana na posicao inicial:

```
  glm::vec3 posicaoInicial(0.0f, 0.0f, 0.0f);
  banana1.m_position = posicaoInicial;
```


Carrega os modelos do canhao e da banana e seus respectivos modos de mapeamento:

```
  loadModel(assetsPath + "cannon.obj");
  loadModel2(assetsPath + "banana.obj");
  m_mappingMode = 3;
  m_mappingMode2 = 3;
  ```

### metodo onPaint():
Renderiza a banana dentro do trackball, acompanhando a orientacao do canhao, caso a variavel "travada" = true.
Renderiza tambem o canhao.

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
Caso a banana nao esteja travada, ou seja, esteja fora da origem, a mesma tera sua posicao atualizada em funcao do tempo de forma proporcional as componentes vetoriais que descrevem sua orientacao no momento do disparo, que coincidem com a orientacao do canhao naquele momento. O resultado e um deslocamento a velocidade constante na direcao e sentido apontado pelo vetor de orientacao do canhao no momento do disparo.

O metodo adiciona tambem um componente exponencial no decremento da posicao em y da banana, para simular uma forca gravitacional:
```
  banana1.m_position.y = banana1.m_position.y - 3*deltaTime*deltaTime;

```

Por fim, quando a banana volta para a origem, sua posicao e realocada para a ponta do canhao para o proximo disparo.