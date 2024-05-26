#include <GL/glut.h> 
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include <conio.h>
#include <png.h>
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"
#include "readObj.h"
using namespace std;

ReadOBJ robj;
int vertexcount = 0;
int facecount = 0;
int height = 1000;
int width = 1000;
const char* filename = "output.png";
void adjacentVertex() {
	robj.m_VertexCollect.clear();
	vector<VertexCollect>().swap(robj.m_VertexCollect);
	robj.m_FaceCollect.clear();
	vector<FaceCollect>().swap(robj.m_FaceCollect);
	for (const auto& face : robj.m_faces) {
		for (int i = 0; i < face.count; i++) {
			VertexCollect vc(face.vi[i]);
			vc.x = robj.m_vertices[face.vi[i]].x;
			vc.y = robj.m_vertices[face.vi[i]].y;
			vc.z = robj.m_vertices[face.vi[i]].z;
			robj.m_VertexCollect.push_back(vc);
			vertexcount++;
		}
	}
	vertexcount = 0;
	facecount = 0;
	for (const auto& face : robj.m_faces) {
		FaceCollect fc(facecount);

		for (int i = 0; i < face.count; i++) {
			fc.adjVertex(&robj.m_VertexCollect[vertexcount]);
			vertexcount++;
		}
		robj.m_FaceCollect.push_back(fc);
		facecount++;
	}

}
void idle() {
	robj.m_angle += 0.01;
	if (robj.m_angle > 360.0)
		robj.m_angle = 0.0;
	if (robj.m_reshape) {
		for (int i = 0; i < robj.m_tempvertices.size(); i++) {
			robj.m_vertices[i].x = robj.m_tempvertices[i].x + sin(robj.m_a + robj.m_t);
			robj.m_vertices[i].y = robj.m_tempvertices[i].y + sin(robj.m_a + robj.m_t);
			robj.m_vertices[i].z = robj.m_tempvertices[i].z + sin(robj.m_a + robj.m_t);
		}
	robj.m_a += robj.m_t;
	}//reshape後的數值
	if (robj.m_a >= 360.0)
		robj.m_a = 0;
	adjacentVertex();
	glutPostRedisplay();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glPointSize(2);
	gluLookAt(0, 5, 5, 0, 0, 0, 0, 1, 0);
	glRotatef(robj.m_angle, 1, 1, 1);//選轉物體

	if (robj.m_mode == 0) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_NORMALIZE);
		robj.drawVertex();
	}
	else if (robj.m_mode == 1) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_NORMALIZE);
		robj.drawLine();
	}
	else
		robj.drawFace();
	
	facecount = 0;
	for (auto fc : robj.m_FaceCollect) {
		robj.comFaceNorm(&fc, facecount);
		facecount++;
	}

	robj.computVertexNorm();

	glutSwapBuffers();


}
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':// vertex render or face render
	case 67:
		robj.m_rendermode++;
		if (robj.m_rendermode >= 2)
			robj.m_rendermode = 0;
		break;
	case 'p'://save png
	case 80: 
		robj.saveImage(filename, width, height, x, y);
		break;
	case 'r'://reshape
	case 82:
		robj.m_reshape++;
		if (robj.m_reshape >= 3)
			robj.m_reshape = 0;
		break;
	case 's':
	case 83://change vertex line fill mode
		robj.m_mode++;
		if (robj.m_mode >= 3)
			robj.m_mode = 0;
		break;
	case 't'://save png
	case 84:
		robj.m_textures_mode++;
		if (robj.m_textures_mode >= 2)
			robj.m_textures_mode = 0;
		break;
	default:
		std::cout << "Key pressed: " << key << std::endl;
		break;
	}
}

void texture() {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;

	unsigned char* data = stbi_load("test.jpg", &width, &height, &nrChannels, 3);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}//放材質上去
int main(int argc, char** argv) {

	robj.readobj("Cube.obj");
	adjacentVertex();//tore the faces adjacent to a vertex
					//using doubly linked list 之後在使用資料時會比較有效率
	facecount = 0;
	for (auto fc : robj.m_FaceCollect) {
		fc.showId();
		robj.comFaceNorm(&fc, facecount);
		cout << "Face norm :" << robj.m_faces[facecount].m_fnorm[0] << " " << robj.m_faces[facecount].m_fnorm[1] << " " << robj.m_faces[facecount].m_fnorm[2] << endl << endl;
		facecount++;
	}//計算normal of face

	robj.computVertexNorm();//計算normal of vertex
	for (int i = 0; i < robj.m_vertices.size(); i++)
		cout << i + 1 << " Vertex Norm :" << robj.m_vertices[i].m_vnorm[0] << " " << robj.m_vertices[i].m_vnorm[1] << " " << robj.m_vertices[i].m_vnorm[2] << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("eye");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	texture();
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}