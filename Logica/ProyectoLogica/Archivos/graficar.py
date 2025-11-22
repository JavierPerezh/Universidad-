import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
from time import time
import numpy as np

# Configuración de estilo
sns.set_theme(style="whitegrid")
plt.rcParams['figure.figsize'] = (12, 8)

# =============================================================================
# FUNCIONES AUXILIARES (las que proporcionaste)
# =============================================================================

def obtiene_tiempos(fun, args, num_it=100):
    tiempos_fun = []
    resultados_fun = []
    for i in range(num_it):
        arranca = time()
        sat, I = fun(*args)
        para = time()
        tiempos_fun.append(para - arranca)
        resultados_fun.append(1 if sat=='Satisfacible' else 0)
    return tiempos_fun, resultados_fun

def compara_entradas_funs(funs, nombres_funs, lista_args, N=100):
    entradas = []
    funcion = []
    tiempos = []
    resultados = []
    lista_dfs = []
    for i, args in enumerate(lista_args):
        for j, fun in enumerate(funs):
            t, res = obtiene_tiempos(fun, [args], N)
            tiempos += t
            resultados += res
            entradas += [i+1]*len(t)
            funcion += [nombres_funs[j]]*len(t)
        df = pd.DataFrame({'Long_entrada':entradas, 
                           'Funcion':funcion,
                           'Tiempo_prom':tiempos,
                           'Resultados':resultados})
        lista_dfs.append(df)
    return pd.concat(lista_dfs).reset_index()

# =============================================================================
# DATOS DE EJEMPLO (usando los tiempos que proporcionaste)
# =============================================================================

def crear_dataframes_tiempos():
    # Datos para reglas individuales
    datos_individuales = {
        'Algoritmo': ['SATtabla']*7 + ['SATtableaux-Profundidad']*7 + ['SATtableaux-Anchura']*7 + 
                     ['SATtableaux-Backtracking']*7 + ['DPLL']*7 + ['SATsolver']*7,
        'Regla': ['1', '2', '3', '4', '5', '6', '7'] * 6,
        'Tiempo': [
            # SATtabla
            0.012335, 0.010642, 0.016946, None, None, None, None,
            # SATtableaux-Profundidad
            0.00044, 0.000443, 0.000344, 0.003788, 0.1487, 0.179112, 0.299487,
            # SATtableaux-Anchura
            0.000356, 0.000291, 0.000369, 0.010041, None, None, None,
            # SATtableaux-Backtracking
            0.001769, 0.002018, 0.002301, 0.0246, 0.155112, 0.416853, 0.348499,
            # DPLL
            0.001991, 0.001065, 0.001159, 0.003169, 0.008799, 0.017111, 0.073208,
            # SATsolver
            0.003069, 0.001561, 0.002154, 0.003072, 0.005249, 0.006493, 0.010582
        ]
    }
    
    # Datos para conjunciones
    datos_conjunciones = {
        'Algoritmo': ['SATtableaux-Profundidad']*6 + ['SATtableaux-Anchura']*6 + 
                     ['SATtableaux-Backtracking']*6 + ['DPLL']*6 + ['SATsolver']*6,
        'Conjuncion': ['1+2', '1+2+3', '1-4', '1-5', '1-6', '1-7'] * 5,
        'Tiempo': [
            # SATtableaux-Profundidad
            0.001385, 0.003719, 0.012635, 0.242369, 0.929977, 2.314223,
            # SATtableaux-Anchura
            0.00164, 0.00559, 0.174096, None, None, None,
            # SATtableaux-Backtracking
            0.009835, 0.02384, 0.103741, 3.722514, None, None,
            # DPLL
            0.001766, 0.002526, 0.007403, 0.029584, 0.078223, 0.194995,
            # SATsolver
            0.00234, 0.002678, 0.003524, 0.007537, 0.014598, 0.031991
        ]
    }
    
    df_individual = pd.DataFrame(datos_individuales)
    df_conjunciones = pd.DataFrame(datos_conjunciones)
    
    # Reemplazar None por NaN para evitar problemas en las gráficas
    df_individual['Tiempo'] = df_individual['Tiempo'].replace({None: np.nan})
    df_conjunciones['Tiempo'] = df_conjunciones['Tiempo'].replace({None: np.nan})
    
    return df_individual, df_conjunciones

