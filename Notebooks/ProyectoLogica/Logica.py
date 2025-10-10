'''
Librería con las clases y funciones
para lógica proposicional
'''
from random import choice
from itertools import product
import numpy as np
from copy import deepcopy
from typing import List, Dict

CONECTIVOS = ['-', 'Y','O','>','=']
CONECTIVOS_BINARIOS = ['Y','O','>','=']

class Formula :

    def __init__(self) :
        pass

    def __str__(self) :
        if type(self) == Letra:
            return self.letra
        elif type(self) == Negacion:
            return '-' + str(self.subf)
        elif type(self) == Binario:
            return "(" + str(self.left) + self.conectivo + str(self.right) + ")"

    def letras(self):
        if type(self) == Letra:
            return set(self.letra)
        elif type(self) == Negacion:
            return self.subf.letras()
        elif type(self) == Binario:
            return self.left.letras().union(self.right.letras())

    def subforms(self):
        if type(self) == Letra:
            return [str(self)]
        elif type(self) == Negacion:
            return list(set([str(self)] + self.subf.subforms()))
        elif type(self) == Binario:
            return list(set([str(self)] + self.left.subforms() + self.right.subforms()))

    def valor(self, I) :
        if type(self) == Letra:
            return I[self.letra]
        elif type(self) == Negacion:
            return not self.subf.valor(I)
        elif type(self) == Binario:
            if self.conectivo == 'Y':
                return self.left.valor(I) and self.right.valor(I)
            if self.conectivo == 'O':
                return self.left.valor(I) or self.right.valor(I)
            if self.conectivo == '>':
                return not self.left.valor(I) or self.right.valor(I)
            if self.conectivo == '=':
                return (self.left.valor(I) and self.right.valor(I)) or (not self.left.valor(I) and not self.right.valor(I))

    def SATtabla(self):
        letras = list(self.letras())
        n = len(letras)
        valores = list(product([True, False], repeat=n))
        for v in valores:
            I = {letras[x]: v[x] for x in range(n)}
            if self.valor(I):
                return I
        return None

    def clasifica_para_tableaux(self):
        if type(self) == Letra:
            return None, 'literal'
        elif type(self) == Negacion:
            if type(self.subf) == Letra:
                return None, 'literal'
            elif type(self.subf) == Negacion:
                return 1, 'alfa'
            elif type(self.subf) == Binario:
                if self.subf.conectivo == 'O':
                    return 3, 'alfa'
                elif self.subf.conectivo == '>':
                    return 4, 'alfa'
                elif self.subf.conectivo == 'Y':
                    return 1, 'beta'
                elif self.subf.conectivo == '=':
                    return 4, 'beta'
        elif type(self) == Binario:
            if self.conectivo == 'Y':
                return 2, 'alfa'
            elif self.conectivo == 'O':
                return 2, 'beta'
            elif self.conectivo == '>':
                return 3, 'beta'
            elif self.conectivo == '=':
                return 5, 'alfa'

    def SATtableaux(self):
        estado = nodos_tableaux([self])
        res = estado.es_hoja()
        if res == 'cerrada':
            return None
        elif res == 'abierta':
            return estado.interp()
        frontera = [estado]
        while len(frontera) > 0:
            estado = frontera.pop()
            hijos = estado.expandir()
            for a in hijos:
                if a != None:
                    res = a.es_hoja()
                    if res == 'abierta':
                        return a.interp()
                    elif res == None:
                        frontera.append(a)
        return None

    def ver(self, D):
        '''
        Visualiza una fórmula A (como string en notación inorder) usando el descriptor D
        '''
        vis = []
        A = str(self)
        for c in A:
            if c == '-':
                vis.append(' no ')
            elif c in ['(', ')']:
                vis.append(c)
            elif c in ['>', 'Y', 'O']:
                vis.append(' ' + c + ' ')
            elif c == '=':
                vis.append(' sii ')
            else:
                try:
                    vis.append(D.escribir(c))
                except:
                    raise("¡Caracter inválido!")
        return ''.join(vis)
    
    def num_conec(self):
        """
        Cuenta el número de conectivos en la fórmula
        """
        if type(self) == Letra:
            return 0
        elif type(self) == Negacion:
            return 1 + self.subf.num_conec()
        elif type(self) == Binario:
            return 1 + self.left.num_conec() + self.right.num_conec()

class Letra(Formula) :
    def __init__ (self, letra:str) :
        self.letra = letra

class Negacion(Formula) :
    def __init__(self, subf:Formula) :
        self.subf = subf

