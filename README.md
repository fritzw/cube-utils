# A faster open-source alternative to CodeX

The [original CodeX tool](https://groups.google.com/forum/#!topic/kisslicer-refugee-camp/ZMuIrtn5Mfo)
can convert G-code in the form of `.bfb` files to `.cubepro` and `.cubex`
files for the CubePro and CubeX lines of 3D-printers.
However, CodeX is quite slow, especially when run on Linux or OSX using wine,
where processing a small file can take several hours.

The cube-encoder in this project is a faster alternative to CodeX, written in plain C. Here's a quick comparison on two comparable processors:

* 11 MB file with CodeX64.exe on Windows on a 2.4 GHz Xeon E5620: 8 minutes and 14 seconds
* 11 MB file with cubepro-encoder on Linux on a 2.1 GHz Opteron 6172: 1.2 seconds

## Installation

Currently the program is designed only for Linux, but it should also compile on Mac OS X.
If anyone wants to add support for Windows, you'd be welcome. 

To compile the project on Linux, the following steps should be sufficient (the first one is Ubuntu/Debian-specific):
```
sudo apt-get install build-essential
git clone 'https://github.com/fritzw/cube-utils.git'
cd cube-utils
make all
make test
```

These commands will install the compiler, download and compile the program, and execute all test cases to verify that it works correctly. After this you will have two files called `cubepro-encoder` and `cubex-encoder`. The first one will encode `.bfb` files into `.cubepro` files and the second one into `.cubex` files. *(In fact both encoders are identical. Only the name makes the difference, so do not rename them).*

## Usage

The synopsis is
```
cubepro-encoder inputfile [outputfile]
cubex-encoder inputfile [outputfile]
```
where the outputfile is optional. To encode a `.bfb` file, simply call `cubepro-encoder somefile.bfb` and it will create `somefile.cubepro`. Same for `cubex-encoder`, but with a `.cubex` file extension. If that doesn't suit you, you can specify any output file name you like.
