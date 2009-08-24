#! /bin/bash

# Note: This builds under OS X.
# Under Linux you'd need other flags to create .so.

gcc src/perlin.c -O2 -Werror -Wall --pedantic -c89 $LDFLAGS -llua -dynamiclib -undefined dynamic_lookup -o perlin.so

gcc src/mesa/prog_noise.c src/simplex.c -O2 -Werror -Wall --pedantic -c89 $LDFLAGS -llua -dynamiclib -undefined dynamic_lookup -o simplex.so
