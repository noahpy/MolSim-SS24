#!/bin/bash
#SBATCH -J simTeamF
#SBATCH -D ./MolSim-SS24/build
#SBATCH --get-user-env
#SBATCH --mem=1000mb
#SBATCH --cpus-per-task=1
#SBATCH --export=NONE
#SBATCH --mail-user=noah.schlenker@tum.de
#SBATCH --time=00:30:00

src/MolSim ../input/eingabe-sonne.txt -e 1000 
