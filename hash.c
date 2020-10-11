#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"
#include "hash.h"
#include "hash_iterador.h"
#define ERROR -1
#define EXITO 0
#define MAX_CLAVE 50

typedef struct nodo_hash{
    const char clave[MAX_CLAVE];
    void* dato;
}nodo_hash_t;

struct hash{
    lista_t* vector;
    size_t cantidad;
    size_t capacidad;
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
    int posicion_actual;
    size_t cantidad;
    size_t capacidad;
    lista_t* vector;
};

/******************* FUNCIONES AUX ***********************/

size_t funcion_hash(const char* clave, size_t capacidad){
    size_t numero =   0;
    for(int i=0;i<2;i++){
        numero = (size_t)clave[i]*(size_t)(i);
    }
    return numero%capacidad;
}

nodo_hash_t* crear_nodo_hash(const char* clave, void* elemento){
    nodo_hash_t* nuevo_nodo  =   calloc(1, sizeof(nodo_hash_t));
    if(nuevo_nodo==NULL) return NULL;

    nuevo_nodo->dato    =   elemento;
    strcpy((char*)nuevo_nodo->clave, clave);

    return nuevo_nodo;
}

int destruir_vector(hash_t* hash){
    if(!hash) return ERROR;

    if(hash->destruir_dato  !=  NULL    &&  hash->cantidad  >   0){
        for(int i=0; i<hash->capacidad; i++){
            i++;
        }
    }
    free(hash->vector);
    return EXITO;
}

int sacar_nodo(hash_t* hash, size_t hasheado){
    if(lista_vacia(&hash->vector[hasheado])  ==  true) return ERROR;
    if(hash->destruir_dato  !=  NULL){
        hash->destruir_dato(hash->vector[hasheado].nodo->dato);
    }
    free(hash->vector[hasheado].nodo);
    hash->vector[hasheado].ocupado  =   false;
    hash->vector[hasheado].estrenado  =   true;
    hash->cantidad--;
    if((double)hash->cantidad   <   ((double)hash->capacidad*0.3)   &&  hash->capacidad>8){
        hash_redimensionar_menos(hash);
    }
    return EXITO;
}

bool misma_clave(hash_t* hash, const char* clave, size_t hasheado){
    if(hash->vector[hasheado].ocupado==false) return false;
    if(strcmp(hash->vector[hasheado].nodo->clave, clave)    ==  0) return true;
    return false;
}

bool a_estrenar(lista_t* posicion){
    if(!posicion    ||  posicion->estrenado ==  true    ||  posicion->ocupado   ==  true) return false;
    return true;
}

void* obtener_nodo(hash_t* hash, const char* clave){
    if(!hash    ||  !clave) return NULL;

    size_t hasheado   =   funcion_hash(clave, hash->capacidad);
    size_t inicio     =   hasheado;

    while(misma_clave(hash, clave, hasheado)    ==  false){
        if(hash->capacidad  <=  hasheado) hasheado    =   0;
        if(a_estrenar(&hash->vector[hasheado])) return NULL;
        hasheado++;
        if(inicio   ==  hasheado) return NULL;
    }
    return hash->vector[hasheado].nodo;
}

bool hay_alguno(lista_t * vector, int actual, size_t final){
    if(!vector   ||  actual>=(int)final-1) return false;
    do{
        if(vector[actual].ocupado ==  true) return true;
        actual++;
    }while(actual<=final);
    return false;
}

int actualizar_nodo(hash_t* hash, size_t hasheado, void* dato){
    if(hash->destruir_dato  !=  NULL)   hash->destruir_dato(hash->vector[hasheado].nodo->dato);
    hash->vector[hasheado].nodo->dato   =   dato;
    return EXITO;
}

