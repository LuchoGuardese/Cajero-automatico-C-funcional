#include <iostream>
#include <ctime>
#include <string>
#include <fstream> 
#include <iomanip> 

using namespace std;

const int MAX_LIBROS = 100;
const int MAX_USUARIOS = 50;
const int MAX_PRESTAMOS = 200;

struct Libro {
    string titulo;
    string autor;
    string ISBN;
    double precio;
    bool disponible;
};

struct Usuario {
    string nombre;
    string contrasena;
    int numLibrosPrestados;
};

struct Prestamo {
    string ISBN;
    string usuario;
    time_t fechaPrestamo;
    time_t fechaDevolucion;
    bool devuelto; 
};

struct Reserva {
    string ISBN;
    string usuario;
    time_t fechaReserva;
    bool activa;
};

Libro libros[MAX_LIBROS];
int numLibros = 0;
Usuario usuarios[MAX_USUARIOS];
int numUsuarios = 0;
Prestamo prestamos[MAX_PRESTAMOS];
int numPrestamos = 0;
Reserva reservas[MAX_PRESTAMOS];
int numReservas = 0;

void Ingreso();
void crearUsuario();
void menuPrincipal(const string& usuario);
void gestionLibros();
void gestionReservas(const string& usuario);
void agregarLibro();
void buscarLibro();
void editarLibro();
void prestarLibro(const string& usuario);
void devolverLibro(const string& usuario);
void reservarLibro(const string& usuario);
void cancelarReserva(const string& usuario);
void consultarReservas(const string& usuario);
void generarReportes();
void calcularMultas(const string& usuario);
void cargarDatos();
void guardarDatos();
void cargarReservas();
void guardarReservas();

int main() {
    cargarDatos();
    cargarReservas();
    
    while (true) {
        cout << "\nBienvenido a la Biblioteca" << endl;
        cout << "--------------------------" << endl;
        cout << "1. Ingresar usuario" << endl;
        cout << "2. Crear usuario" << endl;
        cout << "3. Salir" << endl;
        cout << "--------------------------" << endl;
        cout << "Ingrese una opcion: ";
        
        int opcion;
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                Ingreso(); 
                break;
            case 2:
                crearUsuario(); 
                break;
            case 3:
                guardarDatos();
                guardarReservas();
                cout << "Gracias por usar el sistema." << endl;
                return 0;
            default:
                cout << "Opcion no valida" << endl;
                break;
        }
    }
    return 0;
}

void cargarDatos() {
    ifstream archivoLibros("libros.csv");
    string linea;
    while (getline(archivoLibros, linea) && numLibros < MAX_LIBROS) {
        size_t pos = 0;
        string token;
        pos = linea.find(",");
        libros[numLibros].titulo = linea.substr(0, pos);
        linea.erase(0, pos + 1);
        
        pos = linea.find(",");
        libros[numLibros].autor = linea.substr(0, pos);
        linea.erase(0, pos + 1);
        
        pos = linea.find(",");
        libros[numLibros].ISBN = linea.substr(0, pos);
        linea.erase(0, pos + 1);
        
        pos = linea.find(",");
        libros[numLibros].precio = stod(linea.substr(0, pos));
        linea.erase(0, pos + 1);
        
        libros[numLibros].disponible = (linea == "1");
        numLibros++;
    }
    archivoLibros.close();

    ifstream archivoUsuarios("usuarios.csv");
    while (getline(archivoUsuarios, linea) && numUsuarios < MAX_USUARIOS) {
        size_t pos = linea.find(",");
        usuarios[numUsuarios].nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);
        
        pos = linea.find(",");
        usuarios[numUsuarios].contrasena = linea.substr(0, pos);
        linea.erase(0, pos + 1);
        
        usuarios[numUsuarios].numLibrosPrestados = stoi(linea);
        numUsuarios++;
    }
    archivoUsuarios.close();

    ifstream archivoPrestamos("prestamos.csv");
    while (getline(archivoPrestamos, linea) && numPrestamos < MAX_PRESTAMOS) {
        size_t pos = linea.find(",");
        prestamos[numPrestamos].ISBN = linea.substr(0, pos);
        linea.erase(0, pos + 1);
        
        pos = linea.find(",");
        prestamos[numPrestamos].usuario = linea.substr(0, pos);
        linea.erase(0, pos + 1);
        
        pos = linea.find(",");
        prestamos[numPrestamos].fechaPrestamo = stol(linea.substr(0, pos));
        linea.erase(0, pos + 1);
        
        pos = linea.find(",");
        prestamos[numPrestamos].fechaDevolucion = stol(linea.substr(0, pos));
        linea.erase(0, pos + 1);
        
        prestamos[numPrestamos].devuelto = (linea == "1");
        numPrestamos++;
    }
    archivoPrestamos.close();
}

