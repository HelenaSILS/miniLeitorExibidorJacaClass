#include "meuLeitor.h"
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
    constant_pool_info tc, *constantPool;

    if(argc<3){
        printf("Sem argumentos \n");
    }else
        printf("Argumentos OK\n");

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

void imprimePoolConstante(constant_pool_info *t, uint16_t counter){
    uint16_t a, b, i;
    for(i=0; i<(counter-1); i++){
            a=(((t+i)->cte.valor.u4) >> 16) & 0x0000ffff;
            b=((t+i)->cte.valor.u4) & 0x0000ffff;
            printf("[%u]\t",i);
            if( (t+i)->tag==0){
                printf("\tbits menos significativos: %x\n", (t+i)->cte.valor.u4);
            }if( (t+i)->tag==TAG1 ){
                printf("String: %s\n", (t+i)->cte.itemString);
            }if( (t+i)->tag==TAG3 ){
                printf("inteiro %d\n", (t+i)->cte.valor.u4);
            }if( (t+i)->tag==TAG4 ){
                printf("float %f\n", (float) (t+i)->cte.valor.u4);
            }if( (t+i)->tag==TAG5 ){
                printf("long \tbits mais significativo %x\n", (t+i)->cte.valor.u4);
            }if( (t+i)->tag==TAG6 ){
                printf("double \tbits mais significativo %x\n",  (t+i)->cte.valor.u4 );
            }if( (t+i)->tag==TAG7 ){
                printf("Class reference %d \t",  (t+i)->cte.valor.u2);
                referenciaConstantePool(t,(t+i)->cte.valor.u2,1); printf("\n");
            }if( (t+i)->tag==TAG8 ){
                printf("String reference %d \t",  (t+i)->cte.valor.u2);
                referenciaConstantePool(t,(t+i)->cte.valor.u2,1); printf("\n");
            }if( (t+i)->tag==TAG9 ){
                printf("Field Reference \tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t+i)->tag==TAG10 ){
                printf("Method Reference  \tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t+i)->tag==TAG11 ){
                printf("Interface method  \tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t+i)->tag==TAG12 ){
                printf("Name and type  \tdescriptor %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t+i)->tag==TAG15 ){
                a=(((t+i)->cte.valor.u4) >> 16) & 0x000000ff;
                printf("Method handle\tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t+i)->tag==TAG16 ){
                printf("Method type %.2d\n", (t+i)->cte.valor.u2);
            }if( (t+i)->tag==TAG18 ){
                printf("InvokeDynamic %.8d\n",  (t+i)->cte.valor.u4);
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
constant_pool_info* alocaPoolConstanteTable(uint16_t counter){
    constant_pool_info *t;
    t = (constant_pool_info*) malloc (sizeof(constant_pool_info)*counter);
    return t;

}

/** Retorna o indice do buffer, e preenche a tabela previamente alocada, sempre começa no indice 10 do buffer **/
int setConstantePool(char *buffer, constant_pool_info *t, unsigned int counter){
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
unsigned int getConstante(char *buffer, unsigned int indice, constant_pool_info *u, uint8_t *parte){
    //constant_pool_info u;
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
            u->cte.itemString = str;
            //printf("tag1, %s\n", u->itemString);
            return (1+2+z);
        }else{

        if (T==TAG4 || T==TAG9 || T==TAG10 || T==TAG11 || T==TAG12 || T==TAG18 || T == TAG3){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            d=*(buffer+indice+4);
            w=((a<<24) & 0xff000000)+((b<<16) & 0x00ff0000)+((c<<8) & 0x0000ff00) + (d & 0x000000ff);
            u->cte.valor.u4 = w;
            //printf("tag: %d, cte.valor(hex): %x\n", T, u->cte.valor.u4);
            return 5;
        }else{

        if (T==TAG15){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            w=((a<<16) & 0x00ff0000)+((b<<8) & 0x0000ff00)+(c & 0x000000ff);
            u->cte.valor.u4 = w;
            //printf("tag: %d, cte.valor(hex): %x\n", T, u->cte.valor.u4);
            return 4;
        }else{

        if (T==TAG5 || T==TAG6){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            c=*(buffer+indice+3);
            d=*(buffer+indice+4);
            w=((a<<24) & 0xff000000)+((b<<16) & 0x00ff0000)+((c<<8) & 0x0000ff00) + (d & 0x000000ff);
            u->cte.valor.u4 = w;
            *parte=1;
            //printf("tag: %d, cte.valor(hex): %x\n", T, u->cte.valor.u4);
            return 5;
        }else{

          if(T==TAG7 || T==TAG8 || T==TAG16){
            a=*(buffer+indice+1);
            b=*(buffer+indice+2);
            y=((a<<8)&0x0000ff00)+(b&0x000000ff);
            u->cte.valor.u2 = y;
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
        u->cte.valor.u4 = w;
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
    t=(field_info*) calloc (sizeof(field_info), (counter+1));
    return t;
}

/** preenche a tabela de fields já alocada e devolve o próximo byte do buffer, recebendo o byte em que começa **/
unsigned int setField (char *buffer, field_info *field, unsigned int indice, uint16_t counter, constant_pool_info *tag){
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
unsigned int getField(char *buffer, unsigned int indice, field_info *u, constant_pool_info *tag){
    //constant_pool_info u;
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
    atrb = (attribute_info*)calloc(u->attribute_count, sizeof(attribute_info));
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

void imprimeField (uint16_t counter, field_info *f, constant_pool_info *t){
    uint16_t i, j;
    attribute_info *atrb;
    for(i=0; i< counter; i++){
        printf("field %d:\taccess flag: %d\t",i, (f+i)->access_flag);
        imprimeAccessFlag((f+i)->access_flag);
        printf("\n\t\tname:\t\t\t");
        referenciaConstantePool(t,(f+i)->name_index,1);
        printf("\n\t\t");
        printf("descriptor:\t\t");
        referenciaConstantePool(t, (f+i)->descriptor_index,1);
        printf("\n\t\tattributes counter: %d\n\t", (f+i)->attribute_count,1);
        printf("ATRIBUTOS: \n");
        imprimeAttribute((f+i)->attribute_count, (f+i)->attributes, t);
        printf("\n");
    }
}

/** preenche a tabela de attributes já alocada e devolve o próximo byte do buffer, recebendo o byte em que começa **/
unsigned int setAttributes (char *buffer, attribute_info *atrb, unsigned int indice, uint16_t counter, constant_pool_info *tag){
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
unsigned int getAttribute(char *buffer, unsigned int indice, attribute_info *u, constant_pool_info *t){
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

    return indice;
}

uint8_t getTipoAttribute(attribute_info *u, constant_pool_info *t){
    char *nome;
    nome=referenciaConstantePool(t, u->attribute_name_index,0);
    if(strcmp(nome, "Code")==0){
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

uint32_t estruturaAttribute (char *buffer, unsigned int indice, uint8_t tipo, attribute_info *atrb, constant_pool_info *tag){
    uint8_t a, b, c, d;
    uint16_t w;
    uint32_t z, i;
    line_number_table_type *ln;

    switch (tipo){
    case CODE:
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.Code.max_stack=w;


            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.Code.max_locals=w;


            a=*(buffer+indice); b=*(buffer+indice+1); c=*(buffer+indice+2); d=*(buffer+indice+3);
            z=(a<<24)+(b<<16)+(c<<8)+d; indice += 4;
            atrb->info.Code.code_length=z;

            if(atrb->info.Code.code_length>0){
                atrb->info.Code.code = (uint8_t *) calloc(atrb->info.Code.code_length, sizeof(uint8_t));
                for(i=0; i < atrb->info.Code.code_length; i++)
                    *(atrb->info.Code.code+i) = *(buffer+indice+i);
            }
            indice=indice+i;

            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.Code.exception_table_length = w;

            if(atrb->info.Code.exception_table_length>0){
                atrb->info.Code.exception_table =(exception_table_type *) calloc(atrb->info.Code.exception_table_length, sizeof(exception_table_type));

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
            }else{
		atrb->info.Code.exception_table=NULL;
		}
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.Code.attributes_count=w;

            if(atrb->info.Code.attributes_count>0){
                atrb->info.Code.attributes=(attribute_info*)calloc(sizeof(attribute_info), atrb->info.Code.attributes_count);
                indice=setAttributes(buffer,atrb->info.Code.attributes,indice,atrb->info.Code.attributes_count,tag);
            }
            break;


        case SOURCE_FILE:
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.SourceFile.sourcefile_index = w;
            break;

        case CONSTANT_VALUE: // 0
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.ConstantValue.constantvalue_index = w;
            break;

        case EXCEPTIONS: // 3
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.Exceptions.number_of_exceptions = w;
            atrb->info.Exceptions.exception_index_table = (uint16_t *) calloc (atrb->info.Exceptions.number_of_exceptions, sizeof(uint16_t));
            for(uint16_t * excp = atrb->info.Exceptions.exception_index_table;
                        excp < (atrb->info.Exceptions.exception_index_table + atrb->info.Exceptions.number_of_exceptions); excp++){
                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    *excp=w;
                    }
            break;

        case INNER_CLASSES: // 4
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.InnerClasses.number_of_classes = w;
            atrb->info.InnerClasses.classes = (classes_type *) calloc(atrb->info.InnerClasses.number_of_classes, sizeof(classes_type));
            for(classes_type * ct = atrb->info.InnerClasses.classes;
                        ct < (atrb->info.InnerClasses.classes + atrb->info.InnerClasses.number_of_classes); ct++){
                a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                ct->inner_class_info_index = w;

                a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                ct->outer_class_info_index = w;

                a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                ct->inner_name_index = w;

                a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                ct->inner_class_access_flags = w;
                }
            break;

        case LOCAL_VARIABLE_TABLE:
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.LocalVariableTable.local_variable_table_length = w;
            if(atrb->info.LocalVariableTable.local_variable_table_length>0){
                atrb->info.LocalVariableTable.local_variable_table = (local_variable_table_type *) calloc(atrb->info.LocalVariableTable.local_variable_table_length, sizeof(local_variable_table_type));

                for(local_variable_table_type * lv = atrb->info.LocalVariableTable.local_variable_table;
                                lv < (atrb->info.LocalVariableTable.local_variable_table +
                                     atrb->info.LocalVariableTable.local_variable_table_length); lv++){
                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    lv->start_pc = w;

                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    lv->length = w;

                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    lv->name_index = w;

                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    lv->descriptor_index = w;

                    a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                    lv->index = w;
                    }
                }
                break;

        case SYNTHETIC:
                break;

        case LINE_NUMBER_TABLE:
            a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
            atrb->info.LineNumberTable.line_number_table_length = w;
            if(atrb->info.LineNumberTable.line_number_table_length>0){
                atrb->info.LineNumberTable.line_number_table=(line_number_table_type*)malloc(sizeof(line_number_table_type)*atrb->info.LineNumberTable.line_number_table_length);
                //ln=atrb->info.LineNumberTable.line_number_table;
                for(ln = atrb->info.LineNumberTable.line_number_table;
                            ln < (atrb->info.LineNumberTable.line_number_table + atrb->info.LineNumberTable.line_number_table_length); ln++){
                        a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                        ln->start_pc=w;
                        a=*(buffer+indice); b=*(buffer+indice+1); w=(a<<8)+b; indice += 2;
                        ln->line_number=w;
                    }
            }else{
		atrb->info.LineNumberTable.line_number_table=NULL;
		}
            break;

        default:
            break;


    }
    return indice;
}

void imprimeAttribute(uint16_t counter, attribute_info *f, constant_pool_info *t){
    uint16_t a, b, j, k;
    uint64_t dd;
    uint8_t tipo;
    for(j=0; j<counter; j++){
            k=(f+j)->attribute_name_index;
            printf("\tatrb. %d \n\t\tattribute name index: #%d\t", j, k);
            referenciaConstantePool(t,k,1);
            printf("\n\t\tattribute lenght: %d\n", (f+j)->attribute_lenght);
            tipo=getTipoAttribute(f, t);

	    switch (tipo){
		    case CODE:
            printf("\t\t\tCode:\n");
			printf("\t\t\ttamanho do max stack: %d\n", f->info.Code.max_stack);
			printf("\t\t\ttamanho do max locals: %d\n", f->info.Code.max_locals);
			printf("\t\t\ttamanho do codigo: %d\nCode propriamente dito:\n\toffset:\tbytecode\n", f->info.Code.code_length);
			imprimeCode (t, f->info.Code.code, f->info.Code.code_length);

			printf("\t\t\ttamanho da tabela de excecao: %d\n", f->info.Code.exception_table_length);
			printf("\t\t\tnumero de atributos do atributo: %d\n", f->info.Code.attributes_count);
			printf("\t\t\tATRIBUTO(S) DO ATRIBUTO CODE:\n");
			if(f->info.Code.attributes_count>0){
				imprimeAttribute(f->info.Code.attributes_count, f->info.Code.attributes, t);
				}
			break;

		    case SOURCE_FILE:
			printf("\t\t\tSource file index: #%d ", f->info.SourceFile.sourcefile_index);

			referenciaConstantePool(t, f->info.SourceFile.sourcefile_index,1);
			printf("\n");
			break;

            case CONSTANT_VALUE:
            printf("\t\t\tConstante Value: #%d ", f->info.ConstantValue.constantvalue_index);
            a=(((t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u4) >> 16) & 0x0000ffff;
            b=((t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u4) & 0x0000ffff;
            if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG1 ){
                printf("String: %s\n", (t-1+(f->info.ConstantValue.constantvalue_index))->cte.itemString);
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG3 ){
                printf("inteiro %d\n", (t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u4);
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG4 ){
                printf("float %f\n", (float) (t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u4);
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG5 ){
                dd=((t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u4);
                dd=dd<<32;
                printf("long \tbits mais significativo %l\n", (long) ( dd |((t+f->info.ConstantValue.constantvalue_index)->cte.valor.u4)) );
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG6 ){
                dd=((t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u4);
                dd=dd<<32;
                printf("double \tbits mais significativo %lf\n",  (double)( dd |((t+f->info.ConstantValue.constantvalue_index)->cte.valor.u4)) );
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG7 ){
                printf("Class reference %d \t",  (t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u2);
                referenciaConstantePool(t,(t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u2,1); printf("\n");
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG8 ){
                printf("String reference %d \t",  (t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u2);
                referenciaConstantePool(t,(t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u2,1); printf("\n");
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG9 ){
                printf("Field Reference \tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG10 ){
                printf("Method Reference  \tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG11 ){
                printf("Interface method  \tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG12 ){
                printf("Name and type  \tdescriptor %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG15 ){
                a=(((t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u4) >> 16) & 0x000000ff;
                printf("Method handle\tclasse: %d\t\tnome %d\n\t\t\t\t", a, b);
                referenciaConstantePool(t,a,1); printf("\t");
                referenciaConstantePool(t,b,1); printf("\n");
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG16 ){
                printf("Method type %.2d\n", (t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u2);
            }if( (t-1+(f->info.ConstantValue.constantvalue_index))->tag==TAG18 ){
                printf("InvokeDynamic %.8d\n",  (t-1+(f->info.ConstantValue.constantvalue_index))->cte.valor.u4);
            }

            printf("\n");
            break;

            case EXCEPTIONS:
            printf("\t\t\tExceptions \n");
            printf("\t\t\ttamanho da tabela de excecoes: %d\n", f->info.Exceptions.number_of_exceptions);
            if(f->info.Exceptions.number_of_exceptions>0){
                for(uint16_t *ex=f->info.Exceptions.exception_index_table;
                    ex<(f->info.Exceptions.exception_index_table+f->info.Exceptions.number_of_exceptions); ex++){
                    printf("\t\t\tindex de excecao:  #d ");
                    referenciaConstantePool(t, *f->info.Exceptions.exception_index_table,1);
                    printf("\n");
                }
            }
            break;

            case INNER_CLASSES:
            printf("\t\t\tInner Class(es)\n");
            printf("\t\t\ttamanho da tabela de classes internas: %d \n", f->info.InnerClasses.number_of_classes);
            if(f->info.InnerClasses.number_of_classes>0){
                for(classes_type *cl=f->info.InnerClasses.classes; cl<(f->info.InnerClasses.number_of_classes+f->info.InnerClasses.classes); cl++){
                    printf("\t\t\tinner class info index: #%d ");
                    referenciaConstantePool(t, cl->inner_class_info_index, 1); printf("\n");
                    printf("\t\t\touter class info index: #%d ");
                    referenciaConstantePool(t, cl->outer_class_info_index, 1); printf("\n");
                    printf("\t\t\tinner name index: #%d ");
                    referenciaConstantePool(t, cl->inner_name_index, 1); printf("\n");
                    printf("\t\t\tinner class access flags: #%d ");
                    imprimeAccessFlag(cl->inner_class_access_flags); printf("\n");

                }
            }
            break;

            case LOCAL_VARIABLE_TABLE:
            printf("\t\t\tLocal Variable Table\n");
			printf("\t\t\ttamanho da tabela de variaveis locais: %d\n", f->info.LocalVariableTable.local_variable_table_length);
			if(f->info.LocalVariableTable.local_variable_table_length>0){
				for(local_variable_table_type *lv=f->info.LocalVariableTable.local_variable_table;
					lv<(f->info.LocalVariableTable.local_variable_table_length+f->info.LocalVariableTable.local_variable_table); lv++){
					printf("\t\t\tStar pc: %d\t",lv->start_pc);
					printf("\t\t\tLenght: %d\n",lv->length);
					printf("\t\t\tName index: #%d ",lv->name_index);
					referenciaConstantePool(t, lv->name_index, 1); printf("\n");
					printf("\t\t\tDescritor: %d ",lv->descriptor_index);
					referenciaConstantePool(t, lv->descriptor_index, 1); printf("\n");
					printf("\t\t\tIndex: %d \n",lv->index);
					referenciaConstantePool(t, lv->index, 1); printf("\n");
				}
			}
			break;

		    case LINE_NUMBER_TABLE:
            printf("\t\t\tLine Number Table\n");
			printf("\t\t\ttamanho da tabela de line number: %d\n", f->info.LineNumberTable.line_number_table_length);
			if(f->info.LineNumberTable.line_number_table_length>0){
				for(line_number_table_type *ln=f->info.LineNumberTable.line_number_table;
					ln<(f->info.LineNumberTable.line_number_table_length+f->info.LineNumberTable.line_number_table); ln++){

					printf("\t\t\tStar pc: %d\t",ln->start_pc);
					printf("\t\t\tLine number: %d\n",ln->line_number);
				}
			}
			break;

    		}
    }
}

char* referenciaConstantePool(constant_pool_info *u, uint16_t ref, uint8_t print){
    uint16_t a, b;

    if((u+ref-1)->tag==TAG1){
        if(print)
            printf("%s",(u+ref-1)->cte.itemString);
        return (u+ref-1)->cte.itemString;
    }else{
        a=((u+ref-1)->cte.valor.u2);
        if((u+a-1)->tag==TAG1){
            if(print)
                printf("%s",(u+a-1)->cte.itemString);
            return (u+a-1)->cte.itemString;
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

void imprimeCode (constant_pool_info *t, uint8_t *byte, uint32_t lenght){
    uint32_t offset;
    uint16_t a, b;
    uint64_t d;

    for(offset=0; offset<lenght; offset++){
        printf("\t%d\t", offset);
        switch(*(byte+offset)){
        case aaload:
            printf("aaload"); break;
        case aastore:
            printf("aastore"); break;
        case aconst_null:
            printf("aconst_null"); break;
        case aload_0:
            printf("aload_0"); break;
        case aload_1:
            printf("aload_1");break;
        case aload_2:
            printf("aload_2");break;
        case aload_3:
            printf("aload_3"); break;
        case areturn:
            printf("areturn"); break;
        case arraylength:
            printf("arraylength"); break;
        case astore_0:
            printf("astore_0"); break;
        case astore_1:
            printf("astore_1"); break;
        case astore_2:
            printf("astore_2"); break;
        case astore_3:
            printf("astore_3"); break;
        case athrow:
            printf("athrow"); break;
        case baload:
            printf("baload"); break;
        case bastore:
            printf("bastore"); break;
        case breakpoint:
            printf("breakpoint"); break;
        case caload:
            printf("caload"); break;
        case castore:
            printf("castore"); break;
        case d2f:
            printf("d2f"); break;
        case d2i:
            printf("d2i"); break;
        case d2l:
            printf("d21"); break;
        case dadd:
            printf("dadd"); break;
        case daload:
            printf("daload"); break;
        case dastore:
            printf("dastore"); break;
        case dcmpg:
            printf("dcmpg"); break;
        case dcmpl:
            printf("dcmpl"); break;
        case dconst_0:
            printf("dconst_0"); break;
        case dconst_1:
            printf("dconst_1"); break;
        case ddiv:
            printf("ddiv"); break;
        case dload_0:
            printf("dload_0"); break;
        case dload_1:
            printf("dload_1"); break;
        case dload_2:
            printf("dload_2"); break;
        case dload_3:
            printf("dload_3"); break;
        case dmul:
            printf("dmul"); break;
        case dneg:
            printf("dneg"); break;
        case drem:
            printf("drem"); break;
        case dreturn:
            printf("dreturn"); break;
        case dstore_0:
            printf("dstore_0"); break;
        case dstore_1:
            printf("dstore_1"); break;
        case dstore_2:
            printf("dstore_2"); break;
        case dstore_3:
            printf("dstore_3"); break;
        case dsub:
            printf("dsub"); break;
        case dup:
            printf("dup"); break;
        case dup_x1:
            printf("dup_x1"); break;
        case dup_x2:
            printf("dup_x2"); break;
        case dup2:
            printf("dup2"); break;
        case dup2_x1:
            printf("dup2_x1"); break;
        case dup2_x2:
            printf("dup2_x2"); break;
        case f2d:
            printf("f2d"); break;
        case f2i:
            printf("f2i"); break;
        case f2l:
            printf("f21"); break;
        case fadd:
            printf("fadd"); break;
        case faload:
            printf("fload"); break;
        case fastore:
            printf("fastore"); break;
        case fcmpg:
            printf("fcmpg"); break;
        case fcmpl:
            printf("fcmpl"); break;
        case fconst_0:
            printf("fconst_0"); break;
        case fconst_1:
            printf("fconst_1"); break;
        case fconst_2:
            printf("fconst_2"); break;
        case fdiv:
            printf("fdiv"); break;
        case fload_0:
            printf("fload_0"); break;
        case fload_1:
            printf("fload_1"); break;
        case fload_2:
            printf("fload_2"); break;
        case fload_3:
            printf("fload_3"); break;
        case fmul:
            printf("fmul"); break;
        case fneg:
            printf("fneg"); break;
        case frem:
            printf("frem"); break;
        case freturn:
            printf("freturn"); break;
        case fstore_0:
            printf("fstore"); break;
        case fstore_1:
            printf("fstore_1"); break;
        case fstore_2:
            printf("fstore_2"); break;
        case fstore_3:
            printf("fstore_3"); break;
        case fsub:
            printf("fsub"); break;
        case i2b:
            printf("i2b"); break;
        case i2c:
            printf("i2c"); break;
        case i2d:
            printf("i2d"); break;
        case i2f:
            printf("i2f"); break;
        case i2l:
            printf("i21"); break;
        case i2s:
            printf("i2s"); break;
        case iadd:
            printf("iadd"); break;
        case iaload:
            printf("iaload"); break;
        case iand:
            printf("iand"); break;
        case iastore:
            printf("iastore"); break;
        case iconst_m1:
            printf("iconst_ml"); break;
        case iconst_0:
            printf("iconst_0"); break;
        case iconst_1:
            printf("iconst_1"); break;
        case iconst_2:
            printf("iconst_2"); break;
        case iconst_3:
            printf("iconst_3"); break;
        case iconst_4:
            printf("iconst_4"); break;
        case iconst_5:
            printf("iconst_5"); break;
        case idiv:
            printf("idiv"); break;
        case iload_0:
            printf("iload_0"); break;
        case iload_1:
            printf("iload_1"); break;
        case iload_2:
            printf("iload_2"); break;
        case iload_3:
            printf("iload_3"); break;
        case impdep1:
            printf("impdepl"); break;
        case impdep2:
            printf("impdep2"); break;
        case imul:
            printf("imul"); break;
        case ineg:
            printf("ineg"); break;
        case ior:
            printf("ior"); break;
        case irem:
            printf("irem"); break;
        case ireturn:
            printf("ireturn"); break;
        case ishl:
            printf("ishl"); break;
        case ishr:
            printf("ishr"); break;
        case istore_0:
            printf("istore_0"); break;
        case istore_1:
            printf("istore_1"); break;
        case istore_2:
            printf("istore_2"); break;
        case istore_3:
            printf("istore_3"); break;
        case isub:
            printf("isub"); break;
        case iushr:
            printf("iushr"); break;
        case ixor:
            printf("ixor"); break;
        case l2d:
            printf("l2d"); break;
        case l2f:
            printf("l2f"); break;
        case l2i:
            printf("l2i"); break;
        case ladd:
            printf("ladd"); break;
        case laload:
            printf("laload"); break;
        case land:
            printf("land"); break;
        case lastore:
            printf("lastore"); break;
        case lcmp:
            printf("lcmp"); break;
        case lconst_0:
            printf("lconst_0"); break;
        case lconst_1:
            printf("lconst_1"); break;
        case ldiv_:
            printf("ldiv_"); break;
        case lload_0:
            printf("lload_0"); break;
        case lload_1:
            printf("lload_1"); break;
        case lload_2:
            printf("lload_2"); break;
        case lload_3:
            printf("lload_3"); break;
        case lmul:
            printf("lmul"); break;
        case lneg:
            printf("lneg"); break;
        case lor:
            printf("lor"); break;
        case lrem:
            printf("lrem"); break;
        case lreturn:
            printf("lreturn"); break;
        case lshl:
            printf("lshl"); break;
        case lshr:
            printf("lshr"); break;
        case lstore_0:
            printf("lstore_0"); break;
        case lstore_1:
            printf("lstore_1"); break;
        case lstore_2:
            printf("lstore_2"); break;
        case lstore_3:
            printf("lstore_3"); break;
        case lsub:
            printf("lsub"); break;
        case lushr:
            printf("lushr"); break;
        case lxor:
            printf("lxor"); break;
        case monitorenter:
            printf("monitorenter"); break;
        case monitorexit:
            printf("monitorexit"); break;
        case nop:
            printf("nop"); break;
        case pop:
            printf("pop"); break;
        case pop2:
            printf("pop2"); break;
        case return_:
            printf("return_"); break;
        case saload:
            printf("saload"); break;
        case sastore:
            printf("sastore"); break;
        case swap: //======================================== ? ============================
            printf("swap"); break;
        case aload:
            printf("aload"); break;
        case astore:
            printf("astore"); break;
        case dload:
            printf("dload"); break;
        case dstore:
            printf("dstore"); break;
        case fload:
            printf("fload"); break;
        case fstore:
            printf("fstore"); break;
        case iload:
            printf("iload"); break;
        case istore:
            printf("istore"); break;
        case lload:
            printf("lload"); break;
        case lstore:
            printf("lstore"); break;
        case ret:
            printf("ret ao returnAddress: %x", *(byte+offset+1));
            offset++;
            break; // ====================== incrementa o iterador 2 vezes

        case bipush:
            printf("bipush e sign extension: %d", *(byte+offset+1));
            offset++;
            break; // ====================== incrementa o iterador 2 vezes

        case newarray:
            printf("newarray");
            switch(*(byte+offset+1)){
                                // printa o tipo de array
                case TAG4:
                    printf(" (boolean)\n");break;
                case TAG5:
                    printf(" (char)\n");break;
                case TAG6:
                     printf(" (float)\n"); break;
                case TAG7:
                     printf(" (double)\n");break;
                case TAG8:
                     printf(" (byte)\n");break;
                case TAG9:
                     printf(" (short)\n"); break;
                case TAG10:
                     printf(" (int)\n"); break;
                case TAG11:
                     printf(" (long)\n"); break;
                default:
                     printf("[ERRO] newarray aplicado tipo invalido.\n"); break;
                            }
                offset++;
                break;

                        //  CONSTANTE pool index (integer, float, string) (uint8_t)
        case ldc:
             printf("ldc, index: #%d ",*(byte+offset+1));
             referenciaConstantePool(t, *(byte+offset+1),1); //int or float, or a reference in run time constant pool
             offset++;
             break;

        case anewarray:
            printf("anewarray: #%d ");
            a=*(byte+offset+1);
            b=*(byte+offset+2);
            a=a<<8; a=a|b;
            referenciaConstantePool(t,a,1);
            offset=+2;
            break;
        case checkcast:
            printf("checkcast"); break;
        case instanceof:
            printf("instanceof"); break;
        case new:
            printf("new index1: #%d \t index2: #%d", *(byte+offset+1), *(byte+offset+1));
            break;
        case goto_:
            printf("goto_"); break;
        case if_acmpeq:
            printf("if_acmpeq"); break;
        case if_acmpne:
            printf("if_acmpne"); break;
        case if_icmpeq:
            printf("if_icmpeq"); break;
        case if_icmpge:
            printf("if_icmpge"); break;
        case if_icmpgt:
            printf("if_icmpgt"); break;
        case if_icmple:
            printf("if_icmple"); break;
        case if_icmplt:
            printf("if_icmplt"); break;
        case if_icmpne:
            printf("if_icmpne"); break;
        case ifeq:
            printf("ifeq"); break;
        case ifge:
            printf("ifge"); break;
        case ifgt:
            printf("ifgt"); break;
        case ifle:
            printf("ifle"); break;
        case iflt:
            printf("iflt"); break;
        case ifne:
            printf("ifne"); break;
        case ifnonnull:
            printf("ifnonnull"); break;
        case ifnull:
            printf("ifnull"); break;
        case jsr:
            printf("jst branch: %d", (*(byte+offset+1)<<8|*(byte+offset+2)));
            offset=+2;
            break;
        case ldc_w:
            printf("ldc_w ");
            a=*(byte+offset+1);
            b=*(byte+offset+2);
            a=a<<8; a=a|b;
            if((t+a-1)->tag==TAG3)
                printf("int %d",(t+a-1)->cte.valor.u4);
            else{
                if((t+a-1)->tag==TAG4)
                    printf("float %f",(t+a-1)->cte.valor.u4);
                else
                    referenciaConstantePool(t,a,1);
            }
            offset=offset+2;
            break;
        case getfield:
            printf("getfield"); break;
        case getstatic:
            printf("getstatic"); break;
        case invokespecial:
            printf("invokespecial");
            a=*(byte+offset+1); b= *(byte+offset+2);
            a=a<<8; a=a|b;
            if((t+a-1)->tag==TAG10 || (t+a-1)->tag==TAG11){   //method ou interface method
                b=(((t+a-1)->cte.valor.u4) >> 16) & 0x000000ff;
                a=((t+a-1)->cte.valor.u4) & 0x000000ff;
                printf("classe: ", b);
                referenciaConstantePool(t,b,1);
                printf("\tnome: ");
                referenciaConstantePool(t,a,1);
            }
            offset=offset+2;
            break;
        case invokestatic:
            printf("invokestatic");
            a=*(byte+offset+1); b= *(byte+offset+2);
            a=a<<8; a=a|b;
            if((t+a-1)->tag==TAG10 || (t+a-1)->tag==TAG11){   //method ou interface method
                b=(((t+a-1)->cte.valor.u4) >> 16) & 0x000000ff;
                a=((t+a-1)->cte.valor.u4) & 0x000000ff;
                printf("classe: ", b);
                referenciaConstantePool(t,b,1);
                printf("\tnome: ");
                referenciaConstantePool(t,a,1);
            }
            offset=offset+2;
            break;
        case invokevirtual:
            printf("invokevirtual ");
            a=*(byte+offset+1); b= *(byte+offset+2);
            a=a<<8; a=a|b;
            if((t+a-1)->tag==TAG10){                        //method
                b=(((t+a-1)->cte.valor.u4) >> 16) & 0x000000ff;
                a=((t+a-1)->cte.valor.u4) & 0x000000ff;
                printf("classe: ", b);
                referenciaConstantePool(t,b,1);
                printf("\tnome: ");
                referenciaConstantePool(t,a,1);
            }
            offset=offset+2;
            break;
        case putfield:
            printf("putfield");
            a=*(byte+offset+1); b= *(byte+offset+2);
            a=a<<8; a=a|b;
            if((t+a-1)->tag==9){                        //field
                b=(((t+a-1)->cte.valor.u4) >> 16) & 0x000000ff;
                a=((t+a-1)->cte.valor.u4) & 0x000000ff;
                printf("classe: ", b);
                referenciaConstantePool(t,b,1);
                printf("\tnome: ");
                referenciaConstantePool(t,a,1);
            }
            offset=offset+2;
            break;
        case putstatic:
            printf("putstatic ");
            a=*(byte+offset+1); b= *(byte+offset+2);
            a=a<<8; a=a|b;
            if((t+a-1)->tag==9){                        //field
                b=(((t+a-1)->cte.valor.u4) >> 16) & 0x000000ff;
                a=((t+a-1)->cte.valor.u4) & 0x000000ff;
                printf("classe: ", b);
                referenciaConstantePool(t,b,1);
                printf("\tnome: ");
                referenciaConstantePool(t,a,1);
            }
            offset=offset+2;
            break;
        case ldc2_w:
            printf("ldc2_w: ");
            a=*(byte+offset+1); b= *(byte+offset+2);
            a=a<<8; a=a|b;
            d=(t+a-1)->cte.valor.u4;
            if((t+a-1)->tag==TAG6) //double
                printf("%lf", (double) (d<<32 | (t+a)->cte.valor.u4));
            if((t+a-1)->tag==TAG5) //long
                printf("%ld", (long) (d<<32 | (t+a)->cte.valor.u4));
            offset=offset+2;
            break;
        case sipush:
            printf("sipush #%d", (*(byte+offset+1)<<8 | *(byte+offset+2)));
            offset=offset+2;
            break;
        case iinc: // 1 byte 1 byte
            printf("iinc index: #%d \t const: %d", *(byte+offset+1), *(byte+offset+2));
            offset=offset+2;
            break;
        case multianewarray:
            printf("multianewarray dimensao: %d ", *(byte+offset+3)); //class, array, interface
            a=(*(byte+offset+1)<<8 | *(byte+offset+2));
            if((t+a-1)->tag==TAG7)
                printf("class ");
            if((t+a-1)->tag==TAG11)
                printf("interface ");
            if((t+a-1)->tag==TAG1)
                printf("string ");
            referenciaConstantePool(t, a, 1);
            offset=offset+3;
            break;
        case goto_w:
            printf("goto_w %l", (*(byte+offset+1)<<24 | *(byte+offset+2)<<16 | *(byte+offset+3)<<8 | *(byte+offset+4)));
            offset=offset+4;
            break;
        case jsr_w:
            printf("jsr_w %d", (*(byte+offset+1)<<8 | *(byte+offset+2)));
            offset=offset+2;
            break;
        case invokeinterface:
            printf("invokeinterface ");
            a=(*(byte+offset+1)<<8 | *(byte+offset+2));
            referenciaConstantePool(t,a,1);
            printf("count: %d byte final: %d",*(byte+offset+3),*(byte+offset+4) );
            offset=offset+4;
            break;
        case invokedynamic:
            printf("invokedynamic");
                        break;

                        // 4 ou + bytes
                        //  <0-3 byte pad>; default (s4); n (s4); key1, offobtain_label1 ... keyn, offobtain_labeln (s4)
        case lookupswitch:
            printf("lookupswitch");
                        break;
                        //  <0-3 byte pad>; default (s4); low (s4); high (s4); label1 ... labeln (s4)
        case tableswitch:
            printf("tableswitch");
            break;
                        // 3 ou 5 bytes
        case wide:
             printf("wide ");
                        switch(*(byte+offset+1)){
                            case aload: printf("aload"); break;
                            case dload: printf("dload"); break;
                            case iload: printf("iload"); break;
                            case fload: printf("fload"); break;
                            case lload: printf("lload"); break;
                            case astore: printf("astore"); break;
                            case dstore: printf("dstore"); break;
                            case istore: printf("istore"); break;
                            case fstore: printf("fstore"); break;
                            case lstore: printf("lstore"); break;
                            case ret:
                                printf("ret");
                                offset +=4;
                                break;
                            case iinc:
                                printf("iinc");
                                offset +=6;
                                break;
                            default:
                                printf("[ERRO] wide aplicado a uma instrucao invalida\n");
                        }

        default: break;
        }
    printf("\n");
    }
}


