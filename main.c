#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
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

#define ACC_PUBLIC  0x0001
/*Declared public; may be accessed from outside its package.  */

#define ACC_PRIVATE  0x0002
/*Declared private; accessible only within the defining class.  */

#define ACC_PROTECTED  0x0004
/*Declared protected; may be accessed within subclasses.  */

#define ACC_STATIC  0x0008  /*Declared static.  */

#define ACC_FINAL  0x0010
/*Declared final; may not be overridden.  */

#define ACC_SUPER  0x0020 ///verficar super versus syncronized
/*Declared synchronized; invocation is wrapped in a monitor lock.  */

#define ACC_NATIVE  0x0100
/*Declared native; implemented in a language other than Java.  */

#define ACC_ABSTRACT  0x0400
/*Declared abstract; no implementation is provided.  */

#define ACC_STRICT  0x0800
/*Declared strictfp; floating-point mode is FP-strict  */

//tipos de atributos
#define		CONSTANT_VALUE			0
#define		CODE					1
#define		DEPRECATED				2
#define		EXCEPTIONS				3
#define		INNER_CLASSES			4
#define		LINE_NUMBER_TABLE		5
#define		LOCAL_VARIABLE_TABLE	6
#define		SYNTHETIC				7
#define		SOURCE_FILE				8
#define		UNKNOWN                 9


//estruturas de atributo
//Code Attribute
typedef struct exception_table_type{
	uint16_t	start_pc;
	uint16_t	end_pc;
	uint16_t	handler_pc;
	uint16_t	catch_type;
}exception_table_type;

//InnerClasses Attribute
typedef struct classes_type{
	uint16_t	inner_class_info_index;
	uint16_t	saidaer_class_info_index;
	uint16_t	inner_name_index;
	uint16_t	inner_class_access_flags;
}classes_type;

//LineNumberTable Attribute
typedef struct line_number_table_type{
	uint16_t	start_pc;
	uint16_t	line_number;
}line_number_table_type;

//LocalVariableTable Attribute
typedef struct local_variable_table_type{
	uint16_t	start_pc;
	uint16_t	length;
	uint16_t	name_index;
	uint16_t	descriptor_index;
	uint16_t	index;
}local_variable_table_type;

//=========================================================

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
    union{
		struct{											// 4.7.2
			uint16_t	constantvalue_index;
		}ConstantValue;

		struct{											// 4.7.3
			uint16_t		max_stack;
			uint16_t 		max_locals;
			uint32_t		code_length;
			uint8_t*		code;
			uint16_t		exception_table_length;
			exception_table_type *	exception_table;
			uint16_t		attributes_count;
			struct attribute_info*	attributes;
		}Code;

		struct{											// 4.7.15
		}Deprecated;

		struct{											// 4.7.5
			uint16_t		number_of_exceptions;
			uint16_t*		exception_index_table;
		}Exceptions;

		struct{											// 4.7.6
			uint16_t		number_of_classes;
			classes_type *	classes;
		}InnerClasses;

		struct{											// 4.7.12
			uint16_t		line_number_table_length;
			line_number_table_type *	line_number_table;
		}LineNumberTable;

		struct{
			// local_variable_type_table_length
			uint16_t		local_variable_table_length; // 4.7.14
			local_variable_table_type *	local_variable_table;
		}LocalVariableTable;

		struct{											// 4.7.8
		}Synthetic;

		struct{											// 4.7.10
			uint16_t		sourcefile_index;
		}SourceFile;
	}info;
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

char* referenciaConstantePool(tagConstante *, uint16_t );

void imprimeAccessFlag(uint16_t);

uint16_t* alocaInterfaces(uint16_t );
unsigned int setInterface (char *, uint16_t *, unsigned int , uint16_t );

unsigned int setAttributes (char *, attribute_info *, unsigned int, uint16_t, tagConstante* );
unsigned int getAttribute(char *, unsigned int , attribute_info *,tagConstante *);
uint8_t getTipoAttribute(attribute_info *, tagConstante *);
uint32_t estruturaAttribute (char *, unsigned int, uint8_t, attribute_info *, tagConstante *);
attribute_info* alocaAttribute(uint16_t );
void imprimeAttribute(uint16_t, attribute_info*, tagConstante*);

