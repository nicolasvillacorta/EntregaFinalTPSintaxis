#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct
{
  char identificador[32];
  TOKEN t;
}TS;

typedef struct
{
  TOKEN tipo;
  int valor;
  char nombre[32];
}REG_EXPRESION;

TS tabla[1000] = {{"inicio", INICIO},{"fin", FIN}, {"leer", LEER}, {"escribir", ESCRIBIR}, {"$",99}};

REG_EXPRESION ProcesarId (void)
{
  REG_EXPRESION regid;
  Chequear(buffer);
  regid.tipo = ID;
  strcpy(regid.nombre, buffer);

  return regid;
}

void Chequear (char*s)
{
    TOKEN tok;
    if (!Buscar(s,&tok))
    {
     Colocar(s);
     Generar("Declara", s, "Entera", "");
    }
}

int Buscar (char* s,TOKEN* tok)
{
  int i=0;

  while(strcmp("$",tabla[i].identificador))
  {
    if(!strcmp(s,tabla[i].identificador))
    {
        *tok=tabla[i].t;
        return 1;
    }

    i++;
  }

  return 0;
}

void Colocar(char*s)
{
    int i=4;

    while(strcmp("$",tabla[i].identificador))
    {
        i++;
    }

    if(i<999)
    {
		strcpy(tabla[i].identificador,s);
        tabla[i].t=ID;
        strcpy(tabla[i+1].identificador,"$");
        tabla[i+1].t=99;
    }
}

void Generar(char* op, char* a1, char* a2, char* a3)
{
    if(!strcmp(op,"Detiene"))fputs(op,b);

    else
	{
        if((!strcmp(op,"Declara"))||(!strcmp(op,"Almacena"))||(!strcmp(op,"Write"))||(!strcmp(op,"Read")))
	    {
            fputs(op,b);
            fputs(" ",b);
            fputs(a1,b);
            fputs(",",b);
            fputs(a2,b);
            fputs("\n",b);
        }

        else
	    {
            fputs(op,b);
            fputs(" ",b);
            fputs(a1,b);
            fputs(",",b);
            fputs(a2,b);
            fputs(",",b);
            fputs(a3,b);
            fputs("\n",b);
        }
    }
}

char* Extraer(REG_EXPRESION* registro)
{
  return registro->nombre;
}

void Leer (REG_EXPRESION in)
{
  Generar("Read", in.nombre, "Entera", "");
}

void Primaria(REG_EXPRESION* reg)
{
  TOKEN tok = ProximoToken();

  switch ( tok )
  {
	case ID: /* <Primaria> -> <Identificador> */
	  Identificador(reg);
	  break;

	case CONSTANTE: /* <Primaria> -> CONSTANTE #procesar_cte */
	  Match(CONSTANTE);
	  *reg=ProcesarCte();
	  break;

	case PARENIZQUIERDO: /* <Primaria> -> PARENIZQUIERDO <Expresion> PARENDERECHO */
	  Match(PARENIZQUIERDO);
	  Expresion(reg);
	  Match(PARENDERECHO);
	  break;

	default:
	  return;
  }
}

void Identificador (REG_EXPRESION* reg)
{
  Match(ID);
  *reg=ProcesarId();
}

REG_EXPRESION ProcesarCte (void)
{
  REG_EXPRESION regcte;

  regcte.tipo = CONSTANTE;
  strcpy(regcte.nombre, buffer);
  sscanf(buffer, "%d", &regcte.valor);

  return regcte;
}




// -------------------------------------------------------------------------------------------------------------------------------------------------------

FILE* p1;
FILE* p2;
TOKEN tokenActual;
}

void errorLex()
{
    fprintf(p2, "Se detecto un error lexico.\n")
}

void errorLexCant()
{
    fprintf(p2, "Error lexico, un identificador no puede tener mas de 32 caracteres.\n")
}

void errorSint()
{
    fprintf(p2, "Error sintactico.\n")
}

void Match(TOKEN t)
{
    if(t!=proximoToken()) errorSint();
}

typedef enum { INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
               PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT, ERRORLEXICO, ERRORCANT} TOKEN;


// ------------------------------------------------------------------- FUNCIONES LIBRO --------------------------------------------------------------------------------

void Objetivo(void)
{
    Programa();
    Match(FDT);
}

void Programa(void)
{
    Match(INICIO);
    ListaSentencias();
    Match(FDT);
}

void ListaSentencias(void)
{

    Sentencia();
    while(1)
    {
        switch(proximoToken())
        {
            case ID: case LEER: case ESCRIBIR:
                Sentencia();
                break;
            default:
                return;
        }
        }
    }

void Sentencia(void)
{
    TOKEN tok = proximoToken();
    switch(tok)
    {
    case ID:
        Match(ID); Match(ASIGNACION); Expresion(); Match(PUNTOYCOMA);
        break;
    case LEER:
        Match(LEER); Match(PARENIZQUIERDO); ListaIdentificadores(); Match(PARENDERECHO); Match(PUNTOYCOMA);
        break;
    case ESCRIBIR:
        Match(ESCRIBIR); Match(PARENIZQUIERDO); ListaExpresiones(); Match(PARENDERECHO); Match(PUNTOYCOMA);
        break;
    default:
        errorSint(); break;
    }

}

void Expresion(void)
{
    TOKEN t;
    Primaria();
    for (t = ProximoToken(); t == SUMA || t == RESTA; t = ProximoToken())
    {
        OperadorAditivo(); Primaria();
    }
}

