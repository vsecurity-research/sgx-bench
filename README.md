# Port or Shim? (The Source Code)

This repository (and the ones it links to) contain the source code for the benchmarks designed and executed in the research paper:

Aisha Hasan, Ryan Riley, and Dmitry Ponomarev, "Port or Shim? Stress Testing Application Performance on Intel SGX", Proceedings of the 2020 IEEE International Symposium on Workload Characterization (IISWC-2020), October 2020.

If you use these benchmarks and publish anything based on them, we would love a citation.

Note: This documentation and the source code is research-quality at best, but we're very open to improving it.  Open an issue on the issue tracker if you have questions and/or need some assistance getting this up and running for a project you are doing.

## Hardware/Software Requirements

**Hardware**:  
You'll need hardware with SGX available and enabled.

**Software**:  
In our paper, we used Ubuntu 16.04 with the Intel SGX Linux SDK version 2.7.1.  I strongly suspect this will work fine with Ubuntu 18.04 and a newer version of the SGX SDK as well.

Instructions for installing either of those is outside the scope of this document.  The following may be helpful for install and testing the SGX SDK:

* [https://github.com/intel/linux-sgx](https://github.com/intel/linux-sgx)

## LMbench

In the paper, we made four different versions of part of the LMbench benchmarking suite.  For details about what each of these is, please see the paper.

* The forkless version can be found in this repository.  Just go to [`lmbench/forkless/src`](https://github.com/vsecurity-research/sgx-bench/tree/master/lmbench/forkless/src) and run `make`.
* The noSGX version can be found in this repository.  Each of the three ported benchmarks is in a separate directory in [`lmbench/noSGX`](https://github.com/vsecurity-research/sgx-bench/tree/master/lmbench/noSGX), laid out similar to the way an SGX program is (but SGX is not used).  In each directory, run `make`.
* The SGX version can be found in this repository.  Each of the three ported benchmarks is in a separate directory in [`lmbench/SGX`](https://github.com/vsecurity-research/sgx-bench/tree/master/lmbench/noSGX).  Assuming you have the SGX SDK installed properly, in each directory you can just run `make` to build the application and associated enclave.
* The Graphene-SGX version is not in this repository.  See the Graphene-SGX section below.

## Custom Benchmarks

We also built four different custom benchmarks to really try and zero-in on the type of workloads where porting is better than shimming.  The SGX versions are in the repository in [`custom/`](https://github.com/vsecurity-research/sgx-bench/tree/master/custom).  Assuming you have the SGX SDK installed properly, each can be built by just running `make`.

Two of the benchmarks also have Graphene-SGX versions. See the Graphene-SGX section below.

## Graphene-SGX Versions

Our Graphene implementations of relevant benchmarks can be found at [https://github.com/vsecurity-research/sgx-bench-graphene-tests/](https://github.com/vsecurity-research/sgx-bench-graphene-tests/) in the `shim-*` directories.  However, you can't just clone that repository and use them.  You need to build Graphene-SGX and then compiler them as part of it.

To get you started, we have a fork of the Graphene-SGX repository at [https://github.com/vsecurity-research/graphene](https://github.com/vsecurity-research/graphene).  It has the the appropriate submodule setup to include example code that includes our applications in `LibOS/shim/test/apps/shim-*`.

**Instructions:**

1. Clone [https://github.com/vsecurity-research/graphene](https://github.com/vsecurity-research/graphene)
2. Follow the instructions in the README to build and install Graphene-SGX, including the kernel driver.
3. Make sure you can build and run the `helloworld` sample program.  (Not the Python one.  I don't think that one works.)
4. Follow the README instructions related to "Including Application Test Cases".
5. Now you should be able to build and run our benchmarks in `LibOS/shim/test/apps/shim-*`.  There is a README in each directory with more details.