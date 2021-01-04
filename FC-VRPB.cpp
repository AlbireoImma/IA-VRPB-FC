// C++ program to illustrate the 
// iterators in vector 
#include <iostream> 
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <bits/stdc++.h> 

bool factibilidad(int a, int b) {
    /*
        Función la cual verifica que existe un camino factible entre los nodos dado su tipo
        Conocemos que ciertas relaciones no se pueden dar (backhaul -> linehaul o deposito -> backhaul)
    */
    if (a == 0 && b == 0) return false;
    if (a == 0 && b == 2) return false;
    if (a == 2 && b == 1) return false;
    return true;
}

bool restriccion_carga_linehaul(bool ***Vars, int N, int n, int K, std::vector<float> demanda, float capacidad) {
    float carga;
    for (int k = 0; k < K; k++){
        carga = 0;
        for (int i = 0; i < N; i++){
            for (int j = 1; j < n; j++){
                carga += Vars[k][i][j] * demanda.at(j-1);
            }   
        }
        if (carga > capacidad) return false;
    }
    return true;
}

bool restriccion_carga_backhaul(bool ***Vars, int N, int n, int K, std::vector<float> demanda, float capacidad) {
    float carga;
    for (int k = 0; k < K; k++){
        carga = 0;
        for (int i = 0; i < N; i++){
            for (int j = n + 1; j < N; j++){
                carga += Vars[k][i][j] * demanda.at(j-1);
            }   
        }
        if (carga > capacidad) return false;
    }
    return true;
}

bool nodo_visitado(bool ***Vars, int K, int N, int id_nodo) {
    /*
        Funcion que verifica si un nodo fue visitado o no de manera unica
        Simplemente si se entra y sale a un nodo de manera única y desde la misma ruta
    */
    int ida, vuelta, visita = 0;
    for (int k = 0; k < K; k++){
        ida = 0; vuelta = 0;
        for (int indice = 0; indice < N; indice++){
            ida += Vars[k][id_nodo][indice];
            vuelta += Vars[k][indice][id_nodo];
        }
        if (ida == 1 && vuelta == 1) visita++;
    }
    if (visita == 1) return true;
    else return false;
}

std::vector<int> lector_ruta(bool ***Vars, int k, int N){
    std::vector<int> ruta;
    ruta.push_back(0);
    int nodo=0, camino;
    bool Flag = true;
    // Obtenemos si existe algun nodo inicial 0 -> n?
    for (int i = 0; i < N; i++){
        nodo = Vars[k][0][i];
        if (nodo){
            ruta.push_back(i);
            camino = i;
            break;
        }
    }
    // En caso de ser la ruta vacía
    if (!nodo) return ruta;
    // Verificamos los caminos dentro de las variables para el k
    int count;
    while (Flag) {
        nodo = 0;
        count = 0;
        for (int i = 0; i < N; i++){
            nodo = Vars[k][camino][i];
            count += Vars[k][camino][i];
            if (nodo){
                ruta.push_back(i);
                camino = i;
                if (camino == 0) Flag = false;
                break;
            }
        }
        if (count == 0) break;
    }
    return ruta;
}

bool restriccion_deposito_salida(bool ***Vars, int K, int N) {
    /*
        Función la cual verifica que cada camion salga del depósito una sola vez en cada ruta
    */
    int count;
    for (int k = 0; k < K; k++){
        count = 0;
        for (int j = 0; j < N; j++){
            if (Vars[k][0][j]) count++;
        }
        if (count != 1) return false;
    }
    return true;
}

bool restriccion_deposito_entrada(bool ***Vars, int K, int N) {
    /*
        Función la cual verifica que cada camion llegue al deposito una sola vez en cada ruta
    */
    int count;
    for (int k = 0; k < K; k++){
        count = 0;
        for (int i = 0; i < N; i++){
            if (Vars[k][i][0]) count++;
        }
        if (count != 1) return false;
    }
    return true;
}

