#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <elf.h>

void permisos(unsigned int flags) {
    if (flags == 0)
        printf("N/A ");
    if (flags & SHF_ALLOC)
        printf("r");
    if (flags & SHF_WRITE)
        printf("w");
    if (flags & SHF_EXECINSTR)
        printf("x");
    printf("\n");
}

void imprimir_secciones(Elf64_Shdr *section_header, char *shstrtab) {
    printf("%-20s 0x%016lx     0x%-012lx  %-11lu  ", shstrtab + section_header->sh_name, section_header->sh_addr, section_header->sh_offset, section_header->sh_size);
    permisos(section_header->sh_flags);
}

void procesar_secciones(FILE *archivo, Elf64_Ehdr *header, Elf64_Shdr *shstrtab_header, char *shstrtab, unsigned int permisos, char *nombres_secciones[], int contador_secciones) {
    fseek(archivo, header->e_shoff, SEEK_SET);
    Elf64_Shdr section_header;
    for (int i = 0; i < header->e_shnum; ++i) {
        fread(&section_header, 1, sizeof(section_header), archivo);
        if (permisos != 0) {
            if ((permisos & SHF_ALLOC) != (section_header.sh_flags & SHF_ALLOC) || (permisos & SHF_WRITE) != (section_header.sh_flags & SHF_WRITE) || (permisos & SHF_EXECINSTR) != (section_header.sh_flags & SHF_EXECINSTR))
                continue;
        }
        bool permiso_encontrado = false;
        if (contador_secciones > 0 && nombres_secciones != NULL) {
            for (int j = 0; j < contador_secciones; j++) {
                if (strcmp(shstrtab + section_header.sh_name, nombres_secciones[j]) == 0) {
                    permiso_encontrado = true;
                    break;
                }
            }
        } else {
            permiso_encontrado = true;
        }
        if (permiso_encontrado) {
            imprimir_secciones(&section_header, shstrtab);
        }
    }
}

void procesar_archivo(char *archivo, unsigned int permisos, char *nombres_secciones[], int contador_secciones) {
    FILE *f = fopen(archivo, "r");
    if (f == NULL) {
        fprintf(stderr, "Bleh~\nCould not open file");
        exit(1);
    }
    Elf64_Ehdr header;
    fread(&header, 1, sizeof(header), f);
    if (header.e_ident[EI_MAG0] != ELFMAG0 || header.e_ident[EI_MAG1] != ELFMAG1 ||
        header.e_ident[EI_MAG2] != ELFMAG2 || header.e_ident[EI_MAG3] != ELFMAG3) {
        fprintf(stderr, "Bleh~\nNot an ELF\n");
        fclose(f);
        exit(1);
    }

    printf("Section              Address                Offset     Size(bytes)      Perm\n");
    printf("----------------------------------------------------------------------------------\n");

    fseek(f, header.e_shoff + header.e_shstrndx * sizeof(Elf64_Shdr), SEEK_SET);
    Elf64_Shdr shstrtab_header;
    fread(&shstrtab_header, 1, sizeof(shstrtab_header), f);
    char shstrtab[shstrtab_header.sh_size];
    fseek(f, shstrtab_header.sh_offset, SEEK_SET);
    fread(shstrtab, 1, shstrtab_header.sh_size, f);
    procesar_secciones(f, &header, &shstrtab_header, shstrtab, permisos, nombres_secciones, contador_secciones);
    fclose(f);
}

void procesar_argumentos(int argc, char *argv[], char **archivo, unsigned int *permisos, char **nombres_secciones[], int *contador_secciones) {
    bool permisos_argumento = false;
    bool nombres_argumento = false;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--file=", 7) == 0) {
            *archivo = argv[i] + 7;
        } else if (strncmp(argv[i], "--permissions=", 14) == 0) {
            if (nombres_argumento) {
                fprintf(stderr, "Bleh~\nYou can't use --name and --permissions at the same time\n");
                exit(1);
            }
            permisos_argumento = true;
            char *perm_str = argv[i] + 14;
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
                        fprintf(stderr, "Bleh~\nInvalid permission %c\n", perm_str[j]);
                        exit(1);
                }
            }
        } else if (strncmp(argv[i], "--name=", 7) == 0) {
            if (permisos_argumento) {
                fprintf(stderr, "Bleh~\nYou can't use --permissions and --name at the same time");
                exit(1);
            }
            nombres_argumento = true;
            char *token = strtok(argv[i] + 7, ",");
            int contador = 0;
            while (token != NULL) {
                *nombres_secciones = realloc(*nombres_secciones, (contador + 1) * sizeof(char *));
                (*nombres_secciones)[contador] = token;
                contador++;
                token = strtok(NULL, ",");
            }
            *contador_secciones = contador;
        }
    }
}

int main(int argc, char *argv[]) {
    char *archivo = NULL;
    unsigned int permisos = 0;
    char **nombres_secciones = NULL;
    int contador_secciones = 0;
    if (argc < 2) {
        fprintf(stderr, "Use: %s --file=<ELF> [--permissions=<permissions>] [--name=<section_name>,<section_name>]\n", argv[0]);
        return 1;
    }
    procesar_argumentos(argc, argv, &archivo, &permisos, &nombres_secciones, &contador_secciones);
    if (archivo == NULL) {
        fprintf(stderr, "Bleh~\nThe --file argument is missing\n");
        return 1;
    }
    procesar_archivo(archivo, permisos, nombres_secciones, contador_secciones);
    free(nombres_secciones);
    return 0;
}
