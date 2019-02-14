/*BOMPA Remus-315CB*/
#include"lista.h"
/*
 * data - key to compute hash
 * len - sizeof of the data
 * range - the number of buckets
 * Return the index of the bucket
 */
unsigned int
hash_f (const void *data, size_t len, size_t range)
{
  unsigned int hash = 0u;
  size_t idx = 0;
  size_t cof = 1;
  size_t rate = 2;
  const char *d = (const char *) data;
  for (; idx < len; ++idx)
    {
      hash += d[idx] * cof;
      cof *= rate;
    }
  return hash % range;
}

int
key_cmpS (void *cheie1, void *cheie2)
{
  return strcmp ((char *) cheie1, (char *) cheie2);
}

int
key_cmpD (void *cheie1, void *cheie2)
{
  return *((int *) cheie1) - *((int *) cheie2);
}

void
key_afiD (FILE * out, void *cheie)
{
  fprintf (out, "%d", *((int *) cheie));
}

void
key_afiS (FILE * out, void *cheie)
{
  fprintf (out, "%s", (char *) cheie);
}

void
key_elibS (void *cheie)
{
  free (cheie);
}

void
key_elibD (void *cheie)
{
  free (cheie);
}

void
afi_valoareTMaterie (FILE * out, void *valoare)
{
  fprintf (out, "[Materie: ");
  fprintf (out, "%s, ", ((TMaterie *) valoare)->materie);
  fprintf (out, "Ore_curs: ");
  fprintf (out, "%d, ", ((TMaterie *) valoare)->ore_curs);
  fprintf (out, "Ore_lab: ");
  fprintf (out, "%d, ", ((TMaterie *) valoare)->ore_lab);
  fprintf (out, "Credit: ");
  fprintf (out, "%d, ", ((TMaterie *) valoare)->credit);
  fprintf (out, "Teme: ");
  fprintf (out, "%d]", ((TMaterie *) valoare)->teme);
}

void
afi_valoareTStudent (FILE * out, void *valoare)
{
  fprintf (out, "[Nume: ");
  fprintf (out, "%s, ", ((TStudent *) valoare)->nume);
  fprintf (out, "Grupa: ");
  fprintf (out, "%s, ", ((TStudent *) valoare)->grupa);
  fprintf (out, "Medie: ");
  fprintf (out, "%.2f, ", ((TStudent *) valoare)->medie);
  fprintf (out, "Varsta: ");
  fprintf (out, "%d]", ((TStudent *) valoare)->varsta);
}

void
elib_valoareTStudent (void *valoare)
{
  free (((TStudent *) valoare)->nume);
  free (valoare);
}

void
elib_valoareTMaterie (void *valoare)
{
  free (((TMaterie *) valoare)->materie);
  free (valoare);
}

TH *
CitireTH (FILE * in, char *tcheie, int *hnr)
{
  TH *h;
  int nr_bucketuri = 0;
  char tip_cheie = 0;
  TFAfi key_afi;
  TF key_elib;
  TFCmp key_cmp;
  if (!
      (fscanf (in, " %c %d", &tip_cheie, &nr_bucketuri) == 2
       && (tip_cheie == 'd' || tip_cheie == 's')))
    {
      printf ("eroare la citire TH!\n");
      return NULL;
    }
  if (tip_cheie == 'd')
    {
      key_afi = key_afiD;
      key_elib = key_elibD;
      key_cmp = key_cmpD;
    }
  else
    {
      key_afi = key_afiS;
      key_elib = key_elibS;
      key_cmp = key_cmpS;
    }
  *tcheie = tip_cheie;
  *hnr = nr_bucketuri;
  h = IniTH (nr_bucketuri, hash_f, key_cmp, key_afi, key_elib);
  return h;
}

TH *
IniTH (int nr_bucketuri, TFHash fh, TFCmp key_cmp, TFAfi key_afi, TF key_elib)
{
  TH *h = (TH *) calloc (sizeof (TH), 1);
  if (!h)
    {
      printf ("eroare alocare hash\n");
      return NULL;
    }
  h->fh = fh;
  h->v = (TLG *) calloc (nr_bucketuri, sizeof (TLG));
  if (!h->v)
    {
      printf ("eroare alocare vector de pointeri TLG in hash\n");
      free (h);
      return NULL;
    }
  h->key_cmp = key_cmp;
  h->key_afi = key_afi;
  h->key_elib = key_elib;
  return h;
}

