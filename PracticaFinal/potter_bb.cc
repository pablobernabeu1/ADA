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
#include<queue>
#include<tuple>
using namespace std;

/////////////////////////////////////////
int discarded_promissing_nodes=0; //*
int visited_leaf_nodes=0; //*
int current_best_updated_from_completed_nodes=0; //*
int visited_nodes=0; //*
int current_best_updated_pessimistic_bounds=0; //*
int explored_nodes=0; //*
int no_promissing_discarded_nodes=0; //*
int no_feasible_discarded_nodes=0; //*
/////////////////////////////////////////

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
double potter_bt_optimo_d(const vector<double> &v, const vector<double> &w, vector<int> &m, size_t k,  double W){
  double acc_v=0.0;

  for(unsigned i=k; i<w.size(); i++){
    for(int j=0; j<m[i]; j++){
      if(w[i]<=W){
        acc_v += v[i];
        W -= w[i];
      }
      if(W==0){
        return acc_v;
      }
    }
  }

  return acc_v;
}

double potter_bt_optimo_c(const vector<double> &v, const vector<double> &w, vector<int> &m, size_t k, double W){
    double acc_v = 0;

    for(size_t i = k; i < v.size(); i++){
      for(size_t j=0; j<m[i]; j++){
        if(w[i]>W){
          acc_v += W/w[i] * v[i];
          return acc_v;
        }
        else{
          W -= w[i];
          acc_v += v[i];
        }
      }
    }

    return acc_v;
}


double potter_bb(const vector<double> &v, const vector<double> &w, vector<int> &m, double W){
  
  vector<size_t> idx(v.size());
  iota(begin(idx), end(idx), 0);

  sort(begin(idx), end(idx),
    [&v, &w](size_t i, size_t j)
    {
      return v[i]/w[i] > v[j]/w[j];
    }
  );
  
  vector<double> s_v(v.size()), s_w(w.size());
  vector<int> s_m(m.size());
  for(size_t i=0; i<v.size(); i++){
    s_v[i] = v[idx[i]];
    s_w[i] = w[idx[i]];
    s_m[i] = m[idx[i]];
  }
  
  typedef vector<short> Sol;
  typedef tuple < double, double, double, Sol, unsigned > Node;
  priority_queue<Node> pq;

  double value=0, weight = 0, ignore=0;
  Sol x(v.size());
  size_t k=0;
  Node n;
  
  double best_val = potter_bt_optimo_d(s_v, s_w, s_m,  0, W);
  double opt_bound = potter_bt_optimo_c(s_v, s_w, s_m, 0, W);
  
  pq.emplace(opt_bound, 0.0, 0.0, Sol(v.size()), 0);
  
  while(!pq.empty()) {

    n = pq.top();
    value = get<1>(n);
    weight = get<2>(n);
    x = get<3>(n);
    k = get<4>(n);
    pq.pop();

    /*
    if(ignore>best_val){
      discarded_promissing_nodes++;
    }
    */

    if(k == v.size()) {
      visited_leaf_nodes++;
      if(value>best_val){
        current_best_updated_from_completed_nodes++;
        best_val = value;
      }
      continue;
    }

    for(unsigned j=0; j<=s_m[k]; j++){
      x[k] = j;

      visited_nodes++;

      double new_weigth = weight + x[k]*s_w[k];
      double new_value = value + x[k]*s_v[k];

      if(new_weigth <= W){
        double pes_bound = new_value + potter_bt_optimo_d(s_v, s_w, s_m, k+1, W-new_weigth);
        best_val = max(best_val, pes_bound);

        double opt_bound = new_value + potter_bt_optimo_c(s_v, s_w, s_m, k+1, W-new_weigth);
        if(opt_bound>best_val){
          explored_nodes++;
          pq.emplace(opt_bound, new_value, new_weigth, x, k+1);
        }
        else{
          no_promissing_discarded_nodes++;
        }
      }
      else{
        no_feasible_discarded_nodes++;
      }
    }
  }
  

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
        auto start = clock();
        cout<<potter_bb(v, t, m, T)<<endl;
        auto end = clock();

        cout<<visited_nodes<<" "<<explored_nodes<<" "<<visited_leaf_nodes<<" "<<no_feasible_discarded_nodes<<" "
        <<no_promissing_discarded_nodes<<" "<<discarded_promissing_nodes<<" "<<current_best_updated_from_completed_nodes
        <<" "<<current_best_updated_pessimistic_bounds<<endl;

        cout<<(1000.0*(end-start)/CLOCKS_PER_SEC)<<endl;

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
