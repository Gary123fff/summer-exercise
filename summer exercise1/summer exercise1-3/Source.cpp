#include<stdio.h>
#include<iostream>
#include<cmath>
using namespace std;
float computeDistance(float* p0, float* p1) {
	float d1=p0[0] - p1[0];
	float d2 = p0[1] - p1[1];
	return sqrt(d1 * d1 + d2 * d2);
}
int main() {
	float p0[] = { 1, 3 };
	float p1[] = { 6, 15 };

	float d = computeDistance(p0, p1); // now the value of d is 5
	cout << "Distance:"<< d << endl;

}