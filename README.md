# Tools to process UMI libraries used in the Zamore Lab 
[![Build Status](https://travis-ci.org/ZamoreLab/UMI.svg?branch=master)](https://travis-ci.org/ZamoreLab/UMI)
<br>
This repository stores codes used to parse Illumina libraries made with unique molecular identifier (UMI).<br> 
*Under development*

## Pre-requirement
- Relative new C++ compiler (support C++11)
- Python2

## Installation

```bash
git clone --recursive git@github.com:ZamoreLab/UMI.git
cd UMI && mkdir build && cd build
cmake -DBUILD_TESTS=ON .. && make && make test && make install
```

## Usage



