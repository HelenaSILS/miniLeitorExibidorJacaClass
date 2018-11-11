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
    //uint64_t u8;

} typedef constante;

struct constantePool_infor{
    int8_t      tag;
    constante   valor;
    constante   valor2;
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
    uint16_t    contadorAttribute;
    attribute_info *attributes;

} typedef classCompleta;

void imprimeTudoChar (char *, unsigned long);
uint16_t getCounter(char *);
unsigned int getConstante(char *, unsigned int, tagConstante *, uint8_t * );
tagConstante* alocaPoolConstanteTable(uint16_t );
int setConstantePool(char *, tagConstante *, unsigned );
void imprimePoolConstante(tagConstante *, uint16_t );
void referenciaConstantePool(tagConstante *, uint16_t );

uint16_t* alocaInterfaces(uint16_t );
unsigned int setInterface (char *, uint16_t *, unsigned int , uint16_t );

unsigned int setAttributes (char *, attribute_info *, unsigned int, uint16_t );
unsigned int getAttribute(char *, unsigned int , attribute_info *);
attribute_info* alocaAttribute(uint16_t );
void imprimeAttribute(uint16_t, attribute_info*);

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
    uint16_t w;
    uint16_t counter;
    classCompleta classe;
    tagConstante tc, *constantPool;

    if(argc<3){
        printf("Sem argumentos \n");
    }else
        printf("Argumentos ok\n");

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
    printf("tamanho do buffer em bytes: %li\n", lSize);

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

    classe.constantPool=alocaPoolConstanteTable(classe.contadorPoolConstante);

    //getConstante(buffer, 10, constantPool);
    indice = setConstantePool(buffer, classe.constantPool, classe.contadorPoolConstante);

   //printf("TAMANHO DO INDICE DESPOIS DO SET CONSTANT POLL: %d\n", indice);


    a=*(buffer+indice);
    b=*(buffer+indice+1);
    w=((a<<8)&0xff00)+(b&0x00ff);
    classe.accessFlag=w;
    indice=indice+2;

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    w=((a<<8)&0xff00)+(b&0x00ff);
    classe.thisClass=w;
    indice=indice+2;

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    w=((a<<8)&0xff00)+(b&0x00ff);
    classe.superClass=w;
    indice=indice+2;

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    w=((a<<8)&0xff00)+(b&0x00ff);
    classe.contadorInterfaces=w;
    indice=indice+2;

    if(classe.contadorInterfaces!=0)
        classe.interfaces = alocaInterfaces(classe.contadorInterfaces);
    else
        classe.interfaces=NULL;
    printf("contador de intefaces: %d\n", classe.contadorInterfaces);

    indice = setInterface (buffer, classe.interfaces, indice , classe.contadorInterfaces );

    printf("access flag: 0x%.4x\nthis class referencia: %d\nsuper class reference: %d\n", classe.accessFlag, classe.thisClass, classe.superClass);

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    classe.contadorFields=z;
    indice=indice+2;

    printf("contador de fields: %d\n", classe.contadorFields);

    if(classe.contadorFields==0)
        classe.fields=NULL;
    else
        classe.fields = alocaFields(classe.contadorFields);
    indice = setField(buffer, classe.fields, indice, classe.contadorFields);

    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    classe.contadorMethod=z;
    indice=indice+2;

    printf("contador de metodos: %d\n", classe.contadorMethod);

    if(classe.contadorMethod==0)
        classe.methods=NULL;
    else{
        classe.methods = alocaFields(classe.contadorMethod);
    }
    //field tem a mesma estrutura de method
    indice = setField(buffer, classe.methods, indice, classe.contadorMethod);

    //Atributos da classe
    if(indice<lSize){
        a=*(buffer+indice);
        b=*(buffer+indice+1);
        z=(a<<8)+b;
        classe.contadorAttribute=z;
        indice=indice+2;
    }else{
        classe.contadorAttribute=0;
    }

    if(classe.contadorAttribute==0)
        classe.attributes=NULL;
    else{
        classe.attributes = alocaAttribute(classe.contadorAttribute);
    }
    indice = setAttributes(buffer, classe.attributes, indice, classe.contadorAttribute);
    printf("contador de atributos: %d\n", classe.contadorAttribute);

    printf("\nPOOL DE CONSTANTES:\n");
    imprimePoolConstante(classe.constantPool, classe.contadorPoolConstante);
    printf("\n\n");

    printf("FIELDS:\n");
    imprimeField(classe.contadorFields, classe.fields);

    printf("METHODS:\n");
    imprimeField(classe.contadorMethod, classe.methods);

    printf("CLASS' ATTRIBUTES:\n");
    imprimeAttribute(classe.contadorAttribute, classe.attributes);



    //imprimeTudoChar(buffer, lSize);
    free(buffer);
    //freeClassFile(classe);
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
    uint16_t a, b, i;
    for(i=0; i<(counter-1); i++){
            a=(((t+i)->valor.u4) >> 16) & 0x0000ffff;
            b=((t+i)->valor.u4) & 0x0000ffff;
            printf("[%u]\t",i);
            if( (t+i)->tag==0){
                printf("\tbits menos significativos: %x\n", (t+i)->valor.u4);
            }if( (t+i)->tag==TAG1 ){
                printf("String: %s\n", (t+i)->itemString);
            }if( (t+i)->tag==TAG3 ){
                printf("inteiro %d\n", (t+i)->valor.u4);
            }if( (t+i)->tag==TAG4 ){
                printf("float %f\n", (float) (t+i)->valor.u4);
            }if( (t+i)->tag==TAG5 ){
                printf("long bits mais significativo %x\n", (t+i)->valor.u4);
            }if( (t+i)->tag==TAG6 ){
                printf("double bits mais significativo %x\n",  (t+i)->valor.u4 );
            }if( (t+i)->tag==TAG7 ){
                printf("Class reference %d \t",  (t+i)->valor.u2);
                referenciaConstantePool(t,(t+i)->valor.u2); printf("\n");
            }if( (t+i)->tag==TAG8 ){
                printf("String reference %d \t",  (t+i)->valor.u2);
                referenciaConstantePool(t,(t+i)->valor.u2); printf("\n");
            }if( (t+i)->tag==TAG9 ){
                printf("Field Reference \tclasse: %d\tnome %d\n\t\t\t", a, b);
                referenciaConstantePool(t,b); printf("\t");
                referenciaConstantePool(t,a); printf("\n");
            }if( (t+i)->tag==TAG10 ){
                printf("Method Reference  \tclasse: %d\tnome %d\n\t\t\t", a, b);
                referenciaConstantePool(t,b); printf("\t");
                referenciaConstantePool(t,a); printf("\n");
            }if( (t+i)->tag==TAG11 ){
                printf("Interface method  \tclasse: %d\tnome %d\n\t\t\t", a, b);
                referenciaConstantePool(t,b); printf("\t");
                referenciaConstantePool(t,a); printf("\n");
            }if( (t+i)->tag==TAG12 ){
                printf("Name and type  \tdescriptor %d\tnome %d\n\t\t\t", a, b);
                referenciaConstantePool(t,b); printf("\t");
                referenciaConstantePool(t,a); printf("\n");
            }if( (t+i)->tag==TAG15 ){
                a=(((t+i)->valor.u4) >> 16) & 0x000000ff;
                printf("Method handle\tclasse: %d\tnome %d\n\t\t", a, b);
                referenciaConstantePool(t,b); printf("\t");
                referenciaConstantePool(t,a); printf("\n");
            }if( (t+i)->tag==TAG16 ){
                printf("Method type %.2d\n", (t+i)->valor.u2);
            }if( (t+i)->tag==TAG18 ){
                printf("InvokeDynamic %.8d\n",  (t+i)->valor.u4);
            }
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
    unsigned int i, k, l, j=10;
    uint8_t parte=0;
    for(i=0; i<counter; i++){
        /** k é o numero de bytes que foram percorridos quando se pegou um item **/
        k=getConstante(buffer, j, (t+i), &parte);
        /** j, que começa em 10, será o valor atual mais o tanto percorrido quando se pega um item **/
        j =  j + k;
        if(k==-1){
            j++;
            break;
        }
        //printf("dentro do setconstante, j: %d\n", j);
    }
    return j;


}