# =============================================================================
# FUNCIONES PARA GRÁFICAS INDIVIDUALES POR ALGORITMO
# =============================================================================

def graficar_sattabla_individual(df):
    """Grafica tiempos de SATtabla para reglas individuales"""
    df_algo = df[df['Algoritmo'] == 'SATtabla'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Regla', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de SATtabla - Reglas Individuales')
        plt.yscale('log')
        plt.grid(True, alpha=0.3)
        plt.show()

def graficar_tableaux_profundidad_individual(df):
    """Grafica tiempos de SATtableaux-Profundidad para reglas individuales"""
    df_algo = df[df['Algoritmo'] == 'SATtableaux-Profundidad'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Regla', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de SATtableaux-Profundidad - Reglas Individuales')
        plt.yscale('log')
        plt.grid(True, alpha=0.3)
        plt.show()

def graficar_tableaux_anchura_individual(df):
    """Grafica tiempos de SATtableaux-Anchura para reglas individuales"""
    df_algo = df[df['Algoritmo'] == 'SATtableaux-Anchura'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Regla', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de SATtableaux-Anchura - Reglas Individuales')
        plt.yscale('log')
        plt.grid(True, alpha=0.3)
        plt.show()

def graficar_tableaux_backtracking_individual(df):
    """Grafica tiempos de SATtableaux-Backtracking para reglas individuales"""
    df_algo = df[df['Algoritmo'] == 'SATtableaux-Backtracking'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Regla', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de SATtableaux-Backtracking - Reglas Individuales')
        plt.yscale('log')
        plt.grid(True, alpha=0.3)
        plt.show()

def graficar_dpll_individual(df):
    """Grafica tiempos de DPLL para reglas individuales"""
    df_algo = df[df['Algoritmo'] == 'DPLL'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Regla', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de DPLL - Reglas Individuales')
        plt.yscale('log')
        plt.grid(True, alpha=0.3)
        plt.show()

def graficar_satsolver_individual(df):
    """Grafica tiempos de SATsolver para reglas individuales"""
    df_algo = df[df['Algoritmo'] == 'SATsolver'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Regla', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de SATsolver - Reglas Individuales')
        plt.yscale('log')
        plt.grid(True, alpha=0.3)
        plt.show()

# =============================================================================
# FUNCIONES PARA GRÁFICAS DE CONJUNCIONES POR ALGORITMO
# =============================================================================

def graficar_tableaux_profundidad_conjunciones(df):
    """Grafica tiempos de SATtableaux-Profundidad para conjunciones"""
    df_algo = df[df['Algoritmo'] == 'SATtableaux-Profundidad'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Conjuncion', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de SATtableaux-Profundidad - Conjunciones')
        plt.yscale('log')
        plt.xticks(rotation=45)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.show()

def graficar_tableaux_anchura_conjunciones(df):
    """Grafica tiempos de SATtableaux-Anchura para conjunciones"""
    df_algo = df[df['Algoritmo'] == 'SATtableaux-Anchura'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Conjuncion', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de SATtableaux-Anchura - Conjunciones')
        plt.yscale('log')
        plt.xticks(rotation=45)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.show()

def graficar_tableaux_backtracking_conjunciones(df):
    """Grafica tiempos de SATtableaux-Backtracking para conjunciones"""
    df_algo = df[df['Algoritmo'] == 'SATtableaux-Backtracking'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Conjuncion', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de SATtableaux-Backtracking - Conjunciones')
        plt.yscale('log')
        plt.xticks(rotation=45)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.show()

def graficar_dpll_conjunciones(df):
    """Grafica tiempos de DPLL para conjunciones"""
    df_algo = df[df['Algoritmo'] == 'DPLL'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Conjuncion', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de DPLL - Conjunciones')
        plt.yscale('log')
        plt.xticks(rotation=45)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.show()

