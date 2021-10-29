import scipy
import subprocess
import numpy as np
from scipy import interpolate
from matplotlib import pyplot as plt

# Plot performance chart and export it to a png
def plot_performances(option, name, color):
    xi, yi = [], [] # x and y data for the chart
    out = subprocess.check_output(["./out", option])
    # Parsing data
    for line in out.splitlines():
        # Decoding data from a single line
        line = line.decode("utf-8")
        print(line)
        data = line.split()
        # Getting x and y values
        x = int(data[-2][:-1])/1000
        y = float(data[-1])
        xi.append(x)
        yi.append(y)
    # Plotting and interpolation
    xnew = np.linspace(0, max(xi), 1000)
    p = interpolate.interp1d(xi, yi, kind='cubic') # Cubic polynomial interp.
    plt.plot(xi, yi, f"{color}o", label=name)
    plt.plot(xnew, p(xnew), f"{color}")

if __name__ == "__main__":
    print("Cleaning project...")
    subprocess.call(["make", "clean"])
    
    print("Building project...")
    subprocess.call("make")
    
    print("Plotting performances...")
    for option in [("-n", "Naive (mulpu)", "r"),
            ("-k1",  "Decomposition (mulpuk1)", "b")]:
                   #("-k", "Recursive", "g")]:
        print(f"Plotting \"{option[1]}\"...")
        plot_performances(*option)

    plt.legend(loc="best")
    plt.savefig("performances.png")
    print("Done.")