void OperadorAditivo()
{
    TOKEN t = ProximoToken();
    if(t == SUMA || t == RESTA)
    {
        Match(t);
    }
    else
    {
        errorSint();
    }
}







// ------------------------------------------------------------------- FUNCIONES LIBRO --------------------------------------------------------------------------------

// ---------------------------------------------------------------- FUNCIONES DEL SCANNER --------------------------------------------------------------------------------------------------------------
int mTT[15][13] = {{1, 3, 5, 6, 7, 8, 9, 10, 11, 14, 13, 0,14},
                   {1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 14, 14, 14},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99},
                   {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}};

char buffer[40];

void agregarCaracter(char c)
{
    int i = 0;
    while(buffer[i]!='\0')
    {
        i++;
    }
    buffer[i] = c;
}

TOKEN esReservada()
{
    if(buffer[0]=='i' && buffer[1] == 'n' && buffer[2] == 'i' && buffer[3] == 'c' && buffer[4] == 'i' && buffer[5] == 'o' && buffer[6] == '\0')
    {
        return INICIO;
    }
    if(buffer[0]=='f' && buffer[1] == 'i' && buffer[2] == 'n' && buffer[3] == '\0')
    {
        return FIN;
    }
    if(buffer[0]=='l' && buffer[1] == 'e' && buffer[2] == 'e' && buffer[3] == 'r' && buffer[4] == '\0')
    {
        return LEER;
    }
    if(buffer[0]=='e' && buffer[1] == 's' && buffer[2] == 'c' && buffer[3] == 'r' && buffer[4] == 'i' && buffer[5]=='b' && buffer[6] == 'i' && buffer[7] == 'r' && buffer[8] =='\0')
    {
        return ESCRIBIR;
    }
    return ID;
}

void limpiarBuffer()
{
    int i=0;
    while (i<39)
    {
        buffer[i]='\0';
        i++;
    }
}

int columna(char c)
{
   if ( isalpha(c) ) return 0;
   if ( isdigit(c) ) return 1;
   if ( c == '+' ) return 2;
   if ( c == '-' ) return 3;
   if ( c == '(' ) return 4;
   if ( c == ')' ) return 5;
   if ( c == ',' ) return 6;
   if ( c == ';' ) return 7;
   if ( c == ':' ) return 8;
   if ( c == '=' ) return 9;
   if ( c == EOF ) return 10;
   if ( isspace(c) ) return 11;
   return 12;
}

bool idCorrecto()
{
    int cont = 0, i = 0;
    while(buffer[i]!='\0')
    {
        cont++;
        i++;
    }
    if(cont<32)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void mostrarBuffer()
{
    int i=0;
    while(buffer[i]!='\0')
    {
        fputc(buffer[i], p2);
        i++;
    }
}

bool esAsignacion()
{
    if(buffer[0] == ':' && buffer[1] == '=' && buffer['\0'])
    {
        return true;
    }
    else
    {
        return false;
    }
}

TOKEN scanner()
{
    int estado = 0, f = 0, j;
    char c;
    c = fgetc(p1);
    limpiarBuffer();

    while(!feof(p1) || f == 0)
    {
        if(feof(p1)) f = 1;

        j = columna(c);
        estado = mTT[estado][j];

        switch(estado)
        {
        case 0:
            limpiarBuffer();
            c = fgetc(p1);
            break;
        case 1:
            agregarCaracter(c);
            c = fgetc(p1);
            break;
        case 2:
            ungetc(c, p1);
            mostrarBuffer(p2);
            if(esReservada()!=ID)
            {
                return esReservada();
                estado = 0;
            }
            else
            {
                if(idCorrecto())
                {
                    return ID;
                    estado = 0;
                }
                else
                {
                    return ERRORCANT;
                    estado = 0;
                }
            }
            limpiarBuffer();
            c = fgetc(p1);
            break;
        case 3:
            agregarCaracter(c);
            c = fgetc(p1);
            break;
        case 4:
            ungetc(c, p1);
            mostrarBuffer(p2);
            c = fgetc(p1);
            return CONSTANTE;
            estado = 0;
            limpiarBuffer();
            break;
        case 5:
            c = fgetc(p1);
            return SUMA;
            estado = 0;
            break;
        case 6:
            c = fgetc(p1);
            return RESTA;
            estado = 0;
            break;
        case 7:
            c = fgetc(p1);
            return PARENIZQUIERDO;
            estado = 0;
            break;
        case 8:
            c = fgetc(p1);
            return PARENDERECHO;
            estado = 0;
            break;
        case 9:
            c = fgetc(p1);
            return COMA;
            estado = 0;
            break;
        case 10:
            c = fgetc(p1);
            return PUNTOYCOMA;
            estado = 0;
            break;
        case 12:
            agregarCaracter(c);
            mostrarBuffer(p2);
            c = fgetc(p1);
            estado = 0;
            break;
        case 13:
            break;
        case 14:
            agregarCaracter(c);
            mostrarBuffer(p2);
            c = fgetc(p1);
            if(esAsignacion())
                {
                    return ASIGNACION;
                }
            else
                {
                    return ERRORLEXICO;
                }
            estado = 0;
            limpiarBuffer();
            break;
        }
    }
}

// --------------------------------------------------------------- FIN FUNCIONES DEL SCANNER ------------------------------------------------------------------------------------------------------------

int main()
{
    p1=fopen("Archivo.txt","r");
    p2=fopen("nuevoArchivo.txt","w");
    Objetivo();
    fclose(p1);
    fclose(p2);
    return 0;
}