int
InsTH (TH * h, char tip_cheie, int nr_bucketuri, TLG CelulaAlocata)
{
  int cod, len;
  TLG aux = CelulaAlocata, *aL;
  if (!aux)
    return 0;
  TInfoCel *info_aux = (TInfoCel *) aux->info;
  if (tip_cheie == 's')
    {
      len = strlen ((char *) info_aux->cheie);
      cod = h->fh (info_aux->cheie, len, nr_bucketuri);
    }
  else if (tip_cheie == 'd')
    {
      len = 4;
      cod = h->fh (info_aux->cheie, len, nr_bucketuri);
    }
  else
    {
      printf ("tip_cheie nu este valid!\n");
      return 0;
    }
  if (cod < 0)
    return 0;
  for (aL = h->v + cod; *aL; aL = &((*aL)->urm))
    {
      TInfoCel *info = (TInfoCel *) (*aL)->info;
      if (!h->key_cmp (info->cheie, info_aux->cheie))
	{
	  TLG cel = *aL;
	  aux->urm = (*aL)->urm;
	  *aL = aux;
	  ElibCelula (h, &cel);
	  return 1;
	}
    }
  for (aL = h->v + cod; *aL; aL = &((*aL)->urm));
  *aL = aux;
  return 1;
}

void *
CitireValoare (FILE * in, char *tip_valoare)
{
  void *valoare = NULL;
  if (!strcmp ("stud", tip_valoare))
    {
      TStudent info;
      info.nume = malloc (50);
      if (!info.nume)
	{
	  printf ("Eroare alocare nume pentru citit!\n");
	  return NULL;
	}
      if (fscanf
	  (in, "%s%f%s%d", info.nume, &info.medie, info.grupa,
	   &info.varsta) == 4)
	{
	  valoare = AlocValoare (tip_valoare, &info);
	  free (info.nume);
	}
      else
	{
	  printf ("eroare la citire valori TStudent!\n");
	  return NULL;
	}
    }
  else if (!strcmp ("mat", tip_valoare))
    {
      TMaterie info;
      info.materie = malloc (50);
      if (!info.materie)
	{
	  printf ("Eroare alocare materie pentru citit!\n");
	  return NULL;
	}
      if (fscanf
	  (in, "%s%d%d%d%d", info.materie, &info.ore_curs, &info.ore_lab,
	   &info.credit, &info.teme) == 5)
	{
	  valoare = AlocValoare (tip_valoare, &info);
	  free (info.materie);
	}
      else
	{
	  printf ("eroare la citire valori TMaterie!\n");
	  return NULL;
	}
    }
  else
    {
      printf ("Tip valoare invalid!\n");
      return NULL;
    }
  return valoare;
}

void *
AlocValoare (char *tip_valoare, void *Info)
{
  void *val = NULL;
  if (!strcmp ("stud", tip_valoare))
    {
      TStudent *valoare = (TStudent *) malloc (sizeof (TStudent));
      if (!valoare)
	{
	  printf ("eroare alocare valoare!\n");
	  return NULL;
	}
      TStudent *info = (TStudent *) Info;
      valoare->nume = malloc (strlen (info->nume) + 1);
      if (!valoare->nume)
	{
	  printf ("eroare alocare valoare->nume!\n");
	  return NULL;
	}
      memcpy (valoare->nume, info->nume, strlen (info->nume) + 1);
      memcpy (&valoare->medie, &info->medie, sizeof (float));
      memcpy (valoare->grupa, info->grupa, 6);
      memcpy (&valoare->varsta, &info->varsta, sizeof (int));
      val = (void *) valoare;
    }
  else if (!strcmp ("mat", tip_valoare))
    {
      TMaterie *valoare = (TMaterie *) malloc (sizeof (TMaterie));
      if (!valoare)
	{
	  printf ("eroare alocare valoare!\n");
	  return NULL;
	}
      TMaterie *info = (TMaterie *) Info;
      valoare->materie = malloc (strlen (info->materie) + 1);
      if (!valoare->materie)
	{
	  printf ("eroare alocare valoare->materie!\n");
	  return NULL;
	}
      memcpy (valoare->materie, info->materie, strlen (info->materie) + 1);
      memcpy (&valoare->ore_curs, &info->ore_curs, sizeof (int));
      memcpy (&valoare->ore_lab, &info->ore_lab, sizeof (int));
      memcpy (&valoare->credit, &info->credit, sizeof (int));
      memcpy (&valoare->teme, &info->teme, sizeof (int));
      val = (void *) valoare;
    }
  else
    {
      printf ("Eroare la citire tip_valoare!\n");
      return NULL;
    }
  return val;
}

