#!/bin/zsh


set -xe


clang main.c particles.c -lraylib -lm -o particle -g
