#include <stdio.h>
#include <stdlib.h>

int main()
{
   //-----wczytanie danych-----

   //inicjalizacja pliku in
   FILE* plik_in;
   if( ( plik_in = fopen( "data_in.txt", "r" ) ) == NULL )
   {
      printf( "Blad podczas wczytywania pliku wejsciowego.\n" );
      return 1;
   }

   //klucz
   char* K = (char*)calloc( 6, sizeof(char) ); //d�ugo�� klucza 5 bajt�w (40 bit�w)
   char* K_temp = K;
   char c;
   //wszytanie klucza do tablicy K
   while( ( ( c = (char)fgetc(plik_in) ) != 32 ) && ( c != 44 ) && ( c != 10 ) && ( c != EOF ) ) *(K_temp++) = c;

   //tekst do zaszyfrowania
   char* T = (char*)calloc( 255, sizeof(char) ); //d�ugo�� max tekstu jawnego 255 bajt�w, nie wiem czy da si� najpierw odczyta� z pliku i potem stworzy�
   char* T_temp = T;                             //o takiej d�ugo�ci, w c++ da�oby si� chyba wczyta� do stringa i lenght sprawdzi�, ale w C nie wiem
   int T_len = 0;
   //wczytanie tekstu
   while( (c = fgetc(plik_in) ) != EOF )
   {
      *(T_temp++) = c;
      //przy okazji zapisanie (rzeczywistej) d�ugo�ci tekstu
      T_len++;
   }


   //-----tworzenie tablicy i mieszanie z kluczem-----

   int* S = (int*)calloc( 256, sizeof(int) );
   int i = 0;
   int j = 0;

   //tablica permutacji identyczno�ciowej
   for( i; i < 256; i++ )
   {
      S[i] = i;
   }

   //pierwsze przetwarzanie tablicy i wplecienie znak�w klucza
   for( i = 0; i < 256; i++ )
   {
      j = ( j + S[i] + K[i % 5] ) % 256;
      int temp = S[i];
      S[i] = S[j];
      S[j] = temp;
   }


   //-----algorytm pseudolosowej generacji-----

   char* W = (char*)calloc( T_len, sizeof(char) ); //tablica strumienia szyfruj�cego
   char* W_temp = W;
   i = 0; j = 0;

   int T_len_temp = T_len;
   while( T_len_temp-- )
   {
      i = ( i + 1 )    % 256;
      j = ( j + S[i] ) % 256;
      int temp = S[i];
      S[i] = S[j];
      S[j] = temp;
      *(W_temp++) = S[( S[i] + S[j] ) % 256 ];
   }


   //-----XOR i wpisanie do pliku-----
   
   //XOR
   char* X = (char*)calloc( T_len, sizeof(char) );
   for( i = 0; i < T_len; i++ )
   {
      X[i] = T[i] ^ W[i];
   }

   //wpisanie do pliku
   FILE* plik_out;
   plik_out = fopen( "data_out.txt", "w");
   if(plik_out == NULL)
   {
      printf("Blad podczas wczytywania pliku wyjsciowego.\n");   
      return 2;             
   }
   for( i = 0; i < T_len; i++ ) fprintf(plik_out, "%d", X[i]);
   

   //sprawdzenie czy dobrze zosta� wygenerowany klucz i dobrze zosta�o zaszyfrowane
   //(XOR tekstu zaszyfrowanego i strumienia szyfuj�cego wypisany na cmd)
   for( int i = 0; i < T_len; i++ )
   {
      X[i] = X[i] ^ W[i];
      putchar( X[i] );
   }

   //-----zako�czenie-----

   free(W);
   free(K);
   free(T);
   free(S);
   free(X);
   fclose( plik_in  );
   fclose( plik_out );



   return 0;
}