/** Preenche uma estrutura de constante e retorna o próximo indice a ser lido no buffer**/
unsigned int getConstante(char *buffer, unsigned int indice, tagConstante *u, uint8_t *parte){
    //tagConstante u;
    uint32_t a,b,c,d,e,f,g,h, T;
    uint32_t ad,bd,cd,dd,ed,fd,gd,hd;
    uint16_t y;
    uint32_t w;
    uint64_t z;
    char *str;
    T=*(buffer+indice);
    u->tag=T;
    //indice++;

    if(*parte==0){
      //  printf("parte 0, buffer 0 a 5: %d, %d, %d, %d, %d\n", *(buffer+indice), *(buffer+indice+1), *(buffer+indice+2), *(buffer+indice+3), *(buffer+indice+4));
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
            w=((a<<24) & 0xff000000)+((b<<16) & 0x00ff0000)+((c<<8) & 0x0000ff00) + (d & 0x000000ff);
            u->valor.u4 = w;
            //printf("tag: %d, valor(hex): %x\n", T, u->valor.u4);
            return 5;
        }else{

        if (T==TAG15){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            w=((a<<16) & 0x00ff0000)+((b<<8) & 0x0000ff00)+(c & 0x000000ff);
            u->valor.u4 = w;
            //printf("tag: %d, valor(hex): %x\n", T, u->valor.u4);
            return 4;
        }else{

        if (T==TAG5 || T==TAG6){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            d=*(buffer+indice+4);
            w=((a<<24) & 0xff000000)+((b<<16) & 0x00ff0000)+((c<<8) & 0x0000ff00) + (d & 0x000000ff);
            u->valor.u4 = w;
            *parte=1;
            //printf("tag: %d, valor(hex): %x\n", T, u->valor.u4);
            return 5;
        }else{

          if(T==TAG7 || T==TAG8 || T==TAG16){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            y=((a<<8)&0x0000ff00)+(b&0x000000ff);
            u->valor.u2 = y;
            return 3;
          }else{
            //printf("NAO TA CERTO, T=%d\n",T);
            return -1;
          } } } } }
    }else{
        //parte == 1
        u->tag=0;
        a=*(buffer+indice+0);
        b=*(buffer+indice+1);
        c=*(buffer+indice+2);
        d=*(buffer+indice+3);
        w=((a<<24) & 0xff000000)+((b<<16) & 0x00ff0000)+((c<<8) & 0x0000ff00) + (d & 0x000000ff);
        u->valor.u4 = w;
        *parte=0;
        return 4;

    }

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

    if(counter!=0){
    for(i=0; i< counter; i++){
        a=buffer[indice+(2*i)];
        b=buffer[indice+1+(2*i)];
        z=(a<<8)+b;
        *(interfaces+i)=z;
    }
    return (indice+2+(2*i));
    }else{
    //printf("------------------------indice na interface: %d\n", indice);
	return indice;
    }
}

