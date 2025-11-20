from pysat.solvers import Solver, Minisat22
from Archivos.tsitin import *
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
    S_num = fnc_numero(S)
    numeros = obtener_numeros(S_num)
    # Validar que la CNF no esté vacía ni tenga cláusulas vacías
    if not S_num or any(len(c) == 0 for c in S_num):
        return 'Insatisfacible', {}
    try:
        solucion = pycosat.solve(S_num)
        if solucion == 'UNSAT' or solucion == 'UNKNOWN':
            return 'Insatisfacible', {}
        solucion = [x for x in solucion if abs(x) in numeros]
        solucion_dict = obtener_int(solucion)
    except Exception as e:
        return 'Insatisfacible', {}
    with Minisat22(bootstrap_with=S_num) as m:
        if m.solve():
            modelo = m.get_model()
            modelo = [x for x in modelo if abs(x) in numeros]
            return 'Satisfacible', obtener_int(modelo)
        else:
            return 'Insatisfacible', {}