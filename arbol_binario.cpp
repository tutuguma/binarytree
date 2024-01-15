//Librerias utilizadas para el código del arbolito
#include <stdlib.h>
#include <string>
#include <iostream>

// Determinar la prioridad de los operadores matemáticos, según la regla
//de la aritmética
int prioridad(char c){
    if(c =='^'){
        return 4;
    }
    if (c =='*'){
        return 3;
    }
    if (c== '/'){
        return 2;
    }
    if (c =='+' || c== '-'){
            return 1;
        }
    return 0;
}
// Verificar si un carácter es un operador, en caso de no ser un número
// (valor entre 0-9) retornará True
bool esOperador(char c){
    return ((c > ('0' - 1)) && ((c < '9' + 1))) ? false : true;
}
// Calcular la potencia de un número, en caso de no cargar un exponente 
// el mismo tomará el valor de 1
int pow(int base, int exponente){
    return (!exponente) ? 1 : base*pow(base, exponente - 1);
}
// Estructura para los nodos del árbol (Las estructuras son similares a las clases en
// Python)
struct NodoArbol {
    char caracter = 0;
    NodoArbol* izq = nullptr; // Puntero al hijo izquierdo
    NodoArbol* der = nullptr; // Puntero al hijo derecho
};

// Estructura para los nodos de la pila
struct NodoPila {
    NodoArbol* nodo = nullptr; // Puntero al nodo del árbol
    NodoPila* siguiente = nullptr; // Puntero al nodo siguiente de la pila
};

// Función para insertar un nodo en la pila
void insertar(NodoPila*& tope, NodoArbol* nodo){
   if(NodoPila* nuevo = (NodoPila *)malloc(sizeof(NodoPila))){
       nuevo->nodo = nodo;
       nuevo->siguiente = (!tope ? nullptr : tope); 
       tope = nuevo;
   }
   else {
       std::cout << "Error nodo no insertado";
   }
}

// Funcion que sirve para eliminar y retornar el nodo del tope de la pila.
NodoArbol* quitar(NodoPila*& tope){
    NodoArbol* nuevo = nullptr; 
    if(tope){
        nuevo = tope->nodo;
        tope = tope->siguiente; 
    }
    return nuevo;
}
// Función para crear el árbol a partir de la fórmula cargada
void crearArbol(NodoPila*& operadores, NodoPila*& expresiones, NodoArbol*& token, const std::string& cadena){
    for (int i = 0; i < cadena.length();i=i+1){ // Recorre la cadena de la expresión
     // y construye el árbol binario.
        // Crea un nuevo nodo del árbol para el caracter actual
        if (token = (NodoArbol *)malloc(sizeof(NodoArbol))){
            token->caracter = cadena[i];// Asigna el caracter al nodo
            token->izq = nullptr;
            token->der = nullptr;
        // Utiliza dos pilas: una para operadores y otra para expresiones (números).
        
        // Caso1:Si el caracter actual no es un operador (es decir, es un número)
            if(!esOperador(cadena[i])){
                insertar(expresiones, token);// Inserta el nodo en la pila de expresiones
            }
        // Caso2:Si el caracter es un operador
            else{
                if(cadena[i]=='('){
                    insertar(operadores, token); // Inserta el nodo en la pila de operadores
                }
                // Procesa todos los operadores hasta encontrar ')'
                if(cadena[i]==')'){
                    while(operadores && operadores->nodo->caracter !='('){
                        if (token = quitar(operadores)){
                            token->der = quitar(expresiones);
                            token->izq = quitar(expresiones);
                            insertar(expresiones, token);
                        }
                        quitar(operadores);
                    }
                    quitar(operadores);
                    insertar(operadores, token);
                }
                if (cadena[i]!='(' && cadena[i]!=')'){
                // Carga y procesa los operadores en función a la prioridad
                    while(operadores && prioridad(cadena[i]) <= prioridad(operadores->nodo->caracter)){
                        if(token = quitar(operadores)){
                            token->der = quitar(expresiones);
                            token->izq = quitar(expresiones);
                            insertar(expresiones, token);
                        }
                    }
                    if(token = (NodoArbol *)malloc(sizeof(NodoArbol))){
                        token->caracter = cadena[i];
                    }
                    insertar(operadores, token);
                }
            }
        }
        else{
            std::cout << "Error, no se pudo insertar el caracter" << cadena[i] << "\n";
        }
    }
    while(operadores){
        if (token = quitar(operadores)){
            token->der = quitar(expresiones);
            token->izq = quitar(expresiones);
            insertar(expresiones, token);
        }
    }
    token = quitar(expresiones);
}

// Función para evaluar el árbol binario y calcular el resultado de la expresión.
float evaluar(NodoArbol* raiz){
    int contador = 0;
    if(!esOperador(raiz->caracter)){
        return raiz->caracter - '0';
    }
    else{
        if(raiz->caracter == '^'){
            contador = contador + pow(evaluar(raiz->izq), evaluar(raiz->der));
        }
        if(raiz->caracter == '*'){
            contador = contador + (evaluar(raiz->izq) * evaluar(raiz->der));
        }
        if(raiz->caracter == '/'){
            contador = contador + (evaluar(raiz->izq) / evaluar(raiz->der));
        }
         if(raiz->caracter == '+'){
            contador = contador + (evaluar(raiz->izq) + evaluar(raiz->der));
        }
         if(raiz->caracter == '-'){
            contador = contador + (evaluar(raiz->izq) - evaluar(raiz->der));
        }
    }
    return contador;
}
// Función base o principal
int main(){
    NodoPila *operadores = nullptr; 
    NodoPila *expresiones = nullptr; 
    NodoArbol *token = nullptr; 
    std::string cadena;
    int menu = 0;
    // El programa se ejecuta hasta que el usuario elige salir (Opcion 2)
    while(menu != 2){
        std::cout << "Ingrese una opcion \n 1.Calcular \n 2.Salir \n";
        std::cin >> menu;
        if(menu == 1){
        // Solicita al usuario que ingrese una expresión matemática
            std::cout << "Ingrese una expresion matematica:";
            std::cin.ignore();
            std::getline(std::cin, cadena);
            // Crea un árbol binario basado en la expresión ingresada
            crearArbol(operadores, expresiones, token, cadena);
            std::cout << evaluar(token) << "\n"; 
        }
    }
    return 0;
}
