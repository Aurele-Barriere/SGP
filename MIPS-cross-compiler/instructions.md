# Installation d'un compilateur croisé MIPS sous Debian

Document rédigé le 02/11/2016. Les versions indiquées étaient les plus récentes à cette date.
Adapté de <https://www.linux-mips.org/wiki/Toolchains>

## Outils nécessaires

[binutils-2.27](https://www.gnu.org/software/binutils/)
[gcc-6.2](https://gcc.gnu.org/)

## Variables d'environnement

Il est pratique d'exporter quelques variables d'environnement

```
export WDIR=/tmp
export TARGET=mipsel-elf
export PREFIX=/opt/cross
```

Il est également nécessaire d'exporter un nouveau `PATH`:
```
$ export PATH=${PATH}:${PREFIX}/bin
```

## Installation de Binutils

Nous allons maintenant nous placer dans le "staging directory":
```
cd $WDIR
mkdir ${TARGET}-toolchain
cd ${TARGET}-toolchain
```

Extraction, configuration, compilation et installation de binutils:
```
tar -xzf binutils-2.27.tar.gz
mkdir build-binutils && cd build-binutils
../binutils-2.27/configure --target=$TARGET --prefix=$PREFIX
make
make install
cd ..
```

Cross-binutils est maintenant installé dans le dossier `${PREFIX}/bin/` avec des noms préfixés par `${TARGET}`

## Installation de GCC
Extraction, configuration, compilation et installation de binutils:
```
tar -xzf gcc-6.2.0.tar.gz
mkdir build-gcc-bootstrap && cd build-gcc-bootstrap
../gcc-6.2.0/configure --target=$TARGET --prefix=$PREFIX \
--enable-languages=c --without-headers \
--with-gnu-ld --with-gnu-as \
--disable-shared --disable-threads \
--disable-libmudflap --disable-libgomp \
--disable-libssp --disable-libquadmath \
--disable-libatomic
make -j2
make install
cd ..
```

Le configure peut résulter en une erreur. Dans mon cas, il me manquait deux bibliothèques: MPC (paquet libmpc-dev sous Debian) et MPFR (libmpfr-dev). J'ai installé ces deux bibliothèques avec le gestionnaire de paquet de Debian, mais j'imagine qu'il est possible de les installer autrement.

## Installation et utilisation de GDB
La page originale mentionne la possibilité d'installer un cross-gdb.
Je n'ai pas tenté de le faire, mais je serais intéressé par des retours si quelqu'un a déjà essayé et réussi à faire fonctionner GDB avec Nachos.

## Utilisation
Les nouveaux outils sont installés dans le répertoire voulu. N'oubliez pas de changer votre `PATH` si vous le souhaitez.
