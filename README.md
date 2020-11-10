####```Yarn start``` to launch the server. http://localhost:2222/ to see the result.

####```C:\Users\Bar\emscripten\emsdk>emsdk_env.bat```To set up the env

#### ```emcc -DCGAL_DISABLE_ROUNDING_MATH_CHECK lib/canvas.cc -I C:\dev\boost_1_62_0\boost_1_62_0 -I C:\dev\CGAL-5.1\include -s WASM=1 -s EXPORTED_FUNCTIONS="['_main']" -o public/canvas.js   ``` To generate the JS file