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
 
 int s,sock,r,i,j,sz,k;
 int rm[1];  //rozmiar macierzy kwadratowych(obu)
 server.sin_family=AF_INET;
 server.sin_port=33000;
 server.sin_addr.s_addr=inet_addr("127.0.0.1"); //Adres IP servera
 memset(&(server.sin_zero), 0, 8); //aby zachowac odpowiedni rozmiar struktury
 
  if( (s=socket(AF_INET,SOCK_STREAM,0)) < 0 )
    fprintf(stderr,"Nie mozna otworzyc gniazda(socket stream)\n"), exit(0);
 
 if( bind(s,(struct sockaddr *)&server,sizeof(server)) < 0 )
  {
   puts("ERROR bind()");
  }
 
 if( listen(s,1) < 0)
 {
   puts("ERROR listen()");
 }
 
 
 sz=sizeof(server);
 sock = accept(s,(struct sockaddr *)&server,&sz);
  if( sock < 0 )
    puts("ERROR - blad polaczenia(unable to accept connections");
 
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
 
 if( send(sock, &macierz_C, sizeof(macierz_C),0) < 0)
    {
        puts("Blad wysylania");
        return 1;
    }
    
    
 close(sock);
 close(s); 
 
}