class Binario(Formula) :
    def __init__(self, conectivo:str, left:Formula, right:Formula) :
        assert(conectivo in ['Y','O','>','='])
        self.conectivo = conectivo
        self.left = left
        self.right = right


def inorder_to_tree(cadena:str) -> Formula:
    if len(cadena) == 0:
        raise Exception('¡Error: cadena vacía!')
    if len(cadena) == 1:
        assert(cadena not in CONECTIVOS), f"Error: El símbolo de letra proposicional {cadena} no puede ser un conectivo ({CONECTIVOS})."
        return Letra(cadena)
    elif cadena[0] == '-':
        try:
            return Negacion(inorder_to_tree(cadena[1:]))
        except Exception as e:
            msg_error = f'Cadena incorrecta:\n\t{cadena[1:]}\n'
            msg_error += f'Error obtenido:\n\t{e}'
            raise Exception(msg_error)
    elif cadena[0] == "(":
        assert(cadena[-1] == ")"), f'¡Cadena inválida! Falta un paréntesis final en {cadena}'
        counter = 0 #Contador de parentesis
        for i in range(1, len(cadena)):
            if cadena[i] == "(":
                counter += 1
            elif cadena[i] == ")":
                counter -=1
            elif cadena[i] in CONECTIVOS_BINARIOS and counter == 0:
                try:
                    return Binario(cadena[i], inorder_to_tree(cadena[1:i]),inorder_to_tree(cadena[i + 1:-1]))
                except Exception as e:
                    msg_error = f'{e}\n\n'
                    msg_error += f'Error en la cadena:\n\t{cadena}'
                    msg_error += f'\nSe pide procesar el conectivo principal: {cadena[i]}'
                    msg_error += f'\nRevisar las subfórmulas\t{cadena[1:i]}\n\t{cadena[i + 1:-1]}'
                    raise Exception(msg_error)
    else:
        raise Exception('¡Cadena inválida! Revise la composición de paréntesis de la fórmula.\nRecuerde que solo los conectivos binarios incluyen paréntesis en la fórmula.')


class Descriptor :

    '''
    Codifica un descriptor de N argumentos mediante un solo caracter
    Input:  args_lista, lista con el total de opciones para cada
                     argumento del descriptor
            chrInit, entero que determina el comienzo de la codificación chr()
    Output: str de longitud 1
    '''

    def __init__ (self,args_lista,chrInit=256) -> None:
        self.args_lista = args_lista
        assert(len(args_lista) > 0), "Debe haber por lo menos un argumento"
        self.chrInit = chrInit
        self.rango = [chrInit, chrInit + np.prod(self.args_lista)]

    def check_lista_valores(self,lista_valores: List[int]) -> None:
        for i, v in enumerate(lista_valores) :
            assert(v >= 0), "Valores deben ser no negativos"
            assert(v < self.args_lista[i]), f"Valor debe ser menor o igual a {self.args_lista[i]}"

    def codifica(self,lista_valores: List[int]) -> int:
        self.check_lista_valores(lista_valores)
        cod = lista_valores[0]
        n_columnas = 1
        for i in range(0, len(lista_valores) - 1) :
            n_columnas = n_columnas * self.args_lista[i]
            cod = n_columnas * lista_valores[i+1] + cod
        return cod

    def decodifica(self,n: int) -> int:
        decods = []
        if len(self.args_lista) > 1:
            for i in range(0, len(self.args_lista) - 1) :
                n_columnas = np.prod(self.args_lista[:-(i+1)])
                decods.insert(0, int(n / n_columnas))
                n = n % n_columnas
        decods.insert(0, n % self.args_lista[0])
        return decods

    def ravel(self,lista_valores: List[int]) -> chr:
        codigo = self.codifica(lista_valores)
        return chr(self.chrInit+codigo)

    def unravel(self,codigo: chr) -> int:
        n = ord(codigo)-self.chrInit
        return self.decodifica(n)
    
    def escribir(self, literal: chr) -> str:
        if '-' in literal:
            atomo = literal[1:]
            neg = ' no'
        else:
            atomo = literal
            neg = ''
        x, y, n  = self.unravel(atomo)
        return f"PREDICADO({x, y, n})"        
    
    
