#include <ctime>
#include <iostream>
using namespace std;

//  Grupo conformado por:
//  Juan Berrizbeitia Ci:33003278
//  Luis Grippo Ci: 32534822
//  Samantha Marquez Ci: 32514068
//  Gabriel Khajikian Ci: 32461173
//  Algoritmos y Estructuras de Datos
//  Profesor: Yisheng Leon
//  Operacion Yggdrasil CRUD

// Todos los TDA

struct NodoEscudo {
  int identificador_escudo; // Estos identificadores son escenciales para el
                            // manejo de estos datos en sus actualizadores
  // Son los factores de unicidad
  int tipo_escudo;
  int absorcion;
  NodoEscudo *siguiente_escudo;
};

struct PilaDefensa {
  NodoEscudo *tope;
};
// Cada escudo tipo FISICO tendra una absorcion de 30pts
// Cada escudo tipo ANTIPLASMA tendra una absorcion de 40pts
// Cada escudo de tipo ESPEJO tendra una absorcion de 50pts

// Mas adelante, en el struct, se van a fijar como solo se inicializa el
// operativo con la PilaDefensa, el NodoEscudo no forma parte del struct La idea
// es que PilaDefensa t=contiene al puntero TOPE que apunta al primer escudo de
// la pila. Es un struct que tiene a un puntero que apunta a otro struct Luis se
// va a encargar de inicializar estos escudos a cada operativo dependiendo de la
// clase elegida, y que la PilaDefensa del operativo apunte al primer escudo de
// la pila y que los demas escudos se apunten entre si

// JUGGERNAUT nace con 3 escdos FISICOS
// EJECUTOR nace con 1 escudo ANTIPLASMA3
// HAACKER nace sin escudos.

// Otra cosa importante es que cada escudo va a tener un identificador diferente
// (me enacnatria que se un int generado aleatoriamente) para poder manipularlos
// Todavia no se como podriamos inicializar ese identificador

//Consideraciones de flujo de batalla:
//El uso de habilidades se hace luego de la insercion

struct NodoMunicion {
  int identificador_municion;
  int tipo_blast;
  NodoMunicion *sig_municion;
};

struct ColaArsenal {
  NodoMunicion *frente;
  NodoMunicion *final;
};

// Estructura del operativo
// Aqui se registrara todo acerca de los personajes.
// Sus datos principales y lo mas importante, su id.
struct Operativo {
  int id_numerico;
  int hp_base;
  int bando_perteneciente;
  int clase_tipo;

  PilaDefensa *escudos_tope;
  ColaArsenal *municiones_tope;

  // Aclaracion importante: El ID del personaje sera el uncio dato INMUTABLE en
  // el sistema MEdiante este dato se organiza la insercion, el guardado, se
  // balanceara el arbol en futura entrega Es el parametro mas escencial del
  // sistema de datos. Por eso hemos decidido que sera lo unico inmutable

  Operativo(int id, int bando, int hp, int clase) {
    id_numerico = id;
    bando_perteneciente = bando;
    hp_base = hp;
    clase_tipo = clase;
    // Se crean en el Heap
    escudos_tope = new PilaDefensa();
    escudos_tope->tope = NULL;

    municiones_tope = new ColaArsenal();
    municiones_tope->frente = NULL;
    municiones_tope->final = NULL;
  } // Esto es un constructor. Le asigna directamente al personaje las variables
    // que recibe, para no estar peleando luego con agregarlos manualmente

  ~Operativo() {
    while (escudos_tope->tope != NULL) {
      NodoEscudo *temporal1 = escudos_tope->tope;
      escudos_tope->tope = temporal1->siguiente_escudo;
      delete temporal1;
      temporal1 = NULL;
    }
    while (municiones_tope->frente != NULL) {
      NodoMunicion *temporal1 = municiones_tope->frente;
      municiones_tope->frente = temporal1->sig_municion;
      delete temporal1;
      temporal1 = NULL;
    }
    municiones_tope->final = NULL;
    delete escudos_tope;
    delete municiones_tope;
  } // Este es el DESTRUCTOR
};

struct NodoBTree {
  Operativo* ocupantes[3+1];
  NodoBTree* descendientes[4+1]; //Ambos arrays tienen un ocupante EXTRA para manejar el rebose y el split
  int contador;
};

struct Vocero_Recursion {
  bool hubo_split = false;
  Operativo *mediana_a_subir = NULL;
  NodoBTree *hijo_derecho = NULL;
};

// Asignaciones universales:
// Trabajaremos todos los tipos de datos por ENTEROS. Es mucho mas eficiente que
// trabajar por strings y podemos simplemente comparar enteros En funciones de
// asignacion o actualizacion Bandos:
//  NEON es int 1
//  OMEGA es int 2
// Clases:
//  JUGGERNAUT es int 1
//  EJECUTOR es int 2
//  HACKER es int 3
// Escudos:
//  FISICO es int 1
//  ANTIPLASMA es int 2
//  ESPEJO es int 3

// Lista enlazada principal
// Aqui se guardaran todos los personajes
// El metodo de ordenacion es creciente. De menor a mayor.

//

// Operativo *lista_personajes1 = NULL; 

//

NodoBTree *Arbol = NULL;

void blindaje_entrada(int &entrada) { // Este blindaje verifica si los datos
                                      // introducidos en el cin
  // Concuerdan con el tipo de dato pedido o el rango de ese dato
  // Se usara para que el programa no aborte automaticamente luego de un dato
  // mal introducido
  cin >> entrada;
  while (cin.fail()) {
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "[!] INTERFERENCIA DETECTADA: Por favor, ingrese unicamente "
            "valores numericos.>> "
         << endl;
    cin >> entrada;
  }
}

/*
bool lista_personajes_esta_vacia() { // OBSOLETA: reemplazada por arbol_esta_vacio()
  if (lista_personajes1 == NULL) {
    return true;
  } else {
    return false;
  }
}
*/

bool pila_escudos_esta_vacia(Operativo *op) { // Verificacion basica
  if (op->escudos_tope->tope == NULL) {
    return true;
  } else {
    return false;
  }
}

bool cola_municiones_esta_vacia(Operativo *op) {
  // lo que importa es si el frente de la cola del operativo tiene algo
  if (op->municiones_tope->frente == NULL) {
    return true;
  } else {
    return false;
  }
}

void pop_pila_escudos(Operativo *operativo_recibido) { // Primitiva
  // No hace falta chequeo de si esta vacia o no ya que estara dentro de
  // funciones ya previamente chequeadas
  // Esta, a diferencia del otro pop, saca el nodo y lo elimina automaticamente.
  // Escencial para los algoritmos posteriores de yuso de escudos
  // Tambien para el eliminado del sistema

  NodoEscudo *temporal1 = operativo_recibido->escudos_tope->tope;
  operativo_recibido->escudos_tope->tope = temporal1->siguiente_escudo;
  delete temporal1;
  temporal1 = NULL;
}

void push_pila_escudos(Operativo *operativo_recibido, // Primitiva
                       NodoEscudo *nuevo_escudo) {
  // Insercion simple en pila de escudos

  if (operativo_recibido->escudos_tope->tope == NULL) {
    operativo_recibido->escudos_tope->tope = nuevo_escudo;
    nuevo_escudo->siguiente_escudo = NULL;
  } else {
    nuevo_escudo->siguiente_escudo = operativo_recibido->escudos_tope->tope;
    operativo_recibido->escudos_tope->tope = nuevo_escudo;
  }
}

void pop_cola_municiones(Operativo *operativo_recibido) { // Primitiva
  // A diferencia de los otros pops, este saca el nodo y lo elimina. Este
  // algoritmo es escencial para la eliminacion de la cola Y posteriormente para
  // acciones como disparos

  NodoMunicion *temporal1 = operativo_recibido->municiones_tope->frente;
  operativo_recibido->municiones_tope->frente = temporal1->sig_municion;
  delete temporal1;
  temporal1 = NULL;

  if (operativo_recibido->municiones_tope->frente == NULL) {
    operativo_recibido->municiones_tope->final = NULL;
  }
}

void push_cola_municiones(Operativo *operativo_recibido, // Primitiva
                          NodoMunicion *nuevo_municion) {
  // Insercion simple de un nodo por frente de la cola

  if (operativo_recibido->municiones_tope->frente == NULL) {
    operativo_recibido->municiones_tope->frente = nuevo_municion;
    operativo_recibido->municiones_tope->final = nuevo_municion;
  } else {
    operativo_recibido->municiones_tope->final->sig_municion = nuevo_municion;
    operativo_recibido->municiones_tope->final = nuevo_municion;
  }
}

NodoBTree *nueva_estacion() { //Creacion de una nueva estacion o "nodo" del arbol. Se settean todos sus campos a NULL

  NodoBTree *estacion_nueva = new NodoBTree();

  for (int i = 0; i < 4; i++) {
    estacion_nueva->ocupantes[i] = NULL;
  }

  for (int i = 0; i < 5; i++) {
    estacion_nueva->descendientes[i] = NULL;
  }

  estacion_nueva->contador = 0;
  return estacion_nueva;

}

bool arbol_esta_vacio() {
  return (Arbol == NULL);
}

