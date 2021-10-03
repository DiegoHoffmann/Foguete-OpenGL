# Foguete-OpenGL
Desenho de foguete 3D em OpenGL
###Introdução
Atividade desenvolvida na cadeira de Computação grafica na Ulbra Gravataí. Desenho básico de um foguete seguindo os requisitos do trabalho.

- [x] a) Desenhe os eixos x, y e z coloridos
- [x] b) Desenhe um objeto 3D com ao menos 20 vértices, estruturado em structs (use a bibutil para cálculo das normais)
- [x] c) Tratar rotação, navegação e zoom (botão esquerdo, central e direito do mouse, respectivamente)
- [x] d) Criar uma fonte de luz e tratar a manipulação de seus parâmetros (o material deve refletir totalmente todas as luzes e
   deve-se definir um valor fixo para especularidade)
	- i. Selecionar cor da característica ambiente: 1 para vermelho, 2 para verde e 3 para azul;
	- ii. Selecionar cor da característica difusa: 4 para vermelho, 5 para verde e 6 para azul;
	- iii. Selecionar cor da característica especular: 7 para vermelho, 8 para verde e 9 para azul;
	- iv. Alterar valor da cor selecionada (entre 0 e 1): teclas + e -;
	- v. Alterar posição da fonte de luz:
		-- 1. No eixo X: teclas esquerda e direita;
		-- 2. No eixo Y: teclas cima e baixo;
		-- 3. No eixo Z: teclas pageup e pagedown;
	- vi. Ligar e desligar fonte de luz: tecla espaço;
- [x] e) Descrever TODOS eventos e valores em tela por glBitmapCharacter (use glWindowPos).

**Adicional:** Teclas W, A, S e D rotacionam o objeto.

### Comando para buildar e executar a aplicação
> msbuild trabalho2.2 -p:Configuration=Debug

> start Debug/trabalho2.2.exe


![Trabalho 2 2 2021-10-03 17-23-46](https://user-images.githubusercontent.com/39543693/135771737-4f1b9e5b-9670-448b-ae7a-014ab6208b32.gif)