bool enlace_linehaul_backhaul(bool ***Vars, int K, int N, int n) {
    int count;
    for (int k = 0; k < K; k++){
        count = 0;
        for (int i = 0; i < n+1; i++){
            for (int j = n+1; j < N; j++) if(Vars[k][i][j]) count++;
            if(Vars[k][i][0]) count++;
        }
        if(count != 1) return false;
    }
    return true;
}

bool factibilidad_entrada(bool ***Vars, int K, int N) {
    int count;
    for (int nodo = 1; nodo < N; nodo++){
        count = 0;
        for (int k = 0; k < K; k++){
            for (int i = 0; i < N; i++) if(Vars[k][i][nodo]) count++;
        }
        if(count > 1) return false;
    }
    return true;
}

bool factibilidad_deposito_entrada(bool ***Vars, int K, int N) {
    int count;
    for (int k = 0; k < K; k++){
        count = 0;
        for (int i = 0; i < N; i++) {
            if(Vars[k][i][0]) count++;
        }
        if (count > 1) return false;
    }
    for (int i = 0; i < N; i++){
        count = 0;
        for (int k = 0; k < K; k++) {
            if(Vars[k][i][0]) count++;
        }
        if (count > 1) return false;
    }
    return true;
}

bool factibilidad_deposito_salida(bool ***Vars, int K, int N) {
    int count;
    for (int k = 0; k < K; k++){
        count = 0;
        for (int i = 0; i < N; i++) {
            if(Vars[k][0][i]) count++;
        }
        if (count > 1) return false;
    }
    for (int i = 0; i < N; i++){
        count = 0;
        for (int k = 0; k < K; k++) {
            if(Vars[k][0][i]) count++;
        }
        if (count > 1) return false;
    }
    return true;
}

bool caminos_conexos(bool ***Vars, int k, int N, int entrada) {
    if (entrada == 0) return true;
    for (int i = 0; i < N; i++){
        if(Vars[k][i][entrada]) return true;
    }
    return false;
    
}

bool factibilidad_salida(bool ***Vars, int K, int N) {
    int count;
    for (int nodo = 1; nodo < N; nodo++){
        count = 0;
        for (int k = 0; k < K; k++){
            for (int i = 0; i < N; i++) if(Vars[k][nodo][i]) count++;
        }
        if(count > 1) return false;
    }
    return true;
}

bool factibilidad_deposito(bool ***Vars, int K, int N) {
    int entrada, salida;
    for (int k = 0; k < K; k++) {
        entrada = 0, salida = 0;
        for (int i = 1; i < N; i++) {
            if(Vars[k][0][i]) salida++;
            if(Vars[k][i][0]) entrada++;
        }
        if (salida > 1 || entrada > 1) return false;
    }
    return true;
}

bool es_solucion(bool ***Vars, int N, int n, int K, std::vector<float> demanda, float capacidad) {
    for (int i = 1; i < N; i++){
        if(!nodo_visitado(Vars, K, N, i)) return false;
    }
    if (!restriccion_carga_backhaul(Vars, N, n, K, demanda, capacidad)) return false;
    if (!restriccion_carga_linehaul(Vars, N, n, K, demanda, capacidad)) return false;
    if (!restriccion_deposito_entrada(Vars, K, N)) return false;
    if (!restriccion_deposito_salida(Vars, K, N)) return false;
    return true;
}

