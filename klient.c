#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

main()
{
 struct sockaddr_in client,server;
  
 int rm[1];  //rozmiar macierzy kwadratowych(obu)
 int s,sock,r,i,j;
 
 client.sin_family=AF_INET;
 client.sin_port=33000;
 client.sin_addr.s_addr=inet_addr("127.0.0.1");
 memset(&(client.sin_zero), 0, 8); //aby zachowac odpowiedni rozmiar struktury
 
 if( (s=socket(AF_INET,SOCK_STREAM,0)) < 0 )
    fprintf(stderr,"Nie mozna otworzyc gniazda(socket stream)\n"), exit(0);
    
 if( connect(s,(struct sockaddr *)&client,sizeof(client)) < 0 )
  {
   puts("Blad polaczenia");
   return 1;
  }
 
 
  printf("Mnozenie macierzy kwadratowych, obie maja tyle samo wierszy i kolumn\n");
  printf("Podaj ich rozmiar(Liczba calkowita dodatnia): ");
   scanf("%d", &rm[0]);
  if( rm[0] <= 0 )
   printf("Musisz podac liczbe calkowita dodatnia");
  else
  {
     //send(s, &rm, sizeof(rm), 0)//wyslanie do servera informacji o rozmiarze macierzy kwadratowych
    if( send(s, &rm, sizeof(rm), 0) < 0)
    {
        puts("Blad wysylania");
        return 1;
    }
  
  
   int wierszkolumna = rm[0]; //liczba wiersz/kolumn dla macierzy kwadratowych
   int macierz_A[wierszkolumna][wierszkolumna];
   int macierz_B[wierszkolumna][wierszkolumna];
   int macierz_C[wierszkolumna][wierszkolumna];
   
   printf("Wpisz elementy macierzy A (wierszami):\n");
     for(i=0;i<rm[0];i++)
     {
       for(j=0;j<rm[0];j++)
       {
        scanf("%d",&macierz_A[i][j]);
       }
     }
    //send(s, &macierz_A, sizeof(macierz_A), 0); //wyslanie do servera elementy macierzy A
    if( send(s, &macierz_A, sizeof(macierz_A),0) < 0)
    {
        puts("Blad wysylania");
        return 1;
    }
    
    printf("Wpisz elementy macierzy B (wierszami):\n");
    for(i=0;i<rm[0];i++)
     {
       for(j=0;j<rm[0];j++)
       {
        scanf("%d",&macierz_B[i][j]);
       }
     }
     //send(s, &macierz_B, sizeof(macierz_B), 0); //wyslanie do servera elementy macierzy B
    if( send(s, &macierz_B, sizeof(macierz_B),0) < 0)
    {
        puts("Blad wysylania");
        return 1;
    }
    
 int odmacierzC; 
      odmacierzC = recv(s,&macierz_C,sizeof(macierz_C),0);   //odebranie od servera macierzy wynikowej C
        if( odmacierzC < 0)
          puts("ERROR recv()");
        if(odmacierzC == 0)
          puts("ERROR zostalo zamkniete polaczenie");
    
    
    printf("\nRezultat mnozenia macierzy kwadratowych:\n"); 
     for(i=0;i<rm[0];i++)
     {
      printf("\n");  
      for(j=0;j<rm[0];j++)
       {
        printf(" %d ",macierz_C[i][j]);
       }
     } 
     
  
  } 
  printf("\n");
  close(s);  //zamkniecie gniazda
  
} 
