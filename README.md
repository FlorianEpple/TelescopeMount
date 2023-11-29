# Lovely welcome to Telescope Mount

This software is used to to calculate the difference between two star positions and send this to an Arduino, which is supposed to move a telescope according to these differences.

> Software Version:
> 1.0 Beta

# Where do I find what?

Here you will find lots of tests for the final software.

The final software can be found under /software

# How to install

Use Git to download the entire directory and track versions
```bash
# download via git
git clone https://github.com/FlorianEpple/TelescopeMount.git
```

## Complete installation

 - under Mac

```bash
# clone via git
git clone https://github.com/FlorianEpple/TelescopeMount.git

# build using gcc
gcc -g TelescopeMount/software/telescopesteering.c -o TelescopeMount/software/soft.out

# change current working directory
cd TelescopeMount/software/

# running software
./soft.out
```

> after installation use:
> ```bash
> ./build.sh
> ```

- under windows

```bash
# clone via git
git clone https://github.com/FlorianEpple/TelescopeMount.git

# build using gcc
gcc -g TelescopeMount\software\telescopesteering.c -o TelescopeMount\software\soft.exe

# change current working directory
cd TelescopeMount\software\

# running software
soft
```
