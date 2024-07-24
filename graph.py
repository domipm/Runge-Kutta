import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import rcParams

rcParams['animation.convert_path'] = r'/usr/bin/convert'

data = np.loadtxt("data.dat")

PL = int(len(data[0,:])/2)
IT = int(len(data[:,0]))

x = np.zeros((IT, PL))
y = np.zeros((IT, PL))

for i in range(0,IT):
    for j in range(0,PL):
        x[i][j] = data[i,(2*j)]
        y[i][j] = data[i,(2*j+1)]

fig, ax = plt.subplots()

xldata, yldata = [], []

tierra, = plt.plot(0,0,linestyle='',color="Blue",marker='.',markersize=20,label="Earth",animated=False)
luna, = plt.plot([],[],linestyle='',color="Grey",marker='.',markersize=10,label="Moon",animated=True)
cohete, = plt.plot([],[],linestyle='',color="Red",marker='.',markersize=5,label="Spaceship",animated=True)
lcohete, = plt.plot([],[],linestyle='-',color="Red",alpha=0.5,marker='',markersize=0,label="",animated=True)

def init():

    ax.set_title("Spaceship to the Moon")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_xlim(-1.25,1.25)
    ax.set_ylim(-1.25,1.25)
    ax.legend()

    return

def update(i):

    if (len(xldata)>100):
        del xldata[0]
        del yldata[0]

    xldata.append(x[i][1])
    yldata.append(y[i][1])
    
    lcohete.set_data(xldata, yldata)
    luna.set_data(x[i,0],y[i,0])
    cohete.set_data(x[i,1],y[i,1])

    return luna, cohete, lcohete,

anim = animation.FuncAnimation(fig, update, frames=range(0,IT,1000), init_func=init)

anim.save("cohete.gif", writer=animation.PillowWriter(fps=30), dpi=300)

plt.tight_layout()
plt.close()