from random import choice
from Archivos.Proyecto import Fichas
p = Fichas()

def complemento(l):
    if '-' in l:
        return l[1:]
    else:
        return f'-{l}'
    
def eliminar_literal(S, l):
    S1 = [c for c in S if l not in c]
    lc = complemento(l)
    return [[p for p in c if p != lc] for c in S1]

def extender_I(I, l):
    I1 = {k:I[k] for k in I if k != l}
    if '-' in l:
        I1[l[1:]] = False
    else:
        I1[l] = True
    return I1

def unit_propagate(S, I):
    '''
    Algoritmo para eliminar clausulas unitarias de un conjunto de clausulas, manteniendo su satisfacibilidad
    Input: 
        - S, conjunto de clausulas
        - I, interpretacion (diccionario {literal: True/False})
    Output: 
        - S, conjunto de clausulas
        - I, interpretacion (diccionario {literal: True/False})
    '''
    while [] not in S:
        l = ''
        for x in S:
            if len(x) == 1:
                l = x[0]
                S = eliminar_literal(S, l)
                I = extender_I(I, l)
                break
        if l == '': # Se recorrió todo S y no se encontró unidad
            break
    return S, I

def dpll(S, I):
    '''
    Algoritmo para verificar la satisfacibilidad de una formula, y encontrar un modelo de la misma
    Input: 
        - S, conjunto de clausulas
        - I, interpretacion (diccionario literal->True/False)
    Output: 
        - String, Satisfacible/Insatisfacible
        - I ,interpretacion (diccionario literal->True/False)
    '''
    S,I = unit_propagate(S, I)
    if [] in S:
        return "Insatisfacible", {}
    
    if len(S) == 0:
        I = filtrar_interpretacion(I)
        return "Satisfacible", I
    
    literalesn = set()
    for clausula in S:
        for literal in clausula:
            if literal not in I:
                literalesn.add(literal)
    
    l = choice(list(literalesn))
    
    Sp = eliminar_literal(S, l)
    Ip = extender_I(I, l)

    sat, Ipp = dpll(Sp,Ip)
    
    if  sat == "Satisfacible":
        return "Satisfacible", Ipp
    else:
        Spp = eliminar_literal(Sp, complemento(l))
        Ipp = extender_I(I, complemento(l))
        return dpll(Spp,Ipp)
    
def filtrar_interpretacion(I, descriptor = p.FichaEn):
    """
    Filtra una interpretación I para conservar solo las letras
    que fueron creadas por el descriptor dado.
    """
    letras_validas = [
        descriptor.ravel([x, y, f])
        for x in range(descriptor.args_lista[0])
        for y in range(descriptor.args_lista[1])
        for f in range(descriptor.args_lista[2])
    ]
    return {l: v for l, v in I.items() if l in letras_validas}