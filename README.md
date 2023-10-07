# Lab4_PP

## Instrucciones para compilar

riemann.c
```bash
  gcc -o riemann riemann.c
```

riemann_omp_nocrit.c
```bash
 gcc -o riemann_omp_nocrit riemann_omp_nocrit.c
```

riemann_ompi.c
```bash
  mpicc -o riemann_ompi riemann_ompi.c
```

Todos los programas de openmpi se corren con
```bash
  mpirun -np n ./ejecutable
```
Donde n es la cantidad de procesos y ejecutable es el nombre del ejecutable.

Todos los ejecutables reciben 2 parámetros. El primero es el inicio del intervalo de la integral y el segundo el final.