bool es_factible(bool ***Vars, int N, int n, int K, std::vector<float> demanda, float capacidad, int k, int entrada) {
    if (!caminos_conexos(Vars, k, N, entrada)) {
        //std::cout << "Factibilidad rechazada por: Camino inconexo" << std::endl;
        return false;
    }
    if (!factibilidad_deposito(Vars, K, N)) {
        //std::cout << "Factibilidad rechazada por: Factibilidad deposito {Mas de una salida o entrada al deposito en esta ruta}" << std::endl;
        return false;
    }
    if (!factibilidad_deposito_entrada(Vars, K, N)) {
        //std::cout << "Factibilidad rechazada por: Entrada deposito {Deposito utilizado más de una vez en la ruta o utiliza un nodo al que ya llega otra ruta}" << std::endl;
        return false;
    }
    if (!factibilidad_deposito_salida(Vars, K, N)) {
        //std::cout << "Factibilidad rechazada por: Salida deposito {Deposito utilizado más de una vez en la ruta o utiliza un nodo al que ya llega otra ruta}" << std::endl;
        return false;
    }
    if (!factibilidad_entrada(Vars, K, N)) {
        //std::cout << "Factibilidad rechazada por: Entrada" << std::endl;
        return false;
    }
    if (!factibilidad_salida(Vars, K, N)) {
        //std::cout << "Factibilidad rechazada por: Salida" << std::endl;
        return false;
    }
    if (!restriccion_carga_backhaul(Vars, N, n, K, demanda, capacidad)) {
        //std::cout << "Factibilidad rechazada por: Capacidad backhaul" << std::endl;
        return false;
    }
    if (!restriccion_carga_linehaul(Vars, N, n, K, demanda, capacidad)) {
        //std::cout << "Factibilidad rechazada por: Capacidad linehaul" << std::endl;
        return false;
    }
    //std::cout << "[ERROR] Factibilidad Aceptada" << std::endl;
    return true;
}

std::vector<float> evaluacion(bool ***Vars, int N, int K, float **Distancias) {
    std::vector<float> eval;
    float suma;
    for (int k = 0; k < K; k++){
        suma = 0;
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++) suma += Vars[k][i][j] * Distancias[i][j]; 
        }
        eval.push_back(suma);
    }
    return eval;
}

std::vector<float> demanda_rutas(bool ***Vars, int N, int K, std::vector<float> demandas){
    std::vector<float> D;
    float suma;
    for (int k = 0; k < K; k++){
        suma = 0;
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++) {
                if (j != 0) suma += Vars[k][i][j] * demandas.at(j-1);
            }
        }
        D.push_back(suma);
    }
    return D;
}

