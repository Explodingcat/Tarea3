#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <mpi.h>

using namespace std;

float calcula_perimetro(float coordenadas[])
{
	float perimetro=0, resta1,resta2;
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
	while(!puntos.eof() && m!=3)
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

float leer_triangulo(char* tria, char* punto, int numeros[]){
	int arpuntos[3];
	string linea;
	float perimetro=0;
	ifstream triangulo;
	triangulo.open(tria);
	triangulo.seekg(numeros[0],ios::cur);
	while(!triangulo.eof() && triangulo.tellg()<=numeros[1])
	{
		getline(triangulo,linea);
		istringstream iss(linea);
		if(linea.length()>11)
		{
			for(int i=0;i<3;i++)
			{
				iss>>arpuntos[i];
			}
			ordenar_puntos(arpuntos);
			perimetro=leer_puntos(arpuntos,punto)+perimetro;

	    }
	}	
	return perimetro;
	triangulo.close();
}

int main(int argc, char* argv[])
{
	int         status, rank_actual, tamano_de_procesadores,pos,div,cont,otro,resta,aux,sum=0,numeros[2];
    MPI_Status  rec_stat;  
    int         fuente, destino, numcero[2];   
    MPI_Init(&argc, &argv);           
    MPI_Comm_size(MPI_COMM_WORLD, &tamano_de_procesadores);   
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_actual);  
	float perimetro=0,sumaperimetro=0,pericero=0;

	if(rank_actual==0)
	{
		ifstream fentrada(argv[1], ios::in | ios::binary);
		fentrada.seekg(0, ios::end); 
   		pos = fentrada.tellg();

		div=pos/tamano_de_procesadores;
		otro=div*tamano_de_procesadores;
		resta=pos-otro;
		aux=div+resta;
		numcero[0]=0;
		sum=sum+aux;
		numcero[1]=sum;
		for(int i=1;i<tamano_de_procesadores;i++)
		{

				numeros[0]=sum+1;
				sum=sum+div;
				numeros[1]=sum;
	
			status = MPI_Send(numeros, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		pericero=leer_triangulo(argv[1],argv[2],numcero);
		for(int j=1;j<tamano_de_procesadores;j++)
		{
			status = MPI_Recv(&perimetro, 1, MPI_FLOAT, j, MPI_ANY_TAG, MPI_COMM_WORLD, &rec_stat);
			sumaperimetro=perimetro+sumaperimetro;
		}
		sumaperimetro=pericero+sumaperimetro;
		cout<<"LA SUMA DE PERIMETRO ES: "<<sumaperimetro<<endl;
	}
	else
	{
		status = MPI_Recv(numeros, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &rec_stat);		
		perimetro=leer_triangulo(argv[1],argv[2],numeros);
		//cout<<"PERIMETRO DE "<<rank_actual<<" ES: "<<perimetro<<endl;
		status = MPI_Send(&perimetro, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
	}
	 MPI_Finalize();
    return 0;
}