import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import linprog

#jedzonko
c=[2,1.5,3]
A=[
    [-4,-6,-15], #bialko
    [-2,-2,0], #tluszcz
    [-5,-3,-4], #witaminy
    [-7,-3,-12], #weglowodany
    ]
B=[-250,-60,-100,-220] 

bounds=[(0,None),(0,None),(0,None)]

res=linprog(c=c,A_ub=A,b_ub=B,bounds=bounds,method='highs')

kanapki=np.ceil(res.x[0])
pierogi=np.ceil(res.x[1])
slodycze = np.ceil(res.x[2])
koszt=kanapki*c[0]+pierogi*c[1]+slodycze*c[2]
#makroskladniki
bialko = 4*kanapki + 6*pierogi + 15*slodycze
tluszcz = 2*kanapki + 2*pierogi + 0*slodycze
witaminy = 5*kanapki + 3*pierogi + 4*slodycze
weglowodany = 7*kanapki + 3*pierogi + 12*slodycze
razem=bialko+tluszcz+witaminy+weglowodany

print("Zadanie 1 Optymalne ilości produktów:")
print(f"Kanapki: {kanapki}")
print(f"Pierogi: {pierogi}")
print(f"Słodycze: {slodycze}")
print(f"Minimalny koszt diety: {koszt} zł")
print(f"Bialko: {bialko}")
print(f"Tluszcz: {tluszcz}")
print(f"Witaminy: {witaminy}")
print(f"Weglowodany: {weglowodany}")
print(f"Minimalna ilosc makroskladnikow: {razem}")
print("-----------------------------")
#krzeselka
c = [-100, -200]
A = [
    [5, 25],
    [0.5, 0],
    [100, 250],
    [10, 10],
]
B = [500, 15, 7500, 400]

bounds = [(0, None), (0, None)] 

from scipy.optimize import linprog

res = linprog(c=c, A_ub=A, b_ub=B, bounds=bounds, method='highs')

krzeslo=res.x[0]
stol=res.x[1]
koszt=abs(krzeslo*c[0]+stol*c[1])

drewno = 5*krzeslo + 25*stol 
skora = 0.5*krzeslo + 0*stol 
klej = 100*krzeslo + 250*stol 
naklad = 10*krzeslo + 10*stol 
razem=drewno+skora+klej+naklad
print(" Zadanie 2 Optymalne ilości produktów:")
print(f"Krzesło: {krzeslo}")
print(f"Stół: {stol}")
print(f" Maksymalny zysk: {koszt} zł") 
print(f"Drewno: {drewno}")
print(f"Skora: {skora}")
print(f"Klej: {klej}")
print(f"Naklad: {naklad}")
print(f"Minimalna ilosc makroskladnikow: {razem}")
print("-----------------------------")