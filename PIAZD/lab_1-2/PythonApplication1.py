import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import pearsonr
from scipy.stats import gaussian_kde
df =pd.DataFrame({'x': [1,2,3,4,5], 'y':['a','b','a','b','b']})
print("Tabela")
print(df)

#ZAD1
print("Wynik zadanie 1")
gbx=df.groupby('y')['x'].mean().reset_index()
print(gbx)
#ZAD2
print("Wynik zadanie 2")
print(df['y'].value_counts())
#ZAD3
print("Wczytanie danych za pomoca pandas")
data =pd.read_csv('autos.csv')
print(data)
print("Wczytanie danych za pomoca loadtxt")
data_txt = np.loadtxt('autos.csv', delimiter=',', skiprows=1, dtype='str')  
print(data_txt[:5]) 
#ZAD4
#WYLICZENIE ŚREDNIEJ
data['avg_mpg']=(data['city-mpg'] + data['highway-mpg']) /2
print("Wynik zadanie 4")
avg_fuel=data.groupby('make')[['avg_mpg']].mean()
print(avg_fuel)
#ZAD5
print("Wynik zadanie 5")
fuel_type=data.groupby('make')['fuel-type'].value_counts()
print(fuel_type)
#ZAD6
x =data['length']
y =data['city-mpg']
#WIELOMIAN 1 stopnia
coeffs_1= np.polyfit(x,y,1)
y_fits_1 = np.polyval(coeffs_1,x)
#WIELOMIAN 2 stopnia
coeffs_2 = np.polyfit(x,y,2)
y_fits_2 = np.polyval(coeffs_2,x)
#ZAD7
print("Wynik zadanie 7")
corr,_=pearsonr(x,y)
print(f"Współczynnik korelacji: {corr}");
#ZAD8
plt.scatter(x,y,label="Próbki",color="blue")
plt.plot(x,y_fits_1, label="wielomian 1 stopnia",color="red")
plt.plot(x,y_fits_2, label="wielomian 2 stopnia",color="green")
plt.xlabel('length')
plt.ylabel('city mpg')
plt.title("Wizualizacja dopasowania")
plt.legend()
plt.show() 
#ZAD9
kde=gaussian_kde(x)
x_range=np.linspace(x.min(),x.max(),1000)
y_range=kde(x_range)
plt.hist(x, bins=20, density=True, alpha=0.5,label="Próbki",color="g")
plt.plot(x_range,y_range, label="Estymacja gestosci",color="red")
plt.xlabel('length')
plt.ylabel('Gestosc')
plt.title('Estymator gęstości dla zmiennej Length')
plt.legend()
plt.show() 
#zad10
x_1=data['width']
kde1=gaussian_kde(x_1)
x_range1=np.linspace(x_1.min(),x_1.max(),100)
y_range1=kde1(x_range1)
fig, ax=plt.subplots(1,2,figsize=(12,5))
ax[0].hist(x, bins=20, density=True, alpha=0.5,label="histogram",color="blue")
ax[0].plot(x_range,y_range, label="Estymacja Gestości",color="red")
ax[0].set_xlabel('length')
ax[0].set_ylabel('Gestosc')
ax[0].legend()

ax[1].hist(x_1, bins=20, density=True, alpha=0.5,label="histogram",color="blue")
ax[1].plot(x_range1,y_range1, label="Estymacja Gestosci",color="red")
ax[1].set_xlabel('Width')
ax[1].set_ylabel('Gestosc')
ax[1].legend() 
plt.tight_layout()
plt.show()
#ZAD11
x = data['width']
y = data['length']
xx, yy = np.meshgrid(np.linspace(x.min(),x.max(), 100), np.linspace(y.min(),y.max(), 100))

positions = np.vstack([xx.ravel(), yy.ravel()])
kde_2d = gaussian_kde([x, y])
zz = kde_2d(positions).reshape(xx.shape) 

fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111)
ax.contourf(xx, yy, zz, cmap='Blues')
ax.scatter(x, y, c='red', s=10)
plt.xlabel('Width')
plt.ylabel('Length')
plt.title('Dwuwymiarowy estymator funkcji gęstości')
plt.show()


plt.savefig('density_plot.png')
plt.savefig('density_plot.pdf')