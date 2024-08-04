#!/bin/zsh


set -xe


clang main.c particles.c -lraylib -o particle -g
