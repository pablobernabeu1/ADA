// Pablo Antonio Bernabeu Cartagena 20083111V

#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<stdlib.h>
#include<limits>
using namespace std;

const int SENTINEL = -1;

// Función que convierte un string a numero, en la lectura de los archivos
int convertir_a_numero(char cadena){
  int s = cadena-48;
  return s;
}

// Función para leer los archivos y almacenar la información en sus variables
void leerFichero(ifstream &fichero, string &file, int &n, int &T, vector<int> &t, vector<int> &v, vector<int> &m){

  if(fichero.is_open()){
    string cadena="";
    int i=0;

    fichero >> n;
    fichero >> T;

    while(getline(fichero, cadena)){

      if(i==0){

      }
      if(i==1){
        for(long unsigned int j=0; j<cadena.size(); j = j + 2){
          char c = cadena[j];
          t.push_back(convertir_a_numero(c));
        }
      }

      if(i==2){
        for(long unsigned int j=0; j<cadena.size(); j = j + 2){
          char c = cadena[j];
          v.push_back(convertir_a_numero(c));
        }
      }
      if(i==3){
        for(long unsigned int j=0; j<cadena.size(); j = j + 2){
          char c = cadena[j];
          m.push_back(convertir_a_numero(c));
        }
      }

      i++;
    }

    fichero.close();

  }
  else{
    cerr<<"ERROR: cant´t open file: "<<file<<"."<<endl;
    exit(0);
  }

}




// Recursivo sin almacén (versión ingenua)
int recursivoSinAlmacen(int n, int T, vector<int> &t, vector<int> &v, int &tiempoTotal){

  if(n==0 || T==0){
    return 0;
  }

  double s1 = recursivoSinAlmacen(n-1, T, t, v, tiempoTotal);
  double s2 = 0;


  if(t[n-1] <= T){
    s2 = v[n-1] + recursivoSinAlmacen(n-1, T-t[n-1], t, v, tiempoTotal);
    tiempoTotal = t[n-1] + recursivoSinAlmacen(n-1, T-t[n-1], t, v, tiempoTotal);
  }

  return max(s1, s2);
}

// Recursivo con almacén (memoización), segunda función
int recursivoConAlmacen2(vector<vector<double>> &M, vector<int> &v, vector<int> &t, int n, int T, vector<int> &copiasDeCadaObjeto){
  if(M[n][T]!=SENTINEL) return M[n][T];

  if(n==0) return M[n][T] = 0.0;

  double s1 = recursivoConAlmacen2(M, v, t, n-1, T, copiasDeCadaObjeto);
  double s2 = 0;

  if(t[n-1] <= T){
    s2 = v[n-1] + recursivoConAlmacen2(M, v, t, n-1, T-t[n-1], copiasDeCadaObjeto);
  }

  return M[n][T] = max(s1, s2);

}


// Iterativo con almacén que hace uso de una tabla para almacenar los resultados intermedios
int iterativoConAlmacenYTabla(vector<int> &v, vector<int> &w, int last_n, int last_W, vector<vector<int>> &M){

  for(int W=0; W<=last_W; W++) M[0][W] = 0;
  for(int n=0; n<=last_n; n++) M[n][0] = 0;

  for(int n=1; n<=last_n; n++){
    for(int W=1; W<=last_W; W++){
      double s1 = M[n-1][W];
      double s2 = 0;
      if(W>=w[n-1]){
        s2 = v[n-1] + M[n-1][W-w[n-1]];
      }
      M[n][W] = max(s1, s2);
    }
  }

  return M[last_n][last_W];
}

// Iterativo con almacén con complejidad espacial mejorada
int iterativoConAlmacenConTiempoMejorado(vector<int> &v, vector<int> &w, int last_n, int last_W){
  vector<int> v0(last_W+1);
  vector<int> v1(last_W+1);

  for(int W=0; W<=last_W; W++) v0[W] = 0;

  for(int n=1; n<=last_n; n++){
    for(int W=1; W<=last_W; W++){
      double s1 = v0[W];
      double s2 = 0;

      if(W>=w[n-1]){
        s2 = v[n-1] + v0[W-w[n-1]];
      }

      v1[W] = max(s1, s2);
    }
    swap(v0,v1);
  }

  return v0[last_W];
}

