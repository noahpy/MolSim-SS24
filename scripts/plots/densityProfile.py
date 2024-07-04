import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Dims as defined in the Analyzer
dims = (50, 1, 1)
# Select the two axis to keep (index)
whichAxis = (0,2)
axisNames = ['x-Axis', '']
plotTitle = 'Velocity Profile along x axis'

density = np.loadtxt('./results/analysis_density_1D.csv', delimiter=',')
density = density.reshape((len(density),dims[whichAxis[1]],dims[whichAxis[0]]))

velocity = np.loadtxt('./results/analysis_velocity_1D.csv', delimiter=',')
velocity = velocity.reshape((len(velocity),dims[whichAxis[1]],dims[whichAxis[0]]))

numPlots = len(density)
assert len(density) == len(velocity)

data = density

# Plot an animated heatmap
fig, ax = plt.subplots()

# Add color bar
heatmap = ax.pcolor(data[0])
fig.colorbar(heatmap, ax=ax)

minVal = np.min(data)
maxVal = np.max(data)

def plot(idx):
    ax.clear()
    ax.set_xlabel(axisNames[0])
    ax.set_ylabel(axisNames[1])
    ax.set_title(plotTitle)
    heatmap = ax.pcolor(data[idx], vmin=minVal, vmax=maxVal)
    return heatmap

# Animate
ani = animation.FuncAnimation(fig, plot, frames=numPlots, interval=1)
plt.show()