void *
CitireInfoCel (FILE * in, char tip_cheie)
{
  void *info, *valoare;
  char tip_valoare[5];
  void *cheie = NULL;
  TF elib_valoare;
  TFAfi afi_valoare;
  if (tip_cheie == 's')
    {
      char *Cheie = (char *) malloc (50);
      if (!Cheie)
	{
	  printf ("Eroare alocare cheie pentru citire!\n");
	  return NULL;
	}
      if (!(fscanf (in, "%s", Cheie) == 1))
	{
	  printf ("eroare la citire cheie!\n");
	  return NULL;
	}
      cheie = (void *) Cheie;
    }
  else if (tip_cheie == 'd')
    {
      int *Cheie = (int *) malloc (sizeof (int));
      if (!Cheie)
	{
	  printf ("Eroare alocare cheie pentru citire!\n");
	  return NULL;
	}
      if (!(fscanf (in, "%d", Cheie) == 1))
	{
	  printf ("eroare la citire cheie!\n");
	  return NULL;
	}
      cheie = (void *) Cheie;
    }
  else
    {
      printf ("Tip de cheie invalid!\n");
      return NULL;
    }
  if (!
      (fscanf (in, "%s", tip_valoare) == 1
       && (!strcmp ("stud", tip_valoare) || !strcmp ("mat", tip_valoare))))
    {
      printf ("eroare la citire tip_valoare!\n");
      return NULL;
    }
  valoare = CitireValoare (in, tip_valoare);
  if (!valoare)
    return NULL;
  if (!strcmp (tip_valoare, "stud"))
    {
      elib_valoare = elib_valoareTStudent;
      afi_valoare = afi_valoareTStudent;
    }
  else
    {
      elib_valoare = elib_valoareTMaterie;
      afi_valoare = afi_valoareTMaterie;
    }
  info = AlocTInfoCel (cheie, valoare, elib_valoare, afi_valoare, tip_cheie);
  free (cheie);
  return info;
}

void *
AlocTInfoCel (void *Cheie, void *valoare, TF elib_valoare, TFAfi afi_valoare,
	      char tip_cheie)
{
  TInfoCel *info = (TInfoCel *) malloc (sizeof (TInfoCel));
  if (!info)
    {
      printf ("Eroare alocare TInfoCel!\n");
      return NULL;
    }
  info->valoare = valoare;
  if (!valoare)
    return NULL;
  if (tip_cheie == 's')
    {
      char *cheie = (char *) Cheie;
      info->cheie = (void *) malloc (strlen (cheie) + 1);
      if (!info->cheie)
	{
	  printf ("eroare alocare cheie!\n");
	  return NULL;
	}
      memcpy (info->cheie, cheie, strlen (cheie) + 1);
    }
  else if (tip_cheie == 'd')
    {
      int *cheie = (int *) Cheie;
      info->cheie = (void *) malloc (sizeof (int));
      memcpy (info->cheie, cheie, sizeof (int));
    }
  else
    {
      printf ("Tip de cheie invalid!\n");
      return NULL;
    }
  info->elib_valoare = elib_valoare;
  info->afi_valoare = afi_valoare;
  return (void *) info;
}

TLG
CitireCelula (FILE * in, char tip_cheie)
{
  TLG aux = (TLG) malloc (sizeof (TCelulaG));
  if (!aux)
    {
      printf ("Eroare alocare celula!\n");
      return NULL;
    }
  aux->info = CitireInfoCel (in, tip_cheie);
  if (!aux->info)
    return NULL;
  aux->urm = NULL;
  return aux;
}

void
PrintTH (FILE * out, TH * h, int nr_bucketuri)
{
  int i;
  for (i = 0; i < nr_bucketuri; i++)
    {
      fprintf (out, "[%d] :", i);
      TLG el = h->v[i];
      for (; el; el = el->urm)
	{
	  fprintf (out, " (");
	  h->key_afi (out, ((TInfoCel *) el->info)->cheie);
	  fprintf (out, " -> ");
	  ((TInfoCel *) el->info)->afi_valoare (out,
						((TInfoCel *) el->info)->
						valoare);
	  fprintf (out, ")");
	}
      fprintf (out, "\n");
    }
}

float
load_factor (TH * h, int nr_bucketuri)
{
  int i, k = 0;
  for (i = 0; i < nr_bucketuri; i++)
    {
      TLG el = h->v[i];
      for (; el; el = el->urm)
	k++;
    }
  return (float) k / nr_bucketuri;
}

void
Redimensionare (TH ** h, int *nr_bucketuri, char tip_cheie)
{
  TH *g =
    IniTH (2 * (*nr_bucketuri), (*h)->fh, (*h)->key_cmp, (*h)->key_afi,
	   (*h)->key_elib);
  int i;
  for (i = 0; i < (*nr_bucketuri); i++)
    {
      TLG el = (*h)->v[i];
      while (el)
	{
	  TLG aux = el;
	  el = el->urm;
	  aux->urm = NULL;
	  InsTH (g, tip_cheie, 2 * (*nr_bucketuri), aux);
	}
    }
  (*nr_bucketuri) *= 2;
  free ((*h)->v);
  free (*h);
  (*h) = NULL;
  (*h) = g;
}


