#include <GL/glut.h> 
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include <conio.h>
using namespace std;
class ReadOBJ {
private:
	struct Vertex {
		float x, y, z;
	};
	struct Texture {
		float x, y;
	};
	struct Normal {
		float x, y, z;
	};
	struct Face {
		vector<int> vi, ti, ni;
		int count;
	};

public:
	int mode = 0;
	int length = 0;
	float angle = 0.0f;
	vector<Vertex> vertices;
	vector<Texture> textures;
	vector<Normal> normals;
	vector<Face> faces;
	void drawVertex();
	void drawLine();
	void drawFace();
	void readobj(const char* filename);
};
ReadOBJ robj;
void ReadOBJ::readobj(const char* filename) {
	fstream fs(filename);
	string line;
	if (fs.is_open()) {
		while (getline(fs, line)) {
			istringstream s(line);
			string ty;
			Vertex vertex;
			Texture texture;
			Normal normal;
			Face face;
			s >> ty;
			if (ty == "v") {
				s >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex);
			}
			else if (ty == "vt") {
				s >> texture.x >> texture.y;
				textures.push_back(texture);
			}
			else if (ty == "vn") {
				s >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (ty == "f") {
				Face face;
				string temp;
				char slash;
				int data[3];
				int i = 0;

				while (getline(s, temp, ' ')) {
					if (temp == "" && i == 0)
						continue;
					if (temp == "" && i != 0)
						break;
					istringstream w(temp);
					w >> data[0] >> slash >> data[1] >> slash >> data[2];
					face.vi.push_back(data[0]);
					face.ti.push_back(data[1]);
					face.ni.push_back(data[2]);
					--face.vi[i];
					--face.ti[i];
					--face.ni[i];

					i++;
				}
				face.count = i;
				faces.push_back(face);
			}


		}
	}
	fs.close();
}

void ReadOBJ::drawVertex() {
	glBegin(GL_POINTS);
	for (const auto& vertex : robj.vertices) {
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}
void ReadOBJ::drawLine() {
	glBegin(GL_LINES);
	for (const auto& face : faces) {
		for (int i = 0; i < face.count; i++) {
			if (i != face.count - 1) {
				glVertex3f(vertices[face.vi[i]].x, vertices[face.vi[i]].y, vertices[face.vi[i]].z);
				glVertex3f(vertices[face.vi[i + 1]].x, vertices[face.vi[i + 1]].y, vertices[face.vi[i + 1]].z);
			}
			else {
				glVertex3f(vertices[face.vi[i]].x, vertices[face.vi[i]].y, vertices[face.vi[i]].z);
				glVertex3f(vertices[face.vi[0]].x, vertices[face.vi[0]].y, vertices[face.vi[0]].z);
			}
		}
	}
	glEnd();
}
void ReadOBJ::drawFace() {
	
	for (const auto& face : faces) {
		glBegin(GL_POLYGON);
		for (int i = 0; i < face.count; i++) {
			glVertex3f(vertices[face.vi[i]].x, vertices[face.vi[i]].y, vertices[face.vi[i]].z);
		}
		glEnd();
	}
}
void idle() {
	robj.angle += 0.01;
	if (robj.angle > 360.0)
		robj.angle = 0.0;
	glutPostRedisplay();
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glPointSize(2);
	gluLookAt(0, 5, 5, 0, 0, 0, 0, 1, 0);
	glRotatef(robj.angle, 0.0f, 1.0f, 0.0f);

	if (robj.mode == 0)
		robj.drawVertex();
	else if (robj.mode == 1)
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
		robj.mode++;
		if (robj.mode >= 3)
			robj.mode = 0;
		break;
	default:
		std::cout << "Key pressed: " << key << std::endl;
		break;
	}
}

int main(int argc, char** argv) {

	robj.readobj("Cube.obj");
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