#include<iostream>
using namespace std;


int pasos1=0;
unsigned long pow2_1(unsigned long numero){

  pasos1++;
  if(numero == 1){
		return 1;
  }

	return (2 * pow2_1(numero-1));
}


int pasos2=0;
unsigned long pow2_2(unsigned long numero){

  pasos2++;
  if(numero == 1){
		return 1;
  }

  if(numero%2 == 0)
		return (pow2_2(numero/2)) * (pow2_2(numero/2));
 	return 2 * (pow2_2(numero/2)) * (pow2_2(numero/2));
}


int pasos3=0;
unsigned long pow2_3(unsigned long numero){

  pasos3++;
  if(numero == 1){
		return 1;
  }

  return pow2_3(numero-1) + pow2_3(numero-1);
}


int main(){

  for(unsigned exp = 1; exp< 32; exp++){
		pasos1=0;
    pasos2=0;
    pasos3=0;

		pow2_1(exp);
		cout << pasos1 << "\t";
		pow2_2(exp);
		cout << pasos2 << "\t";
		pow2_3(exp);
		cout << pasos3 << endl;
	}


  return 0;
}