// Iterativo con almacén con complejidad espacial mejorada , con almacenamiento de todas las decisiones
int iterativoConAlmacenConTiempoMejoradoAlmacenandoDecisiones(vector<int> &v, vector<int> &w, int last_n, int last_W, vector<vector<bool>> &trace){
  vector<vector<int>> M(last_n+1, vector<int>(last_W+1));
  vector<vector<bool>>(last_n+1, vector<bool>(last_W+1));


  for(int W=0; W<=last_W; W++){
    M[0][W] = 0;
    trace[0][W] = false;
  }

  for(int n=1; n<=last_n; n++){
    for(int W=1; W<=last_W; W++){
      double s1 = M[n-1][W];
      double s2 = 0;

      if(W>=w[n-1]){
        s2 = v[n-1] + M[n-1][W-w[n-1]];
      }

      M[n][W] = max(s1, s2);
      trace[n][W] = s2>s1;
    }
  }

  return M[last_n][last_W];
}

// Extracción de las decisiones que interesan
void parse(vector<int> &w, vector<vector<bool>> &trace, vector<bool> &sol){

  int last_n = trace.size()-1;
  int W = trace[0].size()-1;

  for(int n=last_n; n>0; n--){
    if(trace[n][W]){
      sol[n-1] = true;
      W-=w[n-1];
    }
    else{
      sol[n-1] = false;
    }
  }

}

// Extracción de la selección (directamente del almacén)
void parse2(vector<vector<int>> &M, vector<int> &v, vector<int> &w, int n, int W, vector<bool> &sol){
  if(n==0) return;

  int s1 = M[n-1][W];
  int s2 = 0;
  if(W>=w[n-1]){
    s2 = v[n-1] + M[n-1][W-w[n-1]];
  }
  if(s1>=s2){
    sol[n-1] = false;
    parse2(M, v, w, n-1, W, sol);
  }
  else{
    sol[n-1] = true;
    parse2(M, v, w, n-1, W-w[n-1], sol);
  }
}

// Función para mostrar la matriz de Memoization
void mostrarMatrizMemo(vector<vector<double>> &M, int n, int T, vector<int> &m){
  int cont=0;

  cout<<"Memoization matrix:"<<endl;
  for(int i=0; i<n; i++){
    M[cont][T+1] = i;
    cont+=m[i];
    for(int j=0; j<T+1; j++){
      if(M[cont][j]==SENTINEL){
        cout<<"- ";
      }
      else{
        cout<<M[cont][j]<<" ";
      }
    }
    cout<<endl;
  }
}

void mostrarMatrizIter(vector<vector<int>> &M, int n, int T, vector<int> &m){
  cout<<"Iterative matrix:"<<endl;

  int cont=0;

  for(int i=0; i<n; i++){
    M[cont][T+1] = i;
    cont+=m[i];
    for(int j=0; j<T+1; j++){
      if(M[cont][j]==SENTINEL){
        cout<<"- ";
      }
      else{
        cout<<M[cont][j]<<" ";
      }
    }
    cout<<endl;
  }
}