bool estacion_es_hoja(NodoBTree *nodo) { //Por logica de arbol, si el nodo no tiene un descendiente, no tiene ninguno
  if (nodo == NULL) {
    return true;
  }
  if (nodo->descendientes[0] == NULL) {
    return true;
  }
  else {
    return false;
  }
}

int posicion_relativa(NodoBTree *estacion, int id_buscar) {
  int i = 0;
  while (i < estacion->contador && estacion->ocupantes[i]->id_numerico < id_buscar) {
    i++;
  }
  return i; //Retorna exactamente el I del hijo por donde bajar
}

int buscar_posicion(NodoBTree *estacion, int id_buscar) {
  int i = 0;
  while (i < estacion->contador && estacion->ocupantes[i]->id_numerico != id_buscar) {
    i++;
  }
  if (i >= estacion->contador) {
    return -1;
  }
  else {
    return i;
  }
}

// La lista de las municiones es una COLA FIFO

/*
void ordenacion_de_ids(Operativo *nuevo) { // OBSOLETA: reemplazada por insercion BTree
  if (lista_personajes_esta_vacia() == true) {
    lista_personajes1 = nuevo;
    nuevo->sig_operativo = NULL;
    return;
  }
  if (nuevo->id_numerico < lista_personajes1->id_numerico) {
    nuevo->sig_operativo = lista_personajes1;
    lista_personajes1 = nuevo;
    return;
  }
  Operativo *actual = lista_personajes1;
  while ((actual->sig_operativo != NULL) &&
         (actual->sig_operativo->id_numerico < nuevo->id_numerico)) {
    actual = actual->sig_operativo;
  }
  nuevo->sig_operativo = actual->sig_operativo;
  actual->sig_operativo = nuevo;
}
*/

Operativo *buscar_en_arbol(NodoBTree *nodo_actual, int id_buscar) {

  if (nodo_actual == NULL) {
    return NULL;
  }
  
  int i = posicion_relativa(nodo_actual, id_buscar);

  if (i < nodo_actual->contador && nodo_actual->ocupantes[i]->id_numerico == id_buscar) {
    return nodo_actual->ocupantes[i];
  }

  if (estacion_es_hoja(nodo_actual)) {
    return NULL;
  }

  return buscar_en_arbol(nodo_actual->descendientes[i], id_buscar);

}

Operativo *busqueda_de_operativo(int id_buscar) {
  return buscar_en_arbol(Arbol, id_buscar);
}

void insertar_en_estacion(NodoBTree *estacion, Operativo *insercion, NodoBTree *hijo_derecho) { //Maneja ambos casos de una vez (La reinsercion en desenpilado reasignando hijo derecho y la insercion comun)
  int i = estacion->contador - 1;
  while (i >= 0 && insercion->id_numerico < estacion->ocupantes[i]->id_numerico) {
    estacion->ocupantes[i+1] = estacion->ocupantes[i];
    estacion->descendientes[i+2] = estacion->descendientes[i+1];
    i--;
  }
  estacion->ocupantes[i+1] = insercion;
  estacion->descendientes[i+2] = hijo_derecho;
  estacion->contador++;
}

Vocero_Recursion split(NodoBTree *estacion) {
  Vocero_Recursion devuelta;
  devuelta.hubo_split = true;

  NodoBTree *estacion_nueva = nueva_estacion();
  devuelta.mediana_a_subir = estacion->ocupantes[2]; //La mediana en 3-4 siempre es [2]
  estacion_nueva->ocupantes[0] = estacion->ocupantes[3]; //Posicion de rebose
  estacion_nueva->descendientes[0] = estacion->descendientes[3];
  estacion_nueva->descendientes[1] = estacion->descendientes[4];
  estacion->descendientes[3] = NULL;
  estacion->descendientes[4] = NULL;
  estacion->ocupantes[3] = NULL;
  estacion->contador = 2;
  estacion_nueva->contador++;
  devuelta.hijo_derecho = estacion_nueva;

  return devuelta;
}

Vocero_Recursion insercion_recursiva (NodoBTree *estacion, Operativo *insertar) {
  Vocero_Recursion resultado;
  if (estacion_es_hoja(estacion)) {
    insertar_en_estacion(estacion, insertar, NULL);
  }
  else {
    int i = posicion_relativa(estacion, insertar->id_numerico);
    Vocero_Recursion abajo = insercion_recursiva(estacion->descendientes[i], insertar);

    if (abajo.hubo_split) {
      insertar_en_estacion(estacion, abajo.mediana_a_subir, abajo.hijo_derecho);
    }

  }
  if (estacion->contador > 3) { //Unico verificador de SPLIT. >3 arbitrario ya que es el maximo de operativos permitidos en estacion
    resultado = split(estacion);
  }
  return resultado;
}

void ordenacion_en_arbol(Operativo *insertar) {
  if (arbol_esta_vacio()) {
    NodoBTree *estacion_nueva = nueva_estacion();
    Arbol = estacion_nueva;
    insertar_en_estacion(estacion_nueva, insertar, NULL);
    return;
  }

  if (busqueda_de_operativo(insertar->id_numerico)) {
    delete insertar;
    insertar = NULL;
    return;
  }

  Vocero_Recursion splitear_raiz = insercion_recursiva(Arbol, insertar);

  if (splitear_raiz.hubo_split) {
    NodoBTree *nueva_raiz = nueva_estacion();
    nueva_raiz->ocupantes[0] = splitear_raiz.mediana_a_subir;
    nueva_raiz->descendientes[0] = Arbol;
    nueva_raiz->descendientes[1] = splitear_raiz.hijo_derecho;
    nueva_raiz->contador++;
    Arbol = nueva_raiz;
  }

}

bool hubo_underflow(NodoBTree *estacion) {
  return (estacion->contador < 1);
}

NodoBTree *buscar_estacion(NodoBTree *estacion, Operativo *referencia) { //A que estacion pertenece x operativo??
  if (arbol_esta_vacio()) {
    return NULL;
  }
  if (estacion_es_hoja(estacion)) { //Llegue a hoja y no he conseguido aun. Hago ultima busqueda en hoja
    int i = 0;
    while (i < estacion->contador && estacion->ocupantes[i]->id_numerico != referencia->id_numerico) {
      i++;
    }
    if (i >= estacion->contador) {
      return NULL;
    }
    else {
      return estacion;
    }
  }

  //Verifico en estacion actual
  int i = buscar_posicion(estacion, referencia->id_numerico);
  if (i == -1) {
    int k = posicion_relativa(estacion, referencia->id_numerico);
    return buscar_estacion(estacion->descendientes[k], referencia);
  }
  else {
    return estacion;
  }
}

NodoBTree *buscar_hoja_mayores(NodoBTree *estacion) {//Busqueda de hoja en base a una sola bajada direccional 
  if (estacion_es_hoja(estacion)) {
    return estacion;
  }
  else {
    int k = estacion->contador;
    return buscar_hoja_mayores(estacion->descendientes[k]);
  }
}

Operativo *busqueda_predecesor(Operativo *a_borrar) {
  NodoBTree *estacion_perteneciente = buscar_estacion(Arbol, a_borrar);
  if (estacion_perteneciente == NULL) { //Si no se encontro, simplemente no hay por donde buscar
    //Creo que es innecesario poner un cout de aviso ya que esta es un helper
    return NULL;
  }
  if (estacion_es_hoja(estacion_perteneciente)) { //Si la estacion es hoja, no hay predecesor el cual buscar. Simplemente se
    // elimina en la estacion
    return NULL;
  }
  //En que posicion esta mi operativo??
  int i = buscar_posicion(estacion_perteneciente, a_borrar->id_numerico);
  if (i == -1) {
    return NULL;
  }
  //Busqueda de la hoja
  estacion_perteneciente = estacion_perteneciente->descendientes[i];
  NodoBTree *estacion_hoja = buscar_hoja_mayores(estacion_perteneciente);

  return estacion_hoja->ocupantes[estacion_hoja->contador - 1];
}

void borrado_en_hoja(NodoBTree *estacion, int id_a_borrar, bool liberar, bool hoja) {
  //Verificacion de hoja (Creo que sera innecesario)
  if (hoja) {
    if (!estacion_es_hoja(estacion)) {
        return;
    }
  }
  int i = buscar_posicion(estacion, id_a_borrar);
  if (i == -1) return;
  //Es el operativo a borrar el ultimo elemento de la estacion?
  if (i + 1 >= estacion->contador) {
    //De ser asi, solo lo elimino
    if (liberar) {delete estacion->ocupantes[i];}
    estacion->contador--;
  }
  else {
    //muevo el/los siguientes
    if (liberar) {delete estacion->ocupantes[i];}
    estacion->contador--;
    for (int k = i; k < estacion->contador; k++) {
      estacion->ocupantes[k] = estacion->ocupantes[k+1];
    }
  }
}

