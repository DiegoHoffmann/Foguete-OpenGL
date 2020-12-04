#include <math.h>
#include <string.h>
#include <vector>
#include "bibutil.h"

using namespace std;

vector<OBJ*> _objetos(0);

// Modo de desenho
char _modo = 'w';

// Normaliza o vetor recebido por parâmetro.
void Normaliza(VERT &norm)
{
	float tam = sqrt(norm.x*norm.x
		+norm.y*norm.y
		+norm.z*norm.z);
	if(tam == 0) return;
	norm.x /= tam;
	norm.y /= tam;
	norm.z /= tam;
}

// Rotina que recebe dois vetores e retorna por parâmetro, 
// através de vresult, o produto vetorial entre eles.
void ProdutoVetorial (VERT &v1, VERT &v2, VERT &vresult)
{
	vresult.x = v1.y * v2.z - v1.z * v2.y;
	vresult.y = v1.z * v2.x - v1.x * v2.z;
	vresult.z = v1.x * v2.y - v1.y * v2.x;
}

// Esta rotina recebe três vértices coplanares e calcula
// o vetor normal a eles (n).
void VetorNormal(VERT vert1, VERT vert2, VERT vert3, VERT &n)
{
	VERT vet1, vet2;
	vet1.x = vert1.x - vert2.x;
	vet1.y = vert1.y - vert2.y;
	vet1.z = vert1.z - vert2.z;
	vet2.x = vert3.x - vert2.x;
	vet2.y = vert3.y - vert2.y;
	vet2.z = vert3.z - vert2.z;
	ProdutoVetorial(vet2, vet1, n);
	Normaliza(n);
}

// Função interna, usada por CarregaObjeto para
// interpretar a definição de uma face em um
// arquivo .OBJ
// 
// Recebe um apontador para a posição corrente na
// string e devolve o valor numérico encontrado
// até o primeiro / ou -1 caso não exista
//
// Também retorna em sep o separador encontrado
int leNum(char **face, char *sep)
{
	char temp[10];
	int pos = 0;
	while(**face)
	{
		if(**face == '/' || **face==' ') // achamos o separador
		{
			*sep = **face;
			(*face)++;
			break;
		}
		temp[pos++] = **face;
		(*face)++;
	}
	temp[pos]=0;
	if(!pos) // string vazia ?
		return -1;
	return atoi(temp);
}

