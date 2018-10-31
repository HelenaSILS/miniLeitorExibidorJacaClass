#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#define TAG1 1
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
    uint16_t u2;
    uint32_t u4;
    uint64_t u8;

} typedef constante;

struct constantePool_infor{
    int8_t      tag;
    constante   valor;
    char        *itemString;
} typedef tagConstante;

struct attribute_info{
    uint16_t    attribute_name_index;
    uint32_t    attribute_lenght;
    uint8_t     *info; //vetor info[attribute_lenght]
} typedef attribute_info;

struct field_info{
    uint16_t    access_flag;
    uint16_t    name_index;
    uint16_t    descriptor_index;
    uint16_t    attribute_count;
    attribute_info  *attributes; //vetor attributes[attribute_count]
} typedef field_info, method_info;




struct classCompleta{
    uint32_t    magicNumber;
    uint16_t    minorVersion;
    uint16_t    majorVersion;
    uint16_t    contadorPoolConstante;
    tagConstante *constantPool;
    uint16_t    accessFlag;
    uint16_t    thisClass;
    uint16_t    superClass;
    uint16_t    contadorInterfaces;
    uint16_t    *interfaces;
    uint16_t    contadorFields;
    field_info  *fields;
    uint16_t    contadorMethod;
    method_info *methods;

} typedef classCompleta;

void imprimeTudoChar (char *, unsigned long);
uint16_t getCounter(char *);
unsigned int getConstante(char *, unsigned int, tagConstante * );
tagConstante* alocaPoolConstanteTable(uint16_t );
int setConstantePool(char *, tagConstante *, unsigned );
void imprimePoolConstante(tagConstante *, uint16_t );

uint16_t* alocaInterfaces(uint16_t );
unsigned int setInterface (char *, uint16_t *, unsigned int , uint16_t );

unsigned int getAttribute(char *, unsigned int , attribute_info *);
attribute_info* alocaAttribute(uint16_t );

field_info* alocaFields(uint16_t );
unsigned int setField (char *, field_info *, unsigned int , uint16_t );
unsigned int getField(char *, unsigned int , field_info *);
void imprimeField (uint16_t, field_info *);

int main(int argc, char **argv){

    FILE *file;
    char *buffer;
    long lSize=1;
    int indice=0;
    uint8_t i, a, b, c, d;
    uint32_t z;
    uint16_t counter;
    classCompleta classe;
    tagConstante tc, *constantPool;

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
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    c=*(buffer+indice+2);
    d=*(buffer+indice+3);
    z=(a<<24)+(b<<16)+(c<<8)+d;
    classe.magicNumber=z;
    printf("magic number: %x\n", classe.magicNumber);
    classe.contadorPoolConstante=getCounter(buffer);
    printf("contador de constantes: %d\n", classe.contadorPoolConstante);
    printf("buffer: %x\n", i);

    classe.constantPool=alocaPoolConstanteTable(classe.contadorPoolConstante);

    //getConstante(buffer, 10, constantPool);
    indice = setConstantePool(buffer, classe.constantPool, classe.contadorPoolConstante);


    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    classe.accessFlag=z;
    indice=indice+2;

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    classe.thisClass=z;
    indice=indice+2;

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    classe.superClass=z;
    indice=indice+2;

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    classe.contadorInterfaces=z;
    indice=indice+2;

    printf("valor do indice antes de setar a interface: %d\n", indice);
    classe.interfaces = alocaInterfaces(classe.contadorInterfaces);
    printf("contador de intefaces%d\n", classe.contadorInterfaces);

    indice = setInterface (buffer, classe.interfaces, indice , classe.contadorInterfaces );

    printf("valor do indice depois de setar a interface: %d\n", indice);

    printf("access flag: %x\nthis class referencia: %d\n", classe.accessFlag, classe.thisClass);

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    classe.contadorFields=z;
    indice=indice+2;

    printf("contador de fields: %d\n", classe.contadorFields);

    classe.fields = alocaFields(classe.contadorFields);
    indice = setField(buffer, classe.fields, indice, classe.contadorFields);

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    classe.contadorMethod=z;
    indice=indice+2;

    printf("contador de metodos: %d\n", classe.contadorMethod);

    classe.methods = alocaFields(classe.contadorMethod);
    indice = setField(buffer, classe.methods, indice, classe.contadorMethod);

    imprimePoolConstante(classe.constantPool, classe.contadorPoolConstante);

    printf("FIELDS:\n");
    imprimeField(classe.contadorFields, classe.fields);
    printf("METHODS:\n");
    // imprimeField(classe.contadorMethod, classe.methods);




    //imprimeTudoChar(buffer, lSize);
    free(buffer);
    fclose(file);

return 0;
}

void imprimeTudoChar (char *buffer, unsigned long lSize){
    uint8_t aux;
    for(unsigned long i=0; i< lSize; i++){
        aux = *(buffer+i);
        printf("%x\n", aux);
    }
}

