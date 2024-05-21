#include<stdio.h>
#include<iostream>
#include<cmath>
using namespace std;
float compute3D_Distance(float* p0, float* p1) {
	float d1 = p0[0] - p1[0];
	float d2 = p0[1] - p1[1];
	float d3 = p0[2] - p1[2];
	return sqrt(d1 * d1 + d2 * d2 + d3 * d3);
}
int main() {
	float p0[] = { 1, 1 ,0};
	float p1[] = { 2, 1 ,2};

	float d = compute3D_Distance(p0, p1); // now the value of d is 5
	cout << "Distance:" << d << endl;

}