
import subprocess


def get_run_script(name: str, num_threads: str, input_file: str,
                   clusters: str = "cm2_tiny", partition: str = "cm2_tiny") -> str:
    SCRIPT = f"""#!/bin/bash
#SBATCH -J {name}
#SBATCH -o ./%x.%j.%N.out
#SBATCH -D .
#SBATCH --get-user-env
#SBATCH --clusters={clusters}
#SBATCH --partition={partition}
#SBATCH --mem=200mb
#SBATCH --cpus-per-task={num_threads}
#SBATCH --export=NONE
#SBATCH --mail-user=ge42joq@mytum.de
#SBATCH --time=00:30:00
OMP_NUM_THREADS={num_threads} src/MolSim {input_file} -x -s 4 -p
"""
    return SCRIPT


if __name__ == "__main__":
    for num_threads in [1, 2, 4, 8, 16, 28, 56]:
        name = f"parallel_{num_threads}"
        run_script = ""
        if (num_threads < 16):
            run_script = get_run_script(
                name, num_threads, "../input/reyleigh_3D_short.xml")
        else:
            run_script = get_run_script(
                name, num_threads, "../input/reyleigh_3D_short.xml", "cm2", "cm2_std")
        with open(f"{name}.sh", "w") as f:
            f.write(run_script)

        subprocess.call(["sbatch", f"{name}.sh"])