void imprimePoolConstante(tagConstante *t, uint16_t counter){
    unsigned int i;
    for(i=0; i<counter; i++){
            if( (t+i)->tag==TAG1 )
                printf("String: %s\n", (t+i)->itemString);
            if( (t+i)->tag==TAG3 )
                printf("inteiro %d\n", (t+i)->tag, (t+i)->valor.u4);
            if( (t+i)->tag==TAG4 )
                printf("float %f\n", (t+i)->tag, (t+i)->valor.u4);
            if( (t+i)->tag==TAG5 )
                printf("long %lf\n", (t+i)->tag, (t+i)->valor.u8);
            if( (t+i)->tag==TAG6 )
                printf("inteiro %l\n", (t+i)->tag, (t+i)->valor.u8);
            if( (t+i)->tag==TAG7 )
                printf("Class reference 0x%x\n", (t+i)->tag, (t+i)->valor.u2);
            if( (t+i)->tag==TAG8 )
                printf("String reference 0x%x\n", (t+i)->tag, (t+i)->valor.u2);
            if( (t+i)->tag==TAG9 )
                printf("Field Reference dois bytes para classe, dois para o nome 0x%.8x\n", (t+i)->tag, (t+i)->valor.u4);
            if( (t+i)->tag==TAG10 )
                printf("Method Reference dois bytes para classe, dois para o nome 0x%.8x\n", (t+i)->tag, (t+i)->valor.u4);
            if( (t+i)->tag==TAG11 )
                printf("Interface method reference dois bytes para classe, dois para o nome 0x%.8x\n", (t+i)->tag, (t+i)->valor.u4);
            if( (t+i)->tag==TAG12 )
                printf("Name and type descriptor dois bytes para classe, dois para o nome 0x%.8x\n", (t+i)->tag, (t+i)->valor.u4);
            if( (t+i)->tag==TAG15 )
                printf("Method handle um byte descritor, dois de referencia %.6d\n", (t+i)->tag, (t+i)->valor.u4);
            if( (t+i)->tag==TAG16 )
                printf("Method type %.2d\n", (t+i)->tag, (t+i)->valor.u2);
            if( (t+i)->tag==TAG18 )
                printf("InvokeDynamic %.8d\n", (t+i)->tag, (t+i)->valor.u4);
    }
}

/** contador de pool de constantes**/
uint16_t getCounter(char *buffer){
    uint16_t b, a = *(buffer+8);
    a<<8;
    b = *(buffer+9);
    return a += b;
}

/** Guarda memória necessária para alocar o pool de constantes**/
tagConstante* alocaPoolConstanteTable(uint16_t counter){
    tagConstante *t;
    t = (tagConstante*) malloc (sizeof(tagConstante)*counter);
    return t;

}

/** Retorna o indice do buffer, e preenche a tabela previamente alocada, sempre começa no indice 10 do buffer **/
int setConstantePool(char *buffer, tagConstante *t, unsigned int counter){
    unsigned int i, k, j=11;
    for(i=0; i<counter; i++){
        /** k é o numero de bytes que foram percorridos quando se pegou um item **/
        k=getConstante(buffer, j, (t+i));
        /** j, que começa em 10, será o valor atual mais o tanto percorrido quando se pega um item **/
        j =  j + k;
        if(getConstante(buffer, j, t)==-1)
            break;
        //printf("dentro do setconstante, j: %d\n", j);
    }
    return j;


}

/** Preenche uma estrutura de constante e retorna o próximo indice a ser lido no buffer**/
unsigned int getConstante(char *buffer, unsigned int indice, tagConstante *u){
    //tagConstante u;
    uint8_t a,b,c,d,e,f,g,h, T;
    uint64_t z;
    char *str;
    T=*(buffer+indice);
    u->tag=T;
    //indice++;

        if(T == TAG1){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);

            z=(a<<8)+b;
            str = (char*)malloc(sizeof(char)*(z+1));
            for(unsigned int i = 1; i<=z; i++){
                *(str+i-1)=*(buffer+indice+i+2);
                //printf("char: %c\n",*(str+i));
            }
            *(str+z)='\0';
            u->itemString = str;
            //printf("tag1, %s\n", u->itemString);
            return (1+2+z);
        }else{

        if (T==TAG4 || T==TAG9 || T==TAG10 || T==TAG11 || T==TAG12 || T==TAG18 || T == TAG3){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            d=*(buffer+indice+4);
            z=(a<<24)+(b<<16)+(c<<8)+d;
            u->valor.u4 = z;
            //printf("tag: %d, valor(hex): %x\n", T, u->valor.u4);
            return 5;
        }else{

        if (T==TAG15){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            z=(a<<16)+(b<<8)+c;
            u->valor.u4 = z;
            //printf("tag: %d, valor(hex): %x\n", T, u->valor.u4);
            return 4;
        }else{

        if (T==TAG5 || T==TAG6){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            d=*(buffer+indice+4);
            e=*(buffer+indice+5);
            f=*(buffer+indice+6);
            g=*(buffer+indice+7);
            h=*(buffer+indice+8);
            z=(a<<56)+(b<<48)+(c<<40)+(d<<32)+(e<<24)+(f<<16)+(g<<8)+h;
            u->valor.u8 = z;
            return 9;
        }else{

          if(T==TAG7 || T==TAG8 || T==TAG16){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            z=(a<<8)+b;
            u->valor.u2 = z;
            return 3;
          }else{
            //printf("NAO TA CERTO, T=%d\n",T);
            return -1;
          } } } } }

};

