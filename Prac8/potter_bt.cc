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
        //cout << acc_v << endl;
        W = 0;
      }
      else{
        W = W - (w[i] * m[i]);
        acc_v += v[i] * m[i];
        //cout << acc_v << endl;
      }
    }

    return acc_v;
}



void potter_bt_optimo(const vector<double> &v, const vector<double> &w, vector<int> &m, double W, size_t k, vector<short> &x, double weigth, double value, double &best_val){
  if((unsigned)k == x.size()){ // base case
		if(value > best_val){
			best_val = value;
		}
		return;
	}

	for(int j = m[k]; j >= 0; j--){ // <== reversing the order
		x[k] = j;
		double new_time = weigth + x[k] * w[k]; 	// updating time
		long new_value = value + x[k] * v[k]; // updating value

		if(new_time <= W){					// is promising
			if(new_value + potter_bt_optimo_c(v, w, m, k+1, W - new_time) > best_val){ // simplified version
				potter_bt_optimo(v, w, m, W, k+1, x, new_time, new_value, best_val);
			}
			else{

			}
		}
		else{

		}
	}
}

double potter_bt_optimo(const vector<double> &v, const vector<double> &w, double W, vector<int> &m){

    vector<size_t> idx(v.size());
    for(unsigned i=0; i<idx.size(); i++) idx[i] = i;

    sort(idx.begin(), idx.end(),
        [v,w](size_t x, size_t y){
            return v[x]/w[x] > v[y]/w[y];
        }
    );

    vector<double> s_v(v.size()),s_w(w.size());
    vector<int> s_m(m.size());
    for(size_t i=0; i<v.size(); i++){
        s_v[i] = v[idx[i]];
        s_w[i] = w[idx[i]];
        s_m[i] = m[idx[i]];
    }
  
    vector<short> x(v.size());
    double best_val = potter_bt_optimo_d(v,w,m,x,W);
    
    potter_bt_optimo(s_v,s_w,s_m,W, 0, x, 0, 0, best_val);

    return best_val;
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

        cout<<potter_bt_optimo(v, t, T, m)<<endl;
        
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
