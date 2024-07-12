#!/bin/bash
#SBATCH -J nano_flow_turbulence
#SBATCH -o ./output/%x.%j.%N.out
#SBATCH -D ./MolSim-SS24-NanoFlow/build
#SBATCH --get-user-env
#SBATCH --clusters=cm2_tiny
#SBATCH --partition=cm2_tiny
#SBATCH --mem=100MB
#SBATCH --get-user-env
#SBATCH --cpus-per-task=8
#SBATCH --export=NONE
#SBATCH --mail-user=ge42joq@mytum.de
#SBATCH --time=48:00:00

src/MolSim ../input/nano_scale_mods/nano_flow_turbulence.xml -x -s 4