/** aloca o vetor de fields, que são valores que apontam para algum lugar do pool de constantes **/
field_info* alocaFields(uint16_t counter){
    field_info *t;
    t=(field_info*) malloc (sizeof(uint16_t)*(counter+1));
    return t;
}

/** preenche a tabela de fields já alocada e devolve o próximo byte do buffer, recebendo o byte em que começa **/
unsigned int setField (char *buffer, field_info *field, unsigned int indice, uint16_t counter){
    uint16_t i;
    if(counter==0)
        return indice;
    else{
    for(i=0; i< counter; i++){
       // printf("======endereço do field: %li\n", (field+i));
        indice = getField(buffer, indice, (field+i));
    }
    return indice;
    }
}

/** Preenche uma estrutura de field e retorna o próximo indice a ser lido no buffer**/
unsigned int getField(char *buffer, unsigned int indice, field_info *u){
    //tagConstante u;
    uint16_t a,b,c,d,e,f,g,h, T;
    uint16_t z, i;
    attribute_info *atrb;

    /** access flag **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=(a<<8)+b;
    indice += 2;
    u->access_flag=z;
   // printf("GET FIELD ACCESS FLAG: %d\n", u->access_flag);

    /** name_index **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=((a<<8)&0xff00)+(b&0x00ff);
    indice += 2;
    u->name_index=z;
   // printf("GET FIELD NAME INDEX: %d\n", u->name_index);

    /** descriptor index **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=((a<<8)&0xff00)+(b&0x00ff);
    indice += 2;
    u->descriptor_index=z;

   // printf("GET FIELD DESCRIPTOR INDEX: %d\n", u->descriptor_index);

    /** attribute count **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    z=((a<<8)&0xff00)+(b&0x00ff);
    indice += 2;
    u->attribute_count=z;
       // printf("GET FIELD, attribute count: %d\n", u->attribute_count);

    /** atributes **/
    atrb = (attribute_info*)malloc(u->attribute_count * sizeof(attribute_info));
    if(atrb==NULL)
        printf("\n\nATRB NULL\n\n");
    //atrb é uma variável auxiliar

    for(i=0; i<u->attribute_count; i++){
        indice = getAttribute(buffer, indice, (atrb+i));
       // printf("\nDEPOIS DO GET ATTRIBUTE: ATRB. NOME: %d ; ATRB. LENGHT: %d\n\n", (atrb+i)->attribute_name_index, (atrb+i)->attribute_lenght);
    }
    u->attributes=atrb;

    return indice;
}

