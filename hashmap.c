#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


int resolverCollision(HashMap*map, int position){
  int a=position;
  while(map->buckets[a]!=NULL && map->buckets[a]->key!=NULL){
    a = (a+1)%map->capacity;
  }
  return a;
}

void insertMap(HashMap * map, char * key, void * value) {
  int position  = hash(key, map->capacity);
    //verificar si la posicion es nula o es invalida
  if (map->buckets[position]==NULL || map->buckets[position]->key == NULL){
    Pair*nuevo=malloc(sizeof(Pair));
    if (nuevo==NULL){
      exit(EXIT_FAILURE);
    }
    
    nuevo->value=value;
    nuevo->key=strdup(key);
    map->buckets[position]=nuevo;
    map->size++;
  }else{
    int newPosition =resolverCollision(map, position);
    Pair*nuevo=malloc(sizeof(Pair));
    nuevo->value=value;
    nuevo->key=key;
    map->buckets[newPosition]=nuevo;
    map->size++;
  }  

}

void enlarge(HashMap * map) {
 enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
  HashMap *map =(HashMap*)malloc(sizeof(HashMap));//para que sea dinamica
  if (map==NULL){//si esta vacio no se puede
    printf("Error al asignar memoria.");
    exit(EXIT_FAILURE);
  }

  map->buckets = (Pair**)malloc(sizeof(Pair*)* capacity);
  if (map->buckets==NULL){
    printf("error al asignar memoria.");
    exit(EXIT_FAILURE);
  }
  for(int i =0; i<capacity; i++){//crea el mapa del porte de capacidad
    map->buckets[i]=NULL;
    
  }
  map->capacity=capacity;
  map->size=0;
  map->current=-1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {    
// Para hacerlo debe buscar el dato y luego marcarlo para que no sea válido.
  int position = hash(key, map->capacity);
  while (map->buckets[position]!=NULL){
    if (map->buckets[position]->key !=NULL && strcmp(map->buckets[position]->key, key)==0){
      map->buckets[position]->key=NULL;
      map->size--;
      return;
    }
    position= (position+1)%map->capacity;
    if (position==map->current){
      break;  
    }
  }

}

Pair * searchMap(HashMap * map,  char * key) {   
  int position  = hash(key, map->capacity);
  //Si la clave no se encuentra avance hasta encontrarla (método de resolución de colisiones)
  while(map->buckets[position]!=NULL){
    if (map->buckets[position]->key!=NULL &&strcmp(map->buckets[position]->key, key)==0){
      map->current=position;
      return map->buckets[position];
    }
    position= (position+1)%map->capacity;
    if (position==map->current){
      break;
    }
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  //retorna el primer Pair válido del arreglo buckets.
  map->current=0;
  // Buscar el primer Pair válido y retornarlo
  while (map->current < map->capacity && (map->buckets[map->current] == NULL || map->buckets[map->current]->key == NULL)){
  map->current++;
  }
  if (map->current < map->capacity){
    return map->buckets[map->current];
  }else{
    return NULL;
  }
    
  return NULL;
}

Pair * nextMap(HashMap * map) {
  map->current++;
  // Buscar el siguiente Pair válido y retornarlo
  while (map->current <map->capacity && ([map->buckets[map->current] ==NULL || map->buckets[map->current]->key ==NULL)){
    map->current ++;
  }
  if (map->current <map->capacity){
    return map->buckets[map->current];
  }else{
    return NULL;
  }
}
