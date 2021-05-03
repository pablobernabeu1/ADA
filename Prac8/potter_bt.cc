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
double weight(const vector<double> &w, const vector<unsigned> &x){
  double acc_w = 0.0;
  for(size_t i=0; i<w.size(); i++) acc_w += x[i]*w[i];
  return acc_w;
}
 
double value(const vector<double> &v, const vector<unsigned> &x){
  double r=0.0;
  for(size_t i=0; i<v.size(); i++) r+=v[i]*x[i];

  return r;
}

void potter_bt_optimo(const vector<double> &v, const vector<double> &w, double W, size_t k, vector<unsigned> &x, double &best_v){
  if(k==x.size()){
    if(weight(w, x)<=W)
      best_v = max(best_v, value(v,x));
    return;
  }

  for(unsigned j=0; j<2; j++){
    x[k]=j;
    potter_bt_optimo(v,w,W,k+1,x,best_v);
  }
}

double potter_bt_optimo(const vector<double> &v, const vector<double> &w, double W, vector<unsigned> &x){

  double best_v = numeric_limits<double>::lowest();

  potter_bt_optimo(v,w,W,0,x,best_v);

  return best_v;
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

        double T2 = T;
        vector<unsigned> x(tiemposMejor.size());

        cout<<potter_bt_optimo(vMejor, tiemposMejor, T, x)<<endl;

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
