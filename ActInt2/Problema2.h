#include <iostream>
#include <climits>
#include <queue>

#define MAX 30

using namespace std;

struct Nodo{
    int niv;
    int costoAcum;
    int costoPos;
    int verticeAnterior;
    int verticeActual;
    bool visitados[MAX] = {false};
    bool operator<(const Nodo &otro) const{ // Para que la fila priorizada tenga prioridad 
        return costoPos >= otro.costoPos;   // Costo Posible menor.
    }
    string ruta;
};

struct TSP{
    int costoOptimo;
    string rutaOptima;
};

void iniciarMat(int matAdj[MAX][MAX]){

    for(int i=0; i<MAX; i++){
        matAdj[i][i] = 0;
        for(int j=i+1; j<MAX; j++){
            matAdj[i][j] = matAdj[j][i] = INT_MAX;
        }
    }
}

void leeArcos(int matAdj[MAX][MAX], int edges, vector<string>  col1, vector<string> col2,
                vector<string> colNames, vector<int> colCentral, vector<int> valEdges){
    int a, b;
    for(int i=0; i<edges; i++){
        for(int j=0; j<colNames.size(); j++){
            if(col1[i]==colNames[j]){
                a = j;
            }
            if(col2[i]==colNames[j]){
                b = j;
            }
        }
        matAdj[a+1][b+1] = matAdj[b+1][a+1] = valEdges[i];
    }
}


void calcularCostoPosible(Nodo &nodoActual, int matAdj[MAX][MAX], int n){
    nodoActual.costoPos = nodoActual.costoAcum;
    int costoObtenido;
    for(int i=1; i<=n; i++){
        costoObtenido = INT_MAX;
        // No he visitado al nodo i o es el último que visite
        if (!nodoActual.visitados[i] || i==nodoActual.verticeActual){
            if(!nodoActual.visitados[i]){   // Al nodo i no lo he visitado
                for(int j=1; j<=n; j++){
                    if(i!=j && (!nodoActual.visitados[j] || j==1)){
                        //cout << "matAdj[" <<i<<"]["<<j<<"]: "<< matAdj[i][j]<< endl;
                        costoObtenido = min(costoObtenido, matAdj[i][j]);
                    }
                }
                //cout << "Nodo no visitado - " << "Costo Obtenido: " << costoObtenido << endl;

            }
            else{   // El nodo i es el último visitado
                for(int j=1; j<=n; j++){
                    if(!nodoActual.visitados[j]){
                        //cout << "matAdj[" <<i<<"]["<<j<<"]: "<< matAdj[i][j]<< endl;
                        costoObtenido = min(costoObtenido, matAdj[i][j]);
                    }
                }
                //cout <<  "Nodo último - " << "Costo Obtenido: " << costoObtenido << endl;
            }
            //cout << "Costo Obtenido: "<< costoObtenido << endl;
            nodoActual.costoPos += costoObtenido;        
        }
    }
}