/******************** PRIMITIVAS *************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){
    hash_t* hashito =   (hash_t*)calloc(1, sizeof(hash_t));
    if(hashito  ==  NULL)   return NULL;

    if(capacidad    <   3) capacidad = 3;

    hashito->vector     =   (lista_t*)calloc(capacidad, sizeof(lista_t));
    if(!hashito->vector){
        free(hashito);
        return NULL;
    }

    hashito->capacidad  =   capacidad;
    hashito->cantidad   =   0;
    hashito->destruir_dato  =   destruir_elemento;

    return hashito;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
    if(!hash    ||  !clave) return ERROR;

    size_t hasheado   =   funcion_hash(clave, hash->capacidad);

    nodo_hash_t* nodito   =   crear_nodo_hash(clave, elemento);
    if(!nodito) return ERROR;

    lista_insertar(&hash->vector[hasheado], nodito);
    hash->cantidad++;

    return EXITO;
}

int hash_quitar(hash_t* hash, const char* clave){
    if(!hash    ||  !clave  ||  !hash->vector) return ERROR;

    size_t hasheado =   funcion_hash(clave, hash->capacidad);
    if(lista_vacia(&hash->vector[hasheado])) return ERROR;

    size_t cantidad_de_elementos_en_la_lista   =   hash->vector[hasheado].cantidad;
    size_t actual  =   0;
    while(actual    <   cantidad_de_elementos_en_la_lista){

    }


    return ERROR;
}

void* hash_obtener(hash_t* hash, const char* clave){
    nodo_t* nodo    =   obtener_nodo(hash, clave);

    if(!nodo    ||  strcmp(nodo->clave, clave)  !=  0){
        return NULL;
    }
    return nodo->dato;
}

bool hash_contiene(hash_t* hash, const char* clave){
    if(!hash    ||  !clave) return false;
    if(obtener_nodo(hash, clave)    ==  NULL) return false;
    return true;
}

size_t hash_cantidad(hash_t* hash){
    if(!hash) return 0;
    return hash->cantidad;
}

void hash_destruir(hash_t* hash){
    if(!hash) return;

    if(hash->destruir_dato  !=  NULL    &&  hash->cantidad  >   0){
        for(int i=0; i<hash->capacidad; i++){
            if(hash->vector[i].ocupado  ==  true){
                hash->destruir_dato(hash->vector[i].nodo->dato);
                free(hash->vector[i].nodo);
            }
        }
    }
    free(hash->vector);
    free(hash);
}

size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
    if(!hash    ||  !funcion) return 0;
    bool checkeador =   false;
    size_t contador =   0;
    for(int i=0; i<hash->capacidad; i++){
        if(hash->vector[i].ocupado  ==  true){
            checkeador  =   (funcion)(hash, hash->vector[i].nodo->clave, aux);
            contador++;
            if(checkeador   ==  true) return contador;
        }
    }
    return contador;
}

/********************* Iterador **************************/

/*hash_iterador_t* hash_iterador_crear(hash_t* hash) {
    hash_iterador_t* iterador   =   (hash_iterador_t*)calloc(1, sizeof(hash_iterador_t));
    if(!iterador) return NULL;

    posicion_t* posicion    =   (posicion_t*)calloc(hash->cantidad, sizeof(posicion_t));
    if(!posicion){
        free(iterador);
        return NULL;
    }

    size_t posicion_vector  =   0;
    size_t posicion_hash    =   0;
    while(posicion_hash   !=  hash->cantidad+1){
        if(hash->vector[posicion_hash].ocupado){
            posicion[posicion_vector].nodo  =   hash->vector[posicion_hash].nodo;
            posicion_vector++;
        }
        posicion_hash++;
    }
    iterador->posicion_actual   =   0;
    iterador->vector    =   posicion;
    iterador->cantidad  =   hash->cantidad;
    return iterador;
}*/
hash_iterador_t* hash_iterador_crear(hash_t* hash) {
    if(!hash) return NULL;

    hash_iterador_t* iterador   =   (hash_iterador_t*)calloc(1, sizeof(hash_iterador_t));
    if(!iterador) return NULL;

    iterador->posicion_actual   =   -1;
    iterador->vector       =   hash->vector;
    iterador->capacidad    =   hash->capacidad;
    iterador->cantidad     =   hash->cantidad;
    return iterador;
}

/*const char* hash_iterador_siguiente(hash_iterador_t* iterador){
    iterador->posicion_actual++;
    return iterador->vector[(iterador->posicion_actual-1)].nodo->clave;
}*/
const char* hash_iterador_siguiente(hash_iterador_t* iterador){
    if(!iterador) return NULL;

    iterador->posicion_actual++;
    if(iterador->posicion_actual  >=  iterador->capacidad) return NULL;

    while(iterador->vector[iterador->posicion_actual].ocupado   ==  false){
        iterador->posicion_actual++;
        if(iterador->capacidad  ==  iterador->posicion_actual) return NULL;
    }

    return iterador->vector[iterador->posicion_actual].nodo->clave;
}

/*bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
    if(iterador->posicion_actual    <  iterador->cantidad) return true;
    return false;
}*/
bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
    if(!iterador    ||     !iterador->vector   ||  iterador->posicion_actual   >  (int)iterador->capacidad) return false;
    if(hay_alguno(iterador->vector, iterador->posicion_actual, iterador->capacidad) ==  false) return false;
    return true;
}

void hash_iterador_destruir(hash_iterador_t* iterador){
    if(!iterador) return;
    free(iterador);
}