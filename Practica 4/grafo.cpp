#include "grafo.h"

void GRAFO ::destroy() {
  for (unsigned i = 0; i < n; i++) {
    LS[i].clear();
    A[i].clear();
    if (dirigido == 1) {
      LP[i].clear();
    }
  }
  LS.clear();
  LP.clear();
  A.clear();
}

void GRAFO ::build(char nombrefichero[85], int &errorapertura) {
  // creo objeto dummy y textfile
  ElementoLista dummy;
  ifstream textfile;

  // lo abro
  textfile.open(nombrefichero);
  if (textfile.is_open()) {
    unsigned i, j, k;
    textfile >> (unsigned &)n >> (unsigned &)m >> (unsigned &)dirigido; // leo n nodos, m arcos y si es dirigido o no
    // redimensiono lista sucesores y predecesores para que sea igual que n
    LS.resize(n);
    LP.resize(n);
    A.resize(n, std::vector<ElementoLista>(n, {0, maxint})); // resize o sino peta (matriz de ady nxm)
    for (k = 0; k < m; k++) { // itero sobre cada arco
      textfile >> (unsigned &)i >> (unsigned &)j >> (int &)dummy.c; // leo costo y nodos del arco
      dummy.j = j - 1; // ajusto el indice del nodo
      LS[i - 1].push_back(dummy); // agrego nodo i a lista sucesores
      if (dirigido == 0) {
        dummy.j = i - 1;
        LS[j - 1].push_back(dummy); // agrego nodo j a lista predecesores
      } else {
        dummy.j = i - 1;
        LP[j - 1].push_back(dummy);
      }
    }
  } else {
    errorapertura = 1;
  }
}

GRAFO::~GRAFO() { destroy(); }

GRAFO::GRAFO(char nombrefichero[85], int &errorapertura) {
  build(nombrefichero, errorapertura);
}

void GRAFO::actualizar(char nombrefichero[85], int &errorapertura) {
  destroy();
  build(nombrefichero, errorapertura);
}

unsigned GRAFO::Es_dirigido() { return dirigido; }

void GRAFO::Info_Grafo() {
  cout << "Grafo dirigido: " << dirigido << endl;
  cout << "Número de nodos: " << n << endl;
  cout << "Número de arcos: " << m << endl;
}

void Mostrar_Lista(vector<LA_nodo> L) { // lista de nodos
  for (unsigned i = 0; i < L.size(); i++) { // recorro la lista
    cout << "Nodo " << i + 1 << ": "; // imprimo nodo actual
    for (unsigned j = 0; j < L[i].size(); j++) { // recorro cada elemento
      cout << L[i][j].j + 1 << " "; // imprimo nodo ady
    }
    cout << endl;
  }
}

void GRAFO ::Mostrar_Listas(int l) { 
  if (l == 0) {
    cout << "Lista de sucesores: " << endl;
    Mostrar_Lista(LS); // lista de sucesores
  } else {
    cout << "Lista de predecesores: " << endl;
    Mostrar_Lista(LP); // lista de predecesores
  }
}
void GRAFO::Mostrar_Matriz()  // Muestra la matriz de adyacencia, tanto los
                              // nodos adyacentes como sus costes
{
  cout << "matriz de adyacencia: " << endl;
  for (unsigned i = 0; i < n; i++) { // filas
    for (unsigned j = 0; j < n; j++) { // columnas
      bool found = false; // bool para decir si encuentro nodo ady
      for (auto &x : LS[i]) { // recorro cada nodo ady en la lista de sucesores
        if (x.j == j) { // nodo ayd = j
          cout << x.c << " "; // imprimo el coste del nodo ady
          found = true; // encontro un nodo ady
          break;
        }
      }
      if (!found) cout << "0 ";
    }
    cout << endl;
  }
}