// Función principal
int main(int argc, char *argv[]){

  string file="";
  bool tOption=false;
  bool ignoreOption=false;

  for(int i=1; i<argc; i++){
    if(strcmp(argv[i], "--ignore-naive")==0){
      ignoreOption=true;
    }
  }


  // Gestión de argumentos
  for(int i=1; i<argc; i++){
    if(strcmp(argv[i], "-f")==0){
      if(i<argc-1){
        i++;
        file=argv[i];

        // Si se encuentra la opcion '-f' se procede a leer el fichero.

        int n=0, T=0;
        int tiempoTotal=0;
        vector<int> t;
        vector<int> v;
        vector<int> m;
        vector<int> copiasDeCadaObjeto;

        vector<int> vMejor;
        vector<int> tiemposMejor;



        ifstream fichero(file);
        leerFichero(fichero, file, n, T, t, v, m);


        for(long unsigned int j=0; j<m.size(); j++){  // Cambiamos la matriz
          for(int k=0; k<m[j]; k++){
            vMejor.push_back(v[j]);
            tiemposMejor.push_back(t[j]);
          }
        }

        int nMejor = vMejor.size();


        vector<vector<double>> M(nMejor+1, vector<double>(T+1, SENTINEL));
        vector<vector<int>> MIter(nMejor+1, vector<int>(T+1));


        if(tOption && ignoreOption){

          cout<<recursivoConAlmacen2(M, vMejor, tiemposMejor, nMejor, T, copiasDeCadaObjeto)<<" ";
          cout<<iterativoConAlmacenYTabla(vMejor, tiemposMejor, nMejor, T, MIter)<<" ";
          cout<<iterativoConAlmacenConTiempoMejorado(vMejor, tiemposMejor, nMejor, T)<<endl;
          cout<<"?"<<endl;
          cout<<"?"<<endl;
          cout<<"Memoization matrix: "<<endl;
          cout<<"?"<<endl;
          cout<<"Iterative matrix: "<<endl;
          cout<<"?"<<endl;

        }
        else if(tOption && !ignoreOption){

          cout<<recursivoSinAlmacen(nMejor, T, tiemposMejor, vMejor, tiempoTotal)<<" ";
          cout<<recursivoConAlmacen2(M, vMejor, tiemposMejor, nMejor, T, copiasDeCadaObjeto)<<" ";
          cout<<iterativoConAlmacenYTabla(vMejor, tiemposMejor, nMejor, T, MIter)<<" ";
          cout<<iterativoConAlmacenConTiempoMejorado(vMejor, tiemposMejor, nMejor, T)<<endl;
          cout<<"?"<<endl;
          cout<<"?"<<endl;
          cout<<"?"<<endl;
          cout<<"Memoization matrix: "<<endl;
          cout<<"?"<<endl;
          cout<<"Iterative matrix: "<<endl;
          cout<<"?"<<endl;

        }
        else if(!tOption && ignoreOption){

          cout<<recursivoConAlmacen2(M, vMejor, tiemposMejor, nMejor, T, copiasDeCadaObjeto)<<" ";
          cout<<iterativoConAlmacenYTabla(vMejor, tiemposMejor, nMejor, T, MIter)<<" ";
          cout<<iterativoConAlmacenConTiempoMejorado(vMejor, tiemposMejor, nMejor, T)<<endl;
          cout<<"?"<<endl;
          cout<<"?"<<endl;

        }
        else if(!tOption && !ignoreOption){

          cout<<recursivoSinAlmacen(nMejor, T, tiemposMejor, vMejor, tiempoTotal)<<" ";
          cout<<recursivoConAlmacen2(M, vMejor, tiemposMejor, nMejor, T, copiasDeCadaObjeto)<<" ";
          cout<<iterativoConAlmacenYTabla(vMejor, tiemposMejor, nMejor, T, MIter)<<" ";
          cout<<iterativoConAlmacenConTiempoMejorado(vMejor, tiemposMejor, nMejor, T)<<endl;
          cout<<"?"<<endl;
          cout<<tiempoTotal<<endl;

          mostrarMatrizMemo(M, n, T, m);
          mostrarMatrizIter(MIter, n, T, m);

        }



        exit(1);
      }
      else{
        cerr<<"ERROR: missing filename."<<endl;
        exit(0);
      }
    }
    else if(strcmp(argv[i], "--ignore-naive")==0 || strcmp(argv[i], "-t")==0 || strcmp(argv[i], "-f")==0){
      if(strcmp(argv[i], "--ignore-naive")==0){
        ignoreOption=false;
      }
      if(strcmp(argv[i], "-t")==0){
        tOption=true;
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
