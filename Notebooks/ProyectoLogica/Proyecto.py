from itertools import combinations
from Logica import *
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.offsetbox import AnnotationBbox, OffsetImage
from types import MethodType

def escribirFichas(self, literal):
    figuras = ["tr", "cu", "ci"]
    if '-' in literal:
        atomo = literal[1:]
        neg = " no"
    else:
        atomo = literal
        neg = ""
    x,y,f = self.unravel(atomo)
    return (f"La figura {figuras[f]}{neg} esta en la casilla ({x}, {y})")


class Fichas:

    def __init__(self, Nx = 4, Ny = 4, Nf = 3):
        
        #Descripcion e inicialización
        self.fichas = {0: 'tr', 1: 'cu', 2: 'ci'}
        self.casillas = list(range(Nx * Ny))
        self.Nf = Nf
        self.Nx = Nx
        self.Ny = Ny
        self.FichaEn = Descriptor([Nx, Ny, Nf])

        #Reglas
        regla1 = self.regla1()
        regla2 = self.regla2()
        regla3 = self.regla3()
        regla4 = self.regla4()
        self.reglas = [regla1, regla2, regla3, regla4]

        #Metodo para escribir
        self.FichaEn.escribir = MethodType(escribirFichas, self.FichaEn)

    #Debe haber minimo dos casillas ocupadas (No pueden haber ni 0 ni 1 casilla ocupada)
    def regla1(self):
        todas_posiciones = [(x,y,f) for x in range(self.Nx) for y in range(self.Ny) for f in range(self.Nf)]
        
        formulas_negadas = ['-' + self.FichaEn.ravel([*pos]) for pos in todas_posiciones]
        formula_cero_fichas = Ytoria(formulas_negadas)
        
        formulas_una_ficha = []
        for pos in todas_posiciones:
            casilla_actual = self.FichaEn.ravel([*pos])

            otras_negadas = []
            for otra_pos in todas_posiciones:
                if otra_pos != pos:
                    otra_casilla = self.FichaEn.ravel([*otra_pos])
                    otras_negadas.append('-' + otra_casilla)
            
            formula_esta_sola = '(' + casilla_actual + 'Y' + Ytoria(otras_negadas) + ')'
            formulas_una_ficha.append(formula_esta_sola)
        
        formula_exactamente_una = Otoria(formulas_una_ficha)

        formula_max_una = '(' + formula_cero_fichas + 'O' + formula_exactamente_una + ')'

        regla = '-' + formula_max_una
        
        print("Regla 1 (mínimo 2 casillas ocupadas):")
        print(regla)
        return regla
    
    #Debe existir simetria horizontal 
    def regla2(self):
        casillas = [(x,y,f) for x in range(self.Nx) for y in range(self.Ny) for f in range(self.Nf)]
        lista = []
        for i in range(len(casillas)):
            x,y,f = casillas[i]
            z = self.Ny-1-y
            casilla1 = self.FichaEn.ravel([x,y,f])
            casilla2 = self.FichaEn.ravel([x,z,f])
            form = '(' + casilla1 + '=' + casilla2 + ')'
            lista.append(form)
        print("===========Regla 2============")
        print(Ytoria(lista))
        return Ytoria(lista)
    
    #Solo puede haber una ficha en cada casilla 
    def regla3(self):
        casillas = [(x,y,f) for x in range(self.Nx) for y in range(self.Ny) for f in range(self.Nf)]
        lista = []
        for i in range(len(casillas)):
            x,y,f = casillas[i]
            lista_o = []
            otras_figuras = [g for g in range(self.Nf) if g != f]
            casilla1 = self.FichaEn.ravel([x,y,f])
            for g in otras_figuras:
                casilla2 = self.FichaEn.ravel([x,y,g])
                lista_o.append(casilla2)
            form = '(' + casilla1 + '>-' + Otoria(lista_o) + ')' 
            lista.append(form)
        print("===========Regla 3============")
        print(Ytoria(lista))
        return Ytoria(lista)

    #No puede repetirse la misma figura en una fila
    def regla4(self):
        casillas = [(x,y,f) for x in range(self.Nx) for y in range(self.Ny) for f in range(self.Nf)]
        lista = []
        for i in range(len(casillas)):
            x,y,f = casillas[i]
            otras_columnas = [c for c in range(self.Nx) if c != x]
            lista_o = []
            casilla1 = self.FichaEn.ravel([x,y,f])
            for c in otras_columnas:
                casilla2 = self.FichaEn.ravel([c,y,f])
                lista_o.append(casilla2)
            form = '(' + casilla1 + '>-' + Otoria(lista_o) + ')'
            lista.append(form)
        print("===========Regla 4============")
        print(Ytoria(lista))
        return Ytoria(lista)
    
    def visualizar(self, I):
        fig, axes = plt.subplots()
        fig.set_size_inches(self.Nx, self.Ny)
        for x in range(self.Nx + 1):
            axes.axhline(x, color='black', linewidth=1)
            axes.axvline(x, color='black', linewidth=1)

        axes.axhline(self.Ny/2, color = 'red', linewidth=2, linestyle='-')

        trImg = plt.imread("./img/triangulo.png", format='png')
        ciImg = plt.imread("./img/circulo.png", format='png')
        cuImg = plt.imread("./img/cuadrado.png", format='png')

        imagenes = [trImg, cuImg, ciImg]

        for l in I:
            if I[l]:
                x,y,f = self.FichaEn.unravel(l)
                x_centro = x + 0.5
                y_centro = (self.Ny - 1 - y) + 0.5
                imagebox = OffsetImage(imagenes[f], zoom = 0.27)
                ab = AnnotationBbox(imagebox, (x_centro, y_centro), frameon = False)
                axes.add_artist(ab)

        axes.set_xticks([])
        axes.set_yticks([])

        axes.set_xlim(0, self.Nx)
        axes.set_ylim(0, self.Ny)
        axes.set_aspect('equal')
        plt.show()

"""
    def regla1(self):
        
        posiciones_fig = [(x,y,f) for x in range(self.Nx) for y in range(self.Ny) for f in range(self.Nf)]
        lista = []

        for i in range(len(posiciones_fig)):
            for j in range(i + 1, len(posiciones_fig)):
                pos1 = posiciones_fig[i]
                pos2 = posiciones_fig[j]
                
                casilla1 = self.FichaEn.ravel([*pos1])
                casilla2 = self.FichaEn.ravel([*pos2])

                form = '(' + casilla1 + 'Y' + casilla2 + ')'
                lista.append(form)
        
        print(Otoria(lista))
        return Otoria(lista)
"""
f = Fichas()
a = inorder_to_tree(f.reglas[1])
I = a.SATtableaux()
f.visualizar(I)