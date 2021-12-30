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

* **Windows:** Just download the `.exe` file you need from the [releases page](https://github.com/fritzw/cube-utils/releases) and place them where you like.
* **Linux/Mac:** Clone the repository and run `make` to build. You need to install the build tools for your operating system first (e.g. `sudo apt-get install build-essential` on Ubuntu).
```
git clone 'https://github.com/fritzw/cube-utils.git'
cd cube-utils
make all
make test
```

These commands will download and compile the program and execute all test cases to verify that it works correctly.
After this you will have three files called `cubepro-encoder`, `cube3-encoder` and `cubex-encoder`.
`cubepro-encoder` will encode `.bfb` files into `.cubepro` files.
`cube3-encoder` will encode `.bfb` files into `.cube3` files.
`cubex-encoder` will encode `.bfb` files into `.cubex` files.
*(In fact all encoders are identical. Only the name makes the difference, so do not rename them).*

## Usage

<img src="https://raw.githubusercontent.com/fritzw/cube-utils/master/windows-screenshot.png"/>

* Option 1: Just drop the `.bfb` file on the correct encoder program with your mouse.
* Option 2: Run the encoder form the command line as follows:
```
cubepro-encoder inputfile [outputfile]
cube3-encoder inputfile [outputfile]
cubex-encoder inputfile [outputfile]
```
where the outputfile is optional. To encode a `.bfb` file, simply call `cubepro-encoder somefile.bfb` and it will create `somefile.cubepro`.
Same for `cube3-encoder` (but with a `.cube3` file extension) and `cubex-encoder` (but with a `.cubex` file extension).
If that doesn't suit you, you can specify any output file name you like.

To decode an encoded `.cubepro` or `.cubex` file, just drop it on the `cube-decoder` program or run the following command. The file type to decode is automatically determined from the file extension of the input file.
```
cube-decoder inputfile [outputfile]
```
