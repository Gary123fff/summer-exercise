#include<stdio.h>
#include<iostream>
using namespace std;
void out_message(const char* msg) {
	cout <<"Incoming message:"<< msg << endl;
}
int main() {
	out_message("You are very good!");
}