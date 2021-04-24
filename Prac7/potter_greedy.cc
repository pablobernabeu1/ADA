// Pablo Antonio Bernabeu Cartagena 20083111V

#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<stdlib.h>
#include<limits>
#include<algorithm>
using namespace std;

const int SENTINEL = -1;

// Función que convierte un string a numero, en la lectura de los archivos
int convertir_a_numero(char cadena){
  int s = cadena-48;
  return s;
}

// Función para leer los archivos y almacenar la información en sus variables
void leerFichero(ifstream &fichero, string &file, int &n, double &T, vector<double> &t, vector<int> &v, vector<int> &m){

  if(fichero.is_open()){
    double aux;

    if(fichero>>n){
      fichero>>T;

      for(int i=0; i<n;i++){
        fichero>>aux;
        t.push_back(aux);
      }

      for(int i=0; i<n;i++){
        fichero>>aux;
        v.push_back(aux);
      }

      for(int i=0; i<n;i++){
        fichero>>aux;
        m.push_back(aux);
      }
    }

    fichero.close();

  }
  else{
    cerr<<"ERROR: cant´t open file: "<<file<<"."<<endl;
    exit(0);
  }

}


double potter_greedy(const vector<int> &v, const vector<double> &w, double &W, vector<double> &x){
  vector<unsigned> idx(w.size());
  for(unsigned i=0; i<idx.size(); i++) idx[i]=i;

  sort(idx.begin(), idx.end(),
    [v,w](unsigned x, unsigned y){
      return (double)v[x]/w[x] > (double)v[y]/w[y];
    }
  );

  double acc_v = 0.0;

  for(unsigned i=0; i<idx.size(); i++){
    if(w[idx[i]]<W){
      acc_v += (double)v[idx[i]];
      W -= w[idx[i]];
      x[idx[i]] = 1.0;
    }
  }

  return acc_v;
}


// Función principal
int main(int argc, char *argv[]){

  string file="";

  // Gestión de argumentos
  for(int i=1; i<argc; i++){
    if(strcmp(argv[i], "-f")==0){
      if(i<argc-1){
        i++;
        file=argv[i];

        // Si se encuentra la opcion '-f' se procede a leer el fichero.

        int n=0;  double T=0;
        vector<double> t; // vector de tiempos
        vector<int> v; // vector de valores
        vector<int> m; // vector de veces que se repite un elemento

        vector<int> vMejor;  // vector de valores alargado con m
        vector<double> tiemposMejor; // vector de tiempos alargado con m

        ifstream fichero(file);
        leerFichero(fichero, file, n, T, t, v, m);

        for(long unsigned int j=0; j<m.size(); j++){  // Cambiamos la matriz
          for(int k=0; k<m[j]; k++){
            vMejor.push_back(v[j]);
            tiemposMejor.push_back(t[j]);
          }
        }

        double T2 = T;
        vector<double> x(tiemposMejor.size(), 0);

        cout<<potter_greedy(vMejor, tiemposMejor, T, x)<<endl;

        int cont1=0;
        int cont2=0;
        for(int j=0;j<n;j++){
            for(int k=0;k<m[j];k++){
                if(x[cont2+k]==1){
                    cont1++;
                }
            }
            cont2=cont2+m[j];
            cout<<cont1;
            if(j+1<n) cout<<" ";

            cont1=0;
        }

        cout<<endl;

        cout<<(T2-T)<<endl;

        exit(1);
      }
      else{
        cerr<<"ERROR: missing filename."<<endl;
        exit(0);
      }
    }
    else{
      cerr<<"ERROR: unknown option "<< argv[i] <<"."<<endl;
      cerr<<"Usage: "<<endl;
      cerr<<"potter [-t] [--ignore-naive] -f file"<<endl;
      exit(0);
    }
  }

  return 0;
}
