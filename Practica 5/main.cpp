
#include <string.h>

#include "grafo.h"

void menu(unsigned dirigido, char &opcion)
// Expresion del menu de opciones segun sea un grafo dirigido o no dirigido
{
  cout << "Optimiza!cion en Grafos,  " << endl;
  cout << "c. [c]argar grafo desde fichero" << endl;
  if (dirigido == 0)  // Grafo no dirigido
  {
    cout << "i. Mostrar [i]nformacion basica del grafo" << endl;
    cout << "a. Mostrar la lista de [a]dyacencia del grafo" << endl;
    cout << "y. Mostrar la matriz de ad[y]acencia del grafo" << endl;
    cout << "k. Mostrar arbol generador minimo coste, [k]ruskal. " << endl;
  } else {
    cout << "i. Mostrar [i]nformacion basica del grafo" << endl;
    cout << "s. Mostrar la lista de [s]ucesores del grafo" << endl;
    cout << "p. Mostrar la lista de [p]redecesores del grafo" << endl;
    cout << "d. Mostrar algoritmo de PDM" << endl;
    // Aqu� se a�aden m�s opciones al men� del grafo dirigido
  };
  // añado los recorridos
  cout << "m. Realizar un recorrido en a[m]plitud del grafo desde un nodo por "
          "sucesores"
       << endl;
  cout << "r. Realizar un recorrido en p[r]ofundidad del grafo desde un nodo "
          "por sucesores"
       << endl;
  cout << "q. Finalizar el programa" << endl;
  cout << "Introduce la letra de la accion a ejecutar  > ";
  cin >> opcion;
};

// El principal es simplemente un gestor de menu, diferenciando acciones para
// dirigido y para no dirigido, y un men� inicial si no hay un grafo cargado
int main(int argc, char *argv[]) {
  int error_apertura;
  char nombrefichero[85], opcion;
  clrscr();
  // Si tenemos el nombre del primer fichero por argumento, es una excepcion, y
  // lo intentamos cargar, si no, lo pedimos desde teclado
  if (argc > 1) {
    cout << "Cargando datos desde el fichero dado como argumento" << endl;
    strcpy(nombrefichero, argv[1]);
  } else {
    cout << "Introduce el nombre completo del fichero de datos" << endl;
    cin >> nombrefichero;
  };
  GRAFO G(nombrefichero, error_apertura);
  if (error_apertura == 1) {
    cout << "Error en la apertura del fichero desde argumento: revisa nombre y "
            "formato"
         << endl;
    return 1;  // return
    pressanykey();
    clrscr();
  } else {
    cout << "Grafo cargado desde el fichero " << nombrefichero << endl;
    // pressanykey();
    // clrscr();
    do {
      menu(G.Es_dirigido(), opcion);
      switch (opcion) {
        case 'c':
          clrscr();
          cout << "Introduce el nombre completo del fichero de datos" << endl;
          cin >> nombrefichero;
          G.actualizar(nombrefichero, error_apertura);
          if (error_apertura == 1) {
            cout << "Error en la apertura del fichero: revisa nombre y formato "
                    ": puedes volver a intentarlo"
                 << endl;
          } else {
            cout << "Fichero cargado correctamente desde " << nombrefichero
                 << endl;
          };
          pressanykey();
          clrscr();
          break;

        case 'i':
          clrscr();
          cout << "Grafo cargado desde " << nombrefichero << endl;
          G.Info_Grafo();
          pressanykey();
          clrscr();
          break;

        case 's':
          clrscr();
          G.Mostrar_Listas(0);
          pressanykey();
          clrscr();
          break;

        case 'p':
          clrscr();
          G.Mostrar_Listas(1);
          pressanykey();
          clrscr();
          break;

        case 'm':
          clrscr();
          G.RecorridoAmplitud();
          pressanykey();
          clrscr();
          break;

        case 'a':
          clrscr();
          G.Mostrar_Listas(0);
          pressanykey();
          clrscr();
          break;

        case 'y':
          clrscr();
          G.Mostrar_Matriz();
          pressanykey();
          clrscr();
          break;

        case 'k':
          clrscr();
          G.Kruskal();
          pressanykey();
          clrscr();
          break;

        case 'd':
          clrscr();
          G.PDM();
          pressanykey();
          clrscr();
          break;

        case 'r':
          clrscr();
          unsigned nodo;
          // cout << "Introduce el nodo inicial: ";
          // cin >> nodo;
          vector<LA_nodo> L;
          vector<unsigned> d;
          vector<bool> visitado;
          vector<unsigned> pred;
          G.RecorridoProfundidad(nodo, L, d, visitado, pred);
          pressanykey();
          clrscr();
          break;
      }

    } while (opcion != 'q');
  }
  cout << "Fin del programa" << endl;
  return (0);
};
