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
        regla5 = self.regla5()
        regla6 = self.regla6()
        regla7 = self.regla7()
        self.reglas = [regla1, regla2, regla3, regla4, regla5, regla6, regla7]

        #Metodo para escribir
        self.FichaEn.escribir = MethodType(escribirFichas, self.FichaEn)

    #Debe haber minimo un circulo
    def regla1():
        casillas = [(x,y) for x in range(self.Nx) for y in range(self.Ny)]
        lista = []
        for i in range(len(casillas)):
            x,y = casillas[i]
            casilla_cir = self.FichaEn.ravel([x,y,2])
            lista.append(casilla_cir)
        return Otoria(lista)
            
    #Debe haber minimo un triangulo 
    def regla2():
        casillas = [(x,y) for x in range(self.Nx) for y in range(self.Ny)]
        lista = []
        for i in range(len(casillas)):
            x,y = casillas[i]
            casilla_tri = self.FichaEn.ravel([x,y,0])
            lista.append(casilla_tri)
        return Otoria(lista)
        
    #Debe haber minimo un cuadrado
    def regla3():
        casillas = [(x,y) for x in range(self.Nx) for y in range(self.Ny)]
        lista = []
        for i in range(len(casillas)):
            x,y = casillas[i]
            casilla_cua = self.FichaEn.ravel([x,y,1])
            lista.append(casilla_cua)
        return Otoria(lista)
        
    #No pueden haber filas vacias
    def regla4():
        casillas = [(y,f) for y in range(self.Ny) for f in range(self.Nf)]
        lista = []
        for i in range(len(casillas)):
            y,f = casillas[i]
            lista_o = []
            for x in range(self.Nx):
                casilla = self.FichaEn.ravel([x,y,f])
                lista_o.append(casilla)
            lista.append(Otoria(lista_o))
        return Ytoria(lista)
            
    #Debe existir simetria horizontal 
    def regla5(self):
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
    def regla6(self):
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
    def regla7(self):
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
