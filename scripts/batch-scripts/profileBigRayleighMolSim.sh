#!/bin/bash
#SBATCH -J big_rayleigh_measure
#SBATCH -o ./%x.%j.%N.out
#SBATCH -D ./MolSim-SS24/build
#SBATCH --get-user-env
#SBATCH --clusters=cm2_tiny
#SBATCH --partition=cm2_tiny
#SBATCH --mem=200MB
#SBATCH --get-user-env
#SBATCH --cpus-per-task=1
#SBATCH --export=NONE
#SBATCH --mail-user=ge42joq@mytum.de
#SBATCH --time=00:30:00

src/MolSim ../input/big_rayleigh_taylor.xml -x -s 4 -p