void borrado_interno(Operativo *a_borrar) {
  //En que estacion esta?
  NodoBTree *estacion_perteneciente_operativo = buscar_estacion(Arbol, a_borrar);
  if (estacion_perteneciente_operativo == NULL) return;
  //Quien es su predecesor??
  Operativo *predecesor = busqueda_predecesor(a_borrar);
  if (predecesor == NULL) return;
  //Ojo al orden de lineas
  int i = buscar_posicion(estacion_perteneciente_operativo, a_borrar->id_numerico);
  if (i == -1) return;
  //Mi operativo a borrar esta en posicion i
  NodoBTree *estacion_perteneciente_predecesor = buscar_estacion(Arbol, predecesor);
  int k = buscar_posicion(estacion_perteneciente_predecesor, predecesor->id_numerico);
  if (k == -1) return;
  //Mi predecesor esta en posicion k
  delete a_borrar;
  estacion_perteneciente_operativo->ocupantes[i] = predecesor;
  borrado_en_hoja(estacion_perteneciente_predecesor, predecesor->id_numerico, false, false);
  //Como el predecesor es siempre el ultimo de la hoja, realmente no se ejecuta ningun corrido de datos en la hoja
  //Importante pararse aqui a explicar lo siguiente:
  //En el borrado interno, lo que se hace es sobreescribir la posicion del nodo a borrar con su predecesor
  //Se borra, el nodo a borrar, y el predecesor toma su posicion, pero, sucede lo siguiente:
  //En el nodo origen del predecesor (donde estaba antes), NO se borra al predecesor, ya que estarias borrando el nodo que acabas de reemplazar.
  //En cambio, simplemente se obsoletiza, restandole al contador para asi decirle al programa: ese operativo ya no existe en ese array, ya que
  //El contador es la "unica verdad". Por eso el flag de "false" donde esta el booleano de liberar, ya que le avisa a borrado en hoja
  //que no se debe liberar ese operativo
}

bool puede_prestar(NodoBTree *estacion) {
  return (estacion->contador > 1);
}

void insertar_hijo_borrow(NodoBTree *padre, int indice_pobre, int derecha_o_izquierda) { //1 por DERECHA, 2 por IZQUIERDA
  if (derecha_o_izquierda == 1) {
    //Es borrow por derecha
    //Busco el mayor elemento de descendientes[indice_pobre]
    int i = padre->descendientes[indice_pobre]->contador; 
    NodoBTree *hijo_pasar = padre->descendientes[indice_pobre + 1]->descendientes[0];
    padre->descendientes[indice_pobre]->descendientes[i+1] = hijo_pasar; //i+1 para no pisar el hijo ya existiente (1 hijo)
    int k = 0;
    //Corro todos los hijos restantes que quedarom MAS ALLA del hijo trasladado
    while (k < padre->descendientes[indice_pobre+1]->contador+1) {
      padre->descendientes[indice_pobre+1]->descendientes[k] = padre->descendientes[indice_pobre+1]->descendientes[k+1];
      k++;
      //Este ciclo ya sobreescribe y desenlaza el array del nodo transferido
    }
  }
  else {
    //Es borrow por izquierda
    //Aqui el nodo prestante es padre->descendientes[indice_pobre - 1]
    //Y pasa el ULTIMO hijo del nodo prestante, que cae en el PRIMER puesto del nodo recibiente
    //como el donante
    //dona su ultimo hijo, no hace falta reorganizar hijos
    //Pero si desenlazar, ojo
    //Busco el ultimo elemento del donante
    int i = padre->descendientes[indice_pobre - 1]->contador-1;
    NodoBTree *hijo_pasar = padre->descendientes[indice_pobre-1]->descendientes[i+1];
    //Muevo los hijos existentes en nodo underflow un lugar exacto para asi hacerle espacio al hijo nuevo
    int k = padre->descendientes[indice_pobre]->contador;
    while (k >= 0) {
      padre->descendientes[indice_pobre]->descendientes[k+1] = padre->descendientes[indice_pobre]->descendientes[k];
      k--;
    }
    padre->descendientes[indice_pobre]->descendientes[0] = hijo_pasar;
    padre->descendientes[indice_pobre-1]->descendientes[i+1] = NULL;
  }
}

void borrow_derecho(NodoBTree *padre, int indice_nodo_pobre, bool hoja) { //Mi hermano de la derecha puede prestar??
  //El nodo pobre es padre->descendientes[indice_nodo_pobre]
  if (indice_nodo_pobre >= padre->contador) {
    return;
  }
  if (padre->descendientes[indice_nodo_pobre + 1] == NULL) {
    return;
  }
  if (!puede_prestar(padre->descendientes[indice_nodo_pobre + 1])) {
    return;
  } 
  //No es null y si puede prestar
  //Baja el separador y toma su lugar el menor del nodo a prestar
  //El separador es naturalmente padre->ocupantes[indice_nodo_pobre]
  Operativo *a_subir = padre->descendientes[indice_nodo_pobre + 1]->ocupantes[0];
  if (hoja) { //Hoja?? Inseramos sin mover hijos
    padre->descendientes[indice_nodo_pobre]->ocupantes[0] = padre->ocupantes[indice_nodo_pobre];
  }
  else { //No hoja?? Toca mover hijos
    insertar_hijo_borrow(padre, indice_nodo_pobre, 1);
    padre->descendientes[indice_nodo_pobre]->ocupantes[0] = padre->ocupantes[indice_nodo_pobre];
  }
  padre->descendientes[indice_nodo_pobre]->contador++;
  padre->ocupantes[indice_nodo_pobre] = a_subir;
  //Ahora la limpieza del nodo donante (cerrar el hueco)
  //Como es borrow derecho, sale el primer ocupante del derecho, por ende, hay que cerrar hueco
  borrado_en_hoja(padre->descendientes[indice_nodo_pobre + 1], a_subir->id_numerico, false, false);
}

void borrow_izquierdo(NodoBTree *padre, int indice_nodo_pobre, bool hoja) {
  if (indice_nodo_pobre <= 0) {
    return;
  }
  if (padre->descendientes[indice_nodo_pobre - 1] == NULL) {
    return;
  }
  if (!puede_prestar(padre->descendientes[indice_nodo_pobre - 1])) {
    return;
  }
  //Puede prestar
  //El separador es padre->ocupantes[indice_nodo_pobre-1] 
  //Busco ultimo elemento de nodo donante
  int i = padre->descendientes[indice_nodo_pobre-1]->contador - 1;
  Operativo *a_subir = padre->descendientes[indice_nodo_pobre - 1]->ocupantes[i];
  if (hoja) { //Estamos en hoja?? Simplemente insertamos sin mover hijos
    padre->descendientes[indice_nodo_pobre]->ocupantes[0] = padre->ocupantes[indice_nodo_pobre-1];
  }
  else { //No hoja, debo reposicionar hijos
    insertar_hijo_borrow(padre, indice_nodo_pobre, 2);
    padre->descendientes[indice_nodo_pobre]->ocupantes[0] = padre->ocupantes[indice_nodo_pobre-1];
  }
  padre->descendientes[indice_nodo_pobre]->contador++;
  padre->ocupantes[indice_nodo_pobre-1] = a_subir;
  //Como estoy pasando el ultimo dato del nodo donante, no hace falta tapar hueco
  //Pero si nullearlo
  padre->descendientes[indice_nodo_pobre-1]->ocupantes[i] = NULL;
  padre->descendientes[indice_nodo_pobre-1]->contador--;
  //En ambos casos, el elemento donado cae en la posicion 0, ya que por definicion, al estar en underflow, no tiene
  //ningun elemento
}

void merge_estaciones(NodoBTree *padre, int indice_izquierda) { //Aqui notas importantes
  //En el merge, el nodo absorbiente siempre sera el izquierdo. Si el izquierdo tiene hermano derecho, se absorbe a ese hermano
  //Pero si no tiene, el izquierdo pasa a ser su hermano izquierdo (se transfiere identidad) y el izquierdo absorbe al pobre
  //Al fusionar nodos, el separador del padre baja a vivir en el nuevo nodo fusionado
  //abriendo asi la posibilidad de el padre tambien quedar en underflow, subiendo asi el problema hasta
  //potencialmente la raiz, donde en caso de quedar en underflow, el arbol pierde un nivel

  //Primero es necesario el chequeo de casos

  //caso A
  if (padre->descendientes[indice_izquierda + 1] != NULL) {
    NodoBTree *izquierda = padre->descendientes[indice_izquierda];
    NodoBTree *derecha = padre->descendientes[indice_izquierda+1];

    izquierda->ocupantes[izquierda->contador] = padre->ocupantes[indice_izquierda];//separador
    //Esta funcion tambien contempla el traslado de hijos
    izquierda->contador++;

    //Ahora absorbemos todos los ocupantes del derecho
    //Pero decir todos no es acertado, ya que como se podra dilucidar logicamente, la condicion sine qua non del merge (en esta arquitectura)
    //es que todos los hermanos tengan exactamente 1 hijo menos el pidiente que es el unico con 0. Sin ambiguedades.
    //Se pasa unicamente el unico ocupante del nodo derecho que esta en posicion [0]

    izquierda->ocupantes[izquierda->contador] = derecha->ocupantes[0];
    izquierda->contador++;
    //Ahora, hay que trasladar los hijos del nodo derecho al izquierdo
    //los cuales son exactamente 2
    //pero ojo, izquierda al comenzar teniendo 0 ocupantes puede tener 1 hijo al tratarse de nodo interno
    //contemplamos ese caso
    for (int i = 0; i < 2; i++) {
      izquierda->descendientes[i+1] = derecha->descendientes[i];
      derecha->descendientes[i] = NULL;
    }
    //Hecho el traslado de ocupantes e hijos, se elimina el nodo derecho
    delete derecha;
    derecha = NULL;

    //Se corren todos los ocupantes del padre ya que el separador bajo
    borrado_en_hoja(padre, padre->ocupantes[indice_izquierda]->id_numerico, false, false);
    //Corro todos los punteros proximos al del hijo fallecido para asi obsoletizarlo
    for (int i = indice_izquierda+1; i < padre->contador+1; i++) { //contador + 1 ya que previamente en borrado se bajo el contador (creo)
      padre->descendientes[i] = padre->descendientes[i+1];
    }
    padre->descendientes[padre->contador+1] = NULL;
  }

  //Caso B
  else {
    NodoBTree *izquierda = padre->descendientes[indice_izquierda-1];
    NodoBTree *derecha = padre->descendientes[indice_izquierda];
    //Ahora, como el derecha es el pobre, no hace falta transferencia de ocupantes, solo del hijo
    izquierda->ocupantes[izquierda->contador] = padre->ocupantes[indice_izquierda-1];
    izquierda->contador++;
    //Traslado de hijos. Izquierda tiene 2 hijos. Se debe trasladar el unico que tiene derecha en la posicion 2
    izquierda->descendientes[2] = derecha->descendientes[0];
    //Eliminacion del nodo derecho
    delete derecha;
    derecha = NULL;
    //Se corren todos los ocupantes del padre
    borrado_en_hoja(padre, padre->ocupantes[indice_izquierda-1]->id_numerico, false, false);
    //Se obsoletiza el puntero del hijo fallecido (es el ultimo)
    padre->descendientes[padre->contador+1] = NULL; //+1 ya que en borrado se resto el contador
  }
}

