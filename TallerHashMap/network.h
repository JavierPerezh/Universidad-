#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class User
{
public:
    string username;
    string password;
    string email;
    string country;
    User *next;

    User(string username,
         string password,
         string email,
         string country) : username(username), password(password), email(email), country(country), next(nullptr) {}
};

class Network
{
private:
    User **users;
    int capacity;
    int size;

    int hashFunction(string key)
    { // Función hash simple vista en clase
        int hash = 0;
        for (char ch : key)
        {
            hash += ch;
        }
        return hash % this->capacity;
    }

public:
    Network(string &file, int capacity = 1) : capacity(capacity), size(0)
    {
        users = new User *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            users[i] = nullptr;
        }
        addUsersFromCSV(file);
    }

    ~Network()
    {
        for (int i = 0; i < capacity; i++)
        {
            User *current = users[i];
            while (current != nullptr)
            {
                User *toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] users;
    }

    void addUsersFromCSV(string &filename)
    {
        // El archivo CSV tiene el formato: username,email,password,country
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "No se pudo abrir el archivo - '" << filename << "'" << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            istringstream ss(line);
            string username, password, email, country;

            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, email, ',');
            getline(ss, country, ',');

            addUser(username, password, email, country);
        }

        file.close();
    }

    void addUser(string username, string password, string email, string country)
    {

        if (findUser(username) != nullptr)
        {
            cout << "Usuario '" << username << "' ya existe" << endl;
            return;
        }

        int index = hashFunction(username);
        User *newUser = new User(username, password, email, country);

        if (users[index] == nullptr)
        {
            users[index] = newUser;
        }
        else
        {
            User *current = users[index];
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newUser;
        }

        size++;

        if ((float)size / capacity > 0.7)
        {
            resize();
        }
    }

    User *findUser(string username)
    {
        int index = hashFunction(username);
        User *current = users[index];

        while (current != nullptr)
        {
            if (current->username == username)
            {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void deleteUser(string username)
    {
        int index = hashFunction(username);

        if (users[index] == nullptr)
        {
            return;
        }

        User *current = users[index];
        User *prev = nullptr;

        while (current != nullptr && current->username != username)
        {
            prev = current;
            current = current->next;
        }

        if (current == nullptr)
        {
            return;
        }

        if (prev == nullptr)
        {
            users[index] = current->next;
        }
        else
        {
            prev->next = current->next;
        }

        delete current;
        size--;
    }

    void changeData(string username, string field, string newValue)
    {
        User *user = findUser(username);

        if (user == nullptr)
            return;

        if (field == "username")
        {
            addUser(newValue, user->password, user->email, user->country);
            deleteUser(username);
        }
        else if (field == "password")
        {
            user->password = newValue;
        }
        else if (field == "email")
        {
            user->email = newValue;
        }
        else if (field == "country")
        {
            user->country = newValue;
        }
        else
        {
            cout << "Campo inexistente" << endl;
        }
    }
    
    void resize()
    {
        int oldCapacity = capacity;
        capacity *= 2;
        User **oldUsers = users;

        users = new User *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            users[i] = nullptr;
        }

        int oldSize = size;
        size = 0;

        for (int i = 0; i < oldCapacity; i++)
        {
            User *current = oldUsers[i];
            while (current != nullptr)
            {
                User *nextUser = current->next;

                int newIndex = hashFunction(current->username);
                current->next = nullptr; // Desconectar de lista antigua

                if (users[newIndex] == nullptr)
                {
                    users[newIndex] = current;
                }
                else
                {
                    User *temp = users[newIndex];
                    while (temp->next != nullptr)
                    {
                        temp = temp->next;
                    }
                    temp->next = current;
                }

                size++; // Incrementar contador manualmente
                current = nextUser;
            }
        }

        // Verificar que el tamaño sea consistente
        if (size != oldSize)
        {
            cerr << "Error: tamaño inconsistente después de resize" << endl;
        }

        // Liberar el array antiguo (solo el array, no los usuarios)
        delete[] oldUsers;
    }

    bool login(string username, string password)
    {
        User *user = findUser(username);

        if (user == nullptr)
            return false;
        if (password == user->password)
        {
            return true;
        }
        return false;
    }

    void display(bool verbose = false)
    {
        int count = 0, usuarios = 0;
        for (int i = 0; i < capacity; i++)
        {
            User *current = users[i];
            if (current != nullptr)
            {
                count++;
                string indexInfo = "Index " + to_string(i) + ": ";
                while (current != nullptr)
                {
                    usuarios++;
                    indexInfo += "[" + current->username + " -> ";
                    current = current->next;
                }
                indexInfo += "]";
                if (verbose)
                    cout << indexInfo << endl;
            }
        }
        cout << "Indices ocupados: " << count << " de " << capacity << endl;
        cout << "Usuarios totales: " << usuarios << endl;
    }
};