// Pablo Antonio Bernabeu Cartagena 20083111V
#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<stdlib.h>
#include<limits>
#include<algorithm>
#include<numeric>
#include<cmath>
using namespace std;

/////////////////////////////////////////
int visited_lead_nodes=0;
int visited_nodes=0;
int explored_nodes=0;
int no_promising_discarded_nodes=0;
int no_feasible_discarded_nodes=0;
/////////////////////////////////////////

// Función que convierte un string a numero, en la lectura de los archivos
int convertir_a_numero(char cadena){
  int s = cadena-48;
  return s;
}

// Función para leer los archivos y almacenar la información en sus variables
void leerFichero(ifstream &fichero, string &file, int &n, double &T, vector<double> &t, vector<double> &v, vector<int> &m){

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double potter_bt_optimo_d(const vector<double> &v, const vector<double> &w, vector<int> &m, vector<short> &x, double W){
  vector<size_t> idx(v.size());
  for(size_t i=0; i<idx.size(); i++) idx[i]=i;

  sort(idx.begin(), idx.end(),
    [v,w](size_t x, size_t y){
      return v[x]/w[x] > v[y]/w[y];
    }
  );

  double acc_v = 0.0;

  for(unsigned i=0; i<idx.size() && W>0; i++){
      double aux = m[idx[i]];
      x[idx[i]] = min(W/w[idx[i]],aux);
      W -= w[idx[i]]*x[idx[i]];
      acc_v += x[idx[i]]*v[idx[i]];
  }

  return acc_v;
}

double potter_bt_optimo_c(const vector<double> &v, const vector<double> &w, vector<int> &m, size_t k, double W){
    double acc_v = 0;

    for(unsigned i = k; i < v.size() && W > 0; i++){
      if(W < w[i] * m[i]){
        acc_v += (v[i] * (W/w[i]));
        W = 0;
      }
      else{
        W = W - (w[i] * m[i]);
        acc_v += v[i] * m[i];
      }
    }

    return acc_v;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
        vector<double> v; // vector de valores
        vector<int> m; // vector de veces que se repite un elemento

        ifstream fichero(file);
        leerFichero(fichero, file, n, T, t, v, m);

        // Implementación del nuevo algoritmo.

        

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
