// Pablo Antonio Bernabeu Cartagena 20083111-V

/*
ADA. 2020-21
Pràctica 1 de laboratori: "Empirical analysis of Quicksort average-time complexity."

Objectiu de la pràctica:

- Refrescar el llenguatge de programació i l'ús de les eines: compilador, make, editor, etc.
- Saber obtenir de manera empírica la complexitat temporal d'un algorisme, generant instàncies aleatòries i cronometrant el temps que tarda a resoldre-les.
- Saber mesurar temps de procés utilitzant la llibreria "chrono" de l'estàndard C++11.
- Conèixer el tipus de dades "size_t" per a representar la grandària dels objectes, útil per a evitar desbordaments dels tipus compostos; 
- Conèixer l'eina Gnuplot per a dibuixar funcions matemàtiques i el seu comando "fit" per a trobar la funció que millor s'ajusta a una sèrie de dades.
*/

/*
ADA. 2020-21
Práctica 1 de laboratorio: "Empirical analysis of Quicksort average-time 
    complexity."

Objetivo de la práctica:

- Refrescar el lenguaje de programación y el uso de las herramientas:
  compilador, make, editor, etc.

- Saber obtener de manera empírica la complejidad tenporal de un algoritmo,
  generando instancias aleatorias y cronometrando el tiempo que tarda en
  resolverlas.

- Saber medir el tiempo de proceso utilizando la libreria "chrono" del
  estandar C++11.

- Conocer el tipo de datos "size_t" para representar el tamaño de los 
  objetos.

- Conocer la herramienta Gnuplot para dibujar funciones matemáticas y
  su comando "fit" para encontrar la función que mejor se ajusta a  una
  serie de datos.
*/

#include <unistd.h>
#include <iostream>
#include <math.h>
#include <chrono>

using namespace std;
using namespace chrono;

//--------------------------------------------------------------
// Bubble sort basic algorithm
void 
middle_QuickSort(int * v, long left, long right){

    long i,j;
    int pivot,aux; 
    if (left<right)
    {
        i=left; j=right;
        pivot=v[(i+j)/2];
        do
        {
            while (v[i]<pivot) i++;
            while (v[j]>pivot) j--;
            if (i<=j)
            {
                aux=v[i]; v[i]=v[j]; v[j]=aux;
                i++; j--;
            }
       } while (i<=j);
       if (left<j)  middle_QuickSort(v,left,j);
       if (i<right) middle_QuickSort(v,i,right);
    }
}
//--------------------------------------------------------------                


int
main(void){

//    srand(getpid());
    srand(0);
    double tiempo=0;

    cout << "# QuickSort CPU times in milliseconds:"
        << endl
        << "# Size \t CPU time (ms.)"
        << endl
        << "# ----------------------------"
        << endl;

    for (int exp = 15; exp <= 22; exp++){

        tiempo=0;
        size_t size = size_t( pow(2,exp) );
        int* v = new int [size];
        if (!v){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        cout << size << "\t\t" << std::flush;

        for(int j=0; j<30; j++){

            for(size_t i=0;i<size;i++){
                v[i] = rand();
            }
            
            auto start = clock();
            middle_QuickSort(v, 0, size-1);
            auto end = clock();

            tiempo = tiempo + (1000.0 * (end-start) / CLOCKS_PER_SEC);

        }
 
        cout<<(tiempo/30)<<endl;

        for (size_t i = 1; i < size; i++)
            if (v[i] < v[i-1]){ 
                cerr << "Panic, array not sorted! " << i << endl; 
                exit(EXIT_FAILURE);            
            }

        delete[] v; 
    }
}