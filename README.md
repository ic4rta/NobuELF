## Nobu-ELF

<p align="center">
  <img src="https://static.wikia.nocookie.net/fategrandorder/images/4/41/Figure_069.png/revision/latest/scale-to-width-down/250?cb=20200331171136" alt="Imagen" width="320" height="260">
</p>

## Español

**ELF parser que permite mostrar las secciones y filtrar por sus permisos o nombre**

Hace uso de elf.h para manipular el archivo ELF

### Uso

```bash
git clone https://github.com/ic4rta/Nobu-ELF.git
gcc nobu-elf.c -o nobu-elf
```

Uso: ./nobu-elf --archivo=\<ELF> [--permisos=\<permisos>] [--nombre=\<nombre_sección>, <nombre_seccion>]

---
Puedes combinar multiples permisos en el argumento ```--permisos```, los permisos validos son:
- r: lectura
- w: escritura
- x: ejecucion

Esta opcion hace match exactamente con las secciones que tengan el permisos especificado, por lo que si pones ```--permisos=x``` te mostrara solo las secciones con permisos de ejecucion, mas no te mostrara las secciones que tengan permisos de ejecucion y otro permiso (la parte de ejemplos se explica mejor)

Tambien puedes filtrar por el nombre de una seccion usando ```--nombre```, pudes especificar mas de una seccion separandolas por una coma
  
#### Ejemplos
- Permisos de lectura: ```nobu-elf --archivo=/ruta/elf --permisos=r```
- Permisos escritura: ```nobu-elf --archivo=/ruta/elf --permisos=w```
- Permisos de lectura y escritura: ```nobu-elf --archivo=/ruta/elf --permisos=rw```
- Permisos de lectura y ejecucion: ```nobu-elf --archivo=/ruta/elf --permisos=rx```
- Filtrar por la seccion .text: ```nobu-elf --archivo=/ruta/elf --nombre=.text```
- Filtrar por la seccion .plt: ```nobu-elf --archivo=/ruta/elf --nombre=.plt```
- Filtrar por la seccion .text e .init: ```nobu-elf --archivo=/ruta/elf --nombre=.text,.init``` 

## English

**ELF parser that allows you to display sections and filter by permissions or name**.

Makes use of elf.h to manipulate the ELF file

### Usage

```bash
git clone https://github.com/ic4rta/Nobu-ELF.git
gcc nobu-elf.c -o nobu-elf
```

Usage: ./nobu-elf --archivo=\<ELF> [--permisos=\<permissions>] [--nombre=\<section_name>, \<section_name>]

---
You can combine multiple permissions in the ```--permisos``` argument, the valid permissions are:
- r: read
- w: write
- x: execute

This option matches exactly the sections that have the specified permissions, so if you put ```---permisos=x``` it will show you only the sections with execute permissions, but it will not show you the sections that have execute permissions and another permission (the examples part is better explained).

You can also filter by the name of a section using ```--nombre```, you can specify more than one section by separating them by a comma.
  
#### Examples
- Read permissions: ```nobu-elf --archivo=/path/elf --permisos=r```.
- Write permissions: ```nobu-elf --archivo=/path/elf --permisos=w```
- Read and write permissions: ```nobu-elf --archivo=/path/elf --permisos=rw```
- Read and execute permissions: ```nobu-elf --archivo=/path/elf --permisos=rx```
- Filter by .text section: ```nobu-elf --archivo=/path/elf --nombre=.text```
- Filter by .plt section: ```nobu-elf --archivo=/path/elf --nombre=.plt```
- Filter by .text and .init section: ```nobu-elf --archivo=/path/elf --nombre=.text,.init```


![](nobu-elf.png)