def graficar_satsolver_conjunciones(df):
    """Grafica tiempos de SATsolver para conjunciones"""
    df_algo = df[df['Algoritmo'] == 'SATsolver'].dropna()
    if len(df_algo) > 0:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df_algo, x='Conjuncion', y='Tiempo', marker='o', linewidth=2.5)
        plt.title('Tiempos de SATsolver - Conjunciones')
        plt.yscale('log')
        plt.xticks(rotation=45)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.show()

# =============================================================================
# FUNCIONES PARA GRÁFICAS COMPARATIVAS
# =============================================================================

def graficar_todos_individuales(df_individual):
    """Grafica todos los algoritmos en una gráfica para reglas individuales"""
    plt.figure(figsize=(12, 8))
    sns.lineplot(data=df_individual.dropna(), x='Regla', y='Tiempo', hue='Algoritmo', 
                 marker='o', linewidth=2.5, palette='tab10')
    plt.title('Comparación de Todos los Algoritmos - Reglas Individuales', fontsize=14)
    plt.yscale('log')
    plt.ylabel('Tiempo (s) - Escala Logarítmica')
    plt.grid(True, alpha=0.3)
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.show()

def graficar_todos_conjunciones(df_conjunciones):
    """Grafica todos los algoritmos en una gráfica para conjunciones"""
    plt.figure(figsize=(12, 8))
    sns.lineplot(data=df_conjunciones.dropna(), x='Conjuncion', y='Tiempo', hue='Algoritmo', 
                 marker='o', linewidth=2.5, palette='tab10')
    plt.title('Comparación de Todos los Algoritmos - Conjunciones', fontsize=14)
    plt.yscale('log')
    plt.ylabel('Tiempo (s) - Escala Logarítmica')
    plt.xticks(rotation=45)
    plt.grid(True, alpha=0.3)
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.show()

# =============================================================================
# FUNCIÓN PRINCIPAL PARA EJECUTAR TODAS LAS GRÁFICAS
# =============================================================================

def ejecutar_todas_graficas():
    """Función principal que ejecuta todas las gráficas"""
    df_individual, df_conjunciones = crear_dataframes_tiempos()
    
    print("\n=== GRÁFICAS INDIVIDUALES POR ALGORITMO ===")
    
    print("\n1. SATtabla - Reglas Individuales")
    graficar_sattabla_individual(df_individual)
    
    print("\n2. SATtableaux-Profundidad - Reglas Individuales")
    graficar_tableaux_profundidad_individual(df_individual)
    
    print("\n3. SATtableaux-Anchura - Reglas Individuales")
    graficar_tableaux_anchura_individual(df_individual)
    
    print("\n4. SATtableaux-Backtracking - Reglas Individuales")
    graficar_tableaux_backtracking_individual(df_individual)
    
    print("\n5. DPLL - Reglas Individuales")
    graficar_dpll_individual(df_individual)
    
    print("\n6. SATsolver - Reglas Individuales")
    graficar_satsolver_individual(df_individual)
    
    print("\n=== GRÁFICAS DE CONJUNCIONES POR ALGORITMO ===")
    
    print("\n1. SATtableaux-Profundidad - Conjunciones")
    graficar_tableaux_profundidad_conjunciones(df_conjunciones)
    
    print("\n2. SATtableaux-Anchura - Conjunciones")
    graficar_tableaux_anchura_conjunciones(df_conjunciones)
    
    print("\n3. SATtableaux-Backtracking - Conjunciones")
    graficar_tableaux_backtracking_conjunciones(df_conjunciones)
    
    print("\n4. DPLL - Conjunciones")
    graficar_dpll_conjunciones(df_conjunciones)
    
    print("\n5. SATsolver - Conjunciones")
    graficar_satsolver_conjunciones(df_conjunciones)
    
    print("\n=== GRÁFICAS COMPARATIVAS ===")
    
    print("\n1. Todos los Algoritmos - Reglas Individuales")
    graficar_todos_individuales(df_individual)
    
    print("\n2. Todos los Algoritmos - Conjunciones")
    graficar_todos_conjunciones(df_conjunciones)


# =============================================================================
# EJECUCIÓN
# =============================================================================
