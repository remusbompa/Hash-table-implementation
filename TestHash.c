/*BOMPA Remus-315CB*/
#include<stdio.h>
#include"lista.h"
int
main ()
{
  char tip_cheie, operatie[20];
  FILE *in = fopen ("input.in", "rt"), *out = fopen ("output.out", "wt");
  TH *h = NULL;
  int nr_comenzi = 0, nr_bucketuri, rez, i;
  if (fscanf (in, "%s", operatie) == 1 && !strcmp (operatie, "inithash"))
    {
      h = CitireTH (in, &tip_cheie, &nr_bucketuri);
      if (!h)
	return 0;
      fscanf (in, "%d", &nr_comenzi);
    }
  for (i = 0; i < nr_comenzi; i++)
    if (fscanf (in, "%s", operatie) == 1)
      {
	if (!strcmp (operatie, "insert"))
	  {
	    rez =
	      InsTH (h, tip_cheie, nr_bucketuri,
		     CitireCelula (in, tip_cheie));
	    if (!rez)
	      return 0;
	  }
	else if (!strcmp (operatie, "print"))
	  PrintTH (out, h, nr_bucketuri);
	else if (!strcmp (operatie, "find"))
	  Find (in, out, h, tip_cheie, nr_bucketuri);
	else if (!strcmp (operatie, "delete"))
	  Delete (in, h, tip_cheie, nr_bucketuri);
	else
	  printf ("Operatie invalida!\n");
	if (load_factor (h, nr_bucketuri) >= 0.75)
	  Redimensionare (&h, &nr_bucketuri, tip_cheie);
      }
  fclose (in);
  fclose (out);
  ElibTH (&h, nr_bucketuri);
  return 0;
}
