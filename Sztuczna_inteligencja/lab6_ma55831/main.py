import numpy as np
import matplotlib.pyplot as plt

#Polecenie 1 Porównanie SGD i Adam dla jednego neuronu – dwa kroki uczenia
x1,x2,y= 1,2,1
w1,w2,b= 0.5,-0.5,0
eta=0.01
B1,B2,eps=0.9, 0.999, 1e-8
t=1

plt.scatter(x1,x2)
x=np.linspace(-10,10,100)
plt.plot(x,b)
plt.grid()
plt.show()

def sigmoid(x):
    return 1/(1+np.exp(-x))
def L(y,y_hat):
    return -(y*np.log(y_hat)+(1-y)*np.log(1-y_hat))

def SGD(w1, w2, b, t):
    print(f"\n===== SGD | krok t = {t} =====")

    # ZAD1: propagacja w przód
    z = w1 * x1 + w2 * x2 + b
    y_hat = sigmoid(z)
    print("ZAD1: propagacja w przód")
    print(f"z = w1*x1 + w2*x2 + b = {z}")
    print(f"ŷ = 1/(1+np.exp(-z))= {y_hat}")

    # ZAD2: funkcja straty
    Loss = L(y, y_hat)
    print("\nZAD2: funkcja straty")
    print(f"L = -(y*np.log(ŷ)+(1-y)*np.log(1-ŷ) = {Loss}")

    # ZAD3: błąd i gradienty
    Neur_err = y_hat - y
    gw1 = Neur_err * x1
    gw2 = Neur_err * x2
    gb = Neur_err
    print("\nZAD3: błąd i gradienty")
    print(f"δ = ŷ - y = {Neur_err}")
    print(f"g_w1 = δ * x1 = {gw1}")
    print(f"g_w2 = δ * x2 = {gw2}")
    print(f"g_b  = δ = {gb}")

    # ZAD4: aktualizacja SGD
    w1_new = w1 - eta * gw1
    w2_new = w2 - eta * gw2
    b_new  = b  - eta * gb
    print("\nZAD4: aktualizacja parametrów (SGD)")
    print(f"w1_new = {w1} - {eta} * {gw1} = {w1_new}")
    print(f"w2_new = {w2} - {eta} * {gw2} = {w2_new}")
    print(f"b_new  = {b} - {eta} * {gb} = {b_new}")

    return w1_new, w2_new, b_new, Loss

