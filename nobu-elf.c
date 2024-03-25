#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

void permisos(unsigned int flags) {
    printf("Permisos: ");
    if (flags == 0) 
        printf("Ninguno ");
    if (flags & SHF_ALLOC)
        printf("Lectura ");
    if (flags & SHF_WRITE)
        printf("Escritura ");
    if (flags & SHF_EXECINSTR)
        printf("Ejecucion ");
    printf("\n");
}

void imprimir_secciones(Elf64_Shdr *section_header, char *shstrtab) {
    printf("---------------------------------\n");
    printf("Sección: %s\n", shstrtab + section_header->sh_name);
    printf("Dirección: 0x%lx\n", section_header->sh_addr);
    printf("Offset: 0x%lx\n", section_header->sh_offset);
    printf("Tamaño: %lu bytes\n", section_header->sh_size);
    permisos(section_header->sh_flags);
}

void procesar_secciones(FILE *archivo, Elf64_Ehdr *header, Elf64_Shdr *shstrtab_header, char *shstrtab, unsigned int permisos, char *nombres_seccion[], int num_nombres) {
    fseek(archivo, header->e_shoff, SEEK_SET);
    Elf64_Shdr section_header;
    for (int i = 0; i < header->e_shnum; ++i) {
        fread(&section_header, 1, sizeof(section_header), archivo);
        if (permisos != 0) {
            if ((permisos & SHF_ALLOC) != (section_header.sh_flags & SHF_ALLOC) || (permisos & SHF_WRITE) != (section_header.sh_flags & SHF_WRITE) || (permisos & SHF_EXECINSTR) != (section_header.sh_flags & SHF_EXECINSTR))
                continue;
        }
        int permiso_encontrado = 0;
        if (num_nombres > 0 && nombres_seccion != NULL) {
            for (int j = 0; j < num_nombres; j++) {
                if (strcmp(shstrtab + section_header.sh_name, nombres_seccion[j]) == 0) {
                    permiso_encontrado = 1;
                    break;
                }
            }
        } else {
            permiso_encontrado = 1;
        }
        if (permiso_encontrado) {
            imprimir_secciones(&section_header, shstrtab);
        }
    }
}

void procesar_archivo(char *archivo, unsigned int permisos, char *nombres_seccion[], int num_nombres) {
    FILE *f = fopen(archivo, "r");
    if (f == NULL) {
        perror("Bleh~\nNo se pudo abrir el archivo");
        exit(1);
    }
    Elf64_Ehdr header;
    fread(&header, 1, sizeof(header), f);
    if (header.e_ident[EI_MAG0] != ELFMAG0 || header.e_ident[EI_MAG1] != ELFMAG1 ||
        header.e_ident[EI_MAG2] != ELFMAG2 || header.e_ident[EI_MAG3] != ELFMAG3) {
        fprintf(stderr, "Bleh~\nNo es ELF\n");
        fclose(f);
        exit(1);
    }
    printf("---------------------------------\n");
    printf("Entry Point: 0x%lx\n", header.e_entry);
    fseek(f, header.e_shoff + header.e_shstrndx * sizeof(Elf64_Shdr), SEEK_SET);
    Elf64_Shdr shstrtab_header;
    fread(&shstrtab_header, 1, sizeof(shstrtab_header), f);
    char shstrtab[shstrtab_header.sh_size];
    fseek(f, shstrtab_header.sh_offset, SEEK_SET);
    fread(shstrtab, 1, shstrtab_header.sh_size, f);
    procesar_secciones(f, &header, &shstrtab_header, shstrtab, permisos, nombres_seccion, num_nombres);
    fclose(f);
}

void procesar_argumentos(int argc, char *argv[], char **archivo, unsigned int *permisos, char **nombres_seccion[], int *num_nombres) {
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--archivo=", 10) == 0) {
            *archivo = argv[i] + 10;
        } else if (strncmp(argv[i], "--permisos=", 11) == 0) {
            char *perm_str = argv[i] + 11;
            for (int j = 0; perm_str[j] != '\0'; j++) {
                switch (perm_str[j]) {
                    case 'r':
                        *permisos |= SHF_ALLOC;
                        break;
                    case 'w':
                        *permisos |= SHF_WRITE;
                        break;
                    case 'x':
                        *permisos |= SHF_EXECINSTR;
                        break;
                    default:
                        fprintf(stderr, "Bleh~\nNo es un permiso valido: %c\n", perm_str[j]);
                        exit(1);
                }
            }
        } else if (strncmp(argv[i], "--nombre=", 9) == 0) {
            char *token = strtok(argv[i] + 9, ",");
            int count = 0;
            while (token != NULL) {
                *nombres_seccion = realloc(*nombres_seccion, (count + 1) * sizeof(char*));
                (*nombres_seccion)[count] = token;
                count++;
                token = strtok(NULL, ",");
            }
            *num_nombres = count;
        }
    }
}

int main(int argc, char *argv[]) {
    char *archivo = NULL;
    unsigned int permisos = 0;
    char **nombres_seccion = NULL;
    int num_nombres = 0;
    if (argc < 2) {
        fprintf(stderr, "Uso: %s --archivo=<ELF> [--permisos=<permisos>] [--nombre=<nombre_sección>,<nombre_sección>]\n", argv[0]);
        return 1;
    }
    procesar_argumentos(argc, argv, &archivo, &permisos, &nombres_seccion, &num_nombres);
    if (archivo == NULL) {
        fprintf(stderr, "Bleh~\nFalta el argumento --archivo.\n");
        return 1;
    }
    procesar_archivo(archivo, permisos, nombres_seccion, num_nombres);
    free(nombres_seccion);
    return 0;
}
