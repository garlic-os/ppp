#!/bin/bash

# make git ignore the submodules from here because putting their names in
# .gitignore doesn't work
git config submodule.libpurple-mini.ignore all
git config submodule.munit.ignore all

# configure libpurple-mini
pushd ./lib/libpurple-mini
make version.h
