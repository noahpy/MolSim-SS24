import numpy as np
import vtk
import vtkmodules.util.numpy_support as vtknp
import glob
import pickle

out_file = "/Users/christianmacbook/Documents/NanoFlowRemote/velocity_profile.csv"

all_files = glob.glob('/Users/christianmacbook/Documents/nanoFlowRemote/base_vy/*.vtu')
# sort by number
all_files = sorted(all_files, key=lambda x: int(x.split('_')[-1].split('.')[0]))

numFiles = len(all_files)

meanPerX = np.zeros((numFiles, 50, 20))
numPerX = np.zeros((numFiles, 50, 20))

for i, filename in enumerate(all_files):
    reader = vtk.vtkXMLUnstructuredGridReader()
    reader.SetFileName(filename)
    reader.Update() 
    output = reader.GetOutput()

    points = output.GetPoints()
    velocities = output.GetPointData().GetArray('velocity')

    pointsData = vtknp.vtk_to_numpy(points.GetData())
    velocitiesData = vtknp.vtk_to_numpy(velocities)

    xCoords = pointsData[:, 0]
    zCoords = pointsData[:, 2]
    yVelocities = velocitiesData[:, 1]

    width = 30 / 50
    depth = 12 / 20

    for x, z, v in zip(xCoords, zCoords, yVelocities):
        coordX = int(x // width)
        coordZ = int(z // depth)
        meanPerX[i, coordX, coordZ] += v
        numPerX[i, coordX, coordZ] += 1

    numPerX[i][numPerX[i] == 0] = 1 # avoid division by zero
    meanPerX[i] = meanPerX[i] / numPerX[i]

    print(f'Processed {i+1}/{numFiles} files')

# write to pkl
with open("out_file_density", 'wb') as f:
    pickle.dump(numPerX, f)

with open("out_file_mean", 'wb') as f:
    pickle.dump(meanPerX, f)