def Adam(w1, w2, b, B1, B2, m_w1, v_w1, m_w2, v_w2, m_b, v_b, t):
    print(f"\n===== Adam | krok t = {t} =====")

    # ZAD1: propagacja w przód
    z = w1*x1 + w2*x2 + b
    y_hat = sigmoid(z)
    print("ZAD1: propagacja w przód")
    print(f"z = w1*x1 + w2*x2 + b = {z}")
    print(f"ŷ =1/(1+np.exp(-z))=  {y_hat}")

    # ZAD2: funkcja straty
    Loss = L(y, y_hat)
    print("\nZAD2: funkcja straty")
    print(f"L = -(y*np.log(ŷ)+(1-y)*np.log(1-ŷ) = {Loss}")

    # ZAD3: błąd i gradienty
    Neur_err = y_hat - y
    gw1 = Neur_err * x1
    gw2 = Neur_err * x2
    gb  = Neur_err
    print("\nZAD3: błąd i gradienty")
    print(f"δ = ŷ - y = {Neur_err}")
    print(f"g_w1 = δ*x1 = {gw1}")
    print(f"g_w2 = δ*x2 = {gw2}")
    print(f"g_b  = δ = {gb}")

    # ZAD5: momenty m i v
    m_w1_new = B1*m_w1 + (1-B1)*gw1
    v_w1_new = B2*v_w1 + (1-B2)*(gw1**2)

    m_w2_new = B1*m_w2 + (1-B1)*gw2
    v_w2_new = B2*v_w2 + (1-B2)*(gw2**2)

    m_b_new  = B1*m_b  + (1-B1)*gb
    v_b_new  = B2*v_b  + (1-B2)*(gb**2)

    print("\nZAD5: momenty (m, v)")
    print(f"m_w1 = {m_w1_new}   v_w1 = {v_w1_new}")
    print(f"m_w2 = {m_w2_new}   v_w2 = {v_w2_new}")
    print(f"m_b  = {m_b_new}   v_b  = {v_b_new}")

    # ZAD6: bias correction
    m_hat_w1 = m_w1_new / (1 - B1**t)
    v_hat_w1 = v_w1_new / (1 - B2**t)

    m_hat_w2 = m_w2_new / (1 - B1**t)
    v_hat_w2 = v_w2_new / (1 - B2**t)

    m_hat_b  = m_b_new  / (1 - B1**t)
    v_hat_b  = v_b_new  / (1 - B2**t)

    print("\nZAD6: bias correction")
    print(f"m̂_w1 = {m_hat_w1}   v̂_w1 = {v_hat_w1}")
    print(f"m̂_w2 = {m_hat_w2}   v̂_w2 = {v_hat_w2}")
    print(f"m̂_b  = {m_hat_b}   v̂_b  = {v_hat_b}")

    # ZAD7: aktualizacja Adam
    w1_new = w1 - eta * (m_hat_w1 / (np.sqrt(v_hat_w1) + eps))
    w2_new = w2 - eta * (m_hat_w2 / (np.sqrt(v_hat_w2) + eps))
    b_new  = b  - eta * (m_hat_b  / (np.sqrt(v_hat_b)  + eps))

    print("\nZAD7: aktualizacja parametrów (Adam)")
    print(f"w1_new = {w1} - {eta} * (m̂_w1/(sqrt(v̂_w1)+eps)) = {w1_new}")
    print(f"w2_new = {w2} - {eta} * (m̂_w2/(sqrt(v̂_w2)+eps)) = {w2_new}")
    print(f"b_new  = {b} - {eta} * (m̂_b /(sqrt(v̂_b )+eps)) = {b_new}")

    return w1_new, w2_new, b_new, Loss, m_w1_new, v_w1_new, m_w2_new, v_w2_new, m_b_new, v_b_new


w1_sgd,w2_sgd,b_sgd,loss_sgd=SGD(w1,w2,b,1)
w1_adam, w2_adam, b_adam, loss_adam, m_w1, v_w1, m_w2, v_w2, m_b, v_b = Adam(
    w1, w2, b,B1, B2,
    0, 0, 0, 0, 0, 0,1
)


print("\n===== SGD | t = 1 =====")
print(f"w1 = {w1_sgd:.6f}")
print(f"w2 = {w2_sgd:.6f}")
print(f"b  = {b_sgd:.6f}")


print("\n===== Adam | t = 1 =====")
print(f"w1 = {w1_adam:.6f}")
print(f"w2 = {w2_adam:.6f}")
print(f"b  = {b_adam:.6f}")

#CZESC DRUGA Wykonać drugą iterację algorytmów SGD i Adam (t = 2), powtarzając
#wszystkie kroki analogicznie do t = 1 dla obu metod

w1_sgd,w2_sgd,b_sgd,loss_sgd=SGD(w1_sgd,w2_sgd,b_sgd,2)
w1_adam, w2_adam, b_adam, loss_adam, m_w1, v_w1, m_w2, v_w2, m_b, v_b = Adam(
    w1_adam, w2_adam, b_adam,B1, B2,m_w1, v_w1, m_w2, v_w2, m_b, v_b,2
)



print("\n===== SGD | t = 2 =====")
print(f"w1 = {w1_sgd:.6f}")
print(f"w2 = {w2_sgd:.6f}")
print(f"b  = {b_sgd:.6f}")


print("\n===== Adam | t = 2 =====")
print(f"w1 = {w1_adam:.6f}")
print(f"w2 = {w2_adam:.6f}")
print(f"b  = {b_adam:.6f}")

#Polecenie 2 Porównanie SGD i Adam dla sieci o architekturze 2–2–1
print("\n===== Sieć 2–2–1 | krok t = 1 =====")
#Wagi Poczatkowe

