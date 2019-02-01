##########################
How to build flom manually
##########################


build requirements
******************

- boost (headers)
- protobuf 3.0.0 or later
- cmake 3.13.2 or later
- c++17 compiler

  - clang 5.0 or later
  - gcc 6.1 or later

- c++17 standard library

  - libc++ 7 or later
  - libstdc++ 6 or later


procedure
*********

.. code-block:: bash

   git clone https://github.com/monomotion/flom --recursive
   cd flom
   mkdir build && cd $_
   cmake ..
   make -j $(nproc)
   sudo make install