def visualizar_formula(A: Formula, D: Descriptor) -> str:
    '''
    Visualiza una fórmula A (como string en notación inorder) usando el descriptor D
    '''
    vis = []
    for c in A:
        if c == '-':
            vis.append(' no ')
        elif c in ['(', ')']:
            vis.append(c)
        elif c in ['>', 'Y', 'O']:
            vis.append(' ' + c + ' ')
        elif c == '=':
            vis.append(' sii ')
        else:
            try:
                vis.append(D.escribir(c))
            except:
                raise("¡Caracter inválido!")
    return ''.join(vis)


def Ytoria(lista_forms):
    form = ''
    inicial = True
    for f in lista_forms:
        if inicial:
            form = f
            inicial = False
        else:
            form = '(' + form + 'Y' + f + ')'
    return form

def Otoria(lista_forms):
    form = ''
    inicial = True
    for f in lista_forms:
        if inicial:
            form = f
            inicial = False
        else:
            form = '(' + form + 'O' + f + ')'
    return form

class nodos_tableaux:

    def __init__(self, fs):
        clasfs = [(A, str(A), *A.clasifica_para_tableaux()) for A in fs]
        self.alfas = [c for c in clasfs if c[3] == 'alfa']
        self.betas = [c for c in clasfs if c[3] == 'beta']
        self.literales = [c for c in clasfs if c[3] == 'literal']

    def __str__(self):
        cadena = f'Alfas:{[str(c[1]) for c in self.alfas]}\n'
        cadena += f'Betas:{[str(c[1]) for c in self.betas]}\n'
        cadena += f'Literales:{[str(c[1]) for c in self.literales]}'
        return cadena

    def tiene_lit_comp(self):
        lits = [c[1] for c in self.literales]
        l_pos = [l for l in lits if '-' not in l]
        l_negs = [l[1:] for l in lits if '-' in l]
        return len(set(l_pos).intersection(set(l_negs))) > 0

    def es_hoja(self):
        if self.tiene_lit_comp():
            return 'cerrada'
        elif ((len(self.alfas) == 0) and (len(self.betas) == 0)):
            return 'abierta'
        else:
            return None

    def interp(self):
        I = {}
        for lit in self.literales:
            l = lit[1]
            if '-' not in l:
                I[l] = True
            else:
                I[l[1:]] = False
        return I

    def expandir(self):
        '''Escoge última alfa, si no última beta, si no None'''
        f_alfas = deepcopy(self.alfas)
        f_betas = deepcopy(self.betas)
        f_literales = deepcopy(self.literales)
        if len(self.alfas) > 0:
            f, s, num_regla, cl = f_alfas.pop(0)
            if num_regla == 1:
                formulas = [f.subf.subf]
            elif num_regla == 2:
                formulas = [f.left, f.right]
            elif num_regla == 3:
                formulas = [Negacion(f.subf.left), Negacion(f.subf.right)]
            elif num_regla == 4:
                formulas = [f.subf.left, Negacion(f.subf.right)]
            elif num_regla == 5:
                formulas = [Binario('>', f.left, f.right), Binario('>', f.right, f.left)]
            for nueva_f in formulas:
                clasf = nueva_f.clasifica_para_tableaux()
                if clasf[1]== 'alfa':
                    lista = f_alfas
                elif clasf[1]== 'beta':
                    lista = f_betas
                elif clasf[1]== 'literal':
                    lista = f_literales
                strs = [c[1] for c in lista]
                if str(nueva_f) not in strs:
                    lista.append((nueva_f, str(nueva_f), *clasf))
            nuevo_nodo = nodos_tableaux([])
            nuevo_nodo.alfas = f_alfas
            nuevo_nodo.betas = f_betas
            nuevo_nodo.literales = f_literales
            return [nuevo_nodo]
        elif len(self.betas) > 0:
            f, s, num_regla, cl = f_betas.pop(0)
            if num_regla == 1:
                B1 = Negacion(f.subf.left)
                B2 = Negacion(f.subf.right)
            elif num_regla == 2:
                B1 = f.left
                B2 = f.right
            elif num_regla == 3:
                B1 = Negacion(f.left)
                B2 = f.right
            elif num_regla == 4:
                B1 = Binario('Y', f.subf.left, Negacion(f.subf.right))
                B2 = Binario('Y', Negacion(f.subf.left), f.subf.right)
            f_alfas2 = deepcopy(f_alfas)
            f_betas2 = deepcopy(f_betas)
            f_literales2 = deepcopy(f_literales)
            clasf = B1.clasifica_para_tableaux()
            if clasf[1]== 'alfa':
                lista = f_alfas
            elif clasf[1]== 'beta':
                lista = f_betas
            elif clasf[1]== 'literal':
                lista = f_literales
            strs = [c[1] for c in lista]
            if str(B1) not in strs:
                lista.append((B1, str(B1), *clasf))
            clasf = B2.clasifica_para_tableaux()
            if clasf[1]== 'alfa':
                lista = f_alfas2
            elif clasf[1]== 'beta':
                lista = f_betas2
            elif clasf[1]== 'literal':
                lista = f_literales2
            strs = [c[1] for c in lista]
            if str(B2) not in strs:
                lista.append((B2, str(B2), *clasf))
            n1 = nodos_tableaux([])
            n1.alfas = f_alfas
            n1.betas = f_betas
            n1.literales = f_literales
            n2 = nodos_tableaux([])
            n2.alfas = f_alfas2
            n2.betas = f_betas2
            n2.literales = f_literales2
            return [n1, n2]
        else:
            return []