#Neuron1
w11,w12,b1=0.2,-0.1,0
#Neuron2
w21,w22,b2=0.4,0.3,0
#Warstwa wyjsciowa
v1,v2,b3=0.5, -0.4, 0
print("\n=====t = 1 =====")
print(f"w11 = {w11}")
print(f"w12 = {w12}")
print(f"b  = {b1}")
print(f"w21 = {w21}")
print(f"w22 = {w22}")
print(f"b  = {b2}")
print(f"v1 = {v1}")
print(f"v2 = {v2}")
print(f"b3  = {b3}")

#ZAD1
z1=w11*x1 + w12*x2 + b1
h1=sigmoid(z1)
z2=w21*x1 + w22*x2 + b2
h2=sigmoid(z2)
z3=v1*h1 + v2*h2 +b3
y_hat = sigmoid(z3)
print("\nZAD1: propagacja w przód")
print("ZAD1: propagacja w przód")
print(f"z1 = w11*x1 + w12*x2 + b1 = {z1}")
print(f"ŷ =1/(1+np.exp(-z1))=  {h1}")
print(f"z2 = w21*x1 + w22*x2 + b2 = {z2}")
print(f"ŷ =1/(1+np.exp(-z2))=  {h2}")
print(f"z3 = v1*h1 + v2*h2+ b3 = {z3}")
print(f"ŷ =1/(1+np.exp(-z3))=  {y_hat}")

#Zad2
print("\nZAD2: funkcja straty")
L=-(y*np.log(y_hat)+(1-y)*np.log(1-y_hat))
print(f"L = -(y*np.log(ŷ)+(1-y)*np.log(1-ŷ) = {L:.6f}")
#ZAD3

# ZAD3
print("\n===== ZAD3: błąd na wyjściu =====")
Neur_err = y_hat - y
print(f"δ3 = ŷ - y = {y_hat} - {y} = {Neur_err}")
#ZAD4
print("\n===== ZAD4: gradienty warstwy wyjściowej =====")
gv1 = Neur_err * h1
gv2 = Neur_err * h2
gb3 = Neur_err

print(f"g_v1 = δ3 * h1 = {Neur_err} * {h1} = {gv1}")
print(f"g_v2 = δ3 * h2 = {Neur_err} * {h2} = {gv2}")
print(f"g_b3 = δ3 = {gb3}")
#Zad5
print("\n===== ZAD5: błędy neuronów ukrytych =====")
Neur_err1 = Neur_err * v1 * h1 * (1 - h1)
Neur_err2 = Neur_err * v2 * h2 * (1 - h2)

print(f"δ1 = δ3 * v1 * h1 * (1 - h1)")
print(f"   = {Neur_err} * {v1} * {h1} * (1 - {h1}) = {Neur_err1}")

print(f"\nδ2 = δ3 * v2 * h2 * (1 - h2)")
print(f"   = {Neur_err} * {v2} * {h2} * (1 - {h2}) = {Neur_err2}")
#ZAD6
print("\n===== ZAD6: gradienty wag i biasów warstwy ukrytej =====")
gw11=Neur_err1*x1
gw12=Neur_err1*x2
gw21=Neur_err2*x1
gw22=Neur_err2*x2
gb1=Neur_err1
gb2=Neur_err2
print(f"g_w11 = δ1 * x1 = {Neur_err1} * {x1} = {gw11}")
print(f"g_w12 = δ1 * x2 = {Neur_err1} * {x2} = {gw12}")

print(f"\ng_w21 = δ2 * x1 = {Neur_err2} * {x1} = {gw21}")
print(f"g_w22 = δ2 * x2 = {Neur_err2} * {x2} = {gw22}")

print(f"\ng_b1 = δ1 = {gb1}")
print(f"g_b2 = δ2 = {gb2}")
#ZAD 7
print("\n===== ZAD7: aktualizacja wag – SGD =====")

w11S = w11 - eta * gw11
w12S = w12 - eta * gw12
b1S  = b1  - eta * gb1

w21S = w21 - eta * gw21
w22S = w22 - eta * gw22
b2S  = b2  - eta * gb2

v1S  = v1  - eta * gv1
v2S  = v2  - eta * gv2
b3S  = b3  - eta * gb3

print("Warstwa ukryta – neuron 1:")
print(f"w11_new = {w11} - {eta} * {gw11} = {w11S}")
print(f"w12_new = {w12} - {eta} * {gw12} = {w12S}")
print(f"b1_new  = {b1} - {eta} * {gb1}  = {b1S}")

