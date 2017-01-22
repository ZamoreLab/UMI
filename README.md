# Tools to process UMI libraries used in the Zamore Lab 
[![Build Status](https://travis-ci.org/ZamoreLab/UMI.svg?branch=master)](https://travis-ci.org/ZamoreLab/UMI)
[![CircleCI](https://circleci.com/gh/ZamoreLab/UMI/tree/master.svg?style=svg)](https://circleci.com/gh/ZamoreLab/UMI/tree/master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a8d1d04811d14b75a1af5b93b76ec91c)](https://www.codacy.com/app/bowhan/UMI?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ZamoreLab/UMI&amp;utm_campaign=Badge_Grade)
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