void
ElibCelula (TH * h, ALG l)
{
  TInfoCel *info = (TInfoCel *) (*l)->info;
  info->elib_valoare (info->valoare);
  h->key_elib (info->cheie);
  free ((*l)->info);
  free (*l);
  *l = NULL;
}

void
ElibBucketuri (TH * h, int nr_bucketuri)
{
  int i;
  for (i = 0; i < nr_bucketuri; i++)
    {
      TLG el = h->v[i], aux;
      while (el)
	{
	  aux = el;
	  el = el->urm;
	  ElibCelula (h, &aux);
	}
    }
  free (h->v);
  h->v = NULL;
}

void
ElibTH (TH ** h, int nr_bucketuri)
{
  ElibBucketuri (*h, nr_bucketuri);
  free (*h);
  *h = NULL;
}

int
Find (FILE * in, FILE * out, TH * h, char tip_cheie, int nr_bucketuri)
{
  void *cheie;
  int len, cod, ok = 0;
  TLG el;
  if (tip_cheie == 's')
    {
      char *Cheie = (char *) malloc (50);
      if (!Cheie)
	{
	  printf ("Eroare alocare cheie pentru cautare!\n");
	  return 0;
	}
      if (!(fscanf (in, "%s", Cheie) == 1))
	{
	  printf ("Eroare citire cheie pentru cautare!\n");
	  return 0;
	}
      cheie = (void *) Cheie;
      len = strlen (Cheie);
      cod = h->fh (cheie, len, nr_bucketuri);
    }
  else if (tip_cheie == 'd')
    {
      int *Cheie = (int *) malloc (sizeof (int));
      if (!Cheie)
	{
	  printf ("Eroare alocare cheie pentru cautare!\n");
	  return 0;
	}
      if (!(fscanf (in, "%d", Cheie) == 1))
	{
	  printf ("Eroare citire cheie pentru cautare!\n");
	  return 0;
	}
      cheie = (void *) Cheie;
      len = 4;
      cod = h->fh (cheie, len, nr_bucketuri);
    }
  else
    {
      printf ("Tip cheie invalid pentru cautare!\n");
      return 0;
    }
  for (el = h->v[cod]; el; el = el->urm)
    {
      TInfoCel *info = (TInfoCel *) el->info;
      if (!h->key_cmp (info->cheie, cheie))
	{
	  info->afi_valoare (out, info->valoare);
	  fprintf (out, "\n");
	  ok = 1;
	}
    }
  if (!ok)
    fprintf (out, "Nu exista\n");
  free (cheie);
  return 1;
}

int
Delete (FILE * in, TH * h, char tip_cheie, int nr_bucketuri)
{
  void *cheie;
  int len, cod;
  TLG *aL = NULL;
  if (tip_cheie == 's')
    {
      char *Cheie = (char *) malloc (50);
      if (!Cheie)
	{
	  printf ("Eroare alocare cheie pentru cautare!\n");
	  return 0;
	}
      if (!(fscanf (in, "%s", Cheie) == 1))
	{
	  printf ("Eroare citire cheie pentru cautare!\n");
	  return 0;
	}
      cheie = (void *) Cheie;
      len = strlen (Cheie);
      cod = h->fh (cheie, len, nr_bucketuri);
    }
  else if (tip_cheie == 'd')
    {
      int *Cheie = (int *) malloc (sizeof (int));
      if (!Cheie)
	{
	  printf ("Eroare alocare cheie pentru cautare!\n");
	  return 0;
	}
      if (!(fscanf (in, "%d", Cheie) == 1))
	{
	  printf ("Eroare citire cheie pentru cautare!\n");
	  return 0;
	}
      cheie = (void *) Cheie;
      len = 4;
      cod = h->fh (cheie, len, nr_bucketuri);
    }
  else
    {
      printf ("Tip cheie invalid pentru cautare!\n");
      return 0;
    }
  aL = h->v + cod;
  while (*aL)
    {
      TInfoCel *info = (TInfoCel *) (*aL)->info;
      if (!h->key_cmp (info->cheie, cheie))
	{
	  TLG aux = *aL;
	  *aL = (*aL)->urm;
	  ElibCelula (h, &aux);
	}
      else
	aL = &((*aL)->urm);
    }
  free (cheie);
  return 1;
}
