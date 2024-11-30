#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int fd;
    char *path = "file.txt";

    // Crear archivo con permisos de lectura/escritura
    fd = open(path, O_CREATE | O_RDWR);
    if (fd < 0) {
        fprintf(2, "Error al crear archivo\n");
        exit(1);
    }

    // Buffer para escritura
    int n = 20;
    char buf[n];
    strcpy(buf, "texto en el archivo");

    // Escribir en archivo con permisos rw (3)
    if (write(fd, buf, n) != n) {
        fprintf(2, "Error al escribir en archivo rw\n");
        exit(1);
    }
    printf("Escritura en archivo rw exitosa!\n");

    // Cambiar permisos a solo lectura (1)
    if (chmod(path, 1) < 0) {
        fprintf(2, "chmod a 1 falló\n");
        exit(1);
    }

    // Intentar escribir en archivo con permisos de solo lectura
    if (write(fd, buf, n) != n) {
        printf("Escritura en archivo r falló (esperado)\n");
    } else {
        printf("Escritura en archivo r exitosa (no esperado)\n");
    }

    // Cambiar permisos a lectura/escritura (3)
    if (chmod(path, 3) < 0) {
        fprintf(2, "chmod a 3 falló\n");
        exit(1);
    }

    // Escribir en archivo con permisos rw
    if (write(fd, buf, n) != n) {
        fprintf(2, "Error al escribir en archivo rw\n");
        exit(1);
    }
    printf("Escritura en archivo rw exitosa!\n");

    // Cambiar permisos a inmutable (5)
    if (chmod(path, 5) < 0) {
        fprintf(2, "chmod a 5 falló\n");
        exit(1);
    }

    // Reabrir archivo en modo escritura con permisos de inmutable
    close(fd);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        fprintf(2, "Error al abrir archivo con permisos inmutables\n");
        exit(1);
    }

    // Intentar escribir en archivo inmutable
    if (write(fd, buf, n) != n) {
        printf("Escritura en archivo inmutable falló (esperado)\n");
    } else {
        printf("Escritura en archivo inmutable exitosa (no esperado)\n");
    }

    // Restaurar permisos a lectura/escritura (3)
    if (chmod(path, 3) < 0) {
        fprintf(2, "Restaurar permisos a 3 falló\n");
        exit(1);
    }

    // Cerrar archivo
    close(fd);
    exit(0);
}
