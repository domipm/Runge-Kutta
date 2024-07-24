
#include<cmath>
#include<cstdio>
#include<iostream>
#include<chrono>

#define MAX 100

//? CONSTANTES UNIVERSALES
#define PI 3.14159265358979323846
#define G 6.674e-11

//? CONSTANTES FÍSICAS
#define MT 5.9736e24 //MASA TIERRA (Kg)
#define ML 0.07349e24 //MASA LUNA (Kg)

#define RT 6.378160e6 //RADIO TIERRA (m)
#define RL 1.7374e6 //RADIO LUNA (m)

#define DTL 3.844e8 //RADIO ORBITA TIERRA-LUNA (m)
#define OMEGA 2.6617e-6 //PERIODO ORBITA LUNA (s)

#define DELTA 7.01474e-12 //PARÁMETRO DELTA
#define MU 0.0123025 //PARÁMETRO MU

//? PARÁMETROS DE LA SIMULACIÓN
#define ITER 5e5 //ITERACIONES A REALIZAR
#define H 1

//? VARIABLES
float k1[MAX], k2[MAX], k3[MAX], k4[MAX]; //VECTORES k_n^i UN VALOR PARA CADA ECUACION DIFERENCIAL 
float y[MAX]; //VECTOR y(t) CON POSICIONES Y MOMENTOS DEL COHETE

bool paso = 0; //COMPROBAR SI EL COHETE HA PASADO YA POR LA POSICIÓN X DE LA LUNA

using namespace std;

//? ECUACIONES DIFERENCIALES (n es la ecuación diferencial)
float f(int n, float r, float phi, float pr, float pphi, float t) {

    float rprima = sqrt(1+r*r-2*r*cos(phi-OMEGA*t));

    if (n == 1) return pr;
    if (n == 2) return pphi/(r*r);
    if (n == 3) return (pphi*pphi) / (r*r*r) - DELTA * ( 1.0 / (r*r) + ( MU / (rprima*rprima*rprima) ) * ( r - cos(phi - OMEGA * t) ) );
    if (n == 4) return - ( DELTA * MU * r / (rprima*rprima*rprima) ) * sin(phi - OMEGA * t);

    return 0;

}

int main() {

    //? VALORES INICIALES
    float theta = PI/3.387; //ÁNGULO VELOCIDAD (Con respecto al eje x)
    float v = 11200.0/DTL; //VELOCIDAD LANZAMIENTO COHETE (En m/s)

    //* ABIRMOS FICHERO DE SALIDA
    FILE *out;
    out = fopen("data.dat", "w");

    //* ALMACENAMOS POSICIONES Y MOMENTOS INICIALES EN EL VECTOR y(t) = {Radio, ángulo, momento radial, momento angular}
    y[1] = RT/DTL;
    y[2] = PI/2.0;
    y[3] = v*cos(theta-y[2]);
    y[4] = y[1]*v*sin(theta-y[2]);

    fprintf(out, "%f\t%f\t%f\t%f\t%f\n", 1.0, 0.0, y[1]*cos(y[2]), y[1]*sin(y[2]), 0.0); //ESCRIBIMOS EN EL FICHERO EL INSTANTE INICIAL

    //! BUCLE PRINCIPAL
    for (int k = 1; k < ITER; k++) {

        //BUCLE SOBRE CADA UNA DE LAS ECUACIONES DIFERENCIALES
        for (int n = 1; n <= 4; n++) k1[n] = H*f(n, y[1],           y[2],           y[3],           y[4],           k*H);
        for (int n = 1; n <= 4; n++) k2[n] = H*f(n, y[1]+k1[1]/2.0, y[2]+k1[2]/2.0, y[3]+k1[3]/2.0, y[4]+k1[4]/2.0, k*H+H/2.0);
        for (int n = 1; n <= 4; n++) k3[n] = H*f(n, y[1]+k2[1]/2.0, y[2]+k2[2]/2.0, y[3]+k2[3]/2.0, y[4]+k2[4]/2.0, k*H+H/2.0);
        for (int n = 1; n <= 4; n++) k4[n] = H*f(n, y[1]+k3[1],     y[2]+k3[2],     y[3]+k3[3],     y[4]+k3[4],     k*H+H);

        //ACTUALIZAMOS y[n] PARA t = t+h
        for (int n = 1; n <= 4; n++) y[n] += 1.0/6.0 * (k1[n]+2*k2[n]+2*k3[n]+k4[n]); 

        //ESCRIBIMOS POSICIÓN NUEVA LUNA, COHETE, Y TIEMPO
        fprintf(out, "%f\t%f\t%f\t%f\t%f\n", cos(OMEGA*k*H), sin(OMEGA*k*H), y[1]*cos(y[2]), y[1]*sin(y[2]), float(k*H)); //ESCRIBIMOS EN EL FICHERO LAS POSICIONES NUEVAS

        //COMPROBAMOS EL ÁNGULO DEL COHETE Y DE LA LUNA
        if (y[1]*cos(y[2]) >= cos(OMEGA*k*H) && paso == 0 ) {
            cout << "Lunar angle: " << atan(sin(OMEGA*k*H)/cos(OMEGA*k*H)) << endl;
            cout << "Rocket angle: " << y[2] << endl;
            cout << "Minimum distance rocket-moon: " << sqrt(1+y[1]*y[1]-2*y[1]*cos(y[2]-OMEGA*H*k))*DTL/1000 << " km" << endl;
            paso = 1;
        }

    }
    
    fclose(out);

    return 0;

}