#include <GL/glut.h> 
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include "readObj.h"
#include <conio.h>
#include <png.h>
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
				m_vertices.push_back(vertex);
			}
			else if (ty == "vt") {
				s >> texture.x >> texture.y;
				m_textures.push_back(texture);
			}
			else if (ty == "vn") {
				s >> normal.x >> normal.y >> normal.z;
				m_normals.push_back(normal);
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
				m_faces.push_back(face);
			}


		}
	}
	fs.close();
}

void ReadOBJ::drawVertex() {
	glBegin(GL_POINTS);
	for (const auto& vertex : m_vertices) {
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}
void ReadOBJ::computVertexNorm() {
	int tempvertexcount = 0;
	for (const auto& vertex : m_vertices) {

		int vertexnumber = 0;
		for (auto fc : m_FaceCollect) {
			VertexCollect* temp = fc.m_nextvertex;
			while (temp) {
				if (temp->m_id == (tempvertexcount)) {
					m_vertices[tempvertexcount].m_vnorm[0] += m_faces[fc.m_id].m_fnorm[0];
					m_vertices[tempvertexcount].m_vnorm[1] += m_faces[fc.m_id].m_fnorm[1];
					m_vertices[tempvertexcount].m_vnorm[2] += m_faces[fc.m_id].m_fnorm[2];
					vertexnumber++;
					break;
				}

				temp = temp->m_nvertex;
			}
		}
		m_vertices[tempvertexcount].m_vnorm[0] /= vertexnumber;
		m_vertices[tempvertexcount].m_vnorm[1] /= vertexnumber;
		m_vertices[tempvertexcount].m_vnorm[2] /= vertexnumber;
		tempvertexcount++;
	}
}
void ReadOBJ::comFaceNorm(FaceCollect* fc, int facecount) {
	VertexCollect* v1 = fc->m_nextvertex;
	VertexCollect* v2 = fc->m_nextvertex->m_nvertex;
	VertexCollect* v3 = fc->m_nextvertex->m_nvertex->m_nvertex;
	float x1 = v1->x - v2->x, y1 = v1->y - v2->y, z1 = v1->z - v2->z;
	float x2 = v3->x - v2->x, y2 = v3->y - v2->y, z2 = v3->z - v2->z;

	float crossx = y1 * z2 - z1 * y2, crossy = x2 * z1 - x1 * z2, crossz = x1 * y2 - y1 * x2;
	float val = sqrt(crossx * crossx + crossy * crossy + crossz * crossz);
	float norm[3];
	norm[0] = crossx / val;
	norm[1] = crossy / val;
	norm[2] = crossz / val;
	if (norm[0] == -0)
		norm[0] = 0;
	if (norm[1] == -0)
		norm[1] = 0;
	if (norm[2] == -0)
		norm[2] = 0;
	m_faces[facecount].m_fnorm[0] = norm[0];
	m_faces[facecount].m_fnorm[1] = norm[1];
	m_faces[facecount].m_fnorm[2] = norm[2];
}

void ReadOBJ::drawLine() {
	glBegin(GL_LINES);
	for (const auto& face : m_faces) {
		for (int i = 0; i < face.count; i++) {
			if (i != face.count - 1) {
				glVertex3f(m_vertices[face.vi[i]].x, m_vertices[face.vi[i]].y, m_vertices[face.vi[i]].z);
				glVertex3f(m_vertices[face.vi[i + 1]].x, m_vertices[face.vi[i + 1]].y, m_vertices[face.vi[i + 1]].z);
			}
			else {
				glVertex3f(m_vertices[face.vi[i]].x, m_vertices[face.vi[i]].y, m_vertices[face.vi[i]].z);
				glVertex3f(m_vertices[face.vi[0]].x, m_vertices[face.vi[0]].y, m_vertices[face.vi[0]].z);
			}
		}
	}
	glEnd();
}//wireframe
void ReadOBJ::drawFace() {

	for (const auto& face : m_faces) {
		glBegin(GL_POLYGON);
		for (int i = 0; i < face.count; i++) {
			glNormal3f(m_vertices[face.vi[i]].m_vnorm[0], m_vertices[face.vi[i]].m_vnorm[1], m_vertices[face.vi[i]].m_vnorm[2]);
			glVertex3f(m_vertices[face.vi[i]].x, m_vertices[face.vi[i]].y, m_vertices[face.vi[i]].z);
		}
		glEnd();
	}
}

void  ReadOBJ::saveImage(const char* filename, int width, int height, int x, int y) {
	unsigned char* data = new unsigned char[width * height * 3];

	glReadPixels(0, 0, 1000, 1000, GL_RGB, GL_UNSIGNED_BYTE, data);
	FILE* fp;
	fopen_s(&fp, filename, "wb");

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	png_infop info_ptr = png_create_info_struct(png_ptr);


	png_init_io(png_ptr, fp);

	png_set_IHDR(png_ptr, info_ptr, width, height,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);
	//因為png 跟 OPENGL的x座標上下相反
	for (int y = height-1; y >= 0; y--) {
		png_write_row(png_ptr, &data[y * width * 3]);
	}

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(fp);

}//存圖function

