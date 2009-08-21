#! /bin/bash

gcc src/perlin.c $LDFLAGS -llua -dynamiclib -undefined dynamic_lookup -o perlin.so
