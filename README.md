## Nobu-ELF

<p align="center">
  <img src="https://static.wikia.nocookie.net/fategrandorder/images/4/41/Figure_069.png/revision/latest/scale-to-width-down/250?cb=20200331171136" alt="Imagen" width="320" height="260">
</p

**ELF parser que permite mostrar los segmentos y filtrar por sus permisos**

Hace uso de elf.h para manipular el archivo ELF

### Uso

```bash
git clone https://github.com/ic4rta/Nobu-ELF.git
gcc nobu-elf.c -o nobu-elf
```
Uso: ./nobu-elf --archivo=\<ELF> --permisos=\<permisos>

---
Puedes combinar multiples permisos en el argumento ```--permisos```, los permisos validos son:
- r: lectura
- w: escritura
- x: ejecucion
  
#### Ejemplos
- Permisos de lectura: ```nobu-elf --archivo=/ruta/elf --permisos=r```
- Permisos escritura: ```nobu-elf --archivo=/ruta/elf --permisos=w```
- Permisos de lectura y escritura: ```nobu-elf --archivo=/ruta/elf --permisos=rw```
- Permisos de lectura y ejecucion: ```nobu-elf --archivo=/ruta/elf --permisos=rx```
