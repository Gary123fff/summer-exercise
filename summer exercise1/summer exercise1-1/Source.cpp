#include<stdio.h>
#include<iostream>
using namespace std;
template <class T>
void swap1(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;

}

int main() {
	int a1 = 3, a2 = 4;
	swap1(a1, a2);
	cout << "a1=" << a1 <<"\t"<< " a1 type:" << typeid(a1).name() << "\t" << ", a2=" << a2 << "\t\t" << " a2 type: " << typeid(a2).name() << endl;

	float b1 = 3.5f, b2 = 4.5f;
	swap1(b1, b2);
	cout << "b1=" << b1 << "\t" << " b1 type:" << typeid(b1).name() << "\t" << ", b2=" << b2 << "\t" << " b2 type: " << typeid(b2).name() << endl;

	double c1 = 3.6, c2 = 4.6;
	swap1(c1, c2);
	cout << "c1=" << c1 << "\t" << " c1 type:" << typeid(c1).name() << "\t" << ", c2=" << c2 << "\t" << " c2 type: " << typeid(c2).name() << endl;

	char d1 = 'a', d2 = 'b';
	swap1(d1, d2);
	cout << "d1=" << d1 << "\t" << " d1 type:" << typeid(d2).name() << "\t" << ", d2=" << d2 << "\t\t" << " d2 type: " << typeid(d2).name() << endl;
}