def complemento(l):
    """
    Calcula el complemento de un literal
    Input: l, literal (string)
    Output: complemento de l (string)
    """
    if l[0] == '-':
        return l[1:]
    else:
        return '-' + l

def eliminar_literal(S, l):
    """
    Elimina un literal de un conjunto de cláusulas
    Input: 
        - S, conjunto de cláusulas
        - l, literal a eliminar
    Output: conjunto de cláusulas modificado
    """
    S1 = [c for c in S if l not in c]
    lc = complemento(l)
    return [[p for p in c if p != lc] for c in S1]

def extender_I(I, l):
    """
    Extiende una interpretación con un nuevo literal
    Input:
        - I, interpretación actual (diccionario)
        - l, literal a agregar
    Output: interpretación extendida
    """
    I1 = I.copy()
    if '-' in l:
        I1[l[1:]] = False
    else:
        I1[l] = True
    return I1

def unit_propagate(S, I):
    """
    Aplica propagación unitaria a un conjunto de cláusulas
    Input:
        - S, conjunto de cláusulas
        - I, interpretación parcial
    Output:
        - S, conjunto de cláusulas después de propagación
        - I, interpretación extendida
    """
    while [] not in S:
        l = ''
        for x in S:
            if len(x) == 1:
                l = x[0]
                # Verificar si el literal no está vacío
                if l == '':
                    S.remove(x)  # Eliminar la cláusula problemática
                    l = ''  # Resetear l para continuar
                    break
                S = eliminar_literal(S, l)
                I = extender_I(I, l)
                break
        if l == '': 
            break
    return S, I

def filtrar_interpretacion(I, descriptor):
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


def dpll(S, I, descriptor=None):
    """
    Algoritmo DPLL para verificar satisfacibilidad
    Input:
        - S: conjunto de cláusulas
        - I: interpretación parcial
        - descriptor: (opcional) objeto Descriptor que define las letras válidas
    Output:
        - String: "Satisfacible" o "Insatisfacible"
        - I: interpretación que satisface S (si es satisfacible)
    """
    # Paso 1: Aplicar propagación unitaria
    limpiar_clausulas(S)
    S, I = unit_propagate(S, I)
    
    # Paso 2: Si hay cláusula vacía, es insatisfacible
    if [] in S:
        return "Insatisfacible", {}
    
    # Paso 3: Si no hay cláusulas, es satisfacible
    if len(S) == 0:
        # Si hay descriptor, filtramos antes de retornar
        if descriptor is not None:
            I = filtrar_interpretacion(I, descriptor)
        return "Satisfacible", I
    
    # Paso 4: Seleccionar literal no asignado
    literales_no_asignados = []
    for clausula in S:
        for literal in clausula:
            atomo = literal[1:] if literal[0] == '-' else literal
            if atomo not in I:
                literales_no_asignados.append(literal)
    
    if not literales_no_asignados:
        return "Insatisfacible", {}
    
    l = choice(literales_no_asignados)
    
    # Paso 5-7: Probar con l = True
    S_prime = eliminar_literal(S, l)
    I_prime = extender_I(I, l)
    resultado, I_resultado = dpll(S_prime, I_prime, descriptor)
    if resultado == "Satisfacible":
        return "Satisfacible", I_resultado
    
    # Paso 8-11: Probar con l = False (complemento de l)
    lc = complemento(l)
    S_double_prime = eliminar_literal(S, lc)
    I_double_prime = extender_I(I, lc)
    
    return dpll(S_double_prime, I_double_prime, descriptor)


