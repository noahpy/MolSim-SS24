#!/bin/bash
#SBATCH -J simTeamF
#SBATCH -o ./%x.%j.%N.out
#SBATCH -D ./MolSim-SS24/build
#SBATCH --get-user-env
#SBATCH --clusters=cm2_tiny
#SBATCH --partition=cm2_tiny
#SBATCH --mem=200mb
#SBATCH --get-user-env
#SBATCH --cpus-per-task=1
#SBATCH --export=NONE
#SBATCH --mail-user=ge42joq@mytum.de
#SBATCH --time=00:02:00

src/MolSim ../input/eingabe-sonne.txt -e 1000 
