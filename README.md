# NAM to Aida-X Converter

This project provides a tool to convert Neural Amp Modeler (NAM) models to Aida-X compatible models using RTNeural.

## Description

The NAM to Aida-X Converter is designed to bridge the gap between Neural Amp Modeler (NAM) and Aida-X, allowing users to leverage NAM models in Aida-X compatible environments. This tool utilizes RTNeural to facilitate the conversion process.

## Features

- Convert NAM models to Aida-X compatible format
- Utilize RTNeural for efficient neural network processing
- Easy-to-use conversion script

## Prerequisites

Before you begin, ensure you have met the following requirements:

- Git
- CMake
- C++ compiler with C++17 support
- libsndfile development package

## Installation

1. Clone the repository with submodules:

```bash
git clone --recursive https://github.com/baptistejamin/nam-to-rtneural.git
```

2. Install libsndfile (if not already installed):

- On Ubuntu or Debian-based systems:
```
sudo apt-get install libsndfile1-dev
```

- On macOS with Homebrew:
```
brew install libsndfile
```

3. Download a nam file a place it in `res/0.nam`

4. Execute `sh nam_to_aida.sh`
