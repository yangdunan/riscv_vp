# RISC-V based Virtual Prototype (VP)

### Key features of our VP:

 - RV32IMAC core + machine mode CSRs
 - Implemented in SystemC TLM-2.0
 - SW debug capabilities (GDB RSP interface) with Eclipse
 - FreeRTOS support
 - Generic and configurable bus
 - CLINT and PLIC-based interrupt controller + additional peripherals
 - Instruction-based timing model + annotated TLM 2.0 transaction delays
 - **New:** Peripherals, e.g. display, flash controller, preliminary ethernet
 - **New:** Example configuration for the SiFive HiFive1 board available
 - **New:** Zephyr operating system support
 - **New:** Support for simulation of multi-core platforms
 - **New:** Supervisor- and user-mode (including user traps) privilege levels
 - **New:** Floating-point extension (F) for the RV32 core
 - **New:** RV64GC core (i.e. RV64IMAFDC)

For related information, e.g. verification, please visit http://www.systemc-verification.org/ or contact <riscv@systemc-verification.org>. 
We accept pull requests and in general contributions are very welcome. 

In the following we provide build instructions and how to compile and run software on the VP.


#### 1) Build the RISC-V GNU Toolchain:

(Cross-)Compiling the software examples, in order to run them on the VP, requires the RISC-V GNU toolchain to be available in PATH. Several standard packages are required to build the toolchain. On Ubuntu the required packages can be installed as follows:

```bash
sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
```

On Fedora, following actions are required:
```bash
sudo dnf install autoconf automake curl libmpc-devel mpfr-devel gmp-devel gawk bison flex texinfo gperf libtool patchutils bc zlib-devel expat-devel cmake boost-devel
sudo dnf groupinstall "C Development Tools and Libraries"
#optional debuginfo
sudo dnf debuginfo-install boost-iostreams boost-program-options boost-regex bzip2-libs glibc libgcc libicu libstdc++ zlib
```

For more information on prerequisites for the RISC-V GNU toolchain visit https://github.com/riscv/riscv-gnu-toolchain. With the packages installed, the toolchain can be build as follows:

```bash
git clone https://github.com/riscv/riscv-gnu-toolchain.git
cd riscv-gnu-toolchain
git submodule update --init --recursive

./configure --prefix=$(pwd)/../riscv-gnu-toolchain-dist-rv32imac-ilp32 --with-arch=rv32imac --with-abi=ilp32

make
```

For additional configurations and options for the RISC-V GNU toolchain visit https://github.com/riscv/riscv-gnu-toolchain.


#### 2) Build this RISC-V Virtual Prototype:

i) in *vp/dependencies* folder (will download and compile SystemC, and build a local version of the softfloat library):

```bash
./build_systemc_233.sh
./build_softfloat.sh
```


ii) in *vp* folder (requires the *boost* C++ library):
 
```bash
mkdir build
cd build
cmake ..
make install
```

The *install* argument is optional, it will copy all VP executables to the local *vp/build/bin* folder.


#### 3) Compile and run some Software:

In *sw*:

```bash
cd simple-sensor    # can be replaced with different example
make                # (requires RISC-V GNU toolchain in PATH)
make sim            # (requires *riscv-vp*, i.e. *vp/build/bin/riscv-vp*, executable in PATH)
```

Please note, if *make* is called without the *install* argument in step 2, then the *riscv-vp* executable is available in *vp/build/src/platform/basic/riscv-vp*.


#### 4) Optional Makefile:

The toplevel Makefile can alternatively be used to build the VP including its dependencies (i.e. step 2 in this README), from the toplevel folder call:

```bash
make
```

This will also copy the VP binaries into the *vp/build/bin* folder.
