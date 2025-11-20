from itertools import combinations
from Archivos.Logica import *
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
    def regla1(self):
        casillas = [(x,y) for x in range(self.Nx) for y in range(self.Ny)]
        lista = []
        for i in range(len(casillas)):
            x,y = casillas[i]
            casilla_cir = self.FichaEn.ravel([x,y,2])
            lista.append(casilla_cir)
        return Otoria(lista)
            
    #Debe haber minimo un triangulo 
    def regla2(self):
        casillas = [(x,y) for x in range(self.Nx) for y in range(self.Ny)]
        lista = []
        for i in range(len(casillas)):
            x,y = casillas[i]
            casilla_tri = self.FichaEn.ravel([x,y,0])
            lista.append(casilla_tri)
        return Otoria(lista)
        
    #Debe haber minimo un cuadrado
    def regla3(self):
        casillas = [(x,y) for x in range(self.Nx) for y in range(self.Ny)]
        lista = []
        for i in range(len(casillas)):
            x,y = casillas[i]
            casilla_cua = self.FichaEn.ravel([x,y,1])
            lista.append(casilla_cua)
        return Otoria(lista)
        
    # No pueden haber filas vacías
    def regla4(self):
        lista = []
        for y in range(self.Ny): 
            figuras_en_fila = []
            for x in range(self.Nx):  
                for f in range(self.Nf):  
                    casilla = self.FichaEn.ravel([x, y, f])
                    figuras_en_fila.append(casilla)
            lista.append(Otoria(figuras_en_fila))
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
        return Ytoria(lista)
    
    #Solo puede haber una ficha en cada casilla 
    def regla6(self):
        lista = []
        for x in range(self.Nx):
            for y in range(self.Ny):
                for f1 in range(self.Nf):
                    for f2 in range(f1 + 1, self.Nf):  # solo pares distintos
                        p1 = self.FichaEn.ravel([x, y, f1])
                        p2 = self.FichaEn.ravel([x, y, f2])
                        # cláusula: (-p1 O -p2)
                        form = f"(-{p1}O-{p2})"
                        lista.append(form)
        return Ytoria(lista)

    #No puede repetirse la misma figura en una fila
    def regla7(self):
        lista = []
        for y in range(self.Ny):
            for f in range(self.Nf):
                for x1 in range(self.Nx):
                    for x2 in range(x1 + 1, self.Nx):  
                        p1 = self.FichaEn.ravel([x1, y, f])
                        p2 = self.FichaEn.ravel([x2, y, f])
                        form = f"(-{p1}O-{p2})"
                        lista.append(form)
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
                x, y, f = self.FichaEn.unravel(l)
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