bool resolver_underflow(NodoBTree *padre, int indice_pobre) {
  if (padre == NULL) {
    return false;
  }

  if (padre->descendientes[indice_pobre+1] != NULL && puede_prestar(padre->descendientes[indice_pobre+1])) {
    bool es_hoja = estacion_es_hoja(padre->descendientes[indice_pobre]);
    borrow_derecho(padre, indice_pobre, es_hoja);
    return (padre->contador < 1);
  }
  else if (indice_pobre>0 && padre->descendientes[indice_pobre-1] != NULL && puede_prestar(padre->descendientes[indice_pobre-1])) {
    bool es_hoja = estacion_es_hoja(padre->descendientes[indice_pobre]);
    borrow_izquierdo(padre, indice_pobre, es_hoja);
    return (padre->contador < 1);
  }
  else {
    merge_estaciones(padre, indice_pobre);
    return (padre->contador < 1);
  }
}

bool propagar_underflow_derecha(NodoBTree *estacion) {//Helper para el caso del borrado interno (necesito saber donde esta el predecesor que se borro
  //y evaluar underflow en esa hoja)
  if (estacion_es_hoja(estacion)) {
    return hubo_underflow(estacion);
  }
  int indice = estacion->contador; //Baja por el mas a la derecha
  bool hijo_underflow = propagar_underflow_derecha(estacion->descendientes[indice]);
  if (hijo_underflow) {
    resolver_underflow(estacion, indice); //en var indice ya que el predecesor seria el mas a la derecha
  }
  return hubo_underflow(estacion);
}

bool borrar_recursivo(NodoBTree *estacion, int id) {
  if (estacion_es_hoja(estacion)) {
    borrado_en_hoja(estacion, id, true, true);
    return hubo_underflow(estacion);
  }

  int i = buscar_posicion(estacion, id);
  if (i != -1) { //lo encontro en nodo actual (y el nodo actual no es hoja)
    borrado_interno(estacion->ocupantes[i]);
    bool hijo_underflow = propagar_underflow_derecha(estacion->descendientes[i]);
    if (hijo_underflow) {
      resolver_underflow(estacion, i);
    }
  }

  else {//No lo encontro en este nodo. Toca bajar
    int i = posicion_relativa(estacion, id);
    bool hijo_underflow = borrar_recursivo(estacion->descendientes[i], id);
    if (hijo_underflow) {
      resolver_underflow(estacion, i);
    }
  }

  return hubo_underflow(estacion); //Este es el ultimo chequeo. Si esto devuelve true, la raiz es underflow y se elimina un nivel

}

void eliminar_operativo(int id_eliminar) {
  if (Arbol == NULL) {
    cout << "[!] ERROR. NO HAY OPERATIVOS EN ARBOL." << endl;
    return;
  }

  Operativo *existe = buscar_en_arbol(Arbol, id_eliminar);
  if (existe == NULL) {
    cout << "[!] ERROR. EL OPERATIVO QUE DESEA ELIMINAR NO EXISTE" << endl;
    return;
  }

  borrar_recursivo(Arbol, id_eliminar);

  //Caso reduccion de nivel
  if (Arbol->contador == 0 && Arbol->descendientes[0] != NULL) {
    NodoBTree *vieja_raiz = Arbol;
    Arbol = Arbol->descendientes[0];
    vieja_raiz->descendientes[0] = NULL;
    delete vieja_raiz;
  }

  else if (Arbol->contador == 0 && estacion_es_hoja(Arbol)) {
    delete Arbol;
    Arbol = NULL;
  }

}

void insertar_escudos_en_pila(Operativo *personaje) { 
  // identificar cuantos y que escudos necesita segun su clase ya asignada
  int cantidad = 0;
  int tipo = 0;
  int absorcion = 0;

  switch (
      personaje
          ->clase_tipo) { // se hace un switch para las clases que son 1, 2 o 3
  case 1:                 // juggernatu
    cantidad = 3;
    tipo = 1; // fisico
    absorcion = 30;
    break;
  case 2: // ejecutor
    cantidad = 1;
    tipo = 2; // antiplasma
    absorcion = 40;
    break;
  case 3:         // hacker osea espectro
    cantidad = 0; // no tiene escudos iniciales
    break;
  }

  // crear y apilar los escudos, es decir, el push
  for (int i = 0; i < cantidad; i++) {
    NodoEscudo *nuevo_escudo = new NodoEscudo();

    // generamos un ID aleatorio para el escudo entre 1000 y 9999 para que se
    // puedan identificar a la hora de buscarlos
    nuevo_escudo->identificador_escudo = rand() % 9000 + 1000;
    nuevo_escudo->tipo_escudo = tipo;
    nuevo_escudo->absorcion = absorcion;
    push_pila_escudos(personaje, nuevo_escudo);
  }
}

void insertar_municiones_en_cola(Operativo *personaje) {
  // definir cuantas municiones recibe de 1 a 4 aleatoriamente
  int cantidad = (rand() % 4) + 1;

  for (int i = 0; i < cantidad; i++) {
    // creamos el nodo de la municion (el blast)
    NodoMunicion *nueva_municion = new NodoMunicion();

    // se le asigna un tipo de Blast aleatorio (del 1 al 5)
    //  1:laser, 2:EMP, 3:troyano, 4:disrupcion, 5:racimo
    nueva_municion->tipo_blast = (rand() % 5) + 1;
    nueva_municion->sig_municion = NULL;

    nueva_municion->identificador_municion =
        rand() % 9000 + 1000; // ID aleatorio para la municion

    push_cola_municiones(personaje, nueva_municion);
  }
}

/* Obsoleto
// Operativo *busqueda_de_operativo(int id_modificar) {
  Operativo *actual = lista_personajes1;
  while (actual != NULL) {
    if (actual->id_numerico == id_modificar) {
      return actual;
    }
    actual = actual->sig_operativo;
  }
  return NULL;
}
*/

void creacion_de_personaje() {
  int id = 0, bando = 0, clase = 0, hp = 0;

  cout << "REGISTRO DE NUEVO OPERATIVO" << endl;
  cout << ">> Ingrese ID Numerico: " << endl;
  blindaje_entrada(id);

  if (busqueda_de_operativo(id) != NULL) {
    cout << "[!] ERROR DE NUCLEO: El ID: " << id
         << " ya esta asignado a un Operativo activo." << endl;
    cout << "[*] Abortando inyeccion..." << endl;
    return;
  }

  do {
    cout << ">> Seleccione Bando (1. NEON | 2. OMEGA): " << endl;
    blindaje_entrada(bando);
    if (bando != 1 && bando != 2) {
      cout << "[!] Directriz invalida. Escoja un bando existente." << endl;
    }
  } while (bando != 1 && bando != 2);

  do {
    cout << ">> Seleccione Clase (1. Juggernaut | 2. Ejecutor | 3. Espectro): "
         << endl;
    blindaje_entrada(clase);
    if (clase < 1 || clase > 3) {
      cout << "[!] Directriz invalida. Escoja una clase existente." << endl;
    }
  } while (clase < 1 || clase > 3);

  if (clase == 1) {
    hp = 150;
  } else if (clase == 2) {
    hp = 100;
  } else {
    hp = 60;
  }

  Operativo *nuevo_personaje = new Operativo(id, bando, hp, clase);

  insertar_escudos_en_pila(nuevo_personaje);
  insertar_municiones_en_cola(nuevo_personaje);
  ordenacion_en_arbol(nuevo_personaje);

  cout << "[+] Operativo [" << id << "] equipado en el sistema exitosamente."
       << endl;
}