void cargarReservas() {
    ifstream archivoReservas("reservas.csv");
    string linea;
    while (getline(archivoReservas, linea) && numReservas < MAX_PRESTAMOS) {
        size_t pos = linea.find(",");
        reservas[numReservas].ISBN = linea.substr(0, pos);
        linea.erase(0, pos + 1);
        
        pos = linea.find(",");
        reservas[numReservas].usuario = linea.substr(0, pos);
        linea.erase(0, pos + 1);
        
        pos = linea.find(",");
        reservas[numReservas].fechaReserva = stol(linea.substr(0, pos));
        linea.erase(0, pos + 1);
        
        reservas[numReservas].activa = (linea == "1");
        numReservas++;
    }
    archivoReservas.close();
}

void guardarDatos() {
    ofstream archivoLibros("libros.csv");
    for (int i = 0; i < numLibros; i++) {
        archivoLibros << libros[i].titulo << ","
                     << libros[i].autor << ","
                     << libros[i].ISBN << ","
                     << libros[i].precio << ","
                     << (libros[i].disponible ? "1" : "0") << endl;
    }
    archivoLibros.close();

    ofstream archivoUsuarios("usuarios.csv");
    for (int i = 0; i < numUsuarios; i++) {
        archivoUsuarios << usuarios[i].nombre << ","
                       << usuarios[i].contrasena << ","
                       << usuarios[i].numLibrosPrestados << endl;
    }
    archivoUsuarios.close();

    ofstream archivoPrestamos("prestamos.csv");
    for (int i = 0; i < numPrestamos; i++) {
        archivoPrestamos << prestamos[i].ISBN << ","
                        << prestamos[i].usuario << ","
                        << prestamos[i].fechaPrestamo << ","
                        << prestamos[i].fechaDevolucion << ","
                        << (prestamos[i].devuelto ? "1" : "0") << endl;
    }
    archivoPrestamos.close();
}

void guardarReservas() {
    ofstream archivoReservas("reservas.csv");
    for (int i = 0; i < numReservas; i++) {
        archivoReservas << reservas[i].ISBN << ","
                       << reservas[i].usuario << ","
                       << reservas[i].fechaReserva << ","
                       << (reservas[i].activa ? "1" : "0") << endl;
    }
    archivoReservas.close();
}

void Ingreso() {
    string usuario, contrasena;
    cout << "Ingrese su usuario: ";
    getline(cin, usuario);
    cout << "Ingrese su contrasena: ";
    getline(cin, contrasena);

    bool encontrado = false;
    for (int i = 0; i < numUsuarios; i++) {
        if (usuarios[i].nombre == usuario && usuarios[i].contrasena == contrasena) {
            encontrado = true;
            cout << "Bienvenido " << usuario << endl;
            menuPrincipal(usuario);
            break;
        }
    }

    if (!encontrado) {
        cout << "Usuario o contrasena incorrectos" << endl;
    }
}

void crearUsuario() {
    if (numUsuarios >= MAX_USUARIOS) {
        cout << "Se excede el maximo de usuarios" << endl;
        return;
    }

    string usuario, contrasena;
    cout << "Ingrese su usuario: ";
    getline(cin, usuario);
    cout << "Ingrese su contrasena: ";
    getline(cin, contrasena);

    usuarios[numUsuarios].nombre = usuario;
    usuarios[numUsuarios].contrasena = contrasena;
    usuarios[numUsuarios].numLibrosPrestados = 0;
    numUsuarios++;

    cout << "Usuario creado exitosamente" << endl;
    guardarDatos();
}

