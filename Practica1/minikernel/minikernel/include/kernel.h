/*
 *  minikernel/include/kernel.h
 *
 *  Minikernel. Versi�n 1.0
 *
 *  Fernando P�rez Costoya
 *
 */

/*
 *
 * Fichero de cabecera que contiene definiciones usadas por kernel.c
 *
 *      SE DEBE MODIFICAR PARA INCLUIR NUEVA FUNCIONALIDAD
 *
 */

#ifndef _KERNEL_H
#define _KERNEL_H

#include "const.h"
#include "HAL.h"
#include "llamsis.h"

// Constantes mutex
#define NO_RECURSIVO 0
#define RECURSIVO 1

/*
 *
 * Definicion del tipo que corresponde con el BCP.
 * Se va a modificar al incluir la funcionalidad pedida.
 *
 */
typedef struct BCP_t *BCPptr;

typedef struct BCP_t {
        int id;				/* ident. del proceso */
        int estado;			/* TERMINADO|LISTO|EJECUCION|BLOQUEADO*/
        contexto_t contexto_regs;	/* copia de regs. de UCP */
        void * pila;			/* dir. inicial de la pila */
		int tiempo_dormir;	/*Almacena el tiempo que le queda al proceso para despertarse*/
	BCPptr siguiente;		/* puntero a otro BCP */
	void *info_mem;			/* descriptor del mapa de memoria */
} BCP;

/*
 *
 * Definicion del tipo que corresponde con la cabecera de una lista
 * de BCPs. Este tipo se puede usar para diversas listas (procesos listos,
 * procesos bloqueados en sem�foro, etc.).
 *
 */

typedef struct{
	BCP *primero;
	BCP *ultimo;
} lista_BCPs;


/*
 * Variable global que identifica el proceso actual
 */

BCP * p_proc_actual=NULL;

/*
 * Variable global que representa la tabla de procesos
 */

BCP tabla_procs[MAX_PROC];

/*
 * Variable global que representa la cola de procesos listos
 */
lista_BCPs lista_listos= {NULL, NULL};
lista_BCPs lista_bloqueados= {NULL, NULL};

/*
 *
 * Definici�n del tipo que corresponde con una entrada en la tabla de
 * llamadas al sistema.
 *
 */
typedef struct{
	int (*fservicio)();
} servicio;

/*
 *
 * Definiciï¿œn del tipo que corresponde con un mutex no recursivo
 *
 */
typedef struct{
 int permiso=1;
 char *nombre;
 int mutexid;
}mutex_no_recursivo;
/*
 *
 * Definiciï¿œn del tipo que corresponde con un mutex recursivo
 *
 */
typedef struct{
 int n_permisos=0;
 char *nombre;
 int mutexid;
}mutex_recursivo;

/*
 *
 * Definiciï¿œn del tipo que corresponde con un almacen de mutex
 *
 */
typedef struct{
 mutex_recursivo * lista_recursivos;
 mutex_no_recursivo * lista_no_recursivos;
}almacen_mutex;

/*
 * Prototipos de las rutinas que realizan cada llamada al sistema
 */
int sis_crear_proceso();
int sis_terminar_proceso();
int sis_escribir();
int sis_obtener_id();

/*
 * Variable global que contiene las rutinas que realizan cada llamada
 */
servicio tabla_servicios[NSERVICIOS]={	{sis_crear_proceso},
					{sis_terminar_proceso},
					{sis_escribir}};

//Metodo dormir
int dormir(unsigned int segundos);
// Metodos de mutex
int crear_mutex(char *nombre, int tipo);
int abrir_mutex(char *nombre);
int lock(unsigned int mutexid);
int unlock(unsigned int mutexid);
int cerrar_mutex(unsigned int mutexid);
// Metodos de almacen
void add(mutex_recursivo mutex);
void add(mutex_no_recursivo mutex);
void delete(mutex_recursivo mutex);
void delete(mutex_no_recursivo mutex);
int check(char *nombre);
int check(char *nombre);

#endif /* _KERNEL_H */

