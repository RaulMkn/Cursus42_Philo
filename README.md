# 🧠 Proyecto Philosophers – Cursus 42

Este proyecto es parte del currículo de la escuela 42 y aborda el clásico problema de los filósofos comensales. El objetivo es gestionar la sincronización y evitar condiciones de carrera y bloqueos (deadlocks) en un entorno multihilo utilizando C y las primitivas de sincronización del sistema operativo.

## 🧾 Descripción

El problema de los filósofos comensales plantea una situación en la que varios filósofos se sientan alrededor de una mesa con un tenedor entre cada par. Cada filósofo necesita dos tenedores para comer, lo que genera desafíos en la gestión de recursos compartidos y la prevención de bloqueos.

Este proyecto implementa una solución al problema utilizando hilos y mecanismos de sincronización como mutexes para garantizar que los filósofos puedan comer sin caer en condiciones de carrera o bloqueos.

## 🛠️ Estructura del Proyecto

El directorio `philo` contiene los siguientes archivos:

* `Makefile`: Script para compilar el proyecto.
* `philo.c`: Función principal que inicializa y ejecuta la simulación.
* `philo.h`: Archivo de encabezado con definiciones y estructuras utilizadas en el proyecto.
* `fork_management.c`: Manejo de los tenedores (recursos compartidos).
* `rutines.c` y `rutine_utils.c`: Funciones que definen el comportamiento de los filósofos y utilidades asociadas.
* `utils.c`: Funciones auxiliares para la gestión del tiempo y otras operaciones.

## 🚀 Compilación y Ejecución

Para compilar el proyecto, navega al directorio `philo` y ejecuta:

```bash
make
```

Esto generará un ejecutable llamado `philo`.

### Uso

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

* `number_of_philosophers`: Número de filósofos y tenedores.
* `time_to_die`: Tiempo en milisegundos antes de que un filósofo muera si no ha comido.
* `time_to_eat`: Tiempo en milisegundos que un filósofo tarda en comer.
* `time_to_sleep`: Tiempo en milisegundos que un filósofo duerme.
* `number_of_times_each_philosopher_must_eat` (opcional): Número de veces que cada filósofo debe comer antes de que la simulación termine.

### Ejemplo

```bash
./philo 5 800 200 200
```

Este comando inicia una simulación con 5 filósofos, donde cada uno morirá si no come en 800 ms, tarda 200 ms en comer y 200 ms en dormir.

## 📚 Recursos y Referencias

* [Problema de los filósofos comensales en Wikipedia](https://es.wikipedia.org/wiki/Problema_de_la_cena_de_los_fil%C3%B3sofos)
* [Documentación de pthreads](https://man7.org/linux/man-pages/man7/pthreads.7.html)