/** aloca o vetor de interface, que são valores que apontam para algum lugar do pool de constantes **/
uint16_t* alocaInterfaces(uint16_t counter){
    uint16_t *t;
    t=(uint16_t*) malloc (sizeof(uint16_t)*counter);
    return t;
}

/** preenche o vetor de interfaces já alocado e devolve o próximo byte do buffer, recebendo o byte em que começa **/
unsigned int setInterface (char *buffer, uint16_t *interfaces, unsigned int indice, uint16_t counter){
    uint16_t i, z;
    uint8_t a, b;
    for(i=0; i< counter; i++){
        a=buffer[indice+(2*i)];
        b=buffer[indice+1+(2*i)];
        z=(a<<8)+b;
        *(interfaces+i)=z;
    }
    return (indice+2+(2*i));
}

/** aloca o vetor de fields, que são valores que apontam para algum lugar do pool de constantes **/
field_info* alocaFields(uint16_t counter){
    field_info *t;
    t=(field_info*) malloc (sizeof(uint16_t)*counter);
    return t;
}

/** preenche a tabela de fields já alocada e devolve o próximo byte do buffer, recebendo o byte em que começa **/
unsigned int setField (char *buffer, field_info *field, unsigned int indice, uint16_t counter){
    uint16_t i, z;
    uint8_t a, b;
    for(i=0; i< counter; i++){
        printf("SET FIELD, i: %d\n", i);
        indice = getField(buffer, indice, (field+i));
        printf("DE VOLTA AO SET FIELDS, field->name index: %d\n", (field+i)->name_index);
    }
    return indice;
}

/** Preenche uma estrutura de field e retorna o próximo indice a ser lido no buffer**/
unsigned int getField(char *buffer, unsigned int indice, field_info *u){
    //tagConstante u;
    uint8_t a,b,c,d,e,f,g,h, T;
    uint16_t z, i;
printf("entrou na get field\n");
    /** access flag **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    indice += 2;
    u->access_flag=z;

        printf("GET FIELD, access flag: %d\n", u->access_flag);
    /** name_index **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    indice += 2;
    u->name_index=z;
        printf("GET FIELD, name index: %d\n", u->name_index);

    /** descriptor index **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    indice += 2;
    u->descriptor_index=z;
        printf("GET FIELD, descriptor index: %d\n", u->descriptor_index);

    /** attribute count **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    indice += 2;
    u->attribute_count=z;
        printf("GET FIELD, attribute count: %d\n", u->attribute_count);

    /** atributes **/
    u->attributes = alocaAttribute(u->attribute_count);

    for(i=0; i<u->attribute_count; i++){
        indice = getAttribute(buffer, indice, u->attributes+i);
    }
    return indice;
}

void imprimeField (uint16_t counter, field_info *f){
    uint16_t i, j;
    for(i=0; i< counter; i++){
        printf("field %d:\taccess flag: %d\n\tname index: %d\n\t",i, f->access_flag, f->name_index);
        printf("descriptor index: %d\n\tattributes counter: %d\n\t", f->descriptor_index, f->attribute_count);
        for(j=0; j<1; j++){
            printf("\tattrb. %d attribute name index: %d\n\t\t", j, f->attributes->attribute_name_index);
            printf("attribute lenght: %d\n", f->attributes->attribute_lenght);
        }
    }
}

/** aloca vetor de atributos **/
attribute_info* alocaAttribute(uint16_t counter){
    attribute_info *t;
    t=(attribute_info*) malloc (sizeof(attribute_info)*counter);
    return t;
}


/** Preenche uma estrutura de attribute e retorna o próximo indice a ser lido no buffer**/
unsigned int getAttribute(char *buffer, unsigned int indice, attribute_info *u){
    uint8_t a, b, c, d;
    uint16_t w;
    uint32_t z, i;

    /** attribute name index **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    w=(a<<8)+b;
    indice += 2;
    u->attribute_name_index=w;

        printf("GET ATTRIBUTE, attribute name index: %d\n", u->attribute_name_index);

    /** attribute lenght **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    c=*(buffer+indice+2);
    d=*(buffer+indice+3);
    z=(a<<24)+(b<<16)+(c<<8)+d;
    indice += 4;
    u->attribute_lenght=z;

        printf("GET ATTRIBUTE, attribute lenght: %d\n", u->attribute_lenght);
        printf("GET ATTRIBUTE indice antes de ler os infos: %d\n", indice);
    /** info **/
    u->info = (uint8_t*) malloc (sizeof(uint8_t)*(u->attribute_lenght));
    for(i=0; i<u->attribute_lenght; i++){
        *(u->info+i)=buffer[indice+i];
        //printf("valor dentro dos infos: %d\n", *(u->info+i));
    }
    printf("indice entregue: %d\n", (indice+i));
    return (indice+i);
}
