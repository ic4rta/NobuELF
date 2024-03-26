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

Uso: ./nobu-elf --file=\<ELF> [--permissions=\<permisos>] [--name=\<nombre_sección>, <nombre_seccion>]

---
Puedes combinar multiples permisos en el argumento ```--permissions```, los permisos validos son:
- r: lectura
- w: escritura
- x: ejecucion

Esta opcion hace match exactamente con las secciones que tengan el permisos especificado, por lo que si pones ```--permissions=x``` te mostrara solo las secciones con permisos de ejecucion, mas no te mostrara las secciones que tengan permisos de ejecucion y otro permiso (la parte de ejemplos se explica mejor)

Tambien puedes filtrar por el nombre de una seccion usando ```--name```, pudes especificar mas de una seccion separandolas por una coma
  
#### Ejemplos
- Permisos de lectura: ```nobu-elf --file=/ruta/elf --permissions=r```
- Permisos escritura: ```nobu-elf --file=/ruta/elf --permissions=w```
- Permisos de lectura y escritura: ```nobu-elf --file=/ruta/elf --permissions=rw```
- Permisos de lectura y ejecucion: ```nobu-elf --file=/ruta/elf --permissions=rx```
- Filtrar por la seccion .text: ```nobu-elf --file=/ruta/elf --name=.text```
- Filtrar por la seccion .plt: ```nobu-elf --file=/ruta/elf --name=.plt```
- Filtrar por la seccion .text e .init: ```nobu-elf --file=/ruta/elf --name=.text,.init``` 

## English

**ELF parser that allows you to display sections and filter by permissions or name**.

Makes use of elf.h to manipulate the ELF file

### Usage

```bash
git clone https://github.com/ic4rta/Nobu-ELF.git
gcc nobu-elf.c -o nobu-elf
```

Usage: ./nobu-elf --file=\<ELF> [--permissions=\<permissions>] [--name=\<section_name>, \<section_name>]

---
You can combine multiple permissions in the ```--permissions``` argument, the valid permissions are:
- r: read
- w: write
- x: execute

This option matches exactly the sections that have the specified permissions, so if you put ```---permissions=x``` it will show you only the sections with execute permissions, but it will not show you the sections that have execute permissions and another permission (the examples part is better explained).

You can also filter by the name of a section using ```--name```, you can specify more than one section by separating them by a comma.
  
#### Examples
- Read permissions: ```nobu-elf --file=/path/elf --permissions=r```.
- Write permissions: ```nobu-elf --file=/path/elf --permissions=w```
- Read and write permissions: ```nobu-elf --file=/path/elf --permissions=rw```
- Read and execute permissions: ```nobu-elf --file=/path/elf --permissions=rx```
- Filter by .text section: ```nobu-elf --file=/path/elf --name=.text```
- Filter by .plt section: ```nobu-elf --file=/path/elf --name=.plt```
- Filter by .text and .init section: ```nobu-elf --file=/path/elf --name=.text,.init```


![](nobu-elf.png)