print("\nWarstwa ukryta – neuron 2:")
print(f"w21_new = {w21} - {eta} * {gw21} = {w21S}")
print(f"w22_new = {w22} - {eta} * {gw22} = {w22S}")
print(f"b2_new  = {b2} - {eta} * {gb2}  = {b2S}")

print("\nWarstwa wyjściowa:")
print(f"v1_new = {v1} - {eta} * {gv1} = {v1S}")
print(f"v2_new = {v2} - {eta} * {gv2} = {v2S}")
print(f"b3_new = {b3} - {eta} * {gb3} = {b3S}")
#ZAD8
print("\n===== ZAD8: momenty Adama (m i v), t = 1 =====")
m_w11 = B1 * 0 + (1 - B1) * gw11
v_w11 = B2 * 0 + (1 - B2) * (gw11 ** 2)
m_w12 = B1 * 0 + (1 - B1) * gw12
v_w12 = B2 * 0 + (1 - B2) * (gw12 ** 2)
m_w21 = B1 * 0 + (1 - B1) * gw21
v_w21 = B2 * 0 + (1 - B2) * (gw21 ** 2)
m_w22 = B1 * 0 + (1 - B1) * gw22
v_w22 = B2 * 0 + (1 - B2) * (gw22 ** 2)
m_b1 = B1* 0 + (1-B1)*gb1;
v_b1 = B2 *0 + (1-B2)*(gb1**2)
m_b2 = B1* 0 + (1-B1)*gb2;
v_b2 =  B2 *0 + (1-B2)*(gb2**2)


m_v1=B1*0 + (1 - B1) * gv1
v_v1=B2 * 0 + (1 - B2) * (gv1 ** 2)
m_v2=B1*0 + (1 - B1) * gv2
v_v2=B2 * 0 + (1 - B2) * (gv2 ** 2)
m_b3=B1*0 + (1-B1) * gb3
v_b3=B2*0 +(1-B2) * (gb3**2)


print("Warstwa ukryta – neuron 1:")
print(f"m_w11 = {m_w11}, v_w11 = {v_w11}")
print(f"m_w12 = {m_w12}, v_w12 = {v_w12}")
print(f"m_b1  = {m_b1},  v_b1  = {v_b1}")

print("\nWarstwa ukryta – neuron 2:")
print(f"m_w21 = {m_w21}, v_w21 = {v_w21}")
print(f"m_w22 = {m_w22}, v_w22 = {v_w22}")
print(f"m_b2  = {m_b2},  v_b2  = {v_b2}")

print("\nWarstwa wyjściowa:")
print(f"m_v1 = {m_v1}, v_v1 = {v_v1}")
print(f"m_v2 = {m_v2}, v_v2 = {v_v2}")
print(f"m_b3 = {m_b3}, v_b3 = {v_b3}")
#ZAD 9
print("\n===== ZAD9: bias correction (m̂, v̂) =====")
m_hat_w11 = m_w11 / (1 - B1 ** t)
v_hat_w11 = v_w11 / (1 - B2 ** t)

m_hat_w12 = m_w12 / (1 - B1 ** t)
v_hat_w12 = v_w12 / (1 - B2 ** t)

m_hat_w21 = m_w21 / (1 - B1 ** t)
v_hat_w21 = v_w21 / (1 - B2 ** t)

m_hat_w22 = m_w22 / (1 - B1 ** t)
v_hat_w22 = v_w22 / (1 - B2 ** t)

m_hat_b1 = m_b1 / (1 - B1 ** t)
v_hat_b1 = v_b1 / (1 - B2 ** t)

m_hat_b2 = m_b2 / (1 - B1 ** t)
v_hat_b2 = v_b2 / (1 - B2 ** t)

m_hat_v1 = m_v1 / (1 - B1 ** t)
v_hat_v1 = v_v1 / (1 - B2 ** t)

m_hat_v2 = m_v2 / (1 - B1 ** t)
v_hat_v2 = v_v2 / (1 - B2 ** t)

