#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
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
				char slash;
				string temp;
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
int main() {
	ReadOBJ robj;
	robj.readobj("Cube.obj");
	for (const auto& vertex : robj.vertices) {
		cout <<"v : "<< vertex.x << " " << vertex.y << " " << vertex.z<<endl;
	}
	for (const auto& texture : robj.textures) {
		cout << "vt : " << texture.x << " " << texture.y <<endl;
	}
	for (const auto& normal : robj.normals) {
		cout << "vn : " << normal.x << " " << normal.y << " " << normal.z << endl;
	}
	for (const auto& face : robj.faces) {
		for (int i = 0; i < 3; ++i) {
			cout<< "f : " << face.vi[i] << "/" << face.ti[i]  << "/" << face.ni[i]  << " ";
		}
		cout << endl;
	}
}