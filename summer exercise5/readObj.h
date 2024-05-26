
#include<vector>
# include "FaceandVertex.h"
using namespace std;
class ReadOBJ {
private:
	struct Vertex {
		float x, y, z;
		float s = -1, t = -1;
		float m_vnorm[3] = { 0,0,0 };
	};
	struct Texture {
		float x, y;
	};
	struct Normal {
		float x, y, z;
	};
	struct Face {
		vector<int> vi, ti, ni;
		float m_fnorm[3] = { 0,0,0 };
		int count;
	};

public:
	int m_mode = 0;
	int m_reshape = 0;
	int m_rendermode = 0;
	int m_length = 0;
	int m_textures_mode = 0;
	float m_a = 0;
	float m_t = 0.001;
	float m_angle = 0.0f;
	vector<Vertex> m_vertices;
	vector<Vertex> m_tempvertices;
	vector<Texture> m_textures;
	vector<Normal> m_normals;
	vector<Face> m_faces;
	vector<FaceCollect>m_FaceCollect;
	vector<VertexCollect>m_VertexCollect;
	void drawVertex();
	void drawLine();
	void drawFace();
	void readobj(const char* filename);
	void computVertexNorm();
	void comFaceNorm(FaceCollect* fc, int facecount);
	void saveImage(const char* filename, int width, int height, int x, int y);

};
#pragma once