m_hat_b3= m_b3 / (1 - B1 ** t)
v_hat_b3 = v_b3 / (1 - B2 ** t)
print("Warstwa ukryta – neuron 1:")
print(f"m̂_w11 = {m_hat_w11}, v̂_w11 = {v_hat_w11}")
print(f"m̂_w12 = {m_hat_w12}, v̂_w12 = {v_hat_w12}")
print(f"m̂_b1  = {m_hat_b1},  v̂_b1  = {v_hat_b1}")

print("\nWarstwa ukryta – neuron 2:")
print(f"m̂_w21 = {m_hat_w21}, v̂_w21 = {v_hat_w21}")
print(f"m̂_w22 = {m_hat_w22}, v̂_w22 = {v_hat_w22}")
print(f"m̂_b2  = {m_hat_b2},  v̂_b2  = {v_hat_b2}")

print("\nWarstwa wyjściowa:")
print(f"m̂_v1 = {m_hat_v1}, v̂_v1 = {v_hat_v1}")
print(f"m̂_v2 = {m_hat_v2}, v̂_v2 = {v_hat_v2}")
print(f"m̂_b3 = {m_hat_b3}, v̂_b3 = {v_hat_b3}")


#ZAd 10 aktualizacja
print("\n===== ZAD10: aktualizacja wag – Adam =====")
w11A = w11 - eta * (m_hat_w11 / (np.sqrt(v_hat_w11) + eps))
w12A = w12 - eta * (m_hat_w12 / (np.sqrt(v_hat_w12) + eps))
w21A = w21 - eta * (m_hat_w21 / (np.sqrt(v_hat_w21) + eps))
w22A = w22 - eta * (m_hat_w22 / (np.sqrt(v_hat_w22) + eps))

b1A  = b1  - eta * (m_hat_b1  / (np.sqrt(v_hat_b1)  + eps))
b2A  = b2  - eta * (m_hat_b2  / (np.sqrt(v_hat_b2)  + eps))

v1A  = v1  - eta * (m_hat_v1  / (np.sqrt(v_hat_v1)  + eps))
v2A  = v2  - eta * (m_hat_v2  / (np.sqrt(v_hat_v2)  + eps))
b3A  = b3  - eta * (m_hat_b3  / (np.sqrt(v_hat_b3)  + eps))

print("Warstwa ukryta – neuron 1:")
print(f"w11_new = {w11} - {eta} * (m̂_w11/(√v̂_w11+ε)) = {w11A}")
print(f"w12_new = {w12} - {eta} * (m̂_w12/(√v̂_w12+ε)) = {w12A}")
print(f"b1_new  = {b1}  - {eta} * (m̂_b1 /(√v̂_b1 +ε)) = {b1A}")

print("\nWarstwa ukryta – neuron 2:")
print(f"w21_new = {w21} - {eta} * (m̂_w21/(√v̂_w21+ε)) = {w21A}")
print(f"w22_new = {w22} - {eta} * (m̂_w22/(√v̂_w22+ε)) = {w22A}")
print(f"b2_new  = {b2}  - {eta} * (m̂_b2 /(√v̂_b2 +ε)) = {b2A}")

print("\nWarstwa wyjściowa:")
print(f"v1_new = {v1} - {eta} * (m̂_v1/(√v̂_v1+ε)) = {v1A}")
print(f"v2_new = {v2} - {eta} * (m̂_v2/(√v̂_v2+ε)) = {v2A}")
print(f"b3_new = {b3} - {eta} * (m̂_b3/(√v̂_b3+ε)) = {b3A}")

print("=== SGD ===")
print(f"w11 = {w11S}")
print(f"w12 = {w12S}")
print(f"b1  = {b1S}")
print(f"w21 = {w21S}")
print(f"w22 = {w22S}")
print(f"b2  = {b2S}")
print(f"v1  = {v1S}")
print(f"v2  = {v2S}")
print(f"b3  = {b3S}")

print("\n=== Adam ===")
print(f"w11 = {w11A}")
print(f"w12 = {w12A}")
print(f"b1  = {b1A}")
print(f"w21 = {w21A}")
print(f"w22 = {w22A}")
print(f"b2  = {b2A}")
print(f"v1  = {v1A}")
print(f"v2  = {v2A}")
print(f"b3  = {b3A}")





