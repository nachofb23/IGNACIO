#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIBROS 100
#define MAX_STR_LEN 100

struct NodoCola {
    char nombreReserva[MAX_STR_LEN];
    struct NodoCola *siguiente;
};

struct ColaReservados {
    struct NodoCola *inicio;
    struct NodoCola *fin;
};

struct Libro {
    char nombre[MAX_STR_LEN];
    char autor[MAX_STR_LEN];
    char genero[MAX_STR_LEN];
    char codigo[MAX_STR_LEN];
    char ubicacion[MAX_STR_LEN];
    char estado[MAX_STR_LEN];
    char reservadoPor[MAX_STR_LEN];
    struct ColaReservados colaReservados; 
};

void encolar(struct ColaReservados *cola, const char *nombreReserva) {
    // Se reserva memoria para un nuevo nodo de la cola.
    struct NodoCola *nuevoNodo = malloc(sizeof(struct NodoCola));

    // Se verifica si se pudo asignar memoria correctamente.
    if (nuevoNodo == NULL) {
        printf("Error: no se pudo asignar memoria para el nodo de la cola.\n");
        exit(1);
    }

    // Se copia el nombre de la reserva en el nuevo nodo.
    strcpy(nuevoNodo->nombreReserva, nombreReserva);
    nuevoNodo->siguiente = NULL;

    // Si la cola está vacía, el nuevo nodo se convierte en el inicio y el fin de la cola.
    if (cola->inicio == NULL) {
        cola->inicio = nuevoNodo;
        cola->fin = nuevoNodo;
    } else {
        // Si la cola no está vacía, el nuevo nodo se agrega al final de la cola.
        cola->fin->siguiente = nuevoNodo;
        cola->fin = nuevoNodo;
    }
}

// Función para desencolar un nombre de reserva
void desencolar(struct ColaReservados *cola) {
    if (cola->inicio == NULL) {
        return; // La cola está vacía
    }

    struct NodoCola *temp = cola->inicio;
    cola->inicio = cola->inicio->siguiente;
    free(temp);
}


void registrarLibro(struct Libro libros[], int *numLibros) {
    // Verificar si la cantidad actual de libros es menor que el máximo permitido (MAX_LIBROS)
    if (*numLibros < MAX_LIBROS) {
      // Solicitar al usuario ingresar el lo que indica el printf del libro y almacenarlo en la estructura de libros
        printf("Ingrese el título del libro: ");
        scanf(" %[^\n]", libros[*numLibros].nombre);

        printf("Ingrese el autor del libro: ");
        scanf(" %[^\n]", libros[*numLibros].autor);

        printf("Ingrese el género del libro: ");
        scanf(" %[^\n]", libros[*numLibros].genero);

        printf("Ingrese el ISBN del libro: ");
        scanf(" %[^\n]", libros[*numLibros].codigo);

        printf("Ingrese la ubicación del libro: ");
        scanf(" %[^\n]", libros[*numLibros].ubicacion);
  
        // Establecer el estado del libro como "Disponible"
        strcpy(libros[*numLibros].estado, "Disponible");

        // Inicializar la información de reservas
        libros[*numLibros].reservadoPor[0] = '\0'; 

        (*numLibros)++;
    } else {
        printf("La biblioteca está llena, no se pueden agregar más libros.\n");
    }
}

void datosDelLibro(struct Libro libros[], int numLibros, const char *titulo, const char *autor) {
    int i;
    int Encontrado = 0; // Variable para rastrear si se encontró el libro

    // Recorre todos los libros en la biblioteca
    for (i = 0; i < numLibros; i++) {
        // Compara el título y el autor del libro actual con los proporcionados
        if (strcmp(libros[i].nombre, titulo) == 0 && strcmp(libros[i].autor, autor) == 0) {
            // Muestra la información del libro encontrado
            printf("Título: %s\n", libros[i].nombre);
            printf("Autor: %s\n", libros[i].autor);
            printf("Género: %s\n", libros[i].genero);
            printf("Código ISBN: %s\n", libros[i].codigo);
            printf("Ubicación: %s\n", libros[i].ubicacion);
            printf("Estado: %s\n", libros[i].estado);
          
            // Verifica si hay reservas para el libro
            if (libros[i].colaReservados.inicio != NULL) {
                printf("Reservado por: ");
                struct NodoCola *temp = libros[i].colaReservados.inicio;
                while (temp != NULL) {
                    printf("%s", temp->nombreReserva);
                    temp = temp->siguiente;
                    if (temp != NULL) {
                        printf(", ");
                    }
                }
                printf("\n");
            } else {
                printf("Reserva: Actualmente no hay reservas\n");
            }
            Encontrado = 1; // Se ha encontrado el libro
            break; // Salir del bucle, ya que hemos encontrado el libro
        }
    }
    if (Encontrado == 0) {
        printf("El libro que usted está buscando no se encuentra existente\n");
    }
}