void menuPrincipal(const string& usuario) {
    while (true) {
        cout << "\nMenu Principal" << endl;
        cout << "-------------" << endl;
        cout << "1. Gestion de Libros" << endl;
        cout << "2. Prestar Libro" << endl;
        cout << "3. Devolver Libro" << endl;
        cout << "4. Gestion de Reservas" << endl;
        cout << "5. Ver Multas" << endl;
        cout << "6. Generar Reportes" << endl;
        cout << "7. Cerrar Sesion" << endl;
        cout << "Ingrese una opcion: ";

        int opcion;
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                gestionLibros(); 
                break;
            case 2:
                prestarLibro(usuario); 
                break;
            case 3:
                devolverLibro(usuario); 
                break;
            case 4:
                gestionReservas(usuario); 
                break;
            case 5:
                calcularMultas(usuario); 
                break;
            case 6:
                generarReportes(); 
                break;
            case 7:
                return;
            default:
                cout << "Opcion no valida" << endl;
        }
    }
}

void gestionLibros() {
    while (true) {
        cout << "\nGestion de Libros" << endl;
        cout << "----------------" << endl;
        cout << "1. Agregar Libro" << endl;
        cout << "2. Buscar Libro" << endl;
        cout << "3. Editar Libro" << endl;
        cout << "4. Volver" << endl;
        cout << "Ingrese una opcion: ";

        int opcion;
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                agregarLibro(); 
                break;
            case 2:
                buscarLibro(); 
                break;
            case 3:
                editarLibro(); 
                break;
            case 4:
                return;
            default:
                cout << "Opcion no valida" << endl;
        }
    }
}

void gestionReservas(const string& usuario) {
    while (true) {
        cout << "\nGestion de Reservas" << endl;
        cout << "------------------" << endl;
        cout << "1. Hacer Nueva Reserva" << endl;
        cout << "2. Cancelar Reserva" << endl;
        cout << "3. Consultar Mis Reservas" << endl;
        cout << "4. Volver" << endl;
        cout << "Ingrese una opcion: ";

        int opcion;
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                reservarLibro(usuario); 
                break;
            case 2:
                cancelarReserva(usuario); 
                break;
            case 3:
                consultarReservas(usuario); 
                break;
            case 4:
                return;
            default:
                cout << "Opcion no valida" << endl;
        }
    }
}

void agregarLibro() {
    if (numLibros >= MAX_LIBROS) {
        cout << "Se excede el maximo de libros" << endl;
        return;
    }

    cout << "-> Ingrese titulo: ";
    getline(cin, libros[numLibros].titulo);
    cout << "-> Ingrese autor: ";
    getline(cin, libros[numLibros].autor);
    cout << "-> Ingrese ISBN: ";
    getline(cin, libros[numLibros].ISBN);
    cout << "-> Ingrese precio: ";
    cin >> libros[numLibros].precio;
    libros[numLibros].disponible = true;
    numLibros++;

    cout << "Libro agregado exitosamente" << endl;
    guardarDatos();
}