// Cria e carrega um objeto 3D que esteja armazenado em um
// arquivo no formato OBJ, cujo nome é passado por parâmetro.
// É feita a leitura do arquivo para preencher as estruturas 
// de vértices e faces, que são retornadas através de um OBJ.
OBJ *CarregaObjeto(char *nomeArquivo)
{
	int i;
	int vcont,ncont,fcont,tcont;
	char aux[256];	
	FILE *fp;
	OBJ *obj;

	fp = fopen(nomeArquivo, "r");  // abre arquivo texto para leitura

	if(fp == NULL)
		return NULL;

	if ( ( obj = (OBJ *) malloc(sizeof(OBJ)) ) == NULL)
		return NULL;

	// Inicializa contadores do objeto
	obj->numVertices  = 0;
	obj->numFaces     = 0;
	obj->numNormais   = 0;	
	obj->normais_por_vertice = false;
	obj->vertices = NULL;
	obj->faces = NULL;
	obj->normais = NULL;
	
	// A primeira passagem serve apenas para contar quantos
	// elementos existem no arquivo - necessário para
	// alocar memória depois
	while(!feof(fp))
	{
		fgets(aux,255,fp);
		if(!strncmp(aux,"v ",2)) // encontramos um vértice
			obj->numVertices++;
		if(!strncmp(aux,"f ",2)) // encontramos uma face
			obj->numFaces++;
		if(!strncmp(aux,"vn ",3)) // encontramos uma normal
			obj->numNormais++;
	}
	// Agora voltamos ao início do arquivo para ler os elementos
	rewind(fp);

	// Aloca os vértices
	if ( ( obj->vertices = (VERT *) malloc((sizeof(VERT)) * obj->numVertices) ) == NULL )
		return NULL;

	// Aloca as faces
	if ( ( obj->faces = (FACE *) malloc((sizeof(FACE)) * obj->numFaces) ) == NULL )
		return NULL;

	// Aloca as normais
	if(obj->numNormais)
		if ( ( obj->normais = (VERT *) malloc((sizeof(VERT)) * obj->numNormais) ) == NULL )
			return NULL;
		
	// A segunda passagem é para ler efetivamente os
	// elementos do arquivo, já que sabemos quantos
	// tem de cada tipo
	vcont = 0;
	ncont = 0;
	tcont = 0;
	fcont = 0;

	// Utilizadas para determinar os limites do objeto
	// em x,y e z
	float minx,miny,minz;
	float maxx,maxy,maxz;

	while(!feof(fp))
	{
		fgets(aux,255,fp);
		aux[strlen(aux)-1]=0;	// elimina o \n lido do arquivo
		// Pula comentários
		if(aux[0]=='#') continue;
				
		// Vértice ?
		if(!strncmp(aux,"v ",2))
		{
			sscanf(aux,"v %f %f %f",&obj->vertices[vcont].x,
				&obj->vertices[vcont].y,
				&obj->vertices[vcont].z);
			if(!vcont)
			{
				minx = maxx = obj->vertices[vcont].x;
				miny = maxy = obj->vertices[vcont].y;
				minz = maxz = obj->vertices[vcont].z;
			}
			else
			{
				if(obj->vertices[vcont].x < minx) minx = obj->vertices[vcont].x;
				if(obj->vertices[vcont].y < miny) miny = obj->vertices[vcont].y;
				if(obj->vertices[vcont].z < minz) minz = obj->vertices[vcont].z;
				if(obj->vertices[vcont].x > maxx) maxx = obj->vertices[vcont].x;
				if(obj->vertices[vcont].y > maxy) maxy = obj->vertices[vcont].y;
				if(obj->vertices[vcont].z > maxz) maxz = obj->vertices[vcont].z;
			}
			vcont++;
		}
		// Normal ?
		if(!strncmp(aux,"vn ",3))
		{
			sscanf(aux,"vn %f %f %f",&obj->normais[ncont].x,
					&obj->normais[ncont].y,
					&obj->normais[ncont].z);
			ncont++;
			// Registra que o arquivo possui definição de normais por
			// vértice
			obj->normais_por_vertice = true;
		}
		
		// Face ?
		if(!strncmp(aux,"f ",2))
		{
			// Pula "f "
			char *ptr = aux+2;		
			
			// Temporários para armazenar os índices desta face
			int vi[10],ti[10],ni[10];
			// Separador encontrado
			char sep;
			obj->faces[fcont].nv = 0;
			int nv = 0;
			bool tem_t = false;
			bool tem_n = false;
			// Interpreta a descrição da face
			while(*ptr)
			{
				// Lê índice do vértice
				vi[nv] = leNum(&ptr,&sep);
				// Se o separador for espaço, significa que
				// não temos texcoord nem normais
				if(sep==' ')
				{
					// Nesse caso, incrementamos o contador
					// de vértices e continuamos
					nv++;
					continue;
				}
				// Lê índice da texcoord
				int aux = leNum(&ptr,&sep);
				// Se o valor for != -1, significa
				// que existe um índice de texcoord
				if(aux!=-1)
				{
					ti[nv] = aux;
					tem_t = true;
				}
				// Mas se o separador for " ", significa
				// que não existe um índice de normal
				if(sep==' ')
				{
					// E nesse caso, pulamos para
					// o próximo vértice
					nv++;
					continue;
				}
				// Lê o índice da normal
				aux = leNum(&ptr,&sep);
				if(aux!=-1)
				{
					ni[nv] = aux;
					tem_n = true;
				}
				// Prepara para próximo vértice
				nv++;
			}
			// Fim da face, aloca memória para estruturas e preenche com
			// os valores lidos
			obj->faces[fcont].nv = nv;
			obj->faces[fcont].vert = (int *) malloc(sizeof(int)*nv);
			// Só aloca memória para normais e texcoords se for necessário
			if(tem_n) obj->faces[fcont].norm = (int *) malloc(sizeof(int)*nv);
			else obj->faces[fcont].norm = NULL;			
			// Copia os índices dos arrays temporários para a face
			for(i=0;i<nv;++i)
			{
				// Subtraímos -1 dos índices porque o formato OBJ começa
				// a contar a partir de 1, não 0
				obj->faces[fcont].vert[i] = vi[i]-1;
				if(tem_n) obj->faces[fcont].norm[i] = ni[i]-1;				
			}
			// Prepara para próxima face
			fcont++;
		}
	}
	// Fim, fecha arquivo e retorna apontador para objeto
	fclose(fp);
	// Adiciona na lista
	_objetos.push_back(obj);
	return obj;
}

