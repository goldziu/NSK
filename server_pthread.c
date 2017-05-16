#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
/*
    kompilacja: gcc -pthread server_pthread.c -o server
*/

//the thread function
void *connection_handler(void *);

main()
{
 struct sockaddr_in client,server;
 int client_sock;
 
 int s,sock,r,i,j,sz,k;
 int rm[1];  //rozmiar macierzy kwadratowych(obu)
 server.sin_family=AF_INET;
 server.sin_port=htons(33000);
 server.sin_addr.s_addr=INADDR_ANY;
 memset(&(server.sin_zero), 0, 8); //aby zachowac odpowiedni rozmiar struktury
 
  if( (s=socket(AF_INET,SOCK_STREAM,0)) < 0 )
    fprintf(stderr,"Nie mozna otworzyc gniazda(socket stream)\n"), exit(0);
    
  puts("Utworzono socket");
 
 if( bind(s,(struct sockaddr *)&server,sizeof(server)) < 0 )
  {
   puts("ERROR bind()");
  }
 puts("BIND DONE");
 
 if( listen(s,10) < 0)
 {
   puts("ERROR listen()");
 }
 
 puts("czekaj na polaczenie");
 
 
 /*
 sock = accept(s,(struct sockaddr *)&server,&sz);
  if( sock < 0 )
    puts("ERROR - blad polaczenia(unable to accept connections");
*/
sz=sizeof(server);

pthread_t thread_id;
unsigned int address_client_size = sizeof(client);

    while( (client_sock = accept(s, (struct sockaddr *)&client, &address_client_size)) )
    {
        puts("Zaakceptowano polaczenie");
         
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("Nie mozna utworzyc watku");
            return 1;
        }
            
        puts("Przydzielono watek dla klienta");
    }
    
    if (client_sock < 0)
    {
        perror("Brak polaczenia");
        return 1;
    }
     
    return 0;
}

//Funkcja dla kazdego klienta

void *connection_handler(void *socket_desc)
{

    // socket descriptor
    int sock = *(int*)socket_desc;
    
     int rm[1];  //rozmiar macierzy kwadratowych(obu)
     int i,j,k;
 
int odrm;
     odrm = recv(sock,&rm,sizeof(rm),0);   //odebranie od klienta rozmiaru macierzy
      if( odrm < 0)
       puts("ERROR recv()");
      if(odrm == 0)
        puts("ERROR zostalo zamkniete polaczenie");
 
 
 int wierszkolumna = rm[0]; //liczba wiersz/kolumn dla macierzy kwadratowych
   int macierz_A[wierszkolumna][wierszkolumna];
   int macierz_B[wierszkolumna][wierszkolumna];
   int macierz_C[wierszkolumna][wierszkolumna];
  
int odmacierzA; 
    odmacierzA = recv(sock,&macierz_A,sizeof(macierz_A),0);  //odebranie od klienta macierzy A
        if( odmacierzA < 0)
          puts("ERROR recv()");
        if(odmacierzA == 0)
          puts("ERROR zostalo zamkniete polaczenie");
  
 printf("\nWypisanie przez server odebranej od klienta macierzy A:\n");
 for(i=0;i<rm[0];i++)
 {
    printf("\n");  
    for(j=0;j<rm[0];j++)
    {
     printf(" %d ",macierz_A[i][j]);
    }
 } 
 
 
int odmacierzB;
     odmacierzB =  recv(sock,&macierz_B,sizeof(macierz_B),0);  //odebranie od klienta macierzy B
    if( odmacierzB < 0)
          puts("ERROR recv()");
        if(odmacierzB == 0)
          puts("ERROR zostalo zamkniete polaczenie");
    
 printf("\nWypisanie przez server odebranej od klienta macierzy B:\n"); 
 for(i=0;i<rm[0];i++)
 {
  printf("\n");  
  for(j=0;j<rm[0];j++)
  {
   printf(" %d ",macierz_B[i][j]);
  }
 } 
 
  printf("\nMnozenie macierzy........\n"); 
 for(i=0;i<rm[0];i++)
 for(j=0;j<rm[0];j++)
 {
  macierz_C[i][j]=0;
  for(k=0;k<rm[0];k++)
   macierz_C[i][j]+=macierz_A[i][k]*macierz_B[k][j];
 }
 
 printf("\nRezultat mnozenia macierzy:\n") ; 
 for(i=0;i<rm[0];i++)
 {
  printf("\n");  
  for(j=0;j<rm[0];j++)
  {
   printf(" %d ",macierz_C[i][j]);
  }
 }
  printf("\n"); 
  
 //send(sock,&macierz_C,sizeof(macierz_C),0);
 
 if( send(sock, &macierz_C, sizeof(macierz_C),0) < 0)   //wysłanie do klienta macierzy C
    {
        puts("Blad wysylania");
        //return 1;
    }
    
    
 close(sock);

 
}