void buscarLibro() {
    string busqueda;
    cout << "Ingrese titulo, autor o ISBN a buscar: ";
    getline(cin, busqueda);

    bool encontrado = false;
    for (int i = 0; i < numLibros; i++) {
        if (libros[i].titulo.find(busqueda) != string::npos ||
            libros[i].autor.find(busqueda) != string::npos ||
            libros[i].ISBN.find(busqueda) != string::npos) {
            
            cout << "\nLibro encontrado:" << endl;
            cout << "Titulo: " << libros[i].titulo << endl;
            cout << "Autor: " << libros[i].autor << endl;
            cout << "ISBN: " << libros[i].ISBN << endl;
            cout << "Precio: $" << fixed << setprecision(2) << libros[i].precio << endl;
            cout << "Estado: " << (libros[i].disponible ? "Disponible" : "No disponible") << endl;
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron libros con ese criterio" << endl;
    }
}

void editarLibro() {
    string ISBN;
    cout << "Ingrese el ISBN del libro a editar: ";
    getline(cin, ISBN);

    for (int i = 0; i < numLibros; i++) {
        if (libros[i].ISBN == ISBN) {
            cout << "Ingrese nuevo titulo (Enter para mantener actual): ";
            string nuevo;
            getline(cin, nuevo);
            if (!nuevo.empty()) libros[i].titulo = nuevo;

            cout << "Ingrese nuevo autor (Enter para mantener actual): ";
            getline(cin, nuevo);
            if (!nuevo.empty()) libros[i].autor = nuevo;

            cout << "Ingrese nuevo precio (0 para mantener actual): ";
            double nuevoPrecio;
            cin >> nuevoPrecio;
            if (nuevoPrecio > 0) libros[i].precio = nuevoPrecio;

            cout << "Libro actualizado exitosamente" << endl;
            guardarDatos();
            return;
        }
    }

    cout << "Libro no encontrado" << endl;
}

void reservarLibro(const string& usuario) {
    string ISBN;
    cout << "Ingrese el ISBN del libro a reservar: ";
    getline(cin, ISBN);

    bool libroExiste = false;
    for (int i = 0; i < numLibros; i++) {
        if (libros[i].ISBN == ISBN) {
            libroExiste = true;
            if (libros[i].disponible) {
                cout << "Este libro esta disponible. ¿Desea prestarlo en vez de reservarlo? (s/n): ";
                char respuesta;
                cin >> respuesta;
                cin.ignore();
                if (respuesta == 's' || respuesta == 'S') {
                    prestarLibro(usuario);
                    return;
                }
            }
            break;
        }
    }

    if (!libroExiste) {
        cout << "El libro no existe en el sistema." << endl;
        return;
    }

    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].ISBN == ISBN && reservas[i].usuario == usuario && reservas[i].activa) {
            cout << "Ya tienes una reserva activa para este libro." << endl;
            return;
        }
    }

    if (numReservas >= MAX_PRESTAMOS) {
        cout << "No se pueden hacer mas reservas en el sistema." << endl;
        return;
    }

    reservas[numReservas].ISBN = ISBN;
    reservas[numReservas].usuario = usuario;
    reservas[numReservas].fechaReserva = time(nullptr);
    reservas[numReservas].activa = true;
    numReservas++;

    cout << "Libro reservado exitosamente." << endl;
    guardarReservas();
}

void cancelarReserva(const string& usuario) {
    string ISBN;
    cout << "Ingrese el ISBN del libro cuya reserva desea cancelar: ";
    getline(cin, ISBN);

    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].ISBN == ISBN && reservas[i].usuario == usuario && reservas[i].activa) {
            reservas[i].activa = false;
            cout << "Reserva cancelada exitosamente." << endl;
            guardarReservas();
            return;
        }
    }

    cout << "No se encontro una reserva activa para este libro." << endl;
}

void consultarReservas(const string& usuario) {
    bool tieneReservas = false;
    cout << "\nTus reservas activas:" << endl;
    cout << "--------------------" << endl;

    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].usuario == usuario && reservas[i].activa) {
            tieneReservas = true;
            for (int j = 0; j < numLibros; j++) {
                if (libros[j].ISBN == reservas[i].ISBN) {
                    cout << "Titulo: " << libros[j].titulo << endl;
                    cout << "ISBN: " << libros[j].ISBN << endl;
                    cout << "Fecha de reserva: " << ctime(&reservas[i].fechaReserva);
                    cout << "--------------------" << endl;
                    break;
                }
            }
        }
    }

    if (!tieneReservas) {
        cout << "No tienes reservas activas." << endl;
    }
}

void prestarLibro(const string& usuario) {
    string ISBN;
    cout << "Ingrese el ISBN del libro a prestar: ";
    getline(cin, ISBN);

    for (int i = 0; i < numLibros; i++) {
        if (libros[i].ISBN == ISBN && libros[i].disponible) {
            libros[i].disponible = false;
            
            prestamos[numPrestamos].ISBN = ISBN;
            prestamos[numPrestamos].usuario = usuario;
            prestamos[numPrestamos].fechaPrestamo = time(nullptr);
            prestamos[numPrestamos].fechaDevolucion = prestamos[numPrestamos].fechaPrestamo + (14 * 24 * 60 * 60);
            prestamos[numPrestamos].devuelto = false;
            numPrestamos++;

            cout << "Libro prestado exitosamente" << endl;
            cout << "Fecha de devolucion: " << ctime(&prestamos[numPrestamos-1].fechaDevolucion);
            guardarDatos();
            return;
        }
    }

    cout << "Libro no disponible o no encontrado" << endl;
}

