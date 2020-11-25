#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define Nodo struct nodo
#define Arista struct arista

Nodo{
	char dato;
	Nodo*siguiente;
	Arista*adyacencia;
};

Arista{
	Nodo*vrt;
	Arista*siguiente;
};

Nodo*inicio= NULL;

void agregarArista(Nodo*aux, Nodo*aux2, Arista*nuevo){
	Arista*a;
	if(aux->adyacencia == NULL){
		aux->adyacencia= nuevo;
		nuevo->vrt = aux2;
	}else{
		a = aux->adyacencia;
		while(a->siguiente != NULL){
			a=a->siguiente;
		}
		nuevo->vrt= aux2;
		a->siguiente = nuevo;
	}
	
}

void InsertarNodo(){
	Nodo*aux;
	Nodo*nuevo = (Nodo*)malloc(sizeof(Nodo));
	fflush(stdin);
	printf("Ingrese Vertice:\n");
	scanf("%c", &nuevo->dato);
	nuevo->siguiente = NULL;
	nuevo->siguiente = NULL;
	nuevo->adyacencia = NULL;
	if(inicio == NULL){
		inicio = nuevo;
	}else{
		aux = inicio;
		while(aux->siguiente != NULL){
			aux = aux->siguiente;
		}
		aux->siguiente = nuevo;
	}
}

void InsertarArista(){
	char ini, fin; //Vertices
	Arista*nuevo = (Arista*)malloc(sizeof(Arista));
	Nodo*aux,*aux2;
	if(inicio == NULL){
		printf("Grafo vacio.");
		return;
	}else{
		fflush(stdin);
		printf("Ingrese un nodo inicial y final: (Ejemplo: A B)\n");
		scanf("%c %c", &ini, &fin);
		aux = inicio;
		aux2 = inicio;
		while(aux2 != NULL){
			if(aux2->dato == fin){
				break;
			}
			aux2 = aux2->siguiente;
		}
		if(aux2 == NULL){
			printf("Error: Vertice no se encontr.\n");
			return;
		}
		while(aux != NULL){
			if(aux->dato == ini){
				agregarArista(aux, aux2, nuevo);
				return;
			}
			aux= aux->siguiente;
		}
		if(aux == NULL){
			printf("Error: Vertice no se encontr.\n");
			return;
		}
	}
}

int main(){
	int opcion, N, i;
	printf("Ingrese numero de vertices: \n");
	scanf("%d", &N);
	for(i=0; i<N; i++){
		InsertarNodo();
	}
	system("cls");
	
	do{
		printf("-----------Menu---------------\n");
		printf("1.- Insertar Vertice.\n");
		printf("2.- Insertar Arista.\n");
		printf("3.- Salir\n");
		printf("------------------------------\n");
		scanf("%d", &opcion);
		
		switch(opcion){
			case 1:InsertarNodo();
				break;
			case 2:InsertarArista();
				break;
			case 3: break;
			default: printf("La opcion no es valida.");
		}
		system("pause");
		system("cls");
	}while(opcion != 3);
	return 0;
}
