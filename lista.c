#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

#define ERROR -1
#define EXITO 0
#define LISTA_NULL  0

lista_t* lista_crear(){
    lista_t* lista_creadora =   calloc(1, sizeof(lista_t));
    if(lista_creadora    !=  NULL){
        lista_creadora->cantidad=0;
    }
    return lista_creadora;
}

/*
 * Devuelve un puntero al nodo creado o NULL en caso de error.
 */
nodo_t* reservar_memoria_nodo(){
    nodo_t* nodo_creador    =   calloc(1, sizeof(nodo_t));
    return nodo_creador;
}

/*
 * Devuelve un puntero al nodo creado + le asigno el void* en caso de exito
 * Devuelve NULL en caso de error
 */
nodo_t* crear_nodo(void* elemento){
    nodo_t* nuevo_nodo  =   reservar_memoria_nodo();
    if(nuevo_nodo==NULL){
        return NULL;
    }

    nuevo_nodo->dato    =   elemento;

    return nuevo_nodo;
}

int lista_insertar(lista_t* lista, void* elemento){
    bool la_cree    =   false;
    if(lista    ==  NULL){
        lista   =   lista_crear();
        la_cree =   true;
    }

    nodo_t* nodo_a_insertar =   crear_nodo(elemento);
    if(nodo_a_insertar  ==  NULL){
        if(la_cree  ==  true) free(lista);
        return ERROR;
    }

    if(lista->cantidad  ==  0){
        lista->primero  =   nodo_a_insertar;
        lista->ultimo   =   nodo_a_insertar;
    }
    else{
        nodo_t* actual_ultimo    =   lista->ultimo;//Preguntar
        actual_ultimo->siguiente_nodo   =   nodo_a_insertar;
        lista->ultimo   =   nodo_a_insertar;
        lista->ultimo->siguiente_nodo   =   NULL;
    }
    lista->cantidad++;

    return EXITO;
}

void destruir_nodo(nodo_t* nodo){
    free(nodo);
}

int insertar_posicion_cero(lista_t* lista, void* elemento){
    nodo_t* nodo_a_insertar =   crear_nodo(elemento);
    if(nodo_a_insertar  ==  NULL){
        return ERROR;
    }
    nodo_a_insertar->siguiente_nodo  =   lista->primero;
    lista->primero  =   nodo_a_insertar;
    lista->cantidad++;
    return EXITO;
}

/*
 * Inserta un elemento en la posicion indicada, donde 0 es insertar
 * como primer elemento y 1 es insertar luego del primer elemento.  
 * En caso de no existir la posicion indicada, lo inserta al final.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if(lista    ==  NULL){
        return ERROR;
    }
    if(lista->cantidad <=  posicion){
        return lista_insertar(lista, elemento);
    }
    if(posicion ==  0){
        return  insertar_posicion_cero(lista, elemento);
    }

    nodo_t* nodo_a_insertar =   crear_nodo(elemento);
    if(nodo_a_insertar  ==  NULL){
        return ERROR;
    }

    nodo_t* nodo_aux    =   lista->primero;
    for(size_t  i=1;    i<posicion; i++){
        nodo_aux  =   nodo_aux->siguiente_nodo;
    }

    nodo_a_insertar->siguiente_nodo =   nodo_aux->siguiente_nodo;
    nodo_aux->siguiente_nodo   =   nodo_a_insertar;
    lista->cantidad++;

    return EXITO;
}

/*
 * Quita de la lista el elemento que se encuentra en la ultima posición.
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int lista_borrar(lista_t* lista){
    if(lista    ==  NULL    ||  lista->cantidad  ==  0){
        return ERROR;
    }

    nodo_t* nodo_aux    =   lista->primero;
    nodo_t* anterior    =   NULL;

    while(nodo_aux->siguiente_nodo  !=  NULL){
        anterior    =   nodo_aux;
        nodo_aux    =   nodo_aux->siguiente_nodo;
    }
    destruir_nodo(nodo_aux);
    if(anterior!=NULL){
        anterior->siguiente_nodo    =   NULL;
        lista->ultimo   =   anterior;
    }
    else{
        lista->primero  =   NULL;
        lista->ultimo  =   NULL;
    }
    lista->cantidad--;

    return EXITO;
}

/*
 * Quita de la lista el elemento que se encuentra en la posición
 * indicada, donde 0 es el primer elemento.  
 * En caso de no existir esa posición se intentará borrar el último
 * elemento.  
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
    if(lista    ==  NULL){
        return ERROR;
    }
    if(posicion >=  lista->cantidad){
        return lista_borrar(lista);
    }

    if(posicion==0) {
        nodo_t* nodo    =   lista->primero;
        if(lista->primero==lista->ultimo){
            destruir_nodo(nodo);
            lista->primero  =   NULL;
            lista->ultimo  =   NULL;
        }
        else{
            lista->primero  =   lista->primero->siguiente_nodo;
            destruir_nodo(nodo);
        }
    }
    else{
        nodo_t* nodo_aux    =   lista->primero;
        nodo_t *anterior;
        for (size_t i = 0; i < posicion; i++) {
            anterior = nodo_aux;
            nodo_aux    =   nodo_aux->siguiente_nodo;
        }
        if (anterior != NULL || anterior->siguiente_nodo->siguiente_nodo != NULL) {
            anterior->siguiente_nodo = nodo_aux->siguiente_nodo;
        }
        destruir_nodo(nodo_aux);
    }
    lista->cantidad--;

    return EXITO;
}

/*
 * Devuelve el elemento en la posicion indicada, donde 0 es el primer
 * elemento.
 *
 * Si no existe dicha posicion devuelve NULL.
 */
