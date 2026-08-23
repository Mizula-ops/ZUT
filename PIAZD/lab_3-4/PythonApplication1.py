import numpy as np
import matplotlib.pyplot as plt 
from skimage import filters 
#ZAD1,2,3
f=10
fs=[20,21,30,45,50,100,150,200,250,1000]


def generate_signal(f,Fs):
    t=np.arange(0,1,1/Fs)
    x=np.sin(2*np.pi *f*t)
    return t,x

for i, Fs in enumerate(fs):
    t, s = generate_signal(f, Fs)
    plt.subplot(4, 3, i+1)
    plt.plot(t, s)
    plt.title(f'Fs = {Fs} Hz')
    plt.xlabel('Czas [s]')
    plt.ylabel('Amplituda')
    plt.grid()

plt.tight_layout()
plt.show()
#ZAD6
obraz = plt.imread(r"aliasing.png")

skala=4

obraz_mniejszy=obraz[::skala,::skala,:]
fig,axs = plt.subplots(1,2,figsize=(10,5))

axs[0].imshow(obraz)
axs[0].set_title("Oryginalny obraz")

axs[1].imshow(obraz_mniejszy)
axs[1].set_title("Zmiejszony obraz")
plt.show(); 
#KWANTYZACJA
#ZAD1,2
print("LIczba wymiarów obrazu:",obraz.ndim)
#Zad 3
print("Liczba wartosci w pojedynczym piskelu", obraz.shape[-1])  
#ZAD4
jasnosc = (np.max(obraz[:, :, :3], axis=2) + np.min(obraz[:, :, :3], axis=2)) 
srednia = np.mean(obraz[:, :, :3], axis=2)
luminacja = 0.21 * obraz[:, :, 0] + 0.72 * obraz[:, :, 1] + 0.07 * obraz[:, :, 2]

hist1, bins1 = np.histogram(jasnosc, bins=256)
hist2, bins2 = np.histogram(srednia, bins=256 )
hist3, bins3 = np.histogram(luminacja, bins=256)

hist4, bin_nowe = np.histogram(srednia, bins=16)
print("Zakresy nowych kolorów:", bin_nowe)

bin_nowe = (bin_nowe[:-1] + bin_nowe[1:]) / 2
nowy_obraz = np.digitize(srednia, bins=bin_nowe, right=True)
nowy_obraz = bin_nowe[nowy_obraz - 1]

fig, axs = plt.subplots(4, 2, figsize=(12, 16))
axs[0, 0].imshow(jasnosc,cmap='gray')
axs[0, 0].set_title("Metoda 1: max-min")
axs[0, 1].plot(hist1)
axs[0, 1].set_title("Histogram metody 1")

axs[1, 0].imshow(srednia,cmap='gray')
axs[1, 0].set_title("Metoda 2: Średnia")
axs[1, 1].plot(hist2)
axs[1, 1].set_title("Histogram metody 2")

axs[2, 0].imshow(luminacja, cmap='gray')
axs[2, 0].set_title("Metoda 3: Luminancja")
axs[2, 1].plot(hist3)
axs[2, 1].set_title("Histogram metody 3")

axs[3, 0].imshow(nowy_obraz,cmap='gray')
axs[3, 0].set_title("Zmiana kolorow")
axs[3, 1].plot(hist4)
axs[3, 1].set_title("Histogram 16 bins")
plt.tight_layout()
plt.show()
#BINARYZACJA 
obraz1 = plt.imread(r"papuga.png") 
srednia = np.mean(obraz1[:, :, :3], axis=2) 
hist, bins = np.histogram(srednia, bins=256 )

prog = filters.threshold_otsu(srednia)
print(f"Wyznaczony próg (Otsu): {prog}") 
binaryzowany = (srednia > prog).astype(np.uint8) * 255 
fig, ax = plt.subplots(1, 3, figsize=(15, 5))
ax[0].imshow(obraz1)
ax[0].set_title("Oryginalny obraz")
ax[0].axis("off")

ax[1].plot(bins[:-1], hist)
ax[1].axvline(prog, color='r', linestyle='dashed', label=f'Próg = {prog:.2f}')
ax[1].set_title("Histogram")
ax[1].legend()

ax[2].imshow(binaryzowany, cmap='gray')
ax[2].set_title("Binaryzowany obraz")
ax[2].axis("off")
plt.tight_layout()
plt.show()