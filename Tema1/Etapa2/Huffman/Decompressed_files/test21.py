import numpy as np
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt

# fanta B
# D = 164.9
# I0 = 186.5
# a = 0.24

# x = np.array([-18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1,
# 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17])
# y = np.array([1.94, 1.12, 1.75, 3.03, 1.19, 1.46, 4.45, 3.65, 1.70, 7.50, 10.78, 2.5, 12.83,
#         35.2, 24.5, 26.6, 101.2, 170.2, 162.2, 72, 17.3, 21.1, 21.4, 5.92,
#         3.2, 9.3, 5.43, 1.54, 4, 3.62, 0.85, 1.75, 2.4, 1.1, 1.03])

# xdata = np.arange(-18, 18, 1)
# ydata = np.array([1.94, 1.12, 1.75, 3.03, 1.19, 1.46, 4.45, 3.65, 1.70, 7.50, 10.78, 2.5, 12.83,
#         35.2, 24.5, 26.6, 101.2, 170.2, 186.5, 162.2, 72, 17.3, 21.1, 21.4, 5.92,
#         3.2, 9.3, 5.43, 1.54, 4, 3.62, 0.85, 1.75, 2.4, 1.1, 1.03])
    
# ics = np.linspace(-18, 18, 100)

# fanta C
D = 176
I0 = 49
a = 0.48

x = np.array([-12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12])
y = np.array([0.63, 0.68, 0.85, 0.75, 0.77, 1.09, 1.24, 1.95, 3.31, 6.43, 18.40, 48.80,
        20.70, 5.70, 1.82, 2.79, 1.84, 1.63, 1.07, 0.90, 0.79, 0.90, 0.65, 0.71])

xdata = np.arange(-12, 13, 1)
ydata = np.array([0.63, 0.68, 0.85, 0.75, 0.77, 1.09, 1.24, 1.95, 3.31, 6.43, 18.40, 48.80, 49.00,
        20.70, 5.70, 1.82, 2.79, 1.84, 1.63, 1.07, 0.90, 0.79, 0.90, 0.65, 0.71])

ics = np.linspace(-12, 12, 100)

def test(x, k):
        # eps = np.pi * a / lambd * x / D 
        eps = k * x
        return I0 * (np.sin(eps) * np.sin(eps)) / ((eps) * (eps))

param = curve_fit(test, x, y)

print("eps / x : " , param[0])

fig, ax = plt.subplots()
ax.set_xlabel('Position [ mm ]', fontweight ='bold')
ax.set_ylabel('I [ mA ]', fontweight ='bold')

ax.plot(xdata, ydata, 'o', label="Teoretic", color="black")

def f(x):
   return I0 * np.sin(x * param[0]) * np.sin(x * param[0]) / (x * param[0] * x * param[0])

ax.plot(ics, f(ics), color='red')

# solution validation purpose only
def epsilon(x):
    return x * a * np.pi / 0.000522 * x / D / 100
ax.plot(ics, I0 * np.sin(epsilon(ics)) * np.sin(epsilon(ics)) / (epsilon(ics) * epsilon(ics)), color = "green")

plt.show()