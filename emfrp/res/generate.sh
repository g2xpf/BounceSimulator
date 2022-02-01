#!/bin/bash -e

shopt -s extglob
emfrp BounceSimulator.mfrp
cp BounceSimulator.c ../lib/BounceSimulator.cpp
cp BounceSimulator.h ../lib/BounceSimulator.h
rm !("BounceSimulator.mfrp"|"generate.sh")

