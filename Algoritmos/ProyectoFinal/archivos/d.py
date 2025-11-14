import csv
import random
from faker import Faker

def generar_csv_facebook():
    fake = Faker('es_ES')
    
    ciudades = [
        "Madrid", "Barcelona", "Valencia", "Sevilla", "Zaragoza", "Málaga", "Murcia", 
        "Palma", "Las Palmas", "Bilbao", "Alicante", "Córdoba", "Valladolid", "Vigo", 
        "Gijón", "Hospitalet", "La Coruña", "Granada", "Vitoria", "Elche", "Oviedo",
        "Santander", "Jerez", "Pamplona", "Santiago", "Albacete", "Logroño", "Badajoz",
        "Salamanca", "Huelva", "Lleida", "Marbella", "Cádiz", "Jaén", "Almería", "Tarragona"
    ]
    
    profesiones = [
        "Ingeniero de Software", "Médico", "Enfermero", "Profesor", "Abogado", 
        "Arquitecto", "Diseñador Gráfico", "Contador", "Periodista", "Psicólogo",
        "Chef", "Mecánico", "Electricista", "Fontanero", "Carpintero", "Agricultor",
        "Veterinario", "Farmacéutico", "Ingeniero Civil", "Piloto", "Azafata",
        "Comercial", "Marketing Digital", "Recursos Humanos", "Administrativo",
        "Investigador", "Científico de Datos", "Desarrollador Web", "Analista de Sistemas",
        "Consultor", "Gerente de Proyectos", "Arqueólogo", "Biólogo", "Químico"
    ]
    
    datos = []
    for id in range(4039):  
        nombre = fake.name()
        ciudad = random.choice(ciudades)
        profesion = random.choice(profesiones)
        
        datos.append({
            'Id': id,
            'Nombre': nombre,
            'Ciudad': ciudad,
            'Profesion': profesion
        })
    
    # Escribir archivo CSV
    nombre_archivo = 'usuarios_facebook.csv'
    with open(nombre_archivo, 'w', newline='', encoding='utf-8') as archivo:
        campos = ['Id', 'Nombre', 'Ciudad', 'Profesion']
        escritor = csv.DictWriter(archivo, fieldnames=campos)
        
        escritor.writeheader()
        escritor.writerows(datos)
    
    print(f"Archivo '{nombre_archivo}' generado exitosamente!")
    print(f"Total de registros: {len(datos)}")
    print("\nPrimeros 5 registros:")
    for i in range(5):
        print(f"ID: {datos[i]['Id']}, Nombre: {datos[i]['Nombre']}, Ciudad: {datos[i]['Ciudad']}, Profesión: {datos[i]['Profesion']}")

if __name__ == "__main__":
    generar_csv_facebook()