void GRAFO::dfs_num(
    unsigned i, vector<LA_nodo> L, vector<bool> &visitado,
    vector<unsigned> &prenum, unsigned &prenum_ind, vector<unsigned> &postnum,
    unsigned &postnum_ind)  // Recorrido en profundidad recursivo con recorridos
                            // enum y postnum
{
  visitado[i] = true; // nodo i visitado

  prenum[i] = prenum_ind++; // asigno y aumento en postorden

  for (unsigned j = 0; j < L[i].size(); j++) // cada nodo ady de i
    if (!visitado[L[i][j].j])
      dfs_num(L[i][j].j, L, visitado, prenum, prenum_ind, postnum, postnum_ind); // llamo a dfs

  postnum[i] = postnum_ind++; // asigno y aumento en postorden
}

void GRAFO::RecorridoProfundidad(unsigned i, vector<LA_nodo> &L,
                                 vector<unsigned> &d, vector<bool> &visitado,
                                 vector<unsigned> &pred) {
  visitado.assign(n, false);  // Inicializa visitado correctamente
  vector<unsigned> prenum(n, 0); 
  vector<unsigned> postnum(n, 0);
  unsigned prenum_ind = 0;
  unsigned postnum_ind = 0;

  cout << "nodo de partida: [1-" << n << "]: ";
  cin >> (unsigned &)i; // leo el nodo 

  if (i < 1 || 1 > n) {
    cout << "elige un nodo entre [1-" << n << "]";
    cin >> (unsigned &)i;
  }

  i--;  // actualizo nodo indice

  dfs_num(i, LS, visitado, prenum, prenum_ind, postnum, postnum_ind); // recorrido en profundidad desde nodo inicial
  cout << "orden de nodos en preoden: " << endl;
  for (unsigned j = 0; j < prenum_ind; j++) { // recorro cada num de postorden
    cout << "[" << prenum[j] + 1 << "]"; // nodo en preorden
    if (j < prenum_ind - 1) cout << " -> "; // mientras no sea el ultlimo nodo ->
  }
  cout << endl;

  cout << "orden de nodos en postorden: " << endl;
  for (unsigned j = 0; j < postnum_ind; j++) {
    cout << "[" << postnum[j] + 1 << "]";
    if (j < postnum_ind - 1) cout << " -> ";
  }
  cout << endl;
}

void GRAFO::bfs_num(unsigned i, vector<LA_nodo> L, vector<unsigned> &pred,
                    vector<unsigned> &d) {
  vector<bool> visitado(n, false); // vector nodos visitados
  pred.assign(n, 0);  // vector de predecesores
  d.assign(n, 0);     // vector de distancias

  visitado[i] = true; // si visitado
  pred[i] = i; // el predecesor es el mismo
  d[i] = 0; // por lo cual d = 0

  queue<unsigned> cola;
  cola.push(i); // agrego nodo i a cola

  while (!cola.empty()) {
    unsigned k = cola.front(); // obtengo el primer elemento de la cola
    cola.pop();

    for (unsigned j = 0; j < L[k].size(); j++) { // recorro cada nodo ady al nodo k
      if (!visitado[L[k][j].j]) {
        visitado[L[k][j].j] = true; // si no ha sido visitado lo marco visitado
        pred[L[k][j].j] = k;
        d[L[k][j].j] = d[k] + 1; // distancia del nodo ady
        cola.push(L[k][j].j); // agrego el nodo ady a la cola
      }
    }
  }
}

void GRAFO::RecorridoAmplitud() {
  vector<unsigned> pred(n,0);  // Vector para almacenar el predecesor de cada nodo
  vector<unsigned> d(n, 0);  // Vector para almacenar la distancia de cada nodo

  unsigned nodo_partida;
  cout << "Ingrese el nodo de partida (1-" << n << "): ";
  cin >> nodo_partida;

  // Validar la entrada del usuario
  if (nodo_partida < 1 || nodo_partida > n) {
    cout << "Nodo inválido. Por favor, ingrese un nodo válido entre 1 y " << n
         << "." << endl;
    return;
  }

  // Convertir a índice base 0
  nodo_partida--;

  // Realizar BFS desde el nodo seleccionado por el usuario
  bfs_num(nodo_partida, LS, pred, d);

  // Imprimir los predecesores y distancias obtenidos
  cout << "Desde el nodo " << nodo_partida + 1 << ":" << endl;
  cout << "Predecesores: ";
  for (unsigned j = 0; j < n; ++j) {
    cout << pred[j] + 1 // predecesor del nodo
         << " ";  
  }
  cout << endl;
  cout << "Distancias: ";
  for (unsigned j = 0; j < n; ++j) {
    cout << d[j] << " "; // distancia a nodo actual
  }
  cout << endl << endl;
}

