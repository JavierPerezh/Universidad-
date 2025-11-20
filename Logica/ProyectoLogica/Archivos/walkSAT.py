from random import random, choice, randint
from copy import deepcopy
from Archivos.dpll import filtrar_interpretacion, complemento

class WalkSatEstado():
    
    def __init__(self, S):
        self.S = S
        self.letrasp = list(set([l[-1] for C in self.S for l in C]))
        self.I = interpretacion_aleatoria(self.letrasp)
        self.I_lits = set([p for p in self.letrasp if self.I[p]] + ['-'+p for p in self.letrasp if not self.I[p]])
        self.clausulas_sat = [C for C in self.S if any((True for x in self.I_lits if x in C))]
        self.clausulas_unsat = [C for C in self.S if C not in self.clausulas_sat]

    def actualizar(self, I):
        self.I = I
        self.I_lits = set([p for p in self.letrasp if self.I[p]] + ['-'+p for p in self.letrasp if not self.I[p]])
        self.clausulas_sat = [C for C in self.S if any((True for x in self.I_lits if x in C))]
        self.clausulas_unsat = [C for C in self.S if C not in self.clausulas_sat]
       
    def SAT(self):
        return len(self.clausulas_unsat) == 0

    def break_count(self, l):
        if l in self.I_lits:
            lit = l
        else:
            lit = complemento(l)
        clausulas_break_count = [C for C in self.clausulas_sat if set(C).intersection(self.I_lits)=={lit}]
        return len(clausulas_break_count)

def interpretacion_aleatoria(letrasp):
    I = {p:randint(0,1)==1 for p in letrasp}
    return I

def flip_literal(I, l):
    p = l[-1]
    valor = False if I[p] else True
    Ip = deepcopy(I)
    Ip[p] = valor
    return Ip

def WalkSAT(S):
    max_flips = 10000
    max_tries = 1000
    p = 0.37
    for i in range(max_tries):
        w = WalkSatEstado(S)
        for j in range(max_flips):
            if w.SAT():
                w.I = filtrar_interpretacion(w.I)
                return "Satisfacible", w.I
            C = choice(w.clausulas_unsat)
            variables_break_cero = []
            for x in C:
                if w.break_count(x) == 0:
                    variables_break_cero.append(x)
            if variables_break_cero:
                v = choice(variables_break_cero)
            else:
                if random() < p:
                    v = choice(C)
                else:
                    break_counts = {x: w.break_count(x) for x in C}
                    min_break = min(break_counts.values())
                    variables_min_break = [x for x in C if break_counts[x] == min_break]
                    v = choice(variables_min_break)
            nueva_I = flip_literal(w.I, v)
            w.actualizar(nueva_I)
    return None