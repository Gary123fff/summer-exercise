#pragma once
#include<iostream>
using namespace std;
struct VertexCollect {
public:
	VertexCollect(int id) : m_id(id), m_nvertex(nullptr), m_pvertex(nullptr) {}
	float x, y, z;
	int m_id;

	VertexCollect* m_nvertex;
	VertexCollect* m_pvertex;

};
struct FaceCollect {
public:
	FaceCollect(int id) : m_id(id), m_nextvertex(nullptr), m_prevertex(NULL), m_tempvertex(NULL) {}
	void adjVertex(VertexCollect* vertex) {
		vertex->m_nvertex = m_nextvertex;//forward
		m_nextvertex = vertex;

		vertex->m_pvertex = nullptr;//backward
		if (m_tempvertex == nullptr) {
			m_prevertex = vertex;
			m_tempvertex = vertex;
		}
		else {
			m_tempvertex->m_pvertex = vertex;
			m_tempvertex = vertex;
		}
	}
	void showId() {
		cout << "Face ID : " << m_id + 1 << endl;
		cout << "forward : ";
		VertexCollect* temp = m_nextvertex;
		do {
			cout << temp->m_id + 1 << " ";
		} while (temp = temp->m_nvertex);

		cout << endl;

		temp = m_prevertex;
		cout << "backward : ";
		do {
			cout << temp->m_id + 1 << " ";
		} while (temp = temp->m_pvertex);
		cout << endl;
	}

	int m_id;
	VertexCollect* m_nextvertex;
	VertexCollect* m_prevertex;
	VertexCollect* m_tempvertex;

};


