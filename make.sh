#! /bin/bash

gcc src/perlin.c -Werror -Wall --pedantic -c89 $LDFLAGS -llua -dynamiclib -undefined dynamic_lookup -o perlin.so
