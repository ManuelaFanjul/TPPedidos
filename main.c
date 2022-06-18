#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

const char clave[20]="pedorro";
///#define PASSWORD "pedorro";
#define ARCHIVO_PEDIDOS "pedidos.dat"
#define ARCHIVO_PRODUCTOS "productos.dat"
#define MAX_PRODUCTOS 8

///ESTRUCTURAS

typedef struct
{
    int idCliente;                 /// AUTO INCREMENTAL
    char nombre[30];
    char apellido[30];
    char userName[20];
    char password[20];
    char mail[30];
    char genero;
    int rol;                      /// 1 ADMIN - 0 USUARIO CLIENTE
    int activo;                   /// 1 ACTIVO - 0 INACTIVO
    char preguntaClave[20];
    char AoC;
} stCliente;

typedef struct
{
    int idPedido;                    /// AUTO INCREMENTAL
    int idCliente;
    int idProducto[MAX_PRODUCTOS];
    int validosProducto;
    int cantProducto[MAX_PRODUCTOS];
    char fecha[12];                 /// DD/MM/AAAA
    char descripcion[300];
    float precio;
    int pedidoAnulado;              /// 1 ANULADO - 0 NO ANULADO
} stPedido;

typedef struct
{
    int idProducto;
    char nombre[20];
    char categoria[20];
    float precio;
    char descripcion[100];
    int activo;                     /// 1 ACTIVO - 0 INACTIVO
} stProducto;

/// FUNCIONES CLIENTE

    void registrarCliente();
    stCliente iniciarSesion();
    void guardarClienteEnArchivo(stCliente* cliente);
    void leerClientesDeArchivo();                       /// Funcion para probar que se guarden los usuarios
    void mostrarCliente(stCliente cliente);             /// Funcion para probar que se guarden los usuarios
    int existeCliente(char userName[20]);
    void bajaDeCliente(stCliente* clienteActual);

/// FUNCIONES PRODUCTO

    int crearProducto();
    int guardarProductoEnArchivo(stProducto* producto);
    void mostrarProducto (stProducto producto);
    void listarProductos ();
    int eliminarProductoPorId (int idProducto);

/// FUNCIONES PEDIDO

    void crearPedido();
    int guardarPedidoEnArchivo(stPedido* pedido);
    void mostrarPedido (stPedido pedido);
    void listarPedidos ();
    int eliminarPedidoPorId (int idPedido);

