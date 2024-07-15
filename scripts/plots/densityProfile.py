import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import matplotlib.animation as animation

# Dims as defined in the Analyzer
dims = (50, 1, 20)
# Select the two axis to keep (index)
whichAxis = (0,2)
axisNames = ['x-Axis', 'z-Axis']

dataType = 'Velocity'
plotTitleHeatmap = f'{dataType} Profile along {axisNames[0]} and {axisNames[1]}'
plotTitleProfile = f'{dataType} Profile along {axisNames[0]} axis'
plotSubTitle = f'{dataType} Profiles'

density = np.loadtxt('/Users/christianmacbook/Documents/NanoFlowRemote/base/build/nano_flow_standard_50x_1y_20z_density.csv', delimiter=',')
density = density.reshape((len(density),dims[whichAxis[1]],dims[whichAxis[0]]))

velocity = np.loadtxt('/Users/christianmacbook/Documents/NanoFlowRemote/base/build/nano_flow_standard_50x_1y_20z_velocity.csv', delimiter=',')
velocity = velocity.reshape((len(velocity),dims[whichAxis[1]],dims[whichAxis[0]]))

numPlots = len(density)
# assert len(density) == len(velocity)

data = velocity

# Plot an animated heatmap
fig = plt.figure()
gs = gridspec.GridSpec(2, 2, width_ratios=[1, 0.05], height_ratios=[1, 1])

ax_heatmap = fig.add_subplot(gs[0, 0])
ax_profile = fig.add_subplot(gs[1, 0], sharex=ax_heatmap)
cax = fig.add_subplot(gs[:, 1])

# Add color bar
heatmap = ax_heatmap.pcolor(data[0])
fig.colorbar(heatmap, cax=cax)

minVal = np.min(data)
maxVal = np.max(data)

minAvg = 0
maxAvg = np.max(np.mean(data, axis=1))

def plot(idx):
    ax_heatmap.clear()
    ax_profile.clear()

    # Update heatmap
    ax_heatmap.set_xlabel(axisNames[0])
    ax_heatmap.set_ylabel(axisNames[1])
    ax_heatmap.set_title(plotTitleHeatmap)
    heatmap = ax_heatmap.pcolor(data[idx], vmin=minVal, vmax=maxVal)

    # Update profile
    ax_profile.set_xlabel(axisNames[0])
    ax_profile.set_ylabel("||" + dataType+ "||")
    ax_profile.set_title(plotTitleProfile)
    matrix = data[idx]
    velocities = np.mean(matrix.T, axis=1)
    x = np.arange(0, len(velocities))
    ax_profile.set_ylim(minAvg, maxAvg)
    ax_profile.grid()
    profile = ax_profile.plot(x, velocities)

    # Add a title
    plt.suptitle(plotSubTitle + f'Iteration {idx} (of {numPlots})')

    return heatmap, profile

# Animate
ani = animation.FuncAnimation(fig, plot, frames=numPlots, interval=1)
plt.tight_layout()
plt.subplots_adjust(hspace=0.5, top=0.85, bottom=0.1, left=0.1, right=0.9)
plt.show()