NodoEscudo *pop_simple_pila(Operativo *operativo_recibido) { // Primitiva
  // Esta funcion simplemente extrae el nodo del tope de la PILA y devuelve su
  // referencia Este procedimiento es escencial ya que se repite en todas las
  // manipulaciones posteriores de la pila
  if (operativo_recibido->escudos_tope->tope == NULL) {
    return NULL;
  }

  NodoEscudo *actual = operativo_recibido->escudos_tope->tope;
  operativo_recibido->escudos_tope->tope = actual->siguiente_escudo;

  actual->siguiente_escudo = NULL;

  return actual;
}

NodoMunicion *pop_simple_cola(Operativo *operativo_recibido) { // Primitiva
  // Notarq como esta funcion solo saca el nodo del frente y devuelve su
  // referencia, es un simple POP Al igual que el de la pila, es un algoritmo
  // escencial
  if (operativo_recibido->municiones_tope->frente == NULL) {
    return NULL;
  }
  // Apuntamos al primero de la fila (frente de la cola)
  NodoMunicion *actual = operativo_recibido->municiones_tope->frente;
  operativo_recibido->municiones_tope->frente = actual->sig_municion;

  // Si despues de mover el frente de la cola queda vacia, el final es null
  if (operativo_recibido->municiones_tope->frente == NULL) {
    operativo_recibido->municiones_tope->final = NULL;
  }

  actual->sig_municion = NULL;

  return actual;
}

/* Obsoleto
// Operativo *procedimiento_busqueda() {
  int id_modificar = 0;
  cout << ">> Introduzca el ID del operativo a modificar/consultar: " << endl;
  blindaje_entrada(id_modificar);
  Operativo *operativo_rec = busqueda_de_operativo(id_modificar);
  if (operativo_rec == NULL) {
    cout << "[!] ERROR: Operativo [" << id_modificar << "] no localizado en el Nucleo." << endl;
  }
  return operativo_rec;
}
*/

NodoEscudo *procedimiento_busqueda_escudo(Operativo *operativo_recibido,
                                          int escudo_modificar) {
  // Esta funcion devuelve la referencia a un nodo deseado. Sigue estrictamente
  // los principios LIFO de pila (Se hace una extraccion iterativa hasta llegar
  // al nodo deseado) Notar como en esta si es mas eficiente hacer break al
  // llegar a la referencia deseada, ya que en pilas no hay alteracion por orden
  // al reintroducir se usa estructura auxiliar
  if (operativo_recibido == NULL ||
      operativo_recibido->escudos_tope->tope == NULL) {
    return NULL;
  }

  PilaDefensa temporal; // Pila auxiliar para no perder los datos de busqueda
  temporal.tope = NULL;

  NodoEscudo *nodo_encontrado = NULL;
  NodoEscudo *actual = NULL;

  // Proceso de vaciar la pila desde el principio LIFO para buscar el dato
  while (operativo_recibido->escudos_tope->tope != NULL) {

    actual = operativo_recibido->escudos_tope->tope;
    operativo_recibido->escudos_tope->tope = actual->siguiente_escudo;

    actual->siguiente_escudo =
        temporal.tope; // Proceso de entrada a la pila auxiliar
    temporal.tope = actual;

    // Si coincide el ID, guardo la dirrecion de memoria en mi variable de
    // retorno
    if (actual->identificador_escudo == escudo_modificar) {
      nodo_encontrado = actual;
    }
  }

  while (temporal.tope !=
         NULL) { // REINTEGRACION de los nodos a la pila original

    NodoEscudo *nodo_restaurar = temporal.tope;
    temporal.tope = temporal.tope->siguiente_escudo;

    nodo_restaurar->siguiente_escudo = operativo_recibido->escudos_tope->tope;
    operativo_recibido->escudos_tope->tope = nodo_restaurar;
  }

  return nodo_encontrado;
}

NodoMunicion *procedimiento_busqueda_municion(Operativo *operativo_recibido,
                                              int municion_modificar) {

  // Esta funcion es una busqueda de una municion especifica que devuelve un
  // nodo (su referencia en memoria), es importante destacar que el nodo
  // devuelto no se desconecta de la estructura, solo se modifica estando en
  // ella.
  //  Como es una COLA, se tiene que hacer un vaciado ENTERO para no tergiversar
  //  el orden que ya se tenia Si se hace un break al encontrar la referencia,
  //  se voltea el ordenamiento que se tenia (a diferencia de las pilas)
  if (operativo_recibido == NULL ||
      operativo_recibido->municiones_tope->frente == NULL) {
    return NULL;
  }

  ColaArsenal cola_temporal; // SE usa estructura auxiliar
  cola_temporal.frente = NULL;
  cola_temporal.final = NULL;

  NodoMunicion *municion_recuperar = NULL;

  // Recorro la cola sacando por el frente
  while (operativo_recibido->municiones_tope->frente != NULL) {

    NodoMunicion *actual = operativo_recibido->municiones_tope->frente;
    operativo_recibido->municiones_tope->frente = actual->sig_municion;

    if (operativo_recibido->municiones_tope->frente == NULL) {
      operativo_recibido->municiones_tope->final = NULL;
    }

    // Si es la municion solicitada, guardo el puntero
    if (actual->identificador_municion == municion_modificar) {
      municion_recuperar = actual;
    }

    // Aqui se mete el nodo en la cola temporal para no dañar el orden
    actual->sig_municion = NULL;
    if (cola_temporal.frente == NULL) {
      cola_temporal.frente = actual;
      cola_temporal.final = actual;
    } else {
      cola_temporal.final->sig_municion = actual;
      cola_temporal.final = actual;
    }
  }

  while (cola_temporal.frente != NULL) { // REINTEGRACION A LA COLA ORIGINAL

    NodoMunicion *actual = cola_temporal.frente;
    cola_temporal.frente = actual->sig_municion;

    if (cola_temporal.frente == NULL) {
      cola_temporal.final = NULL;
    }

    actual->sig_municion = NULL;
    // Se usa la logica del push al final para que queden en el mismo orden
    // original
    if (operativo_recibido->municiones_tope->frente == NULL) {
      operativo_recibido->municiones_tope->frente = actual;
      operativo_recibido->municiones_tope->final = actual;
    } else {
      operativo_recibido->municiones_tope->final->sig_municion = actual;
      operativo_recibido->municiones_tope->final = actual;
    }
  }

  return municion_recuperar;
}

void mostrar_pila_escudos(Operativo *operativo_recibido) { // Mostrado de la
                                                           // pila
  if (operativo_recibido == NULL ||
      operativo_recibido->escudos_tope->tope == NULL) {
    cout << "[*] Operativo sin escudos activos." << endl;
    return;
  }
  // Puntero auxiliar para recorrer la pila sin destruirla
  NodoEscudo *actual_escudo = operativo_recibido->escudos_tope->tope;
  cout << "PILA DE ESCUDOS (ID OPERATIVO: " << operativo_recibido->id_numerico
       << ")" << endl;
  while (actual_escudo != NULL) {
    cout << " -> [ID Escudo: " << actual_escudo->identificador_escudo << "] ";
    // Convierto los enteros de "tipo_escudo" en texto para mejor manejo de
    // opciones al usuario
    if (actual_escudo->tipo_escudo == 1)
      cout << "Tipo: FISICO";
    else if (actual_escudo->tipo_escudo == 2)
      cout << "Tipo: ANTIPLASMA";
    else
      cout << "Tipo: ESPEJO";
    cout << " | Absorcion: " << actual_escudo->absorcion << "pts" << endl;
    actual_escudo = actual_escudo->siguiente_escudo; // salto al siguiente nodo
  }
}

void mostrar_cola_municiones(
    Operativo *operativo_recibido) { // Mostrado de la cola (en su entero)
  if (operativo_recibido == NULL ||
      operativo_recibido->municiones_tope->frente == NULL) {
    cout << "[*] Arsenal vacio." << endl;
    return;
  }
  NodoMunicion *aux = operativo_recibido->municiones_tope->frente;
  cout << "COLA DE MUNICIONES (ID OPERATIVO: "
       << operativo_recibido->id_numerico << ") " << endl;
  int contador = 1;
  while (aux != NULL) {
    cout << "  " << contador << ". Blast: ";
    // Uso de switch para manejo de los tipo de blast de forma mas limpia
    switch (aux->tipo_blast) {
    case 1:
      cout << "Laser Estandar";
      break;
    case 2:
      cout << "Perforante (EMP)";
      break;
    case 3:
      cout << "Troyano (Control Mental)";
      break;
    case 4:
      cout << "Disrupcion";
      break;
    case 5:
      cout << "Racimo";
      break;
    default:
      cout << "Desconocido";
      break;
    }
    cout << " [ID: " << aux->identificador_municion << "]" << endl;
    aux = aux->sig_municion;
    contador++;
  }
}