/// MAIN

    int main()
    {

        char exit='0';
        while(exit!='127')
        {
            menu();
        }
        stCliente clienteActual;
        registrarCliente();
        leerClientesDeArchivo();

        clienteActual=iniciarSesion();
        mostrarCliente(clienteActual);
        bajaDeCliente(&clienteActual);
        mostrarCliente(clienteActual);
        return 0;
    }

    void menu()
    {
        int opcion;
        system("cls");
        printf("---------------- Bienvenido al programa ----------------\n\n");
        printf("|\n|\n|\n|  1- Iniciar Sesion                                 |\n");
        printf("|\n|\n|\n|  2- Registrar Usuario                              |\n");
        printf("---------------------------------------------------------\n\n");

        printf("Ingrese una opcion: ");
        scanf("%d",&opcion);

        if(opcion==2)
        {
            registrarCliente();
        }

        if (opcion==1)
        {
            iniciarSesion();
        }

    }

    void registrarCliente()
    {
        ///Aca pido los datos por consola
        stCliente cliente;
        printf("Ingrese su nombre: \n");
        fflush(stdin);
        gets(cliente.nombre);

        printf("Ingrese su apellido: \n");
        fflush(stdin);
        gets(cliente.apellido);

        int check=0;
        do
        {
            printf("Ingrese su nombre de usuario: \n");
            fflush(stdin);
            gets(cliente.userName);

            check=existeCliente(cliente.userName)==1;

            if(check==1)
            {
                printf("Ya hay un cliente registrado con ese nombre de usuario. Pruebe con un nombre distinto.\n");
            }
        }
        while(check==1);

        printf("Ingrese su contraseña: \n");
        fflush(stdin);
        gets(cliente.password);

        printf("Ingrese su genero (m/f): \n");
        fflush(stdin);
        scanf("%c",&cliente.genero);

        puts("Pregunta clave (obligatoria por si en algun momento cambia la contraseña)\nCual era su apodo de chico");
        fflush(stdin);
        gets(cliente.preguntaClave);

        do
        {
            puts("Eres admin (escribir = a) o un cliente (escribir = c)?");
            fflush(stdin);
            cliente.AoC=getch();

            printf("\nhola: %c\n", cliente.AoC);
            int  j=3,i=3;
            char contra[20];
            int resultado=0;
            if(cliente.AoC == 'a')
            {

                do
                {
                    puts("Escribe la contraseña de los admins");
                    fflush(stdin);
                    gets(contra);
                    if (strcmp(contra,clave)==0)
                    {
                        puts ("Contraseña correcta, ahora eres un admin");
                        resultado=1;
                        j=0;
                    }
                    else
                    {
                        i--;
                        printf("Contraseña incorrecta, quedan %d intentos.\nintente de nuevo\n",i);
                    }

                }
                while (i>0 && resultado==0);

                if (i==0)
                {
                    puts("Usted no se pudo registrar como admin");
                    menu();
                }
            }
        }
        while(cliente.AoC!='c' && cliente.AoC!='a');

        cliente.activo=1;
        cliente.rol=0;

        guardarClienteEnArchivo(&cliente);
    }

    void guardarClienteEnArchivo(stCliente* cliente)
    {
        FILE* aClientes= fopen("clientes.dat","wb");

        ///Esto me da un poco de dudas si es la mejor manera de hacerlo.
        fseek(aClientes,0, SEEK_END);
        int id=ftell(aClientes)/sizeof(stCliente);
        cliente->idCliente=id;

        if(aClientes!=NULL)
        {
            if(fwrite(cliente,sizeof(stCliente),1,aClientes) == 1)
            {
                printf("El usuario se ha registrado correctamente.");
            }
            else
            {
                printf("Error al registrar el usuario.");
            }
        }

        fclose(aClientes);
    }

    stCliente iniciarSesion()
    {
        char userName[20];
        char password[20];
        int resultado=0;

        printf("Ingrese su nombre de usuario: \n");
        fflush(stdin);
        gets(userName);

        printf("Ingrese su contraseña: \n");
        fflush(stdin);
        gets(password);

        FILE* aClientes= fopen("clientes.dat","rb");
        stCliente cliente;
        while(resultado==0 && fread(&cliente, sizeof(stCliente), 1, aClientes))
        {
            if(strcmp(userName,cliente.userName)==0 && strcmp(password,cliente.password)==0 && cliente.activo==1)
            {

                printf("\n----------Bienvenido otra vez,%s!----------\n",cliente.nombre);
                resultado=1;
            }
        }
        if(resultado==0)
        {
            printf("Usuario y/o contraseña incorrectos.");
        }

        fclose(aClientes);

        return cliente;
    }

    void leerClientesDeArchivo()
    {
        FILE* aClientes= fopen("clientes.dat","rb");
        stCliente cliente;
        while(fread(&cliente, sizeof(stCliente), 1, aClientes))
        {
            mostrarCliente(cliente);
        }

        fclose(aClientes);
    }

    void mostrarCliente(stCliente cliente)
    {
        printf("\n--------------------------------------\n");
        printf("ID: %d\n",cliente.idCliente);
        printf("Nombre: %s\n",cliente.nombre);
        printf("Apellido: %s\n",cliente.apellido);
        printf("Nombre de usuario: %s\n",cliente.userName);
        printf("Genero: %c\n",cliente.genero);
        printf("Activo: %d", cliente.activo);
        printf("Palabra clave: %s\n",cliente.preguntaClave);
        printf("Rol: %c\n", cliente.AoC);
        printf("\n--------------------------------------\n");
    }

    int existeCliente(char userName[20])
    {
        FILE* aClientes= fopen("clientes.dat","rb");
        stCliente aux;
        int resultado=0;
        while(resultado==0 && fread(&aux, sizeof(stCliente), 1, aClientes))
        {
            if(strcmp(userName,aux.userName)==0)
            {
                resultado=1;
            }
        }

        fclose(aClientes);

        return resultado;
    }

    void bajaDeCliente(stCliente* clienteActual)
    {

        FILE* aClientes= fopen("clientes.dat","r+b");

        stCliente aux;
        int resultado=0;

        while(resultado==0 && fread(&aux, sizeof(stCliente), 1, aClientes)>0)
        {

            if(clienteActual->idCliente==aux.idCliente)
            {
                resultado=1;
            }
        }

        clienteActual->activo=0;
        fseek(aClientes,sizeof(stCliente)*-1,SEEK_CUR);

        if(fwrite(clienteActual,sizeof(stCliente),1,aClientes) == 1)
        {
            printf("El usuario se ha dado de baja correctamente.");
        }
        else
        {
            printf("Error al dar de baja el usuario.");
        }

        fclose(aClientes);

        return aux;
    }

    int validarAdmin()
    {
        int admin;
        do
        {

            puts("¿Desea registrarse como un administrador?\n 1: Si\n 0: No ");
            scanf("%d", &admin);

        }
        while(admin!=1 && admin!=0);

        int i=3;
        char contra[20];
        puts("Ingrese la clave de administrador: ");
        scanf("%d", &admin);

        if(admin==1)
        {

            while (i>0 && strcmp(contra,clave)!=0)
            {
                printf("Contraseña incorrecta, quedan %d intentos.\nintente de nuevo\n",i);
                i--;
            }
            if(i>0)
            {
                puts ("Contraseña correcta, ahora eres un admin");
            }
            else
            {
                puts("Usted no se pudo registrar como admin");
                admin=0;
                menu();
            }


        }

        return admin;
    }

    void crearPedido()
    {
        char control = 'S';
        int i = 0;

        stPedido pedido;
        printf("Ingrese fecha: \n");
        fflush(stdin);
        gets(pedido.fecha);

        while (i < MAX_PRODUCTOS && control == 's' || control == 'S')
        {

            printf("Ingrese el ID del producto: \n");
            listarProductos();
            scanf("%d",&pedido.idProducto[i]);

            system ("cls");

            printf("Ingrese cantidad: \n");
            scanf ("%d", &pedido.cantProducto[i]);

            system ("cls");

            puts ("Desea agregar otro producto? S/N\n");
            fflush (stdin);
            scanf ("%c", &control);
            i++;
        }

        pedido.precio = 15.0; /// FALTA CALCULAR PRECIO

        pedido.pedidoAnulado = 0;

        printf("Ingrese descripcion: \n");
        fflush(stdin);
        gets(pedido.descripcion);

        guardarPedidoEnArchivo(&pedido);
    }

    int guardarPedidoEnArchivo(stPedido* pedido)
    {
        int resultado = 0;
        FILE* aPedidos = fopen(ARCHIVO_PEDIDOS,"wb");
        int id = ftell(aPedidos)/sizeof(stPedido);
        pedido->idPedido = id;

        if(aPedidos!=NULL)
        {
            if(fwrite(pedido,sizeof(stPedido),1,aPedidos) == 1)
            {
                printf("El pedido se ha realizado correctamente.");
                resultado = 1;
            }
            else
            {
                printf("Error al realizar el pedido.");
            }
            fclose(aPedidos);
        }
        return resultado;
    }

    void mostrarPedido(stPedido pedido)
    {
        printf("\n--------------------------------------\n");
        printf("ID: %d\n",pedido.idPedido);
        printf("Fecha: %s\n",pedido.fecha);
        printf("Descripcion: %s\n",pedido.descripcion);
        printf("Producto: %d", pedido.idProducto);
        printf("Precio: %f\n",pedido.precio);
        printf("\n--------------------------------------\n");
    }

    void listarPedidos ()
    {
        FILE* aPedidos = fopen(ARCHIVO_PEDIDOS,"r+b");

        if(aPedidos!=NULL)
        {
            stPedido aux;

            while(fread(&aux, sizeof(stPedido), 1, aPedidos)>0)
            {
                mostrarPedido (aux);
            }
        }
    }

    int eliminarPedidoPorId (int idPedido)
    {
        int resultado = 0;

        FILE* aPedidos = fopen(ARCHIVO_PEDIDOS,"r+b");

        if(aPedidos!=NULL)
        {
            stPedido aux;

            while(resultado==0 && fread(&aux, sizeof(stPedido), 1, aPedidos)>0)
            {
                if(idPedido == aux.idPedido)
                {
                    resultado = 1;
                }
            }

            aux.pedidoAnulado = 1;

            fseek(aPedidos,sizeof(stPedido)*-1,SEEK_CUR);

            if(fwrite(&aux,sizeof(stPedido),1,aPedidos) == 1)
            {
                printf("El pedido se ha anulado correctamente.");
            }
            else
            {
                printf("Error al anular el pedido.");
            }

            fclose(aPedidos);
        }

        return resultado;
    }

     int crearProducto()
    {
        stProducto producto;

        printf("Ingrese nombre del producto: \n");
        fflush(stdin);
        gets(producto.nombre);

        printf("Ingrese la categoria del producto: \n");
        fflush(stdin);
        gets(producto.categoria);

        printf("Ingrese el precio del producto: \n");
        scanf("%f",&producto.precio);

        printf("Ingrese una descripcion del producto (opcional): \n");
        fflush(stdin);
        gets(producto.descripcion);

        producto.activo=1;

        return guardarProductoEnArchivo(&producto);
    }



     int guardarProductoEnArchivo(stProducto* producto)
    {
        int resultado = 0;
        FILE* aProductos = fopen(ARCHIVO_PRODUCTOS,"wb");
        int id = ftell(aProductos)/sizeof(stProducto);
        producto->idProducto = id;

        if(aProductos!=NULL)
        {
            if(fwrite(producto,sizeof(stProducto),1,aProductos) == 1)
            {
                printf("El producto se ha realizado correctamente.");
                resultado = 1;
            }
            else
            {
                printf("Error al realizar el producto.");
            }
            fclose(aProductos);
        }
        return resultado;
    }

    void mostrarProducto (stProducto producto)
    {
        printf("\n--------------------------------------\n");
        printf("ID: %d\n",producto.idProducto);
        printf("Nombre: %s\n",producto.nombre);
        printf("Categoria: %d", producto.categoria);
        printf("Descripcion: %s\n",producto.descripcion);
        printf("Precio: %f\n",producto.precio);
        printf("\n--------------------------------------\n");
    }

    void listarProductos ()
    {
        FILE* aProductos = fopen(ARCHIVO_PRODUCTOS,"r+b");

        if(aProductos!=NULL)
        {
            stProducto aux;

            while(fread(&aux, sizeof(stProducto), 1, aProductos)>0)
            {
                mostrarProducto(aux);
            }
        }
    }

    int eliminarProductoPorId (int idProducto)
    {
        int resultado = 0;

        FILE* aProductos = fopen(ARCHIVO_PRODUCTOS,"r+b");

        if(aProductos!=NULL)
        {
            stProducto aux;

            while(resultado==0 && fread(&aux, sizeof(stProducto), 1, aProductos)>0)
            {
                if(idProducto == aux.idProducto)
                {
                    resultado = 1;
                }
            }

            aux.activo = 1;

            fseek(aProductos,sizeof(stProducto)*-1,SEEK_CUR);

            if(fwrite(&aux,sizeof(stProducto),1,aProductos) == 1)
            {
                printf("El producto se ha anulado correctamente.");
            }
            else
            {
                printf("Error al anular el producto.");
            }

            fclose(aProductos);
        }
        return resultado;
    }


