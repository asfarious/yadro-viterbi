import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("out.csv", delimiter=',', skiprows=1)

x = data[:, 0]
y = data[:, 1]

fig = plt.figure()

ax1 = fig.add_subplot(111)

#ax1.set_title("Mains power stability")    
ax1.set_xlabel('channel error rate')
ax1.set_ylabel('BER')

ax1.plot(x,y, c='r', label='BER')

leg = ax1.legend()

plt.savefig("ber.png")