// Seta o modo de desenho a ser utilizado para os objetos
// 'w' - wireframe
// 's' - sólido
void SetaModoDesenho(char modo)
{
	if(modo!='w' && modo!='s') return;
	_modo = modo;
}

// Desenha um objeto 3D passado como parâmetro.
void DesenhaObjeto(OBJ *obj)
{
	int i;	// contador
	GLint ult_texid, texid;	// última/atual textura 
	GLenum prim = GL_POLYGON;	// tipo de primitiva
	GLfloat branco[4] = { 1.0, 1.0, 1.0, 1.0 };	// constante para cor branca
	
	// Seleciona GL_LINE_LOOP se o objetivo
	// for desenhar o objeto em wireframe
	if(_modo=='w') prim = GL_LINE_LOOP;

	// Salva atributos de iluminação e materiais
	glPushAttrib(GL_LIGHTING_BIT);
	
	// Armazena id da última textura utilizada
	// (por enquanto, nenhuma)
	ult_texid = -1;
	// Varre todas as faces do objeto
	for(i=0; i<obj->numFaces; i++)
	{
		// Usa normais calculadas por face (flat shading) se
		// o objeto não possui normais por vértice
		if(!obj->normais_por_vertice)
			glNormal3f(obj->normais[i].x,obj->normais[i].y,obj->normais[i].z);

		// Inicia a face
		glBegin(prim);
		// Para todos os vértices da face
		for(int vf=0; vf<obj->faces[i].nv;++vf)
		{
			// Se houver normais definidas para cada vértice,
			// envia a normal correspondente
			if(obj->normais_por_vertice)
				glNormal3f(obj->normais[obj->faces[i].norm[vf]].x,
				obj->normais[obj->faces[i].norm[vf]].y,
				obj->normais[obj->faces[i].norm[vf]].z);
						
			// Envia o vértice em si
			glVertex3f(obj->vertices[obj->faces[i].vert[vf]].x,
		    	obj->vertices[obj->faces[i].vert[vf]].y,
			obj->vertices[obj->faces[i].vert[vf]].z);
		}
		// Finaliza a face
		glEnd();
				
	} // fim da varredura de faces
	// Restaura os atributos de iluminação e materiais
	glPopAttrib();
}

// Calcula o vetor normal de cada face de um objeto 3D.
void CalculaNormaisPorFace(OBJ *obj)
{
	int i;
	// Retorna se o objeto já possui normais por vértice
	if(obj->normais_por_vertice) return;
	// Aloca memória para as normais (uma por face)
	if ( ( obj->normais = (VERT *) malloc((sizeof(VERT)) * obj->numFaces) ) == NULL )
			return;
	// Varre as faces e calcula a normal, usando os 3 primeiros vértices de
	// cada uma
	for(i=0; i<obj->numFaces; i++)
	VetorNormal(obj->vertices[obj->faces[i].vert[0]],
		obj->vertices[obj->faces[i].vert[1]],
		obj->vertices[obj->faces[i].vert[2]],obj->normais[i]);
}

