import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd
import soundfile as sf
import scipy.fftpack
import librosa 
import matplotlib.gridspec as gridspec
#1 SYGNAL AUDIO
#ZAD1, ZAD 2
s,fs=sf.read('audio.wav',dtype='float32')

#sd.play(s,fs)
#status=sd.wait()

ilosc_probek=s.shape[0]
liczba_kanalow=s.shape[1]
if liczba_kanalow==0:
    liczba_kanalow+=1
czas_trwania = ilosc_probek/fs

info = sf.info('audio.wav')
rozdzielczosc_bitowa = info.subtype_info

print(f'Ksztalt sygnału: {s.shape}')
print(f'Czas trwania: {czas_trwania:.2f} s')
print(f'Częstotliwość próbkowania: {fs}') 
print(f'Liczba kanałów: {liczba_kanalow}')
print(f'Rozdzielczość bitowa: {rozdzielczosc_bitowa}')

#ZAD3
s=s[:,0]
max_val=np.max(np.abs(s))

if max_val>1:
    s=s/max_val
else:
    print("Wartosci sa w przedziale")

czas=np.arange(len(s))/fs*1000

plt.plot(czas,s,color="orange")
plt.title("Wykres audio w dziedzinie czasu")
plt.xlabel("Amplituda")
plt.ylabel("Czas[ms]") 
plt.show() 

#2 ZASTOSOWANIE OKIEN KROCZACYCH
def energy(s):
    suma = 0
    for probka in s:
        suma += probka**2
    return suma

def zero(s):
    count = 0
    for i in range(1, len(s)):
        if s[i] >= 0 and s[i - 1] < 0 or s[i] < 0 and s[i - 1] >= 0:
            count += 1
    return count

ilosc_probek = int(fs * 0.01)
ramki = []
ramka_energy = []
ramka_zero = []
for i in range(0, len(s), ilosc_probek):
    ramka = s[i:i+ilosc_probek]
    ramki.append(ramka)
    ramka_energy.append(energy(ramka))
    ramka_zero.append(zero(ramka))


max_val=np.max(np.abs(ramka_energy))
if max_val>1:
    ramka_energy=ramka_energy/max_val

max_val=np.max(np.abs(ramka_zero))

if max_val>1:
    ramka_zero=ramka_zero/max_val

czas_ramek = np.arange(len(ramka_energy)) * 10  # 10 ms co ramka

plt.plot(czas, s, color="green", label="Sygnał audio")
plt.plot(czas_ramek, ramka_energy, color="red", label="Energia")
plt.plot(czas_ramek, ramka_zero, color="blue", label="Przejścia przez zero")
plt.title("Wykres audio w dziedzinie czasu z energią i przejściami")
plt.xlabel("Czas [ms]")
plt.ylabel("Amplituda")
plt.legend()
plt.show()

okna=[0.05,0.2,0.5]

for index, okno in enumerate (okna):
    ilosc_probek = int(fs * okno)

    ramki = []
    ramka_energy = []
    ramka_zero = []
    for i in range(0, len(s), int(ilosc_probek/2)):
        ramka = s[i:i+ilosc_probek]
        ramki.append(ramka)
        ramka_energy.append(energy(ramka))
        ramka_zero.append(zero(ramka))

    max_val=np.max(np.abs(ramka_energy))

    if max_val>1:
        ramka_energy=ramka_energy/max_val

    max_val=np.max(np.abs(ramka_zero))

    if max_val>1:
        ramka_zero=ramka_zero/max_val

    czas_ramek = np.arange(len(ramka_energy)) * okno/2*1000

    plt.subplot(3,1,index+1)
    plt.plot(czas, s, color="green", label="Sygnał audio")
    plt.plot(czas_ramek, ramka_energy, color="red", label="Energia")
    plt.plot(czas_ramek, ramka_zero, color="blue", label="Przejścia przez zero")
    plt.title(f"Wykres audio z nachodzącymi na siebie ramkami {okno*100} ms")
    plt.xlabel("Czas [ms]")
    plt.ylabel("Amplituda")
    plt.legend()
plt.tight_layout() 
plt.show()

#3 ANALIZA CZESTOTLIOWOSCI 

#ZAD1
start =int(1350/1000*fs)
fragment=s[start:start+2048]
czas_f=np.arange(2048)/fs*1000

#ZA2
W=np.hamming(len(fragment))
zamaskowany_fragment= fragment *W


#LOGARYTMICZNE WIDMO
yf=scipy.fftpack.fft(zamaskowany_fragment)

log_amplituda = np.log(np.abs(yf)+1e-10)

freqs=np.linspace(0,fs,len(zamaskowany_fragment))

indices = freqs <= 10000
plt.plot(freqs[indices], log_amplituda[indices],color='red')
plt.title("Amplitude spectrum <=1000")
plt.xlabel("Częstotliwość [Hz]")
plt.ylabel("Log amplituda")
plt.show() 