void reservarLibro(struct Libro libros[], int numLibros, const char *titulo, const char *autor, const char *nombreReserva) {
    for (int i = 0; i < numLibros; i++) {
        // Se verifica si el título y el autor coinciden con los del libro actual.
        if (strcmp(libros[i].nombre, titulo) == 0 && strcmp(libros[i].autor, autor) == 0) {
            // Si hay coincidencia, se realiza lo siguiente:

            // 1. Se encola el nombre de la persona que realiza la reserva en la cola de reservados del libro actual.
            encolar(&libros[i].colaReservados, nombreReserva);

            // 2. Se actualiza el estado del libro como "Reservado".
            strcpy(libros[i].estado, "Reservado");

            // 3. Se imprime un mensaje indicando que el libro ha sido reservado.
            printf("Libro '%s' de '%s' reservado por '%s'.\n", titulo, autor, nombreReserva);
            return;
        }
    }
    printf("\n");
    printf("El libro '%s' de '%s' no está disponible para reserva.\n", titulo, autor);
}

void cancelarReserva(struct Libro libros[], int numLibros, const char *titulo, const char *autor, const char *nombreReserva) {
    for (int i = 0; i < numLibros; i++) {
        if (strcmp(libros[i].nombre, titulo) == 0 && strcmp(libros[i].autor, autor) == 0) {
            // Desencola el nombre de reserva si está presente
            if (libros[i].colaReservados.inicio != NULL) {
                desencolar(&libros[i].colaReservados);
                printf("Reserva cancelada con éxito.\n");
            } else {
                printf("El libro '%s' de '%s' no está reservado.\n", titulo, autor);
            }
            return;
        }
    }
    printf("El libro '%s' de '%s' no se encuentra en la biblioteca.\n", titulo, autor);
}

void retirarLibro(struct Libro libros[], int numLibros, const char *titulo, const char *autor, const char *nombreEstudiante) {
    for (int i = 0; i < numLibros; i++) {
        if (strcmp(libros[i].nombre, titulo) == 0 && strcmp(libros[i].autor, autor) == 0) {
            if (strcmp(libros[i].estado, "Disponible") == 0) {
                // El libro está disponible, se puede prestar
                strcpy(libros[i].estado, "Prestado");
                printf("El libro '%s' de '%s' ha sido prestado a '%s'.\n", titulo, autor, nombreEstudiante);
                return;
            } else if (strcmp(libros[i].estado, "Reservado") == 0) {
                // El libro está reservado, verificamos si el estudiante está en la cola de reservas
                struct NodoCola *temp = libros[i].colaReservados.inicio;
                if (strcmp(temp->nombreReserva, nombreEstudiante) == 0) {
                    // El estudiante está al principio de la cola de reservas, se puede prestar
                    libros[i].colaReservados.inicio = temp->siguiente;
                    free(temp);
                    strcpy(libros[i].estado, "Prestado");
                    printf("\nEl libro '%s' de '%s' ha sido prestado a '%s'.\n", titulo, autor, nombreEstudiante);
                    return;
                } else {
                    // El estudiante no está al principio de la cola de reservas
                    printf("\nEl libro '%s' de '%s' está reservado, pero el estudiante '%s' no tiene prioridad para retirarlo.\n", titulo, autor, nombreEstudiante);
                    return;
                }
            } else {
                // El libro ya está prestado
                printf("El libro '%s' de '%s' ya ha sido prestado.\n", titulo, autor);
                return;
            }
        }
    }
    // Si el libro no se encuentra en la biblioteca
    printf("El libro '%s' de '%s' no se encuentra en la biblioteca.\n", titulo, autor);
}

