# Single, damped pendulum

from numpy import sin, cos
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

G = 9.8  # acceleration due to gravity, in m/s^2
L = 1.0  # length of the pendulum in m
alpha = 0.03  # damping

# create a time array from 0..100 sampled at 0.05 second steps
dt = 0.1
t = np.arange(0.0, 100, dt)

# State at all times:
th = np.zeros_like(t)
w = np.zeros_like(t)

# The initial anglies and angular velocities (degrees per second)
th[0] = np.radians(120)
w[0] = np.radians(0)
th[1] = np.radians(120)
w[1] = np.radians(0)

# integrate the ODE:
# dwdt = -G/L sin(th) - alpha/L*w
# dthdt = w
# using leapfrog
for it in range(1,len(t)-1):
    w[it+1] = w[it-1] - 2*dt*(G/L*np.sin(th[it]) + alpha/L*w[it])
    th[it+1] = th[it-1] + 2*dt*w[it]

# Translate the angles into the end points of lines
x = L*sin(th)
y = -L*cos(th)

# animate the result
fig = plt.figure()
ax = fig.add_subplot(111, autoscale_on=False, xlim=(-1.1, 1.1), ylim=(-1.1, 0.6))
ax.grid()

line, = ax.plot([], [], 'o-', lw=2)
time_template = 'time = %.1fs'
time_text = ax.text(0.05, 0.9, '', transform=ax.transAxes)


def init():
    line.set_data([], [])
    time_text.set_text('')
    return line, time_text


def animate(i):
    thisx = [0, x[i]]
    thisy = [0, y[i]]
    
    line.set_data(thisx, thisy)
    time_text.set_text(time_template % (i*dt))
    return line, time_text

line.set_data([0, x[0]], [0, y[0]])
time_text.set_text(time_template % (0))
plt.savefig("lf.png")

ani = animation.FuncAnimation(fig, animate, np.arange(1, len(y)),
                              interval=100, blit=True, init_func=init)

#ani.save('pendulum_lf.mp4', fps=15)
plt.show()
