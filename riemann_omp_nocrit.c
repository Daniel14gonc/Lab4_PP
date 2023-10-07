/*
* Nombre: Daniel Gonzalez
* Carne: 20293
* Descripcion: Programa que calcula integrales con sumas de Riemmann concurrentes sin critical section.
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS 10

double res[NUM_THREADS];

double first_function(double x) {
    return x*x;
}

double second_function(double x) {
    return 2 * x * x * x;
}

double third_function(double x) {
    return sin(x);
}

double trapezoide(double a, double b, int n, int num_threads) {
    // Se definen los trapezoides que calculara ese thread.
    double h = (b-a)/n;
    double n_local = n / num_threads;
    double a_local = a + (omp_get_thread_num() * n_local * h);
    double b_local = a_local + (n_local * h);
    double local_sum = 0.5 * (third_function(a) + third_function(b));
    // Se hace la suma de los trapezoides.
    for (int i = a_local; i < a_local + n_local; i++) {
        double x = a_local + i * h;
        local_sum += third_function(x);
    }
    // Se actualiza el valor computado para este thread en el array.
    res[omp_get_thread_num()] = local_sum * h;
}

int main(int argc, char* argv[]) {
    double a = 2.0;
    double b = 10.0;
    int n = 1000000;
    int num_threads = NUM_THREADS;
    clock_t start, end;
    // Se verifica si se mandaron argumentos en la linea de comandos.
    if (argc == 4) {
        a = strtod(argv[1], NULL);
        b = strtod(argv[2], NULL);
        num_threads = strtod(argv[3], NULL);
    }
    start = clock();
    // Se ejecuta la seccion paralela de los trapecios.
    #pragma omp parallel num_threads(num_threads) 
    {
        trapezoide(a, b, n, num_threads);
    }
    
    double sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        sum += res[i];
    }

    end = clock();

    printf("La aproximacion de la integral es %f\n", sum);
    printf("El tiempo de ejecucion fue de %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}