void DevolverLibro(struct Libro libros[], int numLibros, const char *titulo, const char *autor) {
    for (int i = 0; i < numLibros; i++) {
        if (strcmp(libros[i].nombre, titulo) == 0 && strcmp(libros[i].autor, autor) == 0) {
            if (strcmp(libros[i].estado, "Prestado") == 0) {
                // El libro estaba prestado, ahora se devuelve
                if (libros[i].colaReservados.inicio != NULL) {
                    // Hay una cola de reservas, cambia el estado a "Reservado"
                    strcpy(libros[i].estado, "Reservado");
                } else {
                    // No hay cola de reservas, cambia el estado a "Disponible"
                    strcpy(libros[i].estado, "Disponible");
                }
                printf("El libro '%s' de '%s' ha sido devuelto.\n", titulo, autor);
                return;
            } else {
                printf("El libro '%s' de '%s' no estaba prestado, no se puede devolver.\n", titulo, autor);
                return;
            }
        }
    }
    printf("El libro '%s' de '%s' no se encuentra en la biblioteca.\n", titulo, autor);
}

void MostrarLibrosPrestados(struct Libro libros[], int numLibros) {
    // Se inicializa una variable para contar los libros prestados encontrados.
    int prestadosEncontrados = 0;

    printf("Libros Prestados:\n");
    printf("---------------------------------------------\n");

    for (int i = 0; i < numLibros; i++) {
        // Se verifica si el estado del libro es "Prestado" utilizando strcmp.
        if (strcmp(libros[i].estado, "Prestado") == 0) {
            // Si se encuentra un libro prestado, se muestra su información.
            printf("Título: %s\n", libros[i].nombre);
            printf("Autor: %s\n", libros[i].autor);
            printf("Estudiante: %s\n", libros[i].reservadoPor);
            printf("---------------------------------------------\n");
            prestadosEncontrados++;
        }
    }

    if (prestadosEncontrados == 0) {
        printf("No hay libros prestados en este momento.\n");
    }

    printf("\n");
}

void ingresarBiblioteca(struct Libro libros[], int *numLibros) {
    if (*numLibros < MAX_LIBROS) {
        FILE *archivo = fopen("biblioteca.csv", "r");
        if (archivo == NULL) {
            printf("No se pudo abrir el archivo 'biblioteca.csv'\n");
            return;
        }

    // Se utiliza fscanf para leer y descartar la primera línea del archivo (cabecera).
    fscanf(archivo, "%*[^\n]\n");
    int i = 0;

    // Se inicia un bucle infinito para procesar las líneas del archivo.
    while (1) {
            // Se utiliza fscanf para leer los campos separados por comas de cada línea.
            int result = fscanf(archivo, " %[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
                                libros[*numLibros].nombre, libros[*numLibros].autor,
                                libros[*numLibros].genero, libros[*numLibros].codigo,
                                libros[*numLibros].ubicacion, libros[*numLibros].estado,
                                libros[*numLibros].reservadoPor);

            if (result == EOF) {
                break;
            } else if (result >= 6) {
                (*numLibros)++;
                // Si se leyeron al menos 6 campos (lo que se esperaría en una línea válida),
                // se incrementa el contador de libros (*numLibros).

                if (strcmp(libros[*numLibros - 1].estado, "Disponible") == 0) {
                    libros[*numLibros - 1].reservadoPor[0] = '\0';
                }
            }

            // Si el campo "reservadoPor" no está vacío, se procesa y se encola en la cola de reservas.
            if (libros[*numLibros - 1].reservadoPor[0] != '\0') {
              char *nombre = strtok(libros[*numLibros - 1].reservadoPor, ",");
              while (nombre != NULL) {
                  encolar(&libros[*numLibros - 1].colaReservados, nombre);
                  nombre = strtok(NULL, ",");
              }
            }
        }
        printf("Los libros del archivo se han importado exitosamente");
        fclose(archivo);
  }
}

void mostrarLibros(struct Libro libros[], int numLibros) {
    int k = 1;

    if (numLibros > 0) {
        printf("Lista de libros en la biblioteca:\n");
        printf("--------------------------------------------\n");
        
        // Se utiliza un bucle for para iterar a través de la lista de libros.
        for (int i = 0; i < numLibros; i++) {
            printf("Libro %d:\n", k);
            printf("Título: %s\n", libros[i].nombre);
            printf("Autor: %s\n", libros[i].autor);
            printf("--------------------------------------------\n");
            k += 1;
        }
    } else {
        printf("La biblioteca está vacía.\n");
    }
  printf("\n");
}