/*
void mostrar_operativos() { // Obsoleto
  if (lista_personajes_esta_vacia()) { return; }
  Operativo *aux = lista_personajes1;
  while (aux != NULL) { aux = aux->sig_operativo; }
  int opcion_consulta = 0;
  do {
    Operativo *operativo_consulta = procedimiento_busqueda();
    (void)operativo_consulta;
  } while (opcion_consulta != 3);
}
*/

/*
void Eliminar_operativos(int id_extirpar) { // OBSOLETA
  if (lista_personajes_esta_vacia()) { return; }
  Operativo *nuevo = lista_personajes1;
  Operativo *anterior = nullptr;
  while (nuevo != nullptr && nuevo->id_numerico != id_extirpar) {
    anterior = nuevo;
    nuevo = nuevo->sig_operativo;
  }
  if (nuevo == nullptr) {
    cout << "[!] No se encuentra el personaje con ID: " << id_extirpar << endl;
  } else {
    if (anterior == nullptr) { lista_personajes1 = nuevo->sig_operativo; }
    else { anterior->sig_operativo = nuevo->sig_operativo; }
    while (nuevo->escudos_tope->tope != NULL) { NodoEscudo *d = pop_simple_pila(nuevo); delete d; }
    while (nuevo->municiones_tope->frente != NULL) { NodoMunicion *d = pop_simple_cola(nuevo); delete d; }
    delete nuevo;
    nuevo = NULL;
  }
}
*/

void Eliminar_pila_escudos(
    Operativo *operativo_recibido,
    int id_escudo) { // Eliminacion que sigue estrictamente principios de la
                     // estructura.

  bool encontrado = false;
  // verificacion de lista vacia
  if (pila_escudos_esta_vacia(operativo_recibido)) {
    cout << "[*] No se encuentran escudos actualmente." << endl;
    return;
  } else {
    PilaDefensa
        aux; // Pila auxiliar creada para eliminar nodos desde el principio LIFO
    aux.tope = NULL;

    while (operativo_recibido->escudos_tope->tope != NULL) {

      NodoEscudo *actual = pop_simple_pila(
          operativo_recibido); // se extraen uno por uno hasta llegar al deseado
      if (actual->identificador_escudo == id_escudo && !encontrado) {
        delete actual; // Eliminacion del nodo encontrado para liberar memoria
        actual =
            NULL; // Puntero local apuntando a null para evitar apuntes a basura
        cout << "[+] Escudo [" << id_escudo << "] eliminado exitosamente."
             << endl;
        encontrado = true;
        break;
      } else {
        // Caso 1
        actual->siguiente_escudo =
            aux.tope; // si no es el escudo identificado, movemos el actual a la
                      // pila auxiliar
        aux.tope = actual;
      }
    }
    // Mover nodos de la pila auxiliar a la original
    while (aux.tope != NULL) {
      NodoEscudo *restaurar = aux.tope; // Puntero auxiliar apuntando a la
                                        // cabeza de la pila auxiliar
      aux.tope = restaurar->siguiente_escudo;

      // restauracion de nodos a la pila original de escudos
      restaurar->siguiente_escudo = operativo_recibido->escudos_tope->tope;
      operativo_recibido->escudos_tope->tope = restaurar;
    }
    // Aviso de que no se encontro el id del escudo
    if (encontrado == false) {
      cout << "[!] No se encontro el escudo con el identificador: " << id_escudo
           << endl;
    }
  }
}

void Eliminar_cola_municiones(
    Operativo *op,
    int id_municion) { // esta eliminacion se hace siguiendo estrictamente los
                       // principios de la estructura de cola . No se puede
                       // hacer por iteracionn simple
  // Chequeo de que el arsenal contenga armas y no este vacia
  if (cola_municiones_esta_vacia(op)) {
    cout << "[*] No hay municiones en el armamento actualmente." << endl;
    return;
  }

  ColaArsenal cola_temporal; // Cola auxiliar para administrar los nodos que no
                             // deseamos eliminar
  cola_temporal.frente = NULL;
  cola_temporal.final = NULL;

  bool destruido = false;

  while (op->municiones_tope->frente != NULL) {
    NodoMunicion *actual =
        pop_simple_cola(op); // se va sacando uno por uno por frente (FIFO)

    if (actual->identificador_municion == id_municion &&
        !destruido) { // Eliminacion del Id encontrado
      delete actual;
      actual = NULL;
      destruido = true;
      cout << "[+] Municion [" << id_municion << "] purgada del arsenal."
           << endl;
    } else {
      if (cola_temporal.frente == NULL) {
        cola_temporal.frente = actual;
        cola_temporal.final = actual;
      } else {
        cola_temporal.final->sig_municion = actual;
        cola_temporal.final = actual;
      }
    }
  }

  // Regreso de los nodos transferidos de la cola auxiliar a la cola original
  // del arsenal
  while (cola_temporal.frente != NULL) {
    NodoMunicion *nodo_restaurar = cola_temporal.frente;
    cola_temporal.frente = cola_temporal.frente->sig_municion;

    if (cola_temporal.frente == NULL) {
      cola_temporal.final = NULL;
    }

    nodo_restaurar->sig_municion = NULL;
    push_cola_municiones(op, nodo_restaurar);
  }
  // Comprobacion de que si bool es falso, el id no se encuentra en la cola
  if (!destruido) {
    cout << "[!] ERROR: Municion [" << id_municion
         << "] no localizada en el arsenal." << endl;
  }
}

/*
void Destruccion_total() { // Obsoleto
  if (lista_personajes1 == nullptr) { return; }
  while (lista_personajes1 != nullptr) {
    Operativo *op_nuevo = lista_personajes1;
    while (op_nuevo->escudos_tope->tope != nullptr) { pop_pila_escudos(op_nuevo); }
    while (op_nuevo->municiones_tope->frente != nullptr) { pop_cola_municiones(op_nuevo); }
    lista_personajes1 = lista_personajes1->sig_operativo;
    delete op_nuevo;
    op_nuevo = NULL;
  }
}
*/

//es esta

/*
void ladron_de_escudos() {

    bool exito = false;
    
    cout<<"[+]ROBO DE ESCUDOS"<<endl;

    cout<<"[->] Ingrese el ID del operativo LADRON: "<<endl;
    int id;
    cin>>id;
    Operativo *recibido = busqueda_de_operativo(id);

    if (recibido == NULL) {
      cout << "[!] El operativo seleccionado no existe. Abortando directriz." << endl;
      return;
    }


    switch(recibido->bando_perteneciente) {
        case 1:
            {
                //NEON es int 1, recibido es neon
                //busqueda de todos los operativos del bando contrario
                Operativo *actual = lista_personajes1;
                if (lista_personajes_esta_vacia()) {
                    return;
                }
                while (actual != NULL) {
                    if (actual->bando_perteneciente == 2) {
                        if (recibido->hp_base > 80) {
                            //Robo 
                            while (actual->escudos_tope->tope != NULL) {
                                NodoEscudo *robar = pop_simple_pila(actual);
                                push_pila_escudos(recibido, robar);
                            }
                            exito = true;
                        }
                        else {
                            cout << "[!]ERROR, HP insuficiente." << endl;
                            exito = false;
                            break;
                        }
                    }
                    actual = actual->sig_operativo;
                }
            }
        case 2:
            {
                //OMEGA es int 2, recibido es omega
                //busqueda de todos los operativos del bando contrario
                Operativo *actual = lista_personajes1;
                if (lista_personajes_esta_vacia()) {
                    return;
                }
                while (actual != NULL) {
                    if (actual->bando_perteneciente == 1) {
                        if (recibido->hp_base > 80) {
                            //Robo 
                            while (actual->escudos_tope->tope != NULL) {
                                NodoEscudo *robar = pop_simple_pila(actual);
                                push_pila_escudos(recibido, robar);
                            }
                            exito = true;
                        }
                        else {
                            cout << "[!] ERROR!, HP insuficiente." << endl;
                            exito = false;
                            break;
                        }
                    }
                    actual = actual->sig_operativo;
                }
            }
    }

    if (exito) {
      cout <<"[+] ORDEN EXITOSA."<<endl;
    }
    else {
      cout << "[!] Abortando directriz." << endl;
    }
}
*/ // Obsoleto

