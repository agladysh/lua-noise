#! /bin/bash

gcc src/perlin.c -O2 -Werror -Wall --pedantic -c89 $LDFLAGS -llua -dynamiclib -undefined dynamic_lookup -o perlin.so
