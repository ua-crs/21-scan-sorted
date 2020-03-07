/*
 *  Project 21-scan-sorted - main.cpp
 *      Este programa muestra como determinar (scan = barrer) redes
 *      WiFi disponibles
 *      Esta basado en 20-wifi-scan.ino pero ordena SSID por orden descendente de RSSI
 *          usando la funcion qsort
 */

/*
 *  Includes
 */

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

/*
 *  Constantes defiidas en platformio.ini
 *     NO_ENCR      Constante para NoEncryption
 *     SERIAL_BAUD  Baud rate del port serie
 */

/*
 *  indices: Guarda los indices de las redes descubiertas por la biblioteca
 *  Este es el arreglo que se ordena en froma decreciente al RSSI de la red
 *  apuntada por su contenido
 */

static int indices[127];

/*
 *  cmpind
 *      CoMPare INDirect
 *      Compara los RSSI almacenados en la funcion de biblioteca
 *          de forma descendente
 */

static int
cmpind( const void *a, const void *b )
{
    return WiFi.RSSI( *(int *)b) - WiFi.RSSI( *(int *)a);
}

/*
 *  print_network:
 *      Funcion para imprimir los datos de una red, la apuntada por 'i'
 */

static void
print_network( int i )
{
    Serial.print("\t");
    Serial.print(WiFi.RSSI(i));     //  Intensidad de señal
    Serial.print( ": ");
    Serial.print(WiFi.SSID(i));     //  Nombre de la red
    Serial.println((WiFi.encryptionType(i) == NO_ENCR) ? " " : "*");
}

/*
 *  sort_display_networks:
 *      Ordena las redes encontradas en forma descendente de RSSI
 *      y las lista en pantalle en ese orden
 */

static void
sort_display_networks( int n )
{
    int i, j;

    for ( i = 0; i < n; ++i)
        indices[i] = i;                 //  generando un arreglo de indices crecientes (apuntan a los indices del arreglo de la biblioteca

    qsort( indices, n, sizeof(int), cmpind );   // Se ordena el arreglo 'indices' basado en el valor de RSSI apuntado por el contenido

    Serial.println( "------ Imprimiendo en orden decreciente de RSSI ------" );
    Serial.println("");

    for( j = 0; j < n; ++j )            //  barriendo indices
    {
        i = indices[j];                 //  apuntando a la posicion del arreglo original
        print_network(i);
    }

}

void
setup( void )
{
    Serial.begin(SERIAL_BAUD);
    delay(100);
    Serial.println("Setup done");
}


void
loop( void )
{
    int n;

    Serial.println("");
    Serial.println("---- Comienzo de barrido ----");

    if( ( n = WiFi.scanNetworks() ) == 0 )  //  retorna la cantidad de redes descubiertas
        Serial.println("No se encontraron redes disponibles");
    else
    {
        Serial.print(n);
        Serial.println(" redes encontradas");
        Serial.println("---- Barrido terminado ----");
        Serial.println("");

        sort_display_networks(n);
    }

    delay(3000);
}
