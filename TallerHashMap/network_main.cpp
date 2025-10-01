#include "network.h"
#include<chrono>



int main(){

    string filename = "datos_prueba.csv";
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();

    cout << "Procesando archivo: " << filename << endl;
    Network network(filename, 500);
    
    start = chrono::high_resolution_clock::now();
    
    // Encontrar un usuario y cambiar su email
    User* user1 = network.findUser("sharbidge9");
    network.changeData("sharbidge9", "email", "sharbidge9@gmail.com");
    if(user1 != nullptr){
        cout << "Usuario encontrado: " << user1 -> username << ", " << user1 -> email << ", " << user1 -> country << endl;
    } else {
        cout << "Usuario no encontrado!" << endl;
    }
    cout << "(Debe imprimir 'Usuario encontrado' y el email nuevo)" << endl;

    // Eliminar un usuario
    network.deleteUser("tbestr0");

    // Cambiar el username (rehashing)
    network.changeData("juanstudent", "username", "juanteacher");

    // Verificar que un usuario existe e imprimir su información o no
    User* userCheck = network.findUser("tbestr0");
    if(userCheck != nullptr){
        cout << "Usuario encontrado: " << userCheck -> username << ", " << userCheck -> email << ", " << userCheck -> country << endl;
    } else {
        cout << "Usuario 'tbestr0' no encontrado!" << endl;
    }
    cout << "(Debe imprimir 'Usuario 'tbestr0' no encontrado!')" << endl;


    // Crear su propio usuario, mostrar sus datos y verificar que se pueda loguear
    network.addUser("juan", "juan1234", "juancorreo@jaja.com",  "Colombia");
    User* user2 = network.findUser("juan");
    if(user2 != nullptr){
        cout << "Usuario encontrado: " << user2 -> username << ", " << user2 -> email << ", " << user2 -> country << endl;
    } else {
        cout << "Usuario no encontrado!" << endl;
    }

    cout << "(Debe imprimir 'Usuario encontrado')" << endl;

    // Login exitoso y fallido
    bool loginExitoso = network.login("juan", "juan1234");
    if (loginExitoso) { // Cambien por su usuario y contraseña
        cout << "Login exitoso!" << endl;
    } else {
        cout << "Login fallido!" << endl;
    }
    cout << "(Debe imprimir 'Login exitoso!')" << endl;


    bool loginFallido = network.login("juanteacher", "rosario2025");
    if (loginFallido) {
        cout << "Login exitoso!" << endl;
    } else {
        cout << "Login fallido!" << endl;
    }
    cout << "(Debe imprimir 'Login Fallido!')" << endl;

    
    end = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ps" << endl;
    network.display(true);
    cout << "Debe imprimir 1000 usuarios" << endl;
    cout << "===============================" << endl;
    
    return 0;
}