void GRAFO::Kruskal() {
    // Crear un vector de aristas pesadas con todos los arcos del grafo
    vector<AristaPesada> Aristas;
    Aristas.reserve(m); 

    for (unsigned i = 0; i < n; i++) {
        for (unsigned j = 0; j < LS[i].size(); j++) {
            // Para evitar aristas duplicadas en grafos no dirigidos
            if (i < LS[i][j].j) {
                Aristas.push_back({i, LS[i][j].j, LS[i][j].c});
            }
        }
    }

    // Vector para almacenar las componentes conexas
    vector<unsigned> Raiz(n);
    for (unsigned i = 0; i < n; i++) {
        Raiz[i] = i; // Inicialmente cada nodo está en su propia componente conexa
    }

    bool tieneCiclo = false; // add
    int pesoMST = 0;
    int cont = 0; // Contador de aristas en el MST
    unsigned head = 0; // Índice de la siguiente arista candidata

    // Bucle principal del algoritmo de Kruskal
    do {
        // Ponemos en la posicion head del vector Aristas la arista de menor coste.
        // Para ello recorremos las aristas desde la posicion head+1 hasta la n-1 intercambiando la de menor coste
        // con la que esta en la posicion head, salvo que head ya apunte a la ultima arista (la de la posicion n-1)
        if (head < m - 1) {
            unsigned minIndex = head;
            for (unsigned i = head + 1; i < m; i++) {
                // Si el peso de la arista en la posicion head es > que el peso de la arista en la posicion i, entonces
                // intercambiamos Aristas[i] y Aristas[head]
                if (Aristas[i].peso < Aristas[minIndex].peso) { // cambio a > para decreciente pero no me toma negativos
                    minIndex = i;
                }
            }
            auto temp = Aristas[head];
            Aristas[head] = Aristas[minIndex];
            Aristas[minIndex] = temp;

        }

        // miramos si los extremos de la arista en la posicion head estan ccxas distintas, y si es asi, metemos esa arista en el
        // MST fusionando las ccxas, es decir, unificando etiquetas
        if (Raiz[Aristas[head].extremo1] != Raiz[Aristas[head].extremo2]) { // control de ciclo
            unsigned kill = Raiz[Aristas[head].extremo1];
            // para todo k desde 0 hasta n-1, hacer
            // si raiz[k] == kill entonces raiz[k] = Raiz[Aristas[head].extremo2];
            for (unsigned k = 0; k < n; k++) {
                if (Raiz[k] == kill) {
                    Raiz[k] = Raiz[Aristas[head].extremo2];
                }
            }

            // incrementamos el contador de aristas en el MST
            cont++;

            // mostramos la arista incorporada al MST
            cout << "Arista número " << cont << " incorporada (" << Aristas[head].extremo1 + 1 << ", " << Aristas[head].extremo2 + 1 << "), con peso " << Aristas[head].peso << endl;

            // actualizamos peso del MST
            pesoMST += Aristas[head].peso;
        }
        else {
          tieneCiclo = true;
        }

        // pasamos a la siguiente arista
        head++;

    } while (cont < (n - 1) && head < m);
      
    if (cont == n -1 ) {
        cout << pesoMST << endl;
    }
    else {
      cout << " No tiene arbol generador" << endl;
    }

    //cout << "Peso total del MST: " << pesoMST << endl;
}

void pressanykey() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpiar el búfer de entrada
  std::cin.get(); 
}

void clrscr() {
  system("clear");
}