field_info* alocaFields(uint16_t );
unsigned int setField (char *, field_info *, unsigned int , uint16_t, tagConstante* );
unsigned int getField(char *, unsigned int , field_info *, tagConstante *);
void imprimeField (uint16_t, field_info *, tagConstante*);

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

    printf("access flag: 0x%.4x\t", classe.accessFlag);
    imprimeAccessFlag(classe.accessFlag);

    printf("\nthis class referencia: %d\nsuper class reference: %d\n", classe.thisClass, classe.superClass);

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
    indice = setField(buffer, classe.fields, indice, classe.contadorFields, classe.constantPool);

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
    indice = setField(buffer, classe.methods, indice, classe.contadorMethod, classe.constantPool);

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
    indice = setAttributes(buffer, classe.attributes, indice, classe.contadorAttribute, classe.constantPool);
    printf("contador de atributos: %d\n", classe.contadorAttribute);

    printf("\nPOOL DE CONSTANTES:\n");
    imprimePoolConstante(classe.constantPool, classe.contadorPoolConstante);
    printf("\n\n");

    printf("FIELDS:\n");
    imprimeField(classe.contadorFields, classe.fields, classe.constantPool);

    printf("METHODS:\n");
    imprimeField(classe.contadorMethod, classe.methods, classe.constantPool);

    printf("CLASS' ATTRIBUTES:\n");
    imprimeAttribute(classe.contadorAttribute, classe.attributes, classe.constantPool);



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
                printf("long \tbits mais significativo %x\n", (t+i)->valor.u4);
            }if( (t+i)->tag==TAG6 ){
                printf("double \tbits mais significativo %x\n",  (t+i)->valor.u4 );
            }if( (t+i)->tag==TAG7 ){
                printf("Class reference %d \t",  (t+i)->valor.u2);
                referenciaConstantePool(t,(t+i)->valor.u2); printf("\n");
            }if( (t+i)->tag==TAG8 ){
                printf("String reference %d \t",  (t+i)->valor.u2);
                referenciaConstantePool(t,(t+i)->valor.u2); printf("\n");
            }if( (t+i)->tag==TAG9 ){
                printf("Field Reference \tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a); printf("\t");
                referenciaConstantePool(t,b); printf("\n");
            }if( (t+i)->tag==TAG10 ){
                printf("Method Reference  \tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a); printf("\t");
                referenciaConstantePool(t,b); printf("\n");
            }if( (t+i)->tag==TAG11 ){
                printf("Interface method  \tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a); printf("\t");
                referenciaConstantePool(t,b); printf("\n");
            }if( (t+i)->tag==TAG12 ){
                printf("Name and type  \tdescriptor %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a); printf("\t");
                referenciaConstantePool(t,b); printf("\n");
            }if( (t+i)->tag==TAG15 ){
                a=(((t+i)->valor.u4) >> 16) & 0x000000ff;
                printf("Method handle\tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a); printf("\t");
                referenciaConstantePool(t,b); printf("\n");
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
unsigned int setField (char *buffer, field_info *field, unsigned int indice, uint16_t counter, tagConstante *tag){
    uint16_t i;
    if(counter==0)
        return indice;
    else{
    for(i=0; i< counter; i++){
       // printf("======endereço do field: %li\n", (field+i));
        indice = getField(buffer, indice, (field+i), tag);
    }
    return indice;
    }
}

/** Preenche uma estrutura de field e retorna o próximo indice a ser lido no buffer**/
unsigned int getField(char *buffer, unsigned int indice, field_info *u, tagConstante *tag){
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
    if(atrb==NULL){
        printf("\n\nATRB NULL\n\n");
        u->attributes=NULL;
    //atrb é uma variável auxiliar
    }else{
        for(i=0; i<u->attribute_count; i++){
            indice = getAttribute(buffer, indice, (atrb+i), tag);
       // printf("\nDEPOIS DO GET ATTRIBUTE: ATRB. NOME: %d ; ATRB. LENGHT: %d\n\n", (atrb+i)->attribute_name_index, (atrb+i)->attribute_lenght);
        }
        u->attributes=atrb;
    }
    return indice;
}

void imprimeField (uint16_t counter, field_info *f, tagConstante *t){
    uint16_t i, j;
    attribute_info *atrb;
    for(i=0; i< counter; i++){
        printf("field %d:\taccess flag: %d\t",i, (f+i)->access_flag);
        imprimeAccessFlag((f+i)->access_flag);
        printf("\n\t\tname:\t\t\t");
        referenciaConstantePool(t,(f+i)->name_index);
        printf("\n\t\t");
        printf("descriptor:\t\t");
        referenciaConstantePool(t, (f+i)->descriptor_index);
        printf("\n\t\tattributes counter: %d\n\t", (f+i)->attribute_count);
        printf("ATRIBUTOS: \n");
        imprimeAttribute((f+i)->attribute_count, (f+i)->attributes, t);
        printf("\n");
    }
}

/** preenche a tabela de attributes já alocada e devolve o próximo byte do buffer, recebendo o byte em que começa **/
unsigned int setAttributes (char *buffer, attribute_info *atrb, unsigned int indice, uint16_t counter, tagConstante *tag){
    uint16_t i;
    if(counter==0)
        return indice;
    else{
    for(i=0; i< counter; i++){
        indice = getAttribute(buffer, indice, (atrb+i), tag);
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
unsigned int getAttribute(char *buffer, unsigned int indice, attribute_info *u, tagConstante *t){
    uint8_t a, b, c, d, tipo;
    uint16_t w;
    uint32_t z, i;

    /** attribute name index **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    w=(a<<8)+b;
    indice += 2;
    u->attribute_name_index=w;

    /** attribute lenght **/
    a=*(buffer+indice);
    b=*(buffer+indice+1);
    c=*(buffer+indice+2);
    d=*(buffer+indice+3);
    z=(a<<24)+(b<<16)+(c<<8)+d;
    indice += 4;
    u->attribute_lenght=z;

    /** info **/
    /*u->info = (uint8_t*) malloc (sizeof(uint8_t)*(u->attribute_lenght));
    for(i=0; i<u->attribute_lenght; i++){
        *(u->info+i)=buffer[indice+i];
        //printf("valor dentro dos infos: %d\n", *(u->info+i));
    }*/
    tipo=getTipoAttribute(u, t);
    indice=estruturaAttribute(buffer,indice,tipo,u,t);
    printf("===ATTRIBUTE LENGHT depois da estrutura attribute: %d\n", u->attribute_lenght);

    return indice;
}

uint8_t getTipoAttribute(attribute_info *u, tagConstante *t){
    char *nome;
    nome=referenciaConstantePool(t, u->attribute_name_index);
    if(strcmp(nome, "Code")==0){

        printf("ta em code\n");
        return CODE;
    }else{
    if(strcmp(nome, "ConstantValue")==0){
        return CONSTANT_VALUE;
    }else{
    if(strcmp(nome, "Deprecated")==0){
        return DEPRECATED;
    }else{
    if(strcmp(nome, "Exceptions")==0){
        return EXCEPTIONS;
    }else{
    if(strcmp(nome, "InnerClasses")==0){
        return INNER_CLASSES;
    }else{
    if(strcmp(nome, "LineNumberTable")==0){
        return LINE_NUMBER_TABLE;
    }else{
    if(strcmp(nome, "LocalVariableTable")==0){
        return LOCAL_VARIABLE_TABLE;
    }else{
    if(strcmp(nome, "Synthetic")==0){
        return SYNTHETIC;
    }else{
    if(strcmp(nome, "SourceFile")==0){
        return SOURCE_FILE;
    }else{
        return UNKNOWN;
    }}}}}}}}}

}

uint32_t estruturaAttribute (char *buffer, unsigned int indice, uint8_t tipo, attribute_info *atrb, tagConstante *tag){
    uint8_t a, b, c, d;
    uint16_t w;
    uint32_t z, i;
    line_number_table_type *ln;
printf("chegou em estrutura attribute, imprindo o atrb lenght: %d\n", atrb->attribute_lenght);
    switch (tipo){
    case CODE:
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.Code.max_stack=w;
            printf("tamanho do max stack: %d %d\n", atrb->info.Code.max_stack, w);

            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.Code.max_locals=w;

            printf("tamanho do max locals: %d %d\n", atrb->info.Code.max_locals, w);

            a=*(buffer+indice); b=*(buffer+indice+1); c=*(buffer+indice+2); d=*(buffer+indice+3);
            z=(a<<24)+(b<<16)+(c<<8)+d; indice += 4;
            atrb->info.Code.code_length=z;
            printf("tamanho do codigo: %d e %d\n", atrb->info.Code.code_length, z);

            if(atrb->info.Code.code_length>0){
                atrb->info.Code.code = (uint8_t *) malloc(atrb->info.Code.code_length * sizeof(uint8_t));
                for(i=0; i < atrb->info.Code.code_length; i++)
                    *(atrb->info.Code.code+i) = *(buffer+indice+i);
            }
            indice=indice+i;
            printf("alocou o codigo\n");
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.Code.exception_table_length = w;
            printf("tamanho da tabela de excecao: %d\n", w);
            if(atrb->info.Code.exception_table_length>0){
                atrb->info.Code.exception_table =(exception_table_type *) malloc(atrb->info.Code.exception_table_length * sizeof(exception_table_type));
            printf("tem excecao\n");
                for(exception_table_type * ex_tb = atrb->info.Code.exception_table;
                        ex_tb < (atrb->info.Code.exception_table + atrb->info.Code.exception_table_length); ex_tb++){
                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    ex_tb->start_pc = w;

                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    ex_tb->end_pc = w;

                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    ex_tb->handler_pc = w;

                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    ex_tb->catch_type = w;
                }
            }
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.Code.attributes_count=w;
printf("numero de atributos de atributos: %d\n", atrb->info.Code.attributes_count);
            if(atrb->info.Code.attributes_count>0){
                atrb->info.Code.attributes=(attribute_info*)malloc(sizeof(attribute_info)*atrb->info.Code.attributes_count);
                indice=setAttributes(buffer,atrb->info.Code.attributes,indice,atrb->info.Code.attributes_count,tag);
            }
            break;

        case SOURCE_FILE: // 8
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.SourceFile.sourcefile_index = w;
            break;

        case LINE_NUMBER_TABLE:
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.LineNumberTable.line_number_table_length = w;
            if(atrb->info.LineNumberTable.line_number_table_length>0){
                atrb->info.LineNumberTable.line_number_table=(line_number_table_type*)malloc(sizeof(line_number_table_type)*atrb->info.LineNumberTable.line_number_table_length);
                ln=atrb->info.LineNumberTable.line_number_table;
                for(i=0; i<atrb->info.LineNumberTable.line_number_table_length; i++){
                        a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                        ln->start_pc=w;
                        a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                        ln->line_number=w;
                        ln++;
                    }
            }
            break;
    }printf("corrompeu o lenght? %d\n", atrb->attribute_lenght);
    return indice;
}

void imprimeAttribute(uint16_t counter, attribute_info *f, tagConstante *t){
    uint16_t j, k;
    for(j=0; j<counter; j++){
            k=(f+j)->attribute_name_index;
            printf("\tatrb. %d \n\t\tattribute name index: %d\t", j, k);
            referenciaConstantePool(t,k);
            printf("\n\t\tattribute lenght: %d\n", (f+j)->attribute_lenght);
    }
}

char* referenciaConstantePool(tagConstante *u, uint16_t ref){
    uint16_t a, b;

    if((u+ref-1)->tag==TAG1){
        printf("%s",(u+ref-1)->itemString);
        return (u+ref-1)->itemString;
    }else{
        a=((u+ref-1)->valor.u2);
        if((u+a-1)->tag==TAG1){
            printf("%s",(u+a-1)->itemString);
            return (u+a-1)->itemString;
        }
    }

}

void imprimeAccessFlag(uint16_t a){
    if((a&ACC_PUBLIC)==ACC_PUBLIC)
        printf("public ");
    if((a&ACC_PRIVATE)==ACC_PRIVATE)
        printf("private ");
    if((a&ACC_PROTECTED)==ACC_PROTECTED)
        printf("protected ");
    if((a&ACC_STATIC)==ACC_STATIC)
        printf("static ");
    if((a&ACC_FINAL)==ACC_FINAL)
        printf("final ");
    if((a&ACC_SUPER)==ACC_SUPER)
        printf("super ");
    if((a&ACC_NATIVE)==ACC_NATIVE)
        printf("native ");
    if((a&ACC_ABSTRACT)==ACC_ABSTRACT)
        printf("abstract ");
    if((a&ACC_STRICT)==ACC_STRICT)
        printf("strict ");
}


