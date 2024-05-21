#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class MyFirstClass {
private :
	string line;
public:
	void readFile(const char* filename);
	void printf() const;
};
void MyFirstClass::readFile(const char* filename) {
	fstream fs(filename);
	if (fs.is_open()) {
		while (getline(fs, line)) {
			printf();
		}
	}
	fs.close();
}
void MyFirstClass::printf() const{
	cout << line << endl;
}
int main() {
	MyFirstClass mfc;
	mfc.readFile("test.txt");
}