void actualizar_pila_escudos(
    Operativo
        *operativo_recibido) { // Actualizador de pilas con principios LIFO

  if (operativo_recibido == NULL) {
    cout << "[!] El operativo seleccionado no existe. Abortando directriz."
         << endl;
    return;
  }

  int escudo_a_modificar = 0;
  int directriz_elegida = 0;

  if (!pila_escudos_esta_vacia(operativo_recibido)) {

    cout << "MODIFICAR PILA DE ESCUDOS" << endl;
    mostrar_pila_escudos(operativo_recibido);

    cout << ">> Introduzca la IDENTIFICACION del escudo a modificar: " << endl;
    blindaje_entrada(escudo_a_modificar);

    NodoEscudo *actual = procedimiento_busqueda_escudo(
        operativo_recibido,
        escudo_a_modificar); // Esta referencia que devuelve se hzo con LIFO, es
                             // una referencia al nodo

    if (actual != NULL) {

      do {
        cout << "1. Cambiar el tipo de escudo" << endl;
        cout << "2. Cambiar el identificador del escudo" << endl;
        cout << "3. Eliminar el escudo" << endl;
        cout << "4. Cancelar" << endl;
        cout << ">> Escoja una directriz: ";
        blindaje_entrada(directriz_elegida);

        switch (directriz_elegida) {

        case 1: {
          int opcion = 0;
          cout << "[*] El tipo de escudo actual es: ";
          if (actual->tipo_escudo == 1)
            cout << "FISICO" << endl;
          else if (actual->tipo_escudo == 2)
            cout << "ANTIPLASMA" << endl;
          else
            cout << "ESPEJO" << endl;

          do {
            cout << ">> Seleccione el nuevo tipo de escudo (1. FISICO | 2. "
                    "ANTIPLASMA | 3. ESPEJO): ";
            blindaje_entrada(opcion);
            if (opcion < 1 || opcion > 3) {
              cout << "[!] Directriz invalida. Escoja una opcion valida."
                   << endl;
            }
          } while (opcion < 1 ||
                   opcion > 3); // Estos while son para evitar que se salga del
                                // rango de datos deseado (1 a 4)

          actual->tipo_escudo = opcion;
          if (opcion == 1)
            actual->absorcion = 30;
          else if (opcion == 2)
            actual->absorcion = 40;
          else
            actual->absorcion = 50;

          cout << "[+] Tipo de escudo actualizado exitosamente." << endl;
          directriz_elegida = 4; // Para salir del menu
          break;
        }

        case 2: {
          int identificador_nuevo = 0;
          cout << "ACTUALIZACION DE IDENTIFICACION" << endl;
          cout << "[*] IDENTIFICADOR actual: " << actual->identificador_escudo
               << endl;
          cout << ">> Introduzca el nuevo identificador para el escudo: ";
          blindaje_entrada(identificador_nuevo);

          NodoEscudo *verificador = operativo_recibido->escudos_tope->tope;
          while (verificador != NULL && verificador->identificador_escudo != identificador_nuevo) {
            verificador = verificador->siguiente_escudo;
          }

          if (verificador != NULL) {
            cout << "[!] ERROR. Identificador ya existente en los escudos del operativo." << endl;
            return;
          }

          actual->identificador_escudo =
              identificador_nuevo; // Simple reasignacion de dato
          cout << "[+] Identificador actualizado correctamente. Nuevo ID: "
               << actual->identificador_escudo << endl;
          directriz_elegida = 4;
          break;
        }

        case 3: {
          Eliminar_pila_escudos(operativo_recibido,
                                actual->identificador_escudo);
          directriz_elegida = 4; // Importante: Se actualiza la directriz para
                                 // salir del bucle do while
          // Volver a mostrar opciones haria que actual trate de acceder a un
          // escudo eliminado (Error) Se hace igualmente con los demas cases, ya
          // que es un menu que realiza una sola accion Y no requiere que se
          // repita
          break;
        }

        case 4: {
          cout << "[*] Operacion cancelada." << endl;
          break;
        }

        default:
          cout << "[!] Directriz invalida. Vuelva a intentarlo." << endl;
        }
      } while (directriz_elegida != 4);
    }

    else {
      cout << "\n[!] El escudo que desea modificar no existe en el arsenal."
           << endl;
    }

  }

  else {
    cout << "[*] La pila de escudos esta vacia. Nada que modificar." << endl;
  }
}

void actualizar_cola_municiones(
    Operativo *operativo_recibido) { // actualizador con principios FIFO

  if (operativo_recibido == NULL) {
    cout << "[!] El operativo seleccionado no existe. Abortando directriz."
         << endl;
    return;
  }

  int municion_modificar = 0; // Se maneja por identificador
  int directriz_elegida = 0;

  if (!cola_municiones_esta_vacia(operativo_recibido)) {

    cout << "\n--- ACTUALIZACION DE MUNICIONES ---" << endl;
    mostrar_cola_municiones(operativo_recibido);

    cout << ">> Introduzca la IDENTIFICACION de la municion a modificar: ";
    blindaje_entrada(municion_modificar);

    NodoMunicion *actual = procedimiento_busqueda_municion(
        operativo_recibido,
        municion_modificar); // Esta municion que devuelve se hace con FIFO, es
                             // una referencia al nodo

    if (actual != NULL) {

      do {
        cout << "1. Cambiar el tipo de BLAST" << endl;
        cout << "2. Cambiar la IDENTIFICACION de la municion" << endl;
        cout << "3. Eliminar la municion" << endl;
        cout << "4. Cancelar" << endl;
        cout << ">> Elija su directriz: ";
        blindaje_entrada(directriz_elegida);

        switch (directriz_elegida) {

        case 1: {
          int opcion = 0;
          cout << "[*] El tipo de BLAST actual es: ";
          switch (actual->tipo_blast) {
          case 1:
            cout << "LASER" << endl;
            break;
          case 2:
            cout << "EMP" << endl;
            break;
          case 3:
            cout << "TROYANO" << endl;
            break;
          case 4:
            cout << "DISRUPCION" << endl;
            break;
          case 5:
            cout << "RACIMO" << endl;
            break;
          }

          do {
            cout << ">> Seleccione el nuevo BLAST (1:LASER, 2:EMP, 3:TROYANO, "
                    "4:DISRUPCION, 5:RACIMO): ";
            blindaje_entrada(opcion);
            if (opcion < 1 || opcion > 5) { // Blindaje de rango de datos
              cout << "[!] Directriz invalida. Escoja una opcion valida."
                   << endl;
            }
          } while (opcion < 1 || opcion > 5);

          actual->tipo_blast = opcion; // Simple reasignacion
          cout << "[+] Tipo de BLAST actualizado exitosamente." << endl;
          directriz_elegida = 4;
          break;
        }

        case 2: {
          int nuevo_id = 0;
          cout << "ACTUALIZACION DE IDENTIFICACION" << endl;
          cout << "[*] Identificador actual: " << actual->identificador_municion
               << endl;
          cout << ">> Introduzca el nuevo identificador: ";
          blindaje_entrada(nuevo_id);

          NodoMunicion *verificador = operativo_recibido->municiones_tope->frente;
          while (verificador != NULL && verificador->identificador_municion != nuevo_id) {
            verificador = verificador->sig_municion;
          }

          if (verificador != NULL) {
            cout << "[!] ERROR. Identificador ya existente en las municiones del operativo." << endl;
            return;
          }

          actual->identificador_municion = nuevo_id; // Reasignacion
          cout << "[+] Identificador actualizado correctamente. Nuevo ID: "
               << actual->identificador_municion << endl;
          directriz_elegida =
              4; // Misma mecanica que el actualizador anterior, como es un menu
                 // que realiza una sola accion, se reasigna
          // directriz para salir del menu
          // Esto sigue la logica de que estos menus (tanto el de pila como el
          // de cola) estan o estaran dentro del actualizador del personaje. Por
          // eso es escencial que salga del menu ya que es una accion dentro de
          // otra
          break;
        }

        case 3: {
          Eliminar_cola_municiones(
              operativo_recibido,
              municion_modificar); // se llama el eliminador de la cola
          directriz_elegida = 4;
          break;
        }

        case 4: {
          cout << "[*] Operacion cancelada." << endl;
          break;
        }

        default: {
          cout << "[!] Directriz invalida. Vuelva a intentarlo." << endl;
          break;
        }
        }
      } while (directriz_elegida != 4);
    }

    else {
      cout << "[!] La municion seleccionada no existe en el arsenal." << endl;
    }
  }

  else {
    cout << "[*] La cola de municiones esta vacia. Nada que modificar." << endl;
  }
}

void reequipar_operativo(
    Operativo *op) { // Esta es una reequipacion del operativo
  // CAda vez que un operativo se actualiza y se cambian datos escenciales como
  // su clase Debe reasignarse escudos, blasts, o hp dependiendo de los datos
  // que le pertenezcan al operativo en su insercion Sumado a que se debe
  // liberar el espacio de datos que ya le pertenecian previamente
  while (op->escudos_tope->tope != NULL) {
    NodoEscudo *destruir =
        pop_simple_pila(op); // Se liberan sus escudos anteriores
    delete destruir;
    destruir = NULL;
  }

  while (op->municiones_tope->frente != NULL) {
    NodoMunicion *destruir = pop_simple_cola(op); // Se liberan sus municiones
    delete destruir;
    destruir = NULL;
  }

  insertar_escudos_en_pila(op); // Se le reasignan escudos y municiones
                                // dependiendo del nuevo operativo elegido
  insertar_municiones_en_cola(op);
}