void ExportarLibrosCSV(const char *nombreArchivo, struct Libro libros[], int numLibros) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo '%s' para escritura.\n", nombreArchivo);
        return;
    }

    // Escribir el encabezado del archivo CSV
    fprintf(archivo, "Título,Autor,Género,Código ISBN,Ubicación,Estado,ReservadoPor\n");

    // Escribir los datos de cada libro en el archivo CSV
    for (int i = 0; i < numLibros; i++) {
        fprintf(archivo, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"",
                libros[i].nombre, libros[i].autor, libros[i].genero,
                libros[i].codigo, libros[i].ubicacion, libros[i].estado);

        // Escribir la cola de reservas si existe
        if (libros[i].colaReservados.inicio != NULL) {
            struct NodoCola *temp = libros[i].colaReservados.inicio;
            while (temp != NULL) {
                fprintf(archivo, ",\"%s\"", temp->nombreReserva);
                temp = temp->siguiente;
            }
        }

        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("Los libros se han exportado exitosamente al archivo '%s'.\n", nombreArchivo);
}

int main() {
    struct Libro biblioteca[MAX_LIBROS];
    int numLibros = 0;
    int opcion;
    char Archivo;
    char Titulo[50], Autor[50], Estudiante[50];

    while (opcion != 11){
      
      printf("Seleccione la opcion que desea realizar =\n");
      printf("1.- Registrar libro\n");
      printf("2.- Mostrar libros\n");
      printf("3.- Mostrar datos del libro\n");
      printf("4.- Reservar libro\n");
      printf("5.- Cancelar reserva de un libro\n");
      printf("6.- Retirar libro\n");
      printf("7.- Devolver libro\n");
      printf("8.- Libros prestados\n");
      printf("9.- Importar archivo de libros\n");
      printf("10.- Exportar libros a un archivo\n");
      printf("11.- Salir\n");

      printf("\nIngrese su opcion aqui = ");
      scanf("%d", &opcion);
      printf("\n");
      
      switch (opcion) {
          case 1:
              registrarLibro(biblioteca, &numLibros);
              printf("\nLibro guardado exitosamente\n");
              break;
          case 2:
              mostrarLibros(biblioteca, numLibros);
              break;
          case 3:
              printf("Ingrese el titulo del libro = ");
              scanf(" %[^\n]s", Titulo);
              getchar();
              printf("Ingrese el autor del libro = ");
              scanf(" %[^\n]s", Autor);
              printf("\n");

              datosDelLibro(biblioteca, numLibros, Titulo, Autor);
              break;
          case 4:
              printf("Ingrese el nombre del libro = ");
              scanf(" %[^\n]s", Titulo);
              getchar();
              printf("Ingrese el autor del libro = ");
              scanf(" %[^\n]s", Autor);
              getchar();
              printf("Ingrese el nombre del estudiante = ");
              scanf(" %[^\n]s", Estudiante);

              reservarLibro(biblioteca, numLibros, Titulo, Autor, Estudiante);
              break;
        case 5:
              printf("Ingrese el nombre del libro  = ");
              scanf(" %[^\n]s", Titulo);
              getchar();
              printf("Ingrese el autor del libro = ");
              scanf(" %[^\n]s", Autor);
              getchar();
              printf("Ingrese el nombre del estudiante = ");
              scanf(" %[^\n]s", Estudiante);
          
              cancelarReserva(biblioteca, numLibros, Titulo, Autor, Estudiante);
              break;
        case 6:
              printf("Ingrese el nombre del libro = ");
              scanf(" %[^\n]s", Titulo);
              getchar();
              printf("Ingrese el autor del libro = ");
              scanf(" %[^\n]s", Autor);
              getchar();
              printf("Ingrese el nombre del estudiante = ");
              scanf(" %[^\n]s", Estudiante);

              retirarLibro(biblioteca, numLibros, Titulo, Autor, Estudiante);
              break;
          case 7:
              printf("Ingrese el nombre del libro = ");
              scanf(" %[^\n]s", Titulo);
              getchar();
              printf("Ingrese el autor del libro = ");
              scanf(" %[^\n]s", Autor);
            
              DevolverLibro(biblioteca, numLibros, Titulo, Autor);
              break;
          case 8:
              printf("\n");
              MostrarLibrosPrestados(biblioteca, numLibros);
              break;
          case 9:
              ingresarBiblioteca(biblioteca, &numLibros);
              break;
          case 10:
              ExportarLibrosCSV("Biblioteca 2", biblioteca, numLibros);
              break;
          case 11:
              printf("Saliendo exitosamente");
              break;
          default:
              printf("Opción no válida, ingrese nuevamente\n");
    }
    printf("\n");
  }
  return 0;
}