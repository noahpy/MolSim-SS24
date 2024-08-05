#!/bin/bash
#SBATCH -J nano_flow_base
#SBATCH -o ./%x.%j.%N.out
#SBATCH -D ./MolSim-SS24/build
#SBATCH --get-user-env
#SBATCH --clusters=cm2_tiny
#SBATCH --partition=cm2_tiny
#SBATCH --mem=100MB
#SBATCH --get-user-env
#SBATCH --cpus-per-task=8
#SBATCH --export=NONE
#SBATCH --mail-user=ge42joq@mytum.de
#SBATCH --time=48:00:00

src/MolSim ../input/nano_scale_flow.xml -x -s 4