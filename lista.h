/*BOMPA Remus-315CB*/
#include<stdio.h>
#include<string.h>
#include"materie.h"
#include"student.h"
#include<stdlib.h>

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

typedef struct celulag
{
  void *info;
  struct celulag *urm;
} TCelulaG, *TLG, **ALG;

typedef unsigned int (*TFHash) (const void *, size_t, size_t);
typedef void (*TF) (void *);
typedef void (*TFAfi) (FILE *, void *);
typedef int (*TFCmp) (void *, void *);
typedef struct
{
  TFHash fh;
  TLG *v;
  TFCmp key_cmp;
  TFAfi key_afi;
  TF key_elib;
} TH;

typedef struct
{
  void *valoare;
  void *cheie;
  TF elib_valoare;
  TFAfi afi_valoare;
} TInfoCel;

int key_cmpS (void *cheie1, void *cheie2);
int key_cmpD (void *cheie1, void *cheie2);
void key_afiS (FILE * out, void *cheie);
void key_afiD (FILE * out, void *cheie);
void key_elibS (void *cheie);
void key_elibD (void *cheie);
void afi_valoareTMaterie (FILE * out, void *valoare);
void afi_valoareTStudent (FILE * out, void *valoare);
void elib_valoareTStudent (void *valoare);
void elib_valoareTMaterie (void *valoare);
TH *CitireTH (FILE * in, char *tcheie, int *nr_bucketuri);
TH *IniTH (int nr_bucketuri, TFHash fh, TFCmp key_cmp, TFAfi key_afi,
	   TF key_elib);
int InsTH (TH * h, char tip_cheie, int nr_bucketuri, TLG CelulaAlocata);
void *CitireValoare (FILE * in, char *tip_valoare);
void *AlocValoare (char *tip_valoare, void *info);
void *CitireInfoCel (FILE * in, char tip_cheie);
void *AlocTInfoCel (void *cheie, void *valoare, TF elib_valoare,
		    TFAfi afi_valoare, char tip_cheie);
TLG CitireCelula (FILE * in, char tip_cheie);

void PrintTH (FILE * out, TH * h, int nr_bucketuri);

void ElibCelula (TH * h, ALG l);
void ElibBucketuri (TH * h, int nr_bucketuri);
void ElibTH (TH ** h, int nr_bucketuri);

float load_factor (TH * h, int nr_bucketuri);
void Redimensionare (TH ** h, int *nr_bucketuri, char tip_cheie);
int Find (FILE * in, FILE * out, TH * h, char tip_cheie, int nr_bucketuri);
int Delete (FILE * in, TH * h, char tip_cheie, int nr_bucketuri);
#endif