void imprimeField (uint16_t counter, field_info *f){
    uint16_t i, j;
    attribute_info *atrb;
    for(i=0; i< counter; i++){
        printf("field %d:\taccess flag: %d\n\t\tname index: %d\n\t\t",i, (f+i)->access_flag, (f+i)->name_index);
        printf("descriptor index: %d\n\t\tattributes counter: %d\n\t", (f+i)->descriptor_index, (f+i)->attribute_count);
        for(j=0; j<(f+i)->attribute_count; j++){
            atrb = (f+i)->attributes;
            printf("\tattrb. %d \n\t\t\tattribute name index: %d\n\t\t\t", j, (atrb+j)->attribute_name_index);
            printf("attribute lenght: %d\n", (atrb+j)->attribute_lenght);
        }
    }
}

/** preenche a tabela de attributes já alocada e devolve o próximo byte do buffer, recebendo o byte em que começa **/
unsigned int setAttributes (char *buffer, attribute_info *atrb, unsigned int indice, uint16_t counter){
    uint16_t i;
    if(counter==0)
        return indice;
    else{
    for(i=0; i< counter; i++){
        indice = getAttribute(buffer, indice, (atrb+i));
    }
    return indice;
    }
}

/** aloca vetor de atributos **/
attribute_info* alocaAttribute(uint16_t counter){
    attribute_info *t;
    t=(attribute_info*) calloc (counter, sizeof(attribute_info));
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

       // printf("GET ATTRIBUTE, attribute name index: %d\n", u->attribute_name_index);

    /** attribute lenght **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    c=*(buffer+indice+2);
    d=*(buffer+indice+3);
    z=(a<<24)+(b<<16)+(c<<8)+d;
    indice += 4;
    //printf("\n\nindice: %d, z: %d\n\n", indice, z);
    u->attribute_lenght=z;

       // printf("GET ATTRIBUTE, attribute lenght: %d\n", u->attribute_lenght);
    /** info **/
    u->info = (uint8_t*) malloc (sizeof(uint8_t)*(u->attribute_lenght));
    for(i=0; i<u->attribute_lenght; i++){
        *(u->info+i)=buffer[indice+i];
        //printf("valor dentro dos infos: %d\n", *(u->info+i));
    }
    return (indice+i);
}

void imprimeAttribute(uint16_t counter, attribute_info *f){
    uint16_t j;
    for(j=0; j<counter; j++){
            printf("attrb. %d \n\t\tattribute name index: %d\n\t\t", j, (f)->attribute_name_index);
            printf("attribute lenght: %d\n", (f)->attribute_lenght);
    }
}

void referenciaConstantePool(tagConstante *u, uint16_t ref){
    uint16_t a, b;

    if((u+ref-1)->tag==TAG1){
        printf("%s",(u+ref-1)->itemString);
    }else{
        a=((u+ref-1)->valor.u2);printf("a: %d ", a);
        if((u+a-1)->tag==TAG1){
            printf("%s",(u+a-1)->itemString);
        }

       /* b=((u+ref-1)->valor.u4) & 0x0000ffff;
        if((u+a-1)->tag==TAG1){
            printf("%s\t",(u+ref-1)->itemString);
        }if((u+b-1)->tag==TAG1){
            printf("%s\t",(u+ref-1)->itemString);
        }*/
    }

}

