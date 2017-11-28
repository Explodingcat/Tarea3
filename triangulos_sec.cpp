#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;
float calcula_perimetro(float coordenadas[])
{
	float perimetro, resta1,resta2;
	resta1=coordenadas[0]-coordenadas[2];
	resta2=coordenadas[1]-coordenadas[3];
	perimetro= sqrt((pow(resta1,2))+(pow(resta2,2)));
	resta1=coordenadas[0]-coordenadas[4];
	resta2=coordenadas[1]-coordenadas[5];
	perimetro= sqrt((pow(resta1,2))+(pow(resta2,2)))+perimetro;
	resta1=coordenadas[2]-coordenadas[4];
	resta2=coordenadas[3]-coordenadas[5];
	perimetro= sqrt((pow(resta1,2))+(pow(resta2,2)))+perimetro;
	return perimetro;
}

float leer_puntos(int arpuntos[], char* punto){
	ifstream puntos;
	puntos.open(punto);
	string linea;
	float coordenadas[6];
	int aux, m=0,k,i,cont=0;
	while(!puntos.eof() || m!=3)
	{
			getline(puntos,linea);
			istringstream iss(linea);
			iss>>aux;
			if(aux==arpuntos[m])
			{
				//cout<<aux<<endl;

				for(i=0; i<2;i++)
				{
					iss>>coordenadas[cont];
					//cout<<coordenadas[cont]<<endl;
					cont++;
				}
				m++;
			}
	}	
	puntos.close();
	return calcula_perimetro(coordenadas);
	//cout<<"PERIMETRO: "<<perimetro<<endl;
}

void ordenar_puntos(int arpuntos[])
{
	int temp;
	   for (int i=1; i<3; i++)
	   {
         	for (int j=0 ; j<3 - 1; j++)
		   	{

             if (arpuntos[j] > arpuntos[j+1])
             {
                temp = arpuntos[j];
                  arpuntos[j] = arpuntos[j+1];
                   arpuntos[j+1] = temp;
               }
           }
	   }
}

float leer_triangulo(char* tria, char* punto){
	int arpuntos[3];
	string linea;
	float perimetro=0;
	ifstream triangulo;
	triangulo.open(tria);
	while(!triangulo.eof())
	{
		getline(triangulo,linea);
		istringstream iss(linea);
		for(int i=0;i<3;i++)
		{
			iss>>arpuntos[i];
		}
		ordenar_puntos(arpuntos);
		perimetro=leer_puntos(arpuntos,punto)+perimetro;

	}	
	triangulo.close();
	return perimetro;
}

int main(int argc, char* argv[])
{
	float perimetro;
	perimetro=leer_triangulo(argv[1],argv[2]);	
	cout<<"LA SUMA DE PERIMETRO ES: "<<perimetro<<endl;

}