/*
Trabalho 2.2
Nome: Diego Hammes Hoffmann

Crie um projeto com as seguintes características:
a. Desenhe os eixos x, y e z coloridos;  ok
b. Desenhe um objeto 3D com ao menos 20 vértices, estruturado em structs (use a bibutil para cálculo das normais);.  ok
c. Tratar rotação, navegação e zoom (botão esquerdo, central e direito do mouse, respectivamente); ok
d. Criar uma fonte de luz e tratar a manipulação de seus parâmetros (o material deve refletir totalmente todas as luzes e
   deve-se definir um valor fixo para especularidade):
	i. Selecionar cor da característica ambiente: 1 para vermelho, 2 para verde e 3 para azul;
	ii. Selecionar cor da característica difusa: 4 para vermelho, 5 para verde e 6 para azul;
	iii. Selecionar cor da característica especular: 7 para vermelho, 8 para verde e 9 para azul;
	iv. Alterar valor da cor selecionada (entre 0 e 1): teclas + e -;
	v. Alterar posição da fonte de luz:
		1. No eixo X: teclas esquerda e direita;
		2. No eixo Y: teclas cima e baixo;
		3. No eixo Z: teclas pageup e pagedown;
	vi. Ligar e desligar fonte de luz: tecla espaço;
e. Descrever TODOS eventos e valores em tela por glBitmapCharacter (use glWindowPos). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include "bibutil.h"
#include "glWindowPos.cpp"
#include <windows.h>

float r = 1, g = 1, b = 1;
int rotX = 0, rotY = 0, zoomX = 30, zoomY = 30, fLuz = 1, envia = 1, flagCor = 5;
float btnC_X = 0, btnC_Y = 0, flagFoc = 0;
float ex, ey, ez;
float lar, lag, lab, ldr, ldg = 1, ldb, ler, leg, leb, PosX, PosY, PosZ;
char texto[100]("Bem-Vindo ao trabalho 2.2 - Onde a mascada nao tem limites");
int statusTecla[256];

unsigned char padrao[128] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x80, 0x01, 0xC0, 0x06, 0xC0, 0x03, 0x60,
	0x04, 0x60, 0x06, 0x20, 0x04, 0x30, 0x0C, 0x20,
	0x04, 0x18, 0x18, 0x20, 0x04, 0x0C, 0x30, 0x20,
	0x04, 0x06, 0x60, 0x20, 0x44, 0x03, 0xC0, 0x22,
	0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
	0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
	0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
	0x66, 0x01, 0x80, 0x66, 0x33, 0x01, 0x80, 0xCC,
	0x19, 0x81, 0x81, 0x98, 0x0C, 0xC1, 0x83, 0x30,
	0x07, 0xe1, 0x87, 0xe0, 0x03, 0x3f, 0xfc, 0xc0,
	0x03, 0x31, 0x8c, 0xc0, 0x03, 0x33, 0xcc, 0xc0,
	0x06, 0x64, 0x26, 0x60, 0x0c, 0xcc, 0x33, 0x30,
	0x18, 0xcc, 0x33, 0x18, 0x10, 0xc4, 0x23, 0x08,
	0x10, 0x63, 0xC6, 0x08, 0x10, 0x30, 0x0c, 0x08,
	0x10, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x08
};

void escreve(char* texto) {
	int i;
	glWindowPos2f(20, 30);
	for (i = 0; texto[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
	}
}

//Desenha eixos
void desenhaEixos() {
	glBegin(GL_LINES);

	//eixo X
	glColor3f(1, 1, 1);
	glVertex3f(-20, 0, 0);
	glVertex3f(20, 0, 0);

	//eixo Y
	glColor3f(0, 1, 0);
	glVertex3f(0, -20, 0);
	glVertex3f(0, 20, 0);

	//eixo Z
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -20);
	glVertex3f(0, 0, 20);

	//finaliza eixos
	glEnd();
}//fim função desenhaeixos

//Desenha foguete
VERT vertices[25] = {
	{0,13,0},    //	id = 0
	{3,7,2},     //	id = 1 
	{-3,7,2},    //	id = 2
	{3,-2,2},    //	id = 3
	{-3,-2,2},   //	id = 4

	{3,7,-2},     // id = 5
	{-3,7,-2},    // id = 6
	{3,-2,-2},    // id = 7
	{-3,-2,-2},   // id = 8
	{-5,-5,-3.5}, // id = 9
	{-5,-1,-3.5}, // id = 10
	{1,-5,-3.5},  // id = 11
	{5,-5,-3.5},  // id = 12

	{-1,-2,2},	  // id = 13
	{-5,-5,3.5},  // id = 14
	{-1,-5,3.5},  // id = 15

	{1,-2,2},	  // id = 16
	{1,-5,3.5},   // id = 17
	{5,-5,3.5},   // id = 18

	{-1,-2,-2},	  // id = 19
	{-5,-5,-3.5},  // id = 20
	{-1,-5,-3.5},  // id = 21

	{1,-2,-2},	  // id = 22
	{1,-5,-3.5},  // id = 23
	{5,-5,-3.5}   // id = 24
};

//juntas os vertices do desenho
int indFaces[25][4] = {
	{0,1,2},
	{0,2,6},
	{0,6,5},
	{0,5,1},
	{1,2,4,3},
	{1,5,7,3},
	{5,7,8,6},
	{8,6,2,4},

	{4,14,15,13},
	{4,8,20,14},
	{8,19,21,20},
	{19,13,15,21},

	{16,3,18,17},
	{3,7,24,18},
	{7,22,23,24},
	{22,16,17,23}
};

FACE faces[25] = {

	{4,indFaces[0],NULL},
	{4,indFaces[1],NULL},
	{4,indFaces[2],NULL},
	{4,indFaces[3],NULL},
	{4,indFaces[4],NULL},
	{4,indFaces[5],NULL},
	{4,indFaces[6],NULL},
	{4,indFaces[7],NULL},
	{4,indFaces[8],NULL},
	{4,indFaces[9],NULL},
	{4,indFaces[10],NULL},
	{4,indFaces[11],NULL},
	{4,indFaces[12],NULL},
	{4,indFaces[13],NULL},
	{4,indFaces[14],NULL},
	{4,indFaces[15],NULL}
};
OBJ obj1 = { 16,16,-1,false,vertices,NULL, faces };

void iluminacao(int flagFoc) {
	float luzAmbienteModel[4] = { 0.5, 0.5, 0.5, 1 };
	glLightModelfv(GL_AMBIENT, luzAmbienteModel);
	switch (flagFoc) {
	case 1:
	case 2:
	case 3:
		if (flagFoc == 1) {
			lag = 0; lab = 0;
		}
		if (flagFoc == 2) {
			lar = 0; lab = 0;
		}
		if (flagFoc == 3) {
			lar = 0; lag = 0;
		}
		ldr = 0; ldg = 0; ldb = 0;
		ler = 0; leg = 0; leb = 0;
		break;
	case 4:
	case 5:
	case 6:
		if (flagFoc == 4) {
			ldg = 0; ldb = 0;
		}
		if (flagFoc == 5) {
			ldr = 0; ldb = 0;
		}
		if (flagFoc == 6) {
			ldr = 0; ldg = 0;
		}
		lar = 0; lag = 0; lab = 0;
		ler = 0; leg = 0; leb = 0;
		break;
	case 7:
	case 8:
	case 9:
		if (flagFoc == 7) {
			leg = 0; leb = 0;
		}
		if (flagFoc == 8) {
			ler = 0; leb = 0;
		}
		if (flagFoc == 9) {
			ler = 0; leg = 0;
		}
		lar = 0; lag = 0; lab = 0;
		ldr = 0; ldg = 0; ldb = 0;
		break;
	}

	float luzAmbiente[4] = { lar, lag, lab, 1 };
	float luzDifusa[4] = { ldr, ldg, ldb, 1 };
	float luzEspecular[4] = { ler, leg,leb, 1 };
	float luzPosicao[4] = { PosX, PosY, PosZ, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
	glEnable(GL_LIGHT0);

	float matAmbiente[4] = { 1,1,1,1 };
	float matDifusa[4] = { 0.5,0.5,0.5,1 };
	float matEspecular[4] = { 0.2,0.2,0.2,1 };
	float matBrilho = 30;

	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbiente);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDifusa);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matEspecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, luzAmbiente);

	if (envia == 1)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

}


void desenha() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//chamada da função desenhaeixos
	desenhaEixos();
	//tratando navegação com o mouse botão central
	glTranslatef(btnC_X, btnC_Y, 0);
	//rotação com o botão esquerdo
	glRotatef(rotX, 1, 0, 0);
	glRotatef(rotY, 0, 1, 0);
	CalculaNormaisPorFace(&obj1);
	SetaModoDesenho('s');
	DesenhaObjeto(&obj1);
	escreve(texto);
	glutSwapBuffers();
}//fim função desenha


void inicializa() {
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, 1, 0.1, 1000);
	//alteração do zoom com a posição do mouse no momento em que botão direito é precionado
	gluLookAt(zoomX, zoomY, 30, 0, 0, 0, 0, 1, 0);
	iluminacao(0);
}//fim função inicializa


//função dos eventos do mouse
void eventos(int tecla, int status, int x, int y) {
	if (tecla == GLUT_MIDDLE_BUTTON && status == GLUT_DOWN) {
		strcpy(texto, "Navegacao do objeto");
		btnC_X = x / 100;
		btnC_Y = y / 100;
		glutPostRedisplay();
	}
	if (tecla == GLUT_LEFT_BUTTON && status == GLUT_DOWN) {
		strcpy(texto, "Rotacao do objeto");
		rotX = x;
		rotY = y;
		glutPostRedisplay();
	}
	if (tecla == GLUT_RIGHT_BUTTON && status == GLUT_DOWN) {
		strcpy(texto, "Zoom sobre o objeto");
		zoomX = x;
		zoomY = y;
		inicializa();
		glutPostRedisplay();
	}

}//fim função eventos

//função do sobre eventos do teclado
void teclado(unsigned char tecla, int x, int y) {

	//statusTecla[tecla] = GetKeyState() != 0 ? 1 : 0;

	switch (tecla) {
	case '1'://luz ambiente vermelha
		lar = 1; lag = 0; lab = 0;
		strcpy(texto, "Luz ambiente vermelha selecionada");
		iluminacao(1);
		flagCor = 1;
		desenha();
		break;
	case '2'://luz ambiente verde
		lag = 1; lar = 0; lab = 0;
		strcpy(texto, "Luz ambiente verde selecionada");
		iluminacao(2);
		flagCor = 2;
		desenha();
		break;
	case '3'://luz ambiente azul
		lab = 1; lag = 0; lar = 0;
		strcpy(texto, "Luz ambiente azul selecionada");
		iluminacao(3);
		flagCor = 3;
		desenha();
		break;
	case '4'://luz difusa vermelha
		ldr = 1; ldg = 0; ldb = 0;
		strcpy(texto, "Luz difusa vermelha selecionada");
		iluminacao(4);
		flagCor = 4;
		desenha();
		break;
	case '5'://luz difusa verde
		ldg = 1; ldr = 0; ldb = 0;
		strcpy(texto, "Luz difusa verde selecionada");
		iluminacao(5);
		flagCor = 5;
		desenha();
		break;
	case '6'://luz difusa azul
		ldb = 1; ldg = 0; ldr = 0;
		strcpy(texto, "Luz difusa azul selecionada");
		iluminacao(6);
		flagCor = 6;
		desenha();
		break;
	case '7'://luz especular vermelha
		ler = 1; leg = 0; leb = 0;
		strcpy(texto, "Luz especular vermelha selecionada");
		iluminacao(7);
		flagCor = 7;
		desenha();
		break;
	case '8'://luz especular verde
		leg = 1; ler = 0; leb = 0;
		strcpy(texto, "Luz especular verde selecionada");
		iluminacao(8);
		flagCor = 8;
		desenha();
		break;
	case '9'://luz especular azul
		leb = 1; leg = 0; ler = 0;
		strcpy(texto, "Luz especular azul selecionada");
		iluminacao(9);
		flagCor = 9;
		desenha();
		break;
	case '+':
		strcpy(texto, "Intensidade da cor aumentada");
		if (flagCor == 1) {
			lar = lar + 0.1;
			iluminacao(1);
		}
		if (flagCor == 2) {
			lag = lag + 0.1;
			iluminacao(2);
		}
		if (flagCor == 3) {
			lab = lab + 0.1;
			iluminacao(3);
		}
		if (flagCor == 4) {
			ldr = ldr + 0.1;
			iluminacao(4);
		}

		if (flagCor == 5) {
			ldg = ldg + 0.1;
			iluminacao(5);
		}
		if (flagCor == 6) {
			ldb = ldb + 0.1;
			iluminacao(6);
		}
		if (flagCor == 7) {
			ler = ler + 0.1;
			iluminacao(7);
		}
		if (flagCor == 8) {
			leg = leg + 0.1;
			iluminacao(8);
		}
		if (flagCor == 9) {
			leb = leb + 0.1;
			iluminacao(9);
		}
		break;
	case '-':
		strcpy(texto, "Intensidade da cor diminuida");
		if (flagCor == 1) {
			lar = lar - 0.1;
			iluminacao(1);
		}
		if (flagCor == 2) {
			lag = lag - 0.1;
			iluminacao(2);
		}
		if (flagCor == 3) {
			lab = lab - 0.1;
			iluminacao(3);
		}
		if (flagCor == 4) {
			ldr = ldr - 0.1;
			iluminacao(4);
		}
		if (flagCor == 5) {
			ldg = ldg - 0.1;
			iluminacao(5);
		}
		if (flagCor == 6) {
			ldb = ldb - 0.1;
			iluminacao(6);
		}
		if (flagCor == 7) {
			ler = ler - 0.1;
			iluminacao(7);
		}
		if (flagCor == 8) {
			leg = leg - 0.1;
			iluminacao(8);
		}
		if (flagCor == 9) {
			leb = leb - 0.1;
			iluminacao(9);
		}
		break;
	case 32: //Espaço

		if (fLuz == 1) {
			//printf("Fonte de luz Desligada.\n\n");
			strcpy(texto, "Fonte de luz Desligada");
			envia = 0;
			fLuz = 0;
			iluminacao(0);
			desenha();
		}
		else {
			// printf("Fonte de luz ligada.\n\n");
			strcpy(texto, "Fonte de luz ligada");
			envia = 1;
			fLuz = 1;
			iluminacao(0);
			desenha();
		}
		break;

	default:
		break;
	}
	
	statusTecla[65] = (GetKeyState(65) & 0x8000) ? 1 : 0;
	statusTecla[68] = (GetKeyState(68) & 0x8000) ? 1 : 0;
	statusTecla[87] = (GetKeyState(87) & 0x8000) ? 1 : 0;
	statusTecla[83] = (GetKeyState(83) & 0x8000) ? 1 : 0;

	if (statusTecla[65]) {
		strcpy(texto, "Tecla A");
		rotY = rotY - 1;
	}

	if (statusTecla[68]) {
		strcpy(texto, "Tecla D");
		rotY = rotY + 1;
	}

	if (statusTecla[87]) {
		strcpy(texto, "Tecla W");
		rotX = rotX + 1;
	}

	if (statusTecla[83]) {
		strcpy(texto, "Tecla S");
		rotX = rotX - 1;
	}

	glutPostRedisplay();
}//fim função teclado

void especiais(int tecla, int x, int y) {
	switch (tecla) {
	case 100://esquerda
	case 102://direita  
		PosX = 20; PosY = 0; PosZ = 0;
		strcpy(texto, "Posicao da fonte de luz no eixo X");
		iluminacao(0);
		desenha();
		break;
	case 101://cima
	case 103://baixo    
		PosY = 20; PosX = 0; PosZ = 0;
		strcpy(texto, "Posicao da fonte de luz no eixo Y");
		iluminacao(0);
		desenha();
		break;
	case GLUT_KEY_PAGE_UP://pageup
	case GLUT_KEY_PAGE_DOWN://pagedown      
		PosZ = 20; PosY = 0; PosX = 0;
		strcpy(texto, "Posicao da fonte de luz no eixo Z");
		iluminacao(0);
		desenha();
		break;
	default:
		break;
	}
}// fim função especiais


int main() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 680);
	glutInitWindowPosition(100, 30);
	glutCreateWindow("Trabalho 2.2");
	glutDisplayFunc(desenha);
	glutKeyboardFunc(teclado);
	glutSpecialFunc(especiais);
	glutMouseFunc(eventos);
	inicializa();
	glutMainLoop();

	return 0;

}//Fim main
