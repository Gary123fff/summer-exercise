#include <GL/glut.h> 
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include <conio.h>
#include <png.h>
#include "readObj.h"
using namespace std;

ReadOBJ robj;
int vertexcount = 0;
int facecount = 0;
int height = 1000;
int width = 1000;
const char* filename = "output.png";
void idle() {
	robj.m_angle += 0.01;
	if (robj.m_angle > 360.0)
		robj.m_angle = 0.0;
	glutPostRedisplay();
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glPointSize(2);
	gluLookAt(0, 5, 5, 0, 0, 0, 0, 1, 0);
	glRotatef(robj.m_angle, 0.0f, 1.0f, 0.0f);

	if (robj.m_mode == 0)
		robj.drawVertex();
	else if (robj.m_mode == 1)
		robj.drawLine();
	else
		robj.drawFace();

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
	case 's':
	case 83: // Escape key
		robj.m_mode++;
		if (robj.m_mode >= 3)
			robj.m_mode = 0;
		break;
	case 'p':
	case 80: // Escape key
		robj.saveImage(filename, width, height, x, y);
		break;
	default:
		std::cout << "Key pressed: " << key << std::endl;
		break;
	}
}
void adjacentVertex() {
	int i = 0;
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

int main(int argc, char** argv) {

	robj.readobj("Cube.obj");
	adjacentVertex();
	facecount = 0;
	for (auto fc : robj.m_FaceCollect) {
		fc.showId();
		robj.comFaceNorm(&fc, facecount);
		cout << "Face norm :" << robj.m_faces[facecount].m_fnorm[0] << " " << robj.m_faces[facecount].m_fnorm[1] << " " << robj.m_faces[facecount].m_fnorm[2] << endl << endl;
		facecount++;
	}

	robj.computVertexNorm();
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
	glutIdleFunc(idle);


	glutMainLoop();
	return 0;
}