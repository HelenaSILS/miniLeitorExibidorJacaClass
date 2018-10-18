#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#define TAG3 3
#define TAG4 4
#define TAG5 5
#define TAG6 6
#define TAG7 7
#define TAG8 8
#define TAG9 9
#define TAG10 10
#define TAG11 11
#define TAG12 12
#define TAG15 15
#define TAG16 16
#define TAG18 18

union ElementoConstantePool{
    int32_t su4;
    int64_t su8;
    uint16_t u2;
    uint32_t u4;
    uint64_t u8;

} typedef constante;

struct TageElementoConstantePool{
    int8_t tag;
    constante valor;
} typedef tagConstante;

void imprimeTudoChar (char *, unsigned long);
uint16_t getCounter(char *);
tagConstante getConstante(char *, unsigned int );

int main(int argc, char **argv){

    FILE *file;
    char *buffer;
    long lSize=1;
    uint8_t i;
    uint16_t counter;
    tagConstante tc;

    if(argc<3){
        printf("Sem argumentos \n");
    }else
        printf("Hah argumento!\n");

    //Definições de projeto
    //argumento 0 é o nome do executável
    //argumento 1 é a opção
    //argumento 2 é o nome do arquivo

    //file é o .class
    if((file = fopen(argv[2], "rb"))==NULL){
            printf("erro, nao foi possivel abrir o arquivo\n");
    }

     // obtain file size:
    fseek (file , 0 , SEEK_END);
    lSize = ftell(file);
    rewind(file);
    buffer=(char*)malloc(sizeof(char)*lSize);
    fread(buffer, 1, lSize, file);

    i=buffer[0];
    counter=getCounter(buffer);
    printf("contador de cosntantes: 0x%x ou %d\n", counter, counter);
    printf("buffer: %x\n", i);
    tc=getConstante(buffer, 10);

    imprimeTudoChar(buffer, lSize);

    fclose(file);

return 0;
}

void imprimeTudoChar (char *buffer, unsigned long lSize){
    uint8_t aux;
    for(int i=0; i< lSize; i++){
        aux = *(buffer+i);
        printf("%x\n", aux);
    }
}

uint16_t getCounter(char *buffer){
    uint16_t b, a = *(buffer+8);
    a<<8;
    b = *(buffer+9);
    return a += b;
}

tagConstante getConstante(char *buffer, unsigned int indice){
    tagConstante u;
    uint8_t a,b,c,d,e,f,g,h, T;
    uint64_t z;
    T=*(buffer+indice);
    u.tag=T;
    indice++;

        if(T == TAG3){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            d=*(buffer+indice+4);
            z=(a<<24)+(b<<16)+(c<<8)+d;
            u.valor.su4 = z;
        }

        if (T==TAG4 || T==TAG9 || T==TAG10 || T==TAG11 || T==TAG12 || T==TAG18){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            d=*(buffer+indice+4);
            z=(a<<24)+(b<<16)+(c<<8)+d;
            u.valor.u4 = z;
            printf("tag: %d, valor(hex): %x\n", T, u.valor.u4);
        }

        if (T==TAG5){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            d=*(buffer+indice+4);
            e=*(buffer+indice+5);
            f=*(buffer+indice+6);
            g=*(buffer+indice+7);
            h=*(buffer+indice+8);
            z=(a<<56)+(b<<48)+(c<<40)+(d<<32)+(e<<24)+(f<<16)+(g<<8)+h;
            u.valor.su8 = z;
        }

         if(T==TAG6){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            d=*(buffer+indice+4);
            e=*(buffer+indice+5);
            f=*(buffer+indice+6);
            g=*(buffer+indice+7);
            h=*(buffer+indice+8);
            z=(a<<56)+(b<<48)+(c<<40)+(d<<32)+(e<<24)+(f<<16)+(g<<8)+h;
            u.valor.u8 = z;
         }

          if(T==TAG7 || T==TAG8 || T==TAG16){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            z=(a<<8)+b;
            u.valor.u2 = z;
          }

        return u;
};


