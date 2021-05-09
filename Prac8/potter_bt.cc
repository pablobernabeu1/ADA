// Pablo Antonio Bernabeu Cartagena 20083111V

#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<stdlib.h>
#include<limits>
#include<algorithm>
#include<numeric>
using namespace std;

const int SENTINEL = -1;


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


// ---------------------------------------------------------------------------------------------------------------------------------
double potter_bt_optimo_c(const vector<double> &v, const vector<double> &w, size_t k, double W){
  vector<unsigned> idx(w.size());
  for(unsigned i=0; i<idx.size(); i++) idx[i]=i;

  sort(idx.begin(), idx.end(),
    [v,w](unsigned x, unsigned y){
      return v[x]/w[x] > v[y]/w[y];
    }
  );

  double acc_v = 0.0;

  for(unsigned i=k; i<v.size(); i++){
    if(w[idx[i]]>W) {
      acc_v += W/w[idx[i]]*v[idx[i]];
      break;
    }
    acc_v += v[idx[i]];
    W -= w[idx[i]];
  }

  return acc_v;
}

double potter_bt_optimo_d(const vector<double> &v, const vector<double> &w, double W){
  vector<size_t> idx(w.size());
  for(size_t i=0; i<idx.size(); i++) idx[i]=i;

  sort(idx.begin(), idx.end(),
    [&v,&w](size_t x, size_t y){
      return v[x]/w[x] > v[y]/w[y];
    }
  );

  double acc_v = 0.0;

  for(auto i : idx){
    if(w[i]<W){
      acc_v += v[i];
      W-=w[i];
    }
  }

  return acc_v;
}

void potter_bt_optimo(const vector<double> &v, const vector<double> &w, double W, size_t k, vector<short> &x, double weigth, double value, double &best_val){
  if(k==x.size()){
    //cout<<"Hola"<<endl;
    best_val = value;
    return;
  }

  for(int j=1; j>=0; j--){
    x[k] = j;
    double new_weigth = weigth + x[k]*w[k];
    double new_value = value + x[k]*v[k];
    
    if(new_weigth<=W && new_value + potter_bt_optimo_c(v,w,k+1, W-new_weigth)>best_val){
      //cout<<"Hola"<<endl;
      potter_bt_optimo(v,w,W,k+1,x,new_weigth, new_value, best_val);
    }
  }
}

double potter_bt_optimo(const vector<double> &v, const vector<double> &w, double W){

  vector<size_t> idx(v.size());
  iota(begin(idx), end(idx), 0);

  sort(begin(idx), end(idx),
    [&v, &w](size_t i, size_t j) {
      return v[i]/w[i]>v[j]/w[j];
    }
  );

  vector<double> s_v(v.size()), s_w(w.size());

  for(size_t i=0; i<v.size(); i++){
    s_v[i] = v[idx[i]];
    s_w[i] = w[idx[i]];
  }

  vector<short> x(v.size());
  double best_val = potter_bt_optimo_d(s_v,s_w,W);

  potter_bt_optimo(s_v,s_w,W, 0, x, 0, 0, best_val);

  return best_val;
}
// ---------------------------------------------------------------------------------------------------------------------------------


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
        vector<double> v; // vector de valores
        vector<int> m; // vector de veces que se repite un elemento

        vector<double> vMejor;  // vector de valores alargado con m
        vector<double> tiemposMejor; // vector de tiempos alargado con m

        ifstream fichero(file);
        leerFichero(fichero, file, n, T, t, v, m);

        for(long unsigned int j=0; j<m.size(); j++){  // Cambiamos las matrices
          for(int k=0; k<m[j]; k++){
            vMejor.push_back(v[j]);
            tiemposMejor.push_back(t[j]);
          }
        }


        // Implementación del nuevo algoritmo.

        cout<<potter_bt_optimo(vMejor, tiemposMejor, T)<<endl;
        
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