void devolverLibro(const string& usuario) {
    string ISBN;
    cout << "Ingrese el ISBN del libro a devolver: ";
    getline(cin, ISBN);

    for (int i = 0; i < numPrestamos; i++) {
        if (prestamos[i].ISBN == ISBN && prestamos[i].usuario == usuario && !prestamos[i].devuelto) {
            prestamos[i].devuelto = true;
            time_t ahora = time(nullptr);

            if (ahora > prestamos[i].fechaDevolucion) {
                int diasRetraso = (ahora - prestamos[i].fechaDevolucion) / (24 * 60 * 60);
                double multa = diasRetraso * 1.0;
                cout << "Multa por retraso: $" << fixed << setprecision(2) << multa << endl;
            }

            bool hayReserva = false;
            string usuarioReserva;
            time_t fechaReservaMasAntigua = time(nullptr);
            int indiceReservaMasAntigua = -1;

            for (int j = 0; j < numReservas; j++) {
                if (reservas[j].ISBN == ISBN && reservas[j].activa) {
                    if (reservas[j].fechaReserva < fechaReservaMasAntigua) {
                        fechaReservaMasAntigua = reservas[j].fechaReserva;
                        usuarioReserva = reservas[j].usuario;
                        indiceReservaMasAntigua = j;
                        hayReserva = true;
                    }
                }
            }

            if (hayReserva) {
                cout << "\nHay una reserva pendiente para este libro." << endl;
                cout << "Usuario con reserva: " << usuarioReserva << endl;
                reservas[indiceReservaMasAntigua].activa = false;
            } else {
                for (int j = 0; j < numLibros; j++) {
                    if (libros[j].ISBN == ISBN) {
                        libros[j].disponible = true;
                        break;
                    }
                }
            }

            cout << "Libro devuelto exitosamente" << endl;
            guardarDatos();
            guardarReservas();
            return;
        }
    }

    cout << "No se encontro el prestamo" << endl;
}

void calcularMultas(const string& usuario) {
    double totalMultas = 0;
    time_t ahora = time(nullptr);
    bool tieneMultas = false;

    cout << "\nMultas pendientes:" << endl;
    for (int i = 0; i < numPrestamos; i++) {
        if (prestamos[i].usuario == usuario && !prestamos[i].devuelto && ahora > prestamos[i].fechaDevolucion) {
            int diasRetraso = (ahora - prestamos[i].fechaDevolucion) / (24 * 60 * 60);
            double multa = diasRetraso * 1.0;
            totalMultas += multa;
            cout << "ISBN: " << prestamos[i].ISBN << " - Multa: $" << fixed << setprecision(2) << multa << endl;
            tieneMultas = true;
        }
    }

    if (!tieneMultas) {
        cout << "No tiene multas pendientes" << endl;
    } else {
        cout << "Total de multas pendientes: $" << fixed << setprecision(2) << totalMultas << endl;
    }
}

void generarReportes() {
    cout << "\nGenerando Reportes..." << endl;

    int prestamosPorLibro[MAX_LIBROS] = {0};
    double totalMultas = 0;

    for (int i = 0; i < numPrestamos; i++) {
        for (int j = 0; j < numLibros; j++) {
            if (prestamos[i].ISBN == libros[j].ISBN) {
                prestamosPorLibro[j]++;
                if (prestamos[i].devuelto) {
                    time_t fechaDevolucionReal = time(nullptr);
                    if (fechaDevolucionReal > prestamos[i].fechaDevolucion) {
                        int diasRetraso = (fechaDevolucionReal - prestamos[i].fechaDevolucion) / (24 * 60 * 60);
                        totalMultas += diasRetraso * 1.0;
                    }
                }
                break;
            }
        }
    }

    cout << "\nLibros mas prestados:" << endl;
    for (int i = 0; i < numLibros; i++) {
        if (prestamosPorLibro[i] > 0) {
            cout << libros[i].titulo << ": " << prestamosPorLibro[i] << " veces" << endl;
        }
    }

    cout << "\nTotal de ingresos por multas: $" << fixed << setprecision(2) << totalMultas << endl;
}