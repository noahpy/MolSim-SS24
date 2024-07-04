import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Dims as defined in the Analyzer
dims = (51, 1, 1)
# Select the two axis to keep (index)
whichAxis = (0,1)

density = np.loadtxt('/Users/christianmacbook/Documents/MolDyn/cmake-build-debug/src/analysis_density.csv', delimiter=',')
density = density.reshape((len(density),dims[whichAxis[0]],dims[whichAxis[1]]))

velocity = np.loadtxt('/Users/christianmacbook/Documents/MolDyn/cmake-build-debug/src/analysis_velocity.csv', delimiter=',')
velocity = velocity.reshape((len(velocity),dims[whichAxis[0]],dims[whichAxis[1]]))

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
    ax.set_xlabel('x')
    ax.set_title('Density Profile along x axis')
    heatmap = ax.pcolor(data[idx].T, vmin=minVal, vmax=maxVal)
    return heatmap

# Animate
ani = animation.FuncAnimation(fig, plot, frames=numPlots, interval=1)
plt.show()
