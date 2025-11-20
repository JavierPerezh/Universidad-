from pysat.solvers import Solver, Minisat22
from Archivos.tseitin import tseitin
from Archivos.dpll import filtrar_interpretacion
import pycosat

def SATsolver(A):
    
    def lit_numero(l):
        if '-' in l:
            return -(ord(l[1:]) - 256)
        else:
            return ord(l) - 256
    
    def clausula_numero(C):
        return [lit_numero(l) for l in C]

    def fnc_numero(S):
        return [clausula_numero(C) for C in S]

    def obtener_int(mod):
        return {chr(256 + abs(n)):n>0 for n in mod}
    
    def obtener_numeros(S):
        lista_plana = [abs(x) for sublist in S for x in sublist]
        lista_plana = list(set(lista_plana))
        return lista_plana
        
    S = tseitin(A)
    #print(S)
    S = fnc_numero(S)
    #print(S)
    numeros = obtener_numeros(S)
    #print(numeros)
    solucion = pycosat.solve(S)
    solucion = [x for x in solucion if abs(x) in numeros]
    #print(solucion)
    solucion = obtener_int(solucion)
    #print(solucion)
    with Minisat22(bootstrap_with=S) as m:
        if m.solve():
            I = obtener_int(m.get_model())
            I = filtrar_interpretacion(I)
            return 'Satisfacible', I
        else:
            return 'Insatisfacible', {}