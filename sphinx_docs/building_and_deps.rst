=====================================
Building the project and dependencies
=====================================

Building
========
This project uses one simple Makefile. It can be modified easily if you need to modify the project (and it is licensed accordingly under the GPL- you are encouraged to do so).

You can make the executable using:

.. code-block:: bash

   make


You can make the Doxygen documentation using:

.. code-block:: bash

   make doxygen


You can also remake the Sphinx documentation if necessary:

.. code-block:: bash

   cd docs
   make html

Dependencies
============

This project depends on GTK+3.0, `gnuplot-cpp`_ from Martin Ruenz, and gnuplot. 
This project installs gnuplot-cpp for you since it is a very small header-only library. If you are having trouble compiling due to GTK-related errors, you may need to install the development files for the GTK library:

.. code-block:: bash

   sudo apt install libgtk-3-dev

If you do not already have gnuplot installed, you can install it with:

.. code-block:: bash

   sudo apt install gnuplot

.. _gnuplot-cpp: https://github.com/martinruenz/gnuplot-cpp