void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if(lista    ==  NULL    ||  lista->cantidad  <=  posicion){
        return NULL;
    }

    nodo_t* nodo_aux    =   lista->primero;

    void* elemento;
    for(size_t  i=0; i<posicion; i++){

        nodo_aux    =   nodo_aux->siguiente_nodo;
    }
    elemento    =   nodo_aux->dato;

    return elemento;
}

void* lista_ultimo(lista_t* lista){
    if(lista    ==  NULL    ||  lista_vacia(lista)){
        return NULL;
    }

    return lista->ultimo->dato;
}

bool lista_vacia(lista_t* lista){
    if(lista    ==  NULL    ||  lista->primero   ==  NULL){
        return true;
    }

    return false;
}

size_t lista_elementos(lista_t* lista){
    if(lista    ==  NULL){
        return  LISTA_NULL;
    }
    return lista->cantidad;
}

int lista_apilar(lista_t* lista, void* elemento){
    if(lista    ==  NULL){
        return ERROR;
    }
    return lista_insertar(lista, elemento);
}

int lista_desapilar(lista_t* lista){
    if(lista    ==  NULL){
        return ERROR;
    }
    return lista_borrar(lista);
}

void* lista_tope(lista_t* lista){
    if(lista    ==  NULL    ||  lista_vacia(lista)){
        return NULL;
    }
    return lista->ultimo->dato;
}

int lista_encolar(lista_t* lista, void* elemento){
    if(lista    ==  NULL){
        return ERROR;
    }
    return lista_insertar(lista, elemento);
}

int lista_desencolar(lista_t* lista){
    if(lista    ==  NULL){
        return ERROR;
    }
    return lista_borrar_de_posicion(lista, 0);
}

void* lista_primero(lista_t* lista){
    if(lista    ==  NULL){
        return NULL;
    }
    return lista_elemento_en_posicion(lista, 0);
}

void lista_destruir(lista_t* lista){
    if(lista    !=  NULL){
        if (!lista_vacia(lista)) {
            for (int i = 0; i < lista->cantidad; i++) {
                lista_borrar_de_posicion(lista, 0);
            }
        }
        free(lista);
    }
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if(lista    ==  NULL    || lista_vacia(lista)){
        return NULL;
    }
    lista_iterador_t*   it  =   calloc(1, sizeof(lista_iterador_t));
    if(it   ==  NULL){
        return NULL;
    }

    it->nodo   =   lista->primero;
    return it;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if(iterador ==  NULL    ||  iterador->nodo->siguiente_nodo  ==  NULL){
        return false;
    }

    return true;
}

/*
 * Devuelve el próximo elemento disponible en la iteración.
 * En caso de error devuelve NULL.
 */
void* lista_iterador_siguiente(lista_iterador_t* iterador){
    if(iterador ==  NULL    ||  lista_iterador_tiene_siguiente(iterador)    ==  false){
        return NULL;
    }

    iterador->nodo  =   iterador->nodo->siguiente_nodo;
    void* elemento  =   iterador->nodo->dato;

    return elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    if(iterador !=  NULL){
        free(iterador);
    }
}

/*
 * Iterador interno. Recorre la lista e invoca la funcion con cada
 * elemento de la misma.
 */
void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*, void*), void *contexto){
    if(lista    ==  NULL    ||  (*funcion)  ==  NULL    ||  lista_vacia(lista)  ==  true){
        return;
    }

    lista_iterador_t*   it  =   lista_iterador_crear(lista);
    if(it   ==  NULL){
        return;
    }

    while(lista_iterador_tiene_siguiente(it)){
        (*funcion)(lista_iterador_siguiente(it), contexto);
    }
    lista_iterador_destruir(it);
}