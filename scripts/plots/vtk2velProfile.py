import numpy as np
import vtk
import vtkmodules.util.numpy_support as vtknp
import glob

out_file = "/Users/christianmacbook/Documents/NanoFlowRemote/velocity_profile.csv"

all_files = glob.glob('/Users/christianmacbook/Documents/NanoFlowRemote/basevy/*.vtu')
# sort by number
all_files = sorted(all_files, key=lambda x: int(x.split('_')[-1].split('.')[0]))

numFiles = len(all_files)

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
    yVelocities = velocitiesData[:, 1]

    meanPerX = np.zeros(50)
    numPerX = np.zeros(50)
    width = 30 / 50

    for x, v in zip(xCoords, yVelocities):
        meanPerX[int(x // width)] += v
        numPerX[int(x // width)] += 1

    numPerX[numPerX == 0] = 1 # avoid division by zero
    meanPerX = meanPerX / numPerX

    # append mean velocity result to csv
    with open(out_file, 'a') as f:
        f.write(','.join(map(str, meanPerX)) + '\n')

    print(f'Processed {i+1}/{numFiles} files')