def a_clausal(A):
    """
    Convierte equivalencias a forma clausal
    Input: A (cadena) de la forma:
           p=-q
           p=(qYr)
           p=(qOr) 
           p=(q>r)
           p=(q=r)
    Output: lista de cláusulas
    """
    assert(len(A)==4 or len(A)==7), "Fórmula incorrecta!"
    B = ''
    p = A[0]
    
    if "-" in A:
        q = A[-1]
        B = "-"+p+"O-"+q+"Y"+p+"O"+q
    elif "Y" in A:
        q = A[3]
        r = A[5]
        B = q+"O-"+p+"Y"+r+"O-"+p+"Y-"+q+"O-"+r+"O"+p
    elif "O" in A:
        q = A[3]
        r = A[5]
        B = "-"+q+"O"+p+"Y-"+r+"O"+p+"Y"+q+"O"+r+"O-"+p
    elif ">" in A:
        q = A[3]
        r = A[5]
        B = q+"O"+p+"Y-"+r+"O"+p+"Y-"+q+"O"+r+"O-"+p
    elif "=" in A:
        q = A[3]
        r = A[5]
        B = q+"O"+"-"+r+"O"+"-"+p+"Y"+"-"+q+"O"+r+"O"+"-"+p+"Y"+"-"+q+"O"+"-"+r+"O"+p+"Y"+q+"O"+r+"O"+p
    else:
        print('Error en a_clausal(): Fórmula incorrecta!')
    
    # Convertir a lista de listas
    B = B.split('Y')
    B = [c.split('O') for c in B]
    return B

def tseitin(A):
    '''
    Algoritmo de transformación de Tseitin
    Input: A (cadena) en notación inorder
    Output: B, lista de cláusulas
    '''
    # Creamos letras proposicionales nuevas
    f = inorder_to_tree(A)
    letrasp = f.letras()
    cods_letras = [ord(x) for x in letrasp]
    m = max(cods_letras) + 256 if cods_letras else 256
    letrasp_tseitin = [chr(x) for x in range(m, m + f.num_conec())]
    letrasp = list(letrasp) + letrasp_tseitin
    
    L = [] # Lista de definiciones
    Pila = [] # Pila para procesamiento
    i = -1 # Contador de variables nuevas
    s = A[0] # Símbolo de trabajo actual
    cadena_restante = A[1:] # Resto de la cadena
    
    while len(A) > 0:
        # print("Pila:", Pila, " L:", L, " s:", s)
        
        if (s in letrasp) and (len(Pila) > 0) and (Pila[-1] == '-'):
            # Caso: negación de un literal
            i += 1
            atomo = letrasp_tseitin[i]
            Pila = Pila[:-1] # Quitar el '-'
            Pila.append(atomo)
            L.append(atomo + "=-" + s)
            A = cadena_restante
            if len(A) > 0:
                s = A[0]
                cadena_restante = A[1:]
                
        elif s == ')':
            # Procesar subfórmula completa
            w = Pila[-1]  # Segundo operando
            O = Pila[-2]  # Operador  
            v = Pila[-3]  # Primer operando
            Pila = Pila[:len(Pila)-4] # Quitar los 4 elementos
            
            i += 1
            atomo = letrasp_tseitin[i]
            L.append(atomo + "=(" + v + O + w + ")")
            Pila.append(atomo)
            A = cadena_restante
            if len(A) > 0:
                s = A[0]
                cadena_restante = A[1:]
                
        else:
            Pila.append(s)
            A = cadena_restante
            if len(A) > 0:
                s = A[0]
                cadena_restante = A[1:]
            else:
                s = ''
    
    # CORRECCIÓN: Determinar el átomo raíz de manera segura
    if i >= 0:
        # Caso normal: hay variables Tseitin creadas
        atomo = letrasp_tseitin[i]
    elif Pila and Pila[-1] in letrasp:
        # Caso: la fórmula es solo un literal
        atomo = Pila[-1]
    else:
        # Caso de error: no se pudo determinar el átomo raíz
        raise Exception("No se pudo determinar el átomo raíz en Tseitin")
    
    # Construir el resultado final
    B = [[atomo]]  # La fórmula completa es equivalente al átomo raíz
    for definicion in L:
        clausulas = a_clausal(definicion)
        B.extend(clausulas)
    
    return B

def limpiar_clausulas(S):
    """
    Limpia cláusulas eliminando literales vacíos y cláusulas problemáticas
    """
    S_limpio = []
    for clausula in S:
        clausula_limpia = [lit for lit in clausula if lit != '']
        if clausula_limpia:  
            S_limpio.append(clausula_limpia)
    return S_limpio