int main(int argc, char *argv[]) { 
    // Abrir el archivo
    time_t start, end;
    time(&start);
    std::ios_base::sync_with_stdio(false);
    std::string filename = argv[1];
    std::ifstream file(filename);
    
    // Parámetros
    int n; // Cantidad de nodos
    int k; // Cantidad de vehículos
    float C, demanda; // Capacidad de los vehículos
    float d_linehaul = 0, d_backhaul = 0;
    int tipo, id; // Vector para almacenar cada nodo
    int N = 0, M = 0; // valor de N [Linehauls] y M [Backhauls]
    float posx, posy; // Coordenadas del nodo
    // Vectores para almacenar los valores
    std::vector<int> IDs, TIPOS;
    std::vector<float> Coords_X, Coords_Y, DEMANDA;
    // Verificar la existencia del archivo
    if (!file){
        //REVIEW Problema al leer el archivo
        std::cout << "File non-existent";
        return 0;
    } else {
        file >> n; // Lectura cantidad de nodos N + M + Deposito
        for (int i = 0; i < n; i++){
            // Lectura de los valores en los nodos
            file >> tipo; TIPOS.push_back(tipo);
            file >> id; IDs.push_back(id);
            file >> posx; Coords_X.push_back(posx);
            file >> posy; Coords_Y.push_back(posy);
            /* Debug lectura Nodos
            std::cout << "Tipo: " << TIPOS.back() << " ID: " << IDs.back() << " Coordenada X: " << Coords_X.back() << " Coordenada Y: " << Coords_Y.back() << std::endl;
            */
            if (tipo == 1) N++;
            if (tipo == 2) M++;
        }
        file >> k;
        file >> C;
         for (int i = 0; i < n-1; i++){
            file >> id;
            file >> demanda; DEMANDA.push_back(demanda);
            /* Debug Lectura Demanda
            std::cout << "ID: " << id << " Demanda: " << demanda << std::endl;
            */
            if(TIPOS.at(id-1) == 1) d_linehaul += demanda;
            if(TIPOS.at(id-1) == 2) d_backhaul += demanda;
        }
    }
    
    // Inicializacion de Matrices
    float** Distancias = new float *[n]; // Distancias euclidianas [i][j]
    bool** Factibilidad = new bool *[n]; // Arco consistencia inicial [i][j]
    bool** Factibilidad_FC = new bool *[n];
    bool*** Variables = new bool **[k]; // Acceso a las variable de la forma [k][i][j]
    bool*** Saltos = new bool **[k]; // Acceso a las variable de la forma [k][i][j]
    //int var_count = n * n * k;
    
    for (int i = 0; i < n; i++) { 
        Distancias[i] = new float[n];
        Factibilidad[i] = new bool[n];
        Factibilidad_FC[i] = new bool[n];
    }

    for (int i = 0; i < k; i++) {
        Variables[i] = new bool *[n];
        Saltos[i] = new bool *[n];
        for (int j = 0; j < n; j++) {
            Variables[i][j] = new bool [n];
            Saltos[i][j] = new bool [n];
        }
    }
    

    float d;
    int tipo1, tipo2;
    for (int i = 0; i < n; i++){
        tipo1 = TIPOS.at(i);
        for (int j = 0; j < n; j++){
            // Calculo de distancia euclidiana entre nodos
            d = std::sqrt(std::pow(Coords_X.at(i) - Coords_X.at(j), 2) + std::pow(Coords_Y.at(i) - Coords_Y.at(j), 2));
            Distancias[i][j] = d;
            tipo2 = TIPOS.at(j);
            if (i != j) Factibilidad[i][j] = factibilidad(tipo1, tipo2);
            else Factibilidad[i][j] = false;
        }
    }
    
    // Debug Stats dataset
    /*
    std::cout << "[Linehauls: " << N << "] [IDs: " << 2 << " - " << N + 1 << "] [Demanda Total: " << d_linehaul << "]" << std::endl;
    std::cout << "[Backhauls: " << M << "] [IDs: " << N + 2 << " - " << n << "] [Demanda Total: " << d_backhaul << "]" << std::endl;
    std::cout << "[Camiones: " << k << "] [Capacidad: " << C << "] [Capacidad Total: " << k*C << "]" << std::endl;
    std::cout << "Cantidad de variables: " << n*n*k << std::endl;
    std::cout << "Demanda linehaul  ";
    for (int i = 0; i < N; i++) std::cout << IDs.at(i+1) << "->[" << DEMANDA.at(i) << "] ";
    std::cout << std::endl;
    std::cout << "Demanda backhaul  ";
    for (size_t i = N; i < DEMANDA.size(); i++) std::cout << IDs.at(i+1) << "->[" << DEMANDA.at(i) << "] ";
    std::cout << std::endl;
    */


    /* Debug Matrices
    std::cout << "Matriz factibilidad" << std::endl;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) std::cout << Factibilidad[i][j] << " ";
        std::cout << std::endl;
    }
    */
    

    /* Debug Restricciones
    bool r1 = restriccion_deposito_ida(Variables, k, n);
    bool r2 = restriccion_deposito_vuelta(Variables, k, n);

    std::cout << "Restricción deposito ida: " << bool(r1) << std::endl;
    std::cout << "Restricción deposito vuelta: " << bool(r2) << std::endl;

    bool lin, back;
    back = restriccion_carga_backhaul(Variables, n, N, k, DEMANDA, C);
    lin = restriccion_carga_linehaul(Variables, n, N, k, DEMANDA, C);

    std::cout << "capacidad linehaul: " << lin << std::endl;
    std::cout << "capacidad backhaul: " << back << std::endl;

    bool r = nodo_visitado(Variables, k, n, 1);
    std::cout << "Nodo 1 Visitado? : " << r << std::endl;
    
    bool r = enlace_linehaul_backhaul(Variables, k, n, N);
    std::cout << "Enlace inicial: " << r << std::endl;
    
    r = enlace_linehaul_backhaul(Variables, k, n, N);
    std::cout << "Enlace Final: " << r << std::endl;

    r = es_solucion(Variables, n, N, k, DEMANDA, C);
    std::cout << "Es solucion?: " << r << std::endl;
    */


    // Debug Rutas
    /*
    std::vector<int> ruta;

    for (int i = 0; i < k; i++){
        std::cout << "Ruta " << i + 1 << std::endl;
        ruta = lector_ruta(Variables, i, n);
        for (size_t j = 0; j < ruta.size(); j++) std::cout << ruta.at(j) << " -> ";
        std::cout << std::endl;
    }

    bool r = es_factible(Variables, n, N, k, DEMANDA, C);
    std::cout << "Factible?: " << r << std::endl;

    r = es_solucion(Variables, n, N, k, DEMANDA, C);
    std::cout << "Solucion?: " << r << std::endl;
    */

    struct mv {
        int i;
        int j;
        int k;
        mv (int K, int I, int J) {
            i = I; k = K; j = J;
        }
        mv (){}
    } move;

    bool solucion;
    
    std::vector<int> ruta;
    std::vector<mv> Moves_1;
    std::vector<mv> Moves_2;
    std::vector<mv> Moves_3;
    std::vector<mv> Moves_4;
    std::vector<mv> Moves_5;
    std::vector<mv> Moves_6;

    std::vector<float> eval;
    std::vector<float> D;

    float FO;
    
    int jump=0, moves[6]={0,0,0,0,0,0};
    bool FC[6]{true,true,true,true,true,true};
    int ciclos=0;
    while(true) {

        //std::cout << "--- Instanciación ---" << std::endl;
        //std::cout << std::endl;
        
        //DEBUG Ciclo por fases
        
        //DEBUG Fase I [0] -> [1,N]
        if(FC[0]){
            moves[0] = 0;
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N + 1; i++){
                    if (Factibilidad[0][i] && !Saltos[truck][0][i]) {
                        Variables[truck][0][i] = true;
                        if (!es_factible(Variables, n, N, k, DEMANDA, C, truck, 0)) { 
                            Variables[truck][0][i] = false;
                        } else {
                            //std::cout << "[Fase I] Movimiento: k=" << truck << " i=" << 0 << " j=" << i << std::endl;
                            Moves_1.push_back(mv(truck, 0, i));
                            moves[0]++;
                        }
                    }
                }
            }
        }

        //DEBUG Fase II [1,N] -> [1,N]
        if(FC[1]){
            moves[1] = 0;
            for (int j = 1; j < N+1; j++) {
                for (int i = 1; i < N+1; i++) {
                    for (int truck = 0; truck < k; truck++) {
                        if (Factibilidad[i][j] && !Saltos[truck][i][j]) {
                            Variables[truck][i][j] = true;
                            if (!es_factible(Variables, n, N, k, DEMANDA, C, truck, i)) { 
                                Variables[truck][i][j] = false;
                            } else {
                                //std::cout << "[Fase II] Movimiento: k=" << truck << " i=" << i << " j=" << j << std::endl;
                                Moves_2.push_back(mv(truck, i, j));
                                moves[1]++;
                            }
                        }
                    }
                }
            }
        }

        //DEBUG Fase III [1,N] -> [N+1, N+M]
        if(FC[2]){
            moves[2] = 0;
            for (int j = N+1; j < n; j++){
                for (int i = 1; i < N+1; i++){
                    for (int truck = 0; truck < k; truck++){
                        if (Factibilidad[i][j] && !Saltos[truck][i][j]) {
                            Variables[truck][i][j] = true;
                            if (!es_factible(Variables, n, N, k, DEMANDA, C, truck, i)) { 
                                Variables[truck][i][j] = false;
                            } else {
                                //std::cout << "[Fase III] Movimiento: k=" << truck << " i=" << i << " j=" << j << std::endl;
                                Moves_3.push_back(mv(truck, i, j));
                                moves[2]++;
                            }
                        }
                    }
                }
            }
        }

        //DEBUG Fase IV [N+1,N+M] -> [N+1,N+M]

        if(FC[3]){
            moves[3] = 0;
            for (int j = N+1; j < n; j++){
                for (int i = N+1; i < n; i++){
                    for (int truck = 0; truck < k; truck++){
                        if (Factibilidad[i][j] && !Saltos[truck][i][j]) {
                            Variables[truck][i][j] = true;
                            if (!es_factible(Variables, n, N, k, DEMANDA, C, truck, i)) { 
                                Variables[truck][i][j] = false;
                            } else {
                                //std::cout << "[Fase IV] Movimiento: k=" << truck << " i=" << i << " j=" << j << std::endl;
                                Moves_4.push_back(mv(truck, i, j));
                                moves[3]++;
                            }
                        }
                    }
                }
            }
        }

        //DEBUG Fase V [N+1,N+M] -> [0]

        if(FC[4]){
            moves[4] = 0;
            for (int i = N+1; i < n; i++){
                for (int truck = 0; truck < k; truck++){
                    if (Factibilidad[i][0] && !Saltos[truck][i][0]) {
                        Variables[truck][i][0] = true;
                        if (!es_factible(Variables, n, N, k, DEMANDA, C, truck, i)) { 
                            Variables[truck][i][0] = false;
                        } else {
                            //std::cout << "[Fase V] Movimiento: k=" << truck << " i=" << i << " j=" << 0 << std::endl;
                            Moves_5.push_back(mv(truck, i, 0));
                            moves[4]++;
                        }
                    }
                }
            }
        }
        
        //DEBUG Fase VI [1,N] -> [0]

        if(FC[5]){
            moves[5] = 0;
            for (int i = 1; i < N+1; i++){
                for (int truck = 0; truck < k; truck++){
                    if (Factibilidad[i][0] && !Saltos[truck][i][0]) {
                        Variables[truck][i][0] = true;
                        if (!es_factible(Variables, n, N, k, DEMANDA, C, truck, i)) { 
                            Variables[truck][i][0] = false;
                        } else {
                            //std::cout << "[Fase VI] Movimiento: k=" << truck << " i=" << i << " j=" << 0 << std::endl;
                            Moves_6.push_back(mv(truck, i, 0));
                            moves[5]++;
                        }
                    }
                }
            }
        }
        //std::cout << "--- Fin Instanciación ---" << std::endl;
        //std::cout << std::endl;

        //DEBUG Evaluacion

        //std::cout << "--- Evaluacion ---" << std::endl;
        //std::cout << std::endl;

        solucion = es_solucion(Variables, n, N, k, DEMANDA, C);
        std::string sol = solucion ? "Si" : "No";
        eval = evaluacion(Variables, n, k, Distancias);
        D = demanda_rutas(Variables, n, k, DEMANDA);
        FO = 0;
        for (size_t i = 0; i < eval.size(); i++) FO += 100 * eval.at(i) / D.at(i);
        
        

        int clientes = 0;
        for (int i = 0; i < k; i++){
            ruta = lector_ruta(Variables, i, n);
            if (ruta.size() > 2) clientes += ruta.size() - 2;
        }
        time(&end);
        if (solucion) {
            std::cout << "Funcion Evaluación: SUM(100 * distancia / demanda) = " << FO << " Clientes: " << clientes << " Vehiculos: " << k << " Tiempo ejecucion [s]: " << end - start << std::setprecision(6) << std::endl;
            for (int i = 0; i < k; i++){
                std::cout << "[Ruta " << i  << "] [Distancia: " << eval.at(i) << "] [Demanda: " << D.at(i) << "] ";
                ruta = lector_ruta(Variables, i, n);
                for (size_t j = 0; j < ruta.size()-1; j++) std::cout << ruta.at(j) << " -> ";
                std::cout << ruta.back();
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        
        /*
        if (!print) {
            //std::cout << "########################################" << std::endl;
            std::cout << "!! No solucion despues de salto" << std::endl << std::endl;
            std::cout << "Min(distancia recorrida) " << clientes << " " << k << " Tiempo ejecucion" << std::endl;
            for (int i = 0; i < k; i++){
                std::cout << "[Ruta " << i << "] [Distancia: " << eval.at(i) << "] [Demanda: " << D.at(i) << "] ";
                ruta = lector_ruta(Variables, i, n);
                for (size_t j = 0; j < ruta.size()-1; j++) std::cout << ruta.at(j) << " -> ";
                std::cout << ruta.back();
                std::cout << std::endl;
            }
            std::cout << std::endl;
            
        }*/

        //std::cout << "--- Fin Evaluacion ---" << std::endl;
        //std::cout << std::endl;

        //DEBUG BackJumping

        //std::cout << "--- Backjumping ---" << std::endl;
        //for (int i = 0; i < 6; i++) std::cout << moves[i] << " ";
        //std::cout << std::endl;
        //std::cout << std::endl;

        if (jump == 0 && Moves_6.size() > 0) {
            for (int i = 0; i < moves[5]; i++) {
                move = Moves_6.back();
                Saltos[move.k][move.i][move.j] = true;
                Variables[move.k][move.i][move.j] = false;
                //std::cout << "[VI] Borrado: " << move.k << " " << move.i << " " << move.j << std::endl;
                Moves_6.pop_back();
            }
            FC[5] = 1; FC[4] = 0; FC[3] = 0; FC[2] = 0; FC[1] = 0; FC[0] = 0;
        } 
        if (jump == 0 && moves[5] == 0) {
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }
            jump++;
            FC[5] = 1; FC[4] = 1; FC[3] = 0; FC[2] = 0; FC[1] = 0; FC[0] = 0;
        }

        if (jump == 1 && Moves_5.size() > 0) {
            for (int i = 0; i < moves[4]; i++) {
                move = Moves_5.back();
                Saltos[move.k][move.i][move.j] = true;
                Variables[move.k][move.i][move.j] = false;
                //std::cout << "[V] Borrado: " << move.k << " " << move.i << " " << move.j << std::endl;
                Moves_5.pop_back();
            }
            FC[5] = 1; FC[4] = 1; FC[3] = 0; FC[2] = 0; FC[1] = 0; FC[0] = 0;
        } 
        if (jump == 1 && moves[4] == 0) {
            for (int truck = 0; truck < k; truck++){
                for (int i = N+1; i < n; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }
            jump++;
            FC[5] = 1; FC[4] = 1; FC[3] = 1; FC[2] = 0; FC[1] = 0; FC[0] = 0;
        }

        if (jump == 2 && Moves_4.size() > 0) {
            for (int i = 0; i < moves[3]; i++) {
                move = Moves_4.back();
                Saltos[move.k][move.i][move.j] = true;
                Variables[move.k][move.i][move.j] = false;
                //std::cout << "[IV] Borrado: " << move.k << " " << move.i << " " << move.j << std::endl;
                Moves_4.pop_back();
            }
            FC[5] = 1; FC[4] = 1; FC[3] = 1; FC[2] = 0; FC[1] = 0; FC[0] = 0;
        } 
        if (jump == 2 && moves[3] == 0) {
            for (int truck = 0; truck < k; truck++){
                for (int i = N+1; i < n; i++){
                    for (int j = N+1; j < n; j++) {Saltos[truck][i][j] = false; Variables[truck][i][j] = false;}
                }
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = N+1; i < n; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }
            jump++;
            FC[5] = 1; FC[4] = 1; FC[3] = 1; FC[2] = 1; FC[1] = 0; FC[0] = 0;
        }

        if (jump == 3 && Moves_3.size() > 0) {
            for (int i = 0; i < moves[2]; i++) {
                move = Moves_3.back();
                Saltos[move.k][move.i][move.j] = true;
                Variables[move.k][move.i][move.j] = false;
                //std::cout << "[III] Borrado: " << move.k << " " << move.i << " " << move.j << std::endl;
                Moves_3.pop_back();
            }
            FC[5] = 1; FC[4] = 1; FC[3] = 1; FC[2] = 1; FC[1] = 0; FC[0] = 0;
        } 
        if (jump == 3 && moves[2] == 0) {
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++){
                    for (int j = N+1; j < n; j++) {Saltos[truck][i][j] = false; Variables[truck][i][j] = false;}
                }
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = N+1; i < n; i++){
                    for (int j = N+1; j < n; j++) {Saltos[truck][i][j] = false; Variables[truck][i][j] = false;}
                }
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = N+1; i < n; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }            
            jump++;
            FC[5] = 1; FC[4] = 1; FC[3] = 1; FC[2] = 1; FC[1] = 1; FC[0] = 0;
        }

        if (jump == 4 && Moves_2.size() > 0) {
            for (int i = 0; i < moves[1]; i++) {
                move = Moves_2.back();
                Saltos[move.k][move.i][move.j] = true;
                Variables[move.k][move.i][move.j] = false;
                //std::cout << "[II] Borrado: " << move.k << " " << move.i << " " << move.j << std::endl;
                Moves_2.pop_back();
            }
            FC[5] = 1; FC[4] = 1; FC[3] = 1; FC[2] = 1; FC[1] = 1; FC[0] = 0;
        } 
        if (jump == 4 && moves[1] == 0) {
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++){
                    for (int j = 1; j < N+1; j++) {Saltos[truck][i][j] = false; Variables[truck][i][j] = false;}
                }
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++){
                    for (int j = N+1; j < n; j++) {Saltos[truck][i][j] = false; Variables[truck][i][j] = false;}
                }
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = N+1; i < n; i++){
                    for (int j = N+1; j < n; j++) {Saltos[truck][i][j] = false; Variables[truck][i][j] = false;}
                }
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = N+1; i < n; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }            
            jump++;
            FC[5] = 1; FC[4] = 1; FC[3] = 1; FC[2] = 1; FC[1] = 1; FC[0] = 1;
        }

        if (jump == 5 && Moves_1.size() > 0) {
            for (int i = 0; i < moves[0]; i++){
                move = Moves_1.back();
                Saltos[move.k][move.i][move.j] = true;
                Variables[move.k][move.i][move.j] = false;
                //std::cout << "[I] Borrado: " << move.k << " " << move.i << " " << move.j << std::endl;
                Moves_1.pop_back();
            }
            FC[5] = 1; FC[4] = 1; FC[3] = 1; FC[2] = 1; FC[1] = 1; FC[0] = 1;
        } 
        if (jump == 5) {
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++){
                    for (int j = 1; j < N+1; j++) {Saltos[truck][i][j] = false; Variables[truck][i][j] = false;}
                }
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++){
                    for (int j = N+1; j < n; j++) {Saltos[truck][i][j] = false; Variables[truck][i][j] = false;}
                }
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = N+1; i < n; i++){
                    for (int j = N+1; j < n; j++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
                }
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = N+1; i < n; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }
            for (int truck = 0; truck < k; truck++){
                for (int i = 1; i < N+1; i++) {Saltos[truck][i][0] = false; Variables[truck][i][0] = false;}
            }
            FC[5] = 1; FC[4] = 1; FC[3] = 1; FC[2] = 1; FC[1] = 1; FC[0] = 1;
            //std::cout << "Finalización" << std::endl;
            jump=0; ciclos++;
            if (ciclos > N+1) break;
        }
        
        //std::cout << "--- Fin Backjumping ---" << std::endl;
        //std::cout << std::endl;
        
        
    }
    // Liberar memoria
    for (int i = 0; i < n; i++) {
        delete[] Distancias[i];
        delete[] Factibilidad[i];
        delete[] Factibilidad_FC[i];
    }

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            delete[] Variables[i][j];
            delete[] Saltos[i][j];
        }
        delete[] Variables[i];
        delete[] Saltos[i];
    }

    //TODO Comentar
    //TODO Informe
    

    delete[] Distancias;
    delete[] Factibilidad;
    delete[] Variables;
    delete[] Saltos;
    
    file.close();
    
    return 0;
} 