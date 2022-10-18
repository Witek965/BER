#include <iostream>
#include <stdexcept>
#include <bitset>
#include <string.h>
#include <time.h>
using namespace std;

int liczba_blednych_bitow(unsigned char zrodlo, unsigned char porownywany)
{
	bitset <8> x = zrodlo ^ porownywany;
    return x.count(); 
}

int main(int argc, char** argv)
{
	unsigned long long liczba_bitow=0, liczba_bledow=0;
	string sciezka_zrodlo,sciezka_porownywany;
	FILE *plik_zrodlowy;
	FILE *plik_porownywany;
	time_t czas_rozpoczecia, czas_zakonczenia, timestamp;
	
	cout << argc;
	if  (argc==1)
	{
		cout << "Program obliczajacy bit error rate.\nPodaj sciezke do pliku zrodlowego:\n";
		for (bool i=false;i!=true;)
		{
		cin >> sciezka_zrodlo;
		plik_zrodlowy = fopen(sciezka_zrodlo.c_str(),"rb");
			if (plik_zrodlowy == NULL)
				cout << "Bledna sciezka do pliku, sprobuj ponownie.\n";
			else
				i = true;
		
		}
		cout << "Podaj sciezke do pliku porownywanego:\n";
		for (bool i=false;i!=true;)
		{
		cin >> sciezka_porownywany;
		plik_porownywany = fopen(sciezka_porownywany.c_str(),"rb");
			if (plik_porownywany == NULL)
				cout << "Bledna sciezka do pliku, sprobuj ponownie.\n";
			else
				i = true;
		}
	}
	else
	{
		sciezka_zrodlo = argv[1];
		sciezka_porownywany = argv[2];
		plik_zrodlowy = fopen(sciezka_zrodlo.c_str(),"rb");
			if (plik_zrodlowy == NULL)
				throw runtime_error("Argument pierwszy - nie ma takiego pliku. Blednie wpisana sciezka pliku?");
		plik_porownywany = fopen(sciezka_porownywany.c_str(),"rb");
			if (plik_porownywany == NULL)
				throw runtime_error("Argument drugi - nie ma takiego pliku. Blednie wpisana sciezka pliku?");
	}
	time(&czas_rozpoczecia);
	while (!feof(plik_zrodlowy) && !feof(plik_porownywany))
	{
		liczba_bitow++;
		if(feof(plik_zrodlowy) || feof(plik_porownywany))
			liczba_bledow=liczba_bledow+8;
		else
			liczba_bledow=liczba_bledow+liczba_blednych_bitow(getc(plik_zrodlowy),getc(plik_porownywany));
	}
	liczba_bitow=(liczba_bitow-1)*8;
	time(&czas_zakonczenia);
	float BER = ((float)liczba_bledow/(float)liczba_bitow)*100;
	cout << "Ilosc porownanych bitow: " << liczba_bitow << "\n";
	cout << "Ilosc roznych bitow: " << liczba_bledow << "\n";
	cout << "Wynik BER: " << BER << "%\n";
	cout << "Czas obliczen: " << difftime (czas_zakonczenia, czas_rozpoczecia) << "s\n";
	system("pause");
	
	FILE* log = fopen("log.txt","a");
	time(&timestamp);
	char * timestamp_char = strtok(ctime(&timestamp), "\n");
	fprintf (log,"[%s] Ilosc porownanych bitow: %i\n", timestamp_char,liczba_bitow);
	fprintf (log,"[%s] Ilosc roznych bitow: %i\n",timestamp_char,liczba_bledow);
	fprintf (log,"[%s] Wynik BER: %f%\n",timestamp_char,BER);
	fprintf (log,"[%s] Czas obliczen: %is\n",timestamp_char,difftime (czas_zakonczenia, czas_rozpoczecia));
}
