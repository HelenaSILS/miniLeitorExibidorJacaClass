#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include"opcodes.h"
#define TAG1 1 //string
#define TAG3 3 //int
#define TAG4 4 //float
#define TAG5 5 //long
#define TAG6 6 //double
#define TAG7 7 //class
#define TAG8 8 //string reference
#define TAG9 9 //field reference
#define TAG10 10 //method reference
#define TAG11 11 //interface method
#define TAG12 12 //name and type
#define TAG15 15 //method handle
#define TAG16 16 //method type
#define TAG18 18 //invoke dynamic

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
	uint16_t	outer_class_info_index;
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

struct constantePool_info{
    int8_t      tag;
    union{;
        constante   valor;
        char        *itemString;
    }cte;
} typedef constant_pool_info;

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
    constant_pool_info *constantPool;
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
unsigned int getConstante(char *, unsigned int, constant_pool_info *, uint8_t * );
constant_pool_info* alocaPoolConstanteTable(uint16_t );
int setConstantePool(char *, constant_pool_info *, unsigned );
void imprimePoolConstante(constant_pool_info *, uint16_t );

char* referenciaConstantePool(constant_pool_info *, uint16_t, uint8_t );

void imprimeAccessFlag(uint16_t);

uint16_t* alocaInterfaces(uint16_t );
unsigned int setInterface (char *, uint16_t *, unsigned int , uint16_t );

unsigned int setAttributes (char *, attribute_info *, unsigned int, uint16_t, constant_pool_info* );
unsigned int getAttribute(char *, unsigned int , attribute_info *,constant_pool_info *);
uint8_t getTipoAttribute(attribute_info *, constant_pool_info *);
uint32_t estruturaAttribute (char *, unsigned int, uint8_t, attribute_info *, constant_pool_info *);
attribute_info* alocaAttribute(uint16_t );
void imprimeAttribute(uint16_t, attribute_info*, constant_pool_info*);
void imprimeCode (constant_pool_info *, uint8_t *, uint32_t );

field_info* alocaFields(uint16_t );
unsigned int setField (char *, field_info *, unsigned int , uint16_t, constant_pool_info* );
unsigned int getField(char *, unsigned int , field_info *, constant_pool_info *);
void imprimeField (uint16_t, field_info *, constant_pool_info*);