/*
void actualizar_operativos() { // Obsoleto
  // Esta funcion comoprende el menu principal de actualizaciones
  // Aqui se integran los actualizadores de colas y pilas, siendo este el de
  // jerarquia principal

  if (!lista_personajes_esta_vacia()) {

    int opcion_elegida = 0;

    cout << "ACTUALIZACION DE OPERATIVOS" << endl;

    Operativo *operativo_recibido = procedimiento_busqueda();

    if (operativo_recibido != NULL) {

      do {
        cout << "MENU DE MODIFICACIONES" << endl;
        cout << "1. Modificar BANDO" << endl;
        cout << "2. Modificar TIPO DE CLASE" << endl;
        cout << "3. Actualizar ESCUDOS" << endl;
        cout << "4. Actualizar MUNICIONES" << endl;
        cout << "5. Eliminar OPERATIVO" << endl;
        cout << "6. Finalizar y guardar" << endl;
        cout << ">> Elija su directriz: ";
        blindaje_entrada(opcion_elegida);

        switch (opcion_elegida) {

        case 1: {
          int directriz_elegida = 0;
          cout << "MODIFICAR BANDO" << endl;
          cout << "[*] El bando actual es: ";
          if (operativo_recibido->bando_perteneciente == 1) {
            cout << "NEON";
          } else {
            cout << "OMEGA";
          }
          cout << endl;

          do {
            cout << "1. Cambiar al bando contrario\n2. Permanecer en el bando "
                    "actual\n>> Elija su directriz: ";
            blindaje_entrada(directriz_elegida);
            if (directriz_elegida < 1 || directriz_elegida > 2) {
              cout << "[!] Directriz invalida." << endl;
            }
          } while (directriz_elegida < 1 || directriz_elegida > 2);

          if (directriz_elegida == 1) {
            if (operativo_recibido->bando_perteneciente ==
                1) { // Se cambia de bando por manejo de INTS
              // Esto se basa en las asignaciones universales que se dieron
              // arriba para que resulte en un simple if else de reasignar
              // valores
              operativo_recibido->bando_perteneciente = 2;
            } else {
              operativo_recibido->bando_perteneciente = 1;
            }
            cout << "[+] Bando cambiado exitosamente." << endl;
          } else {
            cout << "[*] Operativo permanece en su bando original." << endl;
          }
          break;
        }

        case 2: {
          int nueva_clase = 0;
          cout << "MODIFICAR CLASE" << endl;
          // Aqui se hace uso del procedimiento de reequipar operativo.
          // Es escencial para cambiar su equipaciones dependiendo de la clase
          // a la que se cambie
          cout << "[!] Tenga en cuenta que un cambio de clase conlleva un "
                  "cambio de HP, ESCUDOS y MUNICIONES"
               << endl;

          cout << "[*] La clase actual del operativo es: ";
          if (operativo_recibido->clase_tipo == 1)
            cout << "JUGGERNAUT" << endl;
          else if (operativo_recibido->clase_tipo == 2)
            cout << "EJECUTOR" << endl;
          else
            cout << "HACKER" << endl;

          do {
            cout << ">> Seleccione nueva clase (1. Juggernaut | 2. Ejecutor | "
                    "3. Espectro) o ingrese su clase actual para cancelar: ";
            blindaje_entrada(nueva_clase);
            if (nueva_clase < 1 || nueva_clase > 3) {
              cout << "[!] Directriz invalida. Escoja una clase existente."
                   << endl;
            }
          } while (nueva_clase < 1 || nueva_clase > 3);

          if (nueva_clase != operativo_recibido->clase_tipo) {
            operativo_recibido->clase_tipo = nueva_clase;
            if (nueva_clase == 1) {
              operativo_recibido->hp_base =
                  150; // En esta seccion se reasignan los HP de los operativos
              cout << "[+] Clase cambiada a JUGGERNAUT." << endl;
            } else if (nueva_clase == 2) {
              operativo_recibido->hp_base = 100;
              cout << "[+] Clase cambiada a EJECUTOR." << endl;
            } else {
              operativo_recibido->hp_base = 60;
              cout << "[+] Clase cambiada a HACKER." << endl;
            }
            reequipar_operativo(operativo_recibido); // Y aqui se reequipan
            cout << "[+] Arsenal reequipado de acuerdo a la nueva clase."
                 << endl;
          } else {
            cout << "[*] Operativo permanece en su clase actual." << endl;
          }
          break;
        }

        case 3: {
          actualizar_pila_escudos(
              operativo_recibido); // CAso 3 y 4: llamado de los actualizadores
                                   // previamente desarrollados
          break;
        }

        case 4: {
          actualizar_cola_municiones(
              operativo_recibido); // Notar como estas opciones no fuerzan
                                   // salida del menu luego
          // de realizar la accion, a diferencia de los actualizadores de los
          // escudos y las municiones Este es un menu recursivo (en el sentido
          // de que se repite hasta que el usuario decida salir de el)
          break;
        }

        case 5: {
          int id_confirmar = operativo_recibido->id_numerico;
          cout << "[!] Se Procedera a eliminar el Operativo: [" << id_confirmar
               << "]" << endl;
          Eliminar_operativos(id_confirmar);
          operativo_recibido =
              NULL; // Reasigna puntero a NULL para evitar apuntar a basura
          opcion_elegida = 6; // Forzar salida del menu tras eliminar para
                              // evitar tratar de reacceder a ese operativo
          // (error forzado)
          cout << "[*] Regresando al Nucleo: el Operativo ya no existe en el "
                  "Sistema."
               << endl;
          break;
        }

        case 6: {
          break; // Sale del ciclo
        }

        default:
          cout << "[!] Directriz invalida. Vuelva a intentarlo." << endl;
          break;
        }

      } while (opcion_elegida != 6);

      // Solo imprimir datos si no fue eliminado
      // De lo contrario, el puntero intentaria acceder a un dato no existente
      // Otro error forzado
      if (operativo_recibido != NULL &&
          busqueda_de_operativo(operativo_recibido->id_numerico) !=
              NULL) { // Esta condicion reafirma que el operativo exista
        // En el sistema para luego imprimir sus datos actualizados
        cout << "DATOS DE OPERATIVO ACTUALIZADOS" << endl;
        cout << "[*] ID: " << operativo_recibido->id_numerico << endl;
        cout << "[*] Bando: ";
        if (operativo_recibido->bando_perteneciente == 1) {
          cout << "NEON";
        } else {
          cout << "OMEGA";
        }
        cout << endl;
        cout << "[*] Clase: ";
        if (operativo_recibido->clase_tipo == 1)
          cout << "JUGGERNAUT" << endl;
        else if (operativo_recibido->clase_tipo == 2)
          cout << "EJECUTOR" << endl;
        else
          cout << "HACKER" << endl;
        cout << "[*] HP: " << operativo_recibido->hp_base << endl;
      }
    }
  }

  else {
    cout << "[*] Lista de operativos vacia. No hay nada que modificar." << endl;
  }
}
*/ // fin actualizar_operativos OBSOLETA

void centro_de_mando() {
  int directriz = 0;

  while (directriz != 5) {
    cout << "==========================================" << endl;
    cout << "   CENTRO DE MANDO: OPERACION YGGDRASIL   " << endl;
    cout << "==========================================" << endl;
    cout << "1. INYECTAR OPERATIVO" << endl;
    cout << "2. ACTUALIZACION DE OPERATIVOS" << endl;
    cout << "3. BUSQUEDA DE OPERATIVOS" << endl;
    cout << "4. MOSTRAR OPERATIVOS" << endl;
    cout << "5. SALIR DEL SIMULADOR" << endl;
    cout << "6.Robar" << endl;
    cout << ">> Ingrese directriz: ";

    blindaje_entrada(directriz);

    // CRUD

    switch (directriz) {
    case 1: { // Create
      creacion_de_personaje();
      break;
    }

    case 2: { // Update - PENDIENTE: reconectar con logica BTree
      // actualizar_operativos();
      break;
    }

    case 3: { // Read - PENDIENTE: reconectar con buscarEnArbol()
      /* cout << "BUSQUEDA DE INTELIGENCIA" << endl;
      Operativo *encontrado = procedimiento_busqueda();
      if (encontrado != NULL) {
        cout << "[+] Datos clasificados del Operativo:" << endl;
        cout << "[*] ID: " << encontrado->id_numerico << endl;
        cout << "[*] Bando: ";
        if (encontrado->bando_perteneciente == 1) { cout << "NEON"; }
        else { cout << "OMEGA"; }
        cout << endl;
        cout << "[*] Clase: ";
        if (encontrado->clase_tipo == 1) cout << "JUGGERNAUT" << endl;
        else if (encontrado->clase_tipo == 2) cout << "EJECUTOR" << endl;
        else cout << "HACKER" << endl;
        cout << "[*] HP: " << encontrado->hp_base << endl;
      } */
      break;
    }

    case 4: { // Mostrarlos a todos - PENDIENTE: reemplazar con recorrido BTree
      // mostrar_operativos();
      break;
    }

    case 5: { // Limpieza del sistema - PENDIENTE: reemplazar con destruirArbol()
      // Destruccion_total();
      break;
    }

    case 6: { // PENDIENTE: reescribir con recorrido BTree
        // ladron_de_escudos();
        break;
    }

    default: {
      cout << "[!] Directriz invalida. Por favor seleccione una opcion del 1 "
              "al 5."
           << endl;
      break;
    }
    }
  }
}

// //

// Las variables que mas se usaran (si no todas) estan dentro de los structs. Y
// la lista principal de acceso a todo (o el puntero jefe) Esta como variable
// global, asi que creo que no deberia haber problemas de dependencia (con
// respecto a las variables)

int main() {
  srand(time(NULL));
  centro_de_mando();
  return 0;
}