// Complejidad: O(2^n)
TSP tsp(int matAdj[MAX][MAX], int n, vector<string> colNames, vector<int> colCentral){
    int costoOptimo = INT_MAX;
    int costoReal;
    bool completo;
    TSP answer;
    Nodo raiz, init;
    raiz.niv = 0;
    raiz.costoAcum = 0;
    raiz.verticeActual = 1;
    raiz.verticeAnterior = 0;
    raiz.visitados[1]=  true;
    raiz.ruta = colNames[0];
    calcularCostoPosible(raiz, matAdj, n);
    //cout << raiz.verticeActual << ". Costo posible: "<< raiz.costoPos << endl;
    priority_queue<Nodo> pq;
    pq.push(raiz);
    while(!pq.empty()){
        // sacar de pq;
        // Ver si el costoPos < costoOptimo
        // Si sí, generar todos los posibles hijos de este nodo
        // Para cada hijo generar un nuevo nodo, actualizar los datos
        // y cuando el nivel == n-2 calcular el costo real, y si este
        // mejora el costo optimo ==> actualizarlo
        // si el nivel < n-2 ... checar si el costo posible es mejor
        // que el costo optimo y lo metes a la pq
        init = pq.top(); 
        pq.pop(); // sacar de pq;
        //if(init.costoPos < costoOptimo){    // Ver si el costoPos < costoOptimo
        int i = init.verticeActual;
        for(int j=1; j<=n; j++){        // Si sí, generar todos los posibles hijos de este nodo
            if(matAdj[i][j] != INT_MAX && !init.visitados[j]){
                Nodo hijo;
                hijo.niv = init.niv + 1;
                hijo.costoAcum = init.costoAcum + matAdj[i][j];
                hijo.verticeActual = j;
                hijo.verticeAnterior = i;
                hijo.ruta = init.ruta + " - " + colNames[j-1];
                completo = false;
                //cout << "Nodos visitados del Nodo " << hijo.verticeActual <<": ";
                for(int c = 1; c <=n; c++){
                    if(init.visitados[c]){
                        //cout << c << " ";
                        hijo.visitados[c]=true;
                    }
                }
                hijo.visitados[j]=  true;
                //cout << j << endl;
                //cout << "Costo acumulado del Nodo " << hijo.verticeActual <<": " << hijo.costoAcum << endl;
                if(hijo.niv < n-2){
                    calcularCostoPosible(hijo, matAdj, n);
                    // cout << hijo.verticeActual << ". Costo posible: "<< hijo.costoPos << endl << endl;
                    //if(hijo.costoPos < costoOptimo){ 
                        pq.push(hijo);
                    //}
                    int cuentaColonia = 0, cuentaColoniaVisitada = 0;
                    for(int nColonia=0; nColonia<n; nColonia++){
                        if(!colCentral[nColonia]){
                            cuentaColonia++;
                            if(hijo.visitados[nColonia+1]) cuentaColoniaVisitada++;
                        }
                    }
                    if(cuentaColonia == cuentaColoniaVisitada) completo = true;
                }
                if(completo){
                    if (matAdj[hijo.verticeActual][hijo.verticeActual]!= INT_MAX && matAdj[hijo.verticeActual][1]!= INT_MAX){
                        costoReal = hijo.costoAcum + matAdj[hijo.verticeActual][1];
                        if(costoReal<costoOptimo){
                            costoOptimo = costoReal;
                            answer.rutaOptima = hijo.ruta + " - "+ colNames[0];
                            // cout << "Costo Real: " << costoOptimo << endl;
                        } 
                    }
                }
                if(hijo.niv == n-2){  
                    for(int faltante=1; faltante<=n; faltante++){
                        if(!hijo.visitados[faltante]){
                            if(!colCentral[faltante-1]){
                                hijo.ruta += " - " + colNames[faltante-1];
                                // cout << "Faltante: " << k<< endl;
                                if (matAdj[hijo.verticeActual][faltante]!= INT_MAX && matAdj[faltante][1]!= INT_MAX){
                                    costoReal = hijo.costoAcum + matAdj[hijo.verticeActual][faltante] + matAdj[faltante][1];
                                    if(costoReal<costoOptimo){
                                        costoOptimo = costoReal;
                                        answer.rutaOptima = hijo.ruta + " - "+ colNames[0];
                                        // cout << "Costo Real: " << costoOptimo << endl;
                                    }
                                }
                            }
                            else{
                                if (matAdj[hijo.verticeActual][hijo.verticeActual]!= INT_MAX && matAdj[hijo.verticeActual][1]!= INT_MAX){
                                    costoReal = hijo.costoAcum + matAdj[hijo.verticeActual][1];
                                    if(costoReal<costoOptimo){
                                        costoOptimo = costoReal;
                                        answer.rutaOptima = hijo.ruta + " - "+ colNames[0];
                                        // cout << "Costo Real: " << costoOptimo << endl;
                                    } 
                                }
                            }
                        }
                    }
                    //cout << endl;                        
                } 
                
            }
        }
        //}
    }

    answer.costoOptimo = costoOptimo;
    return answer;
}