#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.1415926
#endif

// Define a estrutura de um vértice
typedef struct {
	float x,y,z;
} VERT;

// Define a estrutura de uma face
typedef struct {
	int nv;		// número de vértices na face
	int *vert;	// índices dos vértices
	int *norm;	// índices das normais	
} FACE;

// Define a estrutura de um objeto 3D
typedef struct {
	int numVertices;
	int numFaces;
	int numNormais; //-1 vazio
	bool normais_por_vertice;	// true se houver normais por vértice
	VERT *vertices;
	VERT *normais;
	FACE *faces;	
} OBJ;

// Protótipos das funções

// Funções para cálculos diversos
void Normaliza(VERT &norm);
void ProdutoVetorial (VERT &v1, VERT &v2, VERT &vresult);
void VetorNormal(VERT vert1, VERT vert2, VERT vert3, VERT &n);

// Funções para carga e desenho de objetos
OBJ *CarregaObjeto(char *nomeArquivo);
void DesenhaObjeto(OBJ *obj);
void SetaModoDesenho(char modo);

// Funções para cálculo de normais
void CalculaNormaisPorFace(OBJ *obj);

// Constantes utilizadas caso não existam em GL/gl.h
#ifndef GL_ARB_texture_cube_map
# define GL_NORMAL_MAP					0x8511
# define GL_REFLECTION_MAP				0x8512
# define GL_TEXTURE_CUBE_MAP			0x8513
# define GL_TEXTURE_BINDING_CUBE_MAP	0x8514
# define GL_TEXTURE_CUBE_MAP_POSITIVE_X	0x8515
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_X	0x8516
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Y	0x8517
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	0x8518
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Z	0x8519
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	0x851A
# define GL_PROXY_TEXTURE_CUBE_MAP		0x851B
# define GL_MAX_CUBE_MAP_TEXTURE_SIZE	0x851C
#endif