#WIZUALIZACJA
fig = plt.figure(figsize=(12, 8))
gs = gridspec.GridSpec(2, 4, height_ratios=[2, 1])  

ax1 = fig.add_subplot(gs[0, :])
ax1.plot(czas, s, color="lightgreen", label="Sygnał audio")
ax1.plot(czas_ramek, ramka_energy, color="red", label="Energia")
ax1.plot(czas_ramek, ramka_zero, color="blue", label="Przejścia przez zero")
ax1.set_title("Wykres audio w dziedzinie czasu z energią i przejściami")
ax1.set_xlabel("Czas [ms]")
ax1.set_ylabel("Amplituda")
ax1.legend()

ax2 = fig.add_subplot(gs[1, 0])
ax2.plot(czas_f, fragment,color='lightgreen')
ax2.set_title("Signal analysed window W")
ax2.set_xlabel("time in ms (2048 samples)")
ax2.set_ylabel("signal in value")

ax3 = fig.add_subplot(gs[1, 1])
ax3.plot(czas_f, W,color='blue')
ax3.set_title("Hamming window W")
ax3.set_xlabel("2048 samples")
ax3.set_ylabel("signal in value")

ax4 = fig.add_subplot(gs[1, 2])
ax4.plot(czas_f, zamaskowany_fragment,color='blue')
ax4.set_title("W*H")
ax4.set_xlabel("2048 samples")
ax4.set_ylabel("signal in value")

ax5 = fig.add_subplot(gs[1, 3])
ax5.plot(freqs, log_amplituda,color='red')
ax5.set_title("Amplitude spectrum")


plt.tight_layout()
plt.show()
#ROZPOZNAWANIE SAMOGLOSEK USTNYCH /a, e, i, o, u, y

start = int(1350 / 1000 * fs)
fragment = s[start:start+2048]

czas_f = np.arange(2048) / fs * 1000
w = np.hamming(len(fragment))
zamaskowany_fragment = fragment * w
yf = scipy.fftpack.fft(zamaskowany_fragment)
log_amplituda = np.log(np.abs(yf) + 1e-10)
freqs = np.linspace(0, fs, len(fragment))

# lpc
lpc = librosa.lpc(fragment, order=20)
a = list(lpc) 

while len(a) < 2048:
    a.append(0.0)

a_padded = np.array(a)
a = scipy.fftpack.fft(a)
widmolpc = -np.log(np.abs(a) + 1e-10) 
log_amplituda = log_amplituda - np.mean(log_amplituda)
widmolpc = widmolpc - np.mean(widmolpc)

plt.plot(freqs, log_amplituda, label="widmo sygnału (fft)", color="blue")
plt.plot(freqs, widmolpc, label="wygładzone widmo lpc", color="red")
plt.title("widmo amplitudowe i widmo lpc")
plt.xlabel("częstotliwość [hz]")
plt.ylabel("log-amplituda")
plt.legend()
plt.xlim([0, 6000])  
plt.grid()
plt.show() 

###ZADA 6
import librosa
from scipy.signal import find_peaks
s,fs=sf.read('audio2.wav',dtype='float32')
s=s[:,0]
max_val=np.max(np.abs(s))
if max_val>1:
    s=s/max_val
czas=np.arange(len(s))/fs*1000

plt.plot(czas,s,color="orange")
plt.title("Wykres audio w dziedzinie czasu")
plt.xlabel("Amplituda")
plt.ylabel("Czas[ms]") 
plt.show() 

starts=[1200,2400,3700,5250,7100,8600]
x=[]
y=[]
for i, start in enumerate(starts):
    start =int(start/1000*fs)
    okno = s[start : start + 2048]
      
    best_p = 0
    best_F1 = 0
    best_F2 = 0
    min_F1 = np.inf
    print(f"Dla wartosci {starts[i]}")
    for p in [15, 25, 30,70]:
        a = librosa.lpc(okno, order=p)
        a = list(a)
        while len(a) < 2048:
            a.append(0.0)
        widmoLPC = -np.log(np.abs(np.fft.fft(a)))
        peaks, _ = find_peaks(widmoLPC[:1024]) 
        if len(peaks)>=2:
            formants = peaks[:2] 
            freqs = np.fft.fftfreq(len(a), d=1/fs)
            F1 = freqs[formants[0]]
            F2 = freqs[formants[1]]
            print(f'F1 = {F1} Hz, F2 = {F2} Hz dla p={p}')
            if F2 < min_F1:
                min_F1 = F2
                best_F1 = F1
                best_F2 = F2
                best_p = p
        else:
            print(f'  p={p}: Formanty nie znalezione')
    x.append(best_F1)
    y.append(best_F2)

colors=('orange','blue','green','black','yellow','pink','red','cyan','brown','gray')
for i, start in enumerate(starts):
    plt.scatter(x[i],y[i],color=colors[i],label=start)
plt.xlabel("F1")
plt.ylabel("F2")
plt.legend(loc='center left', bbox_to_anchor=(1.0, 0.5), title='Czas [ms]')
plt.show() 