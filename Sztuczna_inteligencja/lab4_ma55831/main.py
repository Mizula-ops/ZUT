
def isTerminal(s):
    return s==0


def generateT(s):
    if s>=2:
        return [s-1,s-2]
    else:
        return [0]


historia=[]

a = float('-inf')
b = float('inf')
node_id = 0
def AlphaBetaEvaluateMaxState(s, a, b, p, depth, side):
    global node_id
    node_id += 1
    current_id = node_id
    if isTerminal(s):
        v = 1
        historia.append({
            "s": s, "t": None, "p": p,
            "alpha": a, "beta": b, "v": v,
            "kto": "MAX", "change": "isTerminal",
            "depth": depth, "side": side,
            "which": current_id
        })
        return v

    allT = generateT(s)

    for i, t in enumerate(allT):

        child_side = side
        if depth == 0:
            child_side = i

        v= AlphaBetaEvaluateMinState(t, a, b, s, depth + 1, child_side)

        if v > a:
            a = v
        if a >= b:
            historia.append({
                "s": s, "t": t, "p": p,
                "alpha": a, "beta": b, "v": v,
                "kto": "MAX", "change": "przycinanie aB",
                "depth": depth, "side": side,
                "which": current_id
            })
            return a

        historia.append({
            "s": s, "t": t, "p": p,
            "alpha": a, "beta": b, "v": v,
            "kto": "MAX", "change": "return a",
            "depth": depth, "side": side,
            "which": current_id
        })

    return a
def AlphaBetaEvaluateMinState(s, a, b, p, depth, side):
    global node_id
    node_id += 1
    current_id = node_id
    if isTerminal(s):
        v = -1
        historia.append({
            "s": s, "t": None, "p": p,
            "alpha": a, "beta": b, "v": v,
            "kto": "MIN", "change": "isTerminal",
            "depth": depth, "side": side,
            "which": current_id
        })
        return v

    allT = generateT(s)

    for t in allT:

        v= AlphaBetaEvaluateMaxState(t, a, b, s, depth + 1, side)

        if v < b:
            b = v
        if a >= b:
            historia.append({
                "s": s, "t": t, "p": p,
                "alpha": a, "beta": b, "v": v,
                "kto": "MIN", "change": "przycinanie aB",
                "depth": depth, "side": side,
                "which": current_id
            })
            return b

        historia.append({
            "s": s, "t": t, "p": p,
            "alpha": a, "beta": b, "v": v,
            "kto": "MIN", "change": "return b",
            "depth": depth, "side": side,
            "which": current_id
        })

    return b
score=AlphaBetaEvaluateMaxState(5,a,b,None,0,None)
print(score)


def TreeVisualisation1():


    drzewo = {}

    for h in historia:
        key = (h["p"], h["s"], h["depth"], h["kto"], h["side"])

        if key not in drzewo:
            drzewo[key] = {
                "alpha": [],
                "beta": [],
                "v": [],
                "change": []
            }

        if h["alpha"] not in drzewo[key]["alpha"]:
            drzewo[key]["alpha"].append(h["alpha"])

        if h["beta"] not in drzewo[key]["beta"]:
            drzewo[key]["beta"].append(h["beta"])

        if h["v"] not in drzewo[key]["v"]:
            drzewo[key]["v"].append(h["v"])

        drzewo[key]["change"].append(h["change"])

    print("\n===== ZAD1. Wizualizacja drzewa =====\n")


    poziomy = {}
    for key, data in drzewo.items():
        _, _, depth, kto1, _ = key
        if depth not in poziomy:
            poziomy[depth] = []
        poziomy[depth].append((key, data))

    for depth in sorted(poziomy.keys()):
        if depth%2==0:
            print(f"POZIOM {depth} WYBOR MAX:", end="")
        else:
            print(f"POZIOM {depth} WYBOR MIN:", end="")
        if depth==0:
            print(" "*320,end="")
        elif depth==1:
            print(" " * 300, end="")
        elif depth==2:
            print(" " * 105, end="")
        elif depth==3:
            print(" " * 40, end="")
        for (p, s, d, kto, side), data in poziomy[depth]:
            podr=-1
            if side is None:
                podr = "korzeń"
            elif side == 0:
                podr = "lewe"
            else:
                podr = "prawe"

            label = (
                f"[Ojciec={p}, Monety={s}, poddrzewo={podr}, "
                f"a={data['alpha']}, b={data['beta']}, v={data['v']}]"
            )
            if depth==2:
                print(label, end=" "*5)
                if p==4 and s==3:
                    print(" " * 113, end="")
                elif p==4 and s==2:
                    print(" " * 40, end="")
                elif p==3 and s==2:
                    print(" " * 50, end="")
            elif depth==3 and p==3 and s==2:
                print(label, end="    "*15)
            elif depth==3 and p==3 and s==1:
                print(label, end="    "*14)
            elif depth==3 and p==2 and s==1:
                print(label, end="    "*5)
            elif depth==4 and p==1 and s==0:
                print(label, end="    " * 37)
            else:
                print(label, end="      ")
        print()
def Wartosci():
    print("===== Zad 2 i 3 zmiana wartosci v, a i B =====")
    for h in historia:
        parent = h["p"]
        stan = h["s"]
        alpha = h["alpha"]
        beta = h["beta"]
        v = h["v"]
        depth = h["depth"]
        change = h["change"]

        print(
            f"parent={parent}, stan={stan}, depth={depth}, "
            f"alpha={alpha}, beta={beta}, v={v}, change={change}"
        )
    print("==========ZAD 4=========")
    print("Z logów i z drzewa widać, że tylko przejście z 5 do 4 daje wygrywającą gałąź – prowadzi do sekwencji 5 → 4 → 3 → 1, która kończy się wygraną A. "
          "Dlatego A powinien w pierwszym ruchu zabrać 1 monetę.")
    print("==========ZAD 5=========")
    print(" Przy wizualizacji widzimy, że łacznie wygenerowano 16 wezlow")
    print("==========ZAD 6=========")
    print(" Podczas wyświetlenia logow widać, że przycinanie alpha beta nastapilo 3 razy")
    for h in historia:
        if h["change"] == "przycinanie aB":
            print(
                f"Rodzic={h['p']}, Ilosc monet={h['s']}, depth={h['depth']}, "
                f"poddrzewo={'lewe' if h['side'] == 0 else 'prawe'}, "
                f"alpha={h['alpha']}, beta={h['beta']}, v={h['v']}"
            )
    print("===Zad 7 ===")
    print("Kolejnosc odwiedzonych wezlow")


    unikalne = sorted({h["which"] for h in historia})

    for w in unikalne:
        h = next(item for item in historia if item["which"] == w)

        parent = h["p"]
        stan = h["s"]
        depth = h["depth"]
        side = h["side"]

        print(f"which={w} | parent={parent}, stan={stan}, depth={depth}, side={side}")


TreeVisualisation1()
Wartosci()
