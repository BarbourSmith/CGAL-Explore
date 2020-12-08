emcc -O3 -DCGAL_DISABLE_ROUNDING_MATH_CHECK -fdeclspec lib/canvas.cc -I C:\dev\boost_1_74_0 -I C:\dev\CGAL-5.1.1\auxiliary\gmp\include -I C:\dev\CGAL-5.1.1\include -L C:\dev\CGAL-5.1.1\auxiliary\gmp -s WASM=1 -s EXPORTED_FUNCTIONS="['_main', '_displayShape', '_getDisplayBufferAddress']" -s TOTAL_MEMORY=16MB -lgmp -o public/canvas.js