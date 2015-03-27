// BOBOC DIANA-ANDREEA 312 CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int ALLOC (unsigned char *c,int l,int n) {
    int i;
    if (*(int *)&c[0]==0 ) {    //Se verifica daca arena nu are niciun bloc de date
    /* Se verifica daca blocul de date ce trebuie alocat are loc in spatiul liber
    din arena,iar in caz afirmativ se leaga la indexul de start si se returneaza
    indicele blocului de date */
        if (l+4*sizeof(int)<=n){                                 
          *(int *)&c[0]=sizeof(int);
          *(int*)&c[*(int *)&c[0]]=0;
          *(int*)&c[*(int *)&c[0]+sizeof(int)]=0;
          *(int*)&c[*(int *)&c[0]+2*sizeof(int)]=3*sizeof(int)+l;
          return 4*sizeof(int);
        }
        else return 0;
    }
    else {
        /* Se verifica daca blocul de date ce trebuie alocat incape in eventualul
        spatiu liber dintre indexul de start si primul bloc de date,iar in caz
        afirmativ se fac legaturile intre cele 3 blocuri si se returneaza indicele
        de start al noului bloc de date */
        if (*(int *)&c[0]-sizeof(int)>=l+3*sizeof(int)) {
            *(int*)&c[sizeof(int)]=*(int *)&c[0];
            *(int*)&c[2*sizeof(int)]=0;
            *(int*)&c[3*sizeof(int)]=l+3*sizeof(int);
            *(int*)&c[*(int *)&c[0]+sizeof(int)]=sizeof(int);
            *(int *)&c[0]=sizeof(int);
            return *(int*)&c[0]+3*sizeof(int);
         }
       else {
            //Se merge din bloc de date in bloc de date,pana la ultimul din arena.
                i=*(int *)&c[0];
                while (*(int *)&c[i]!=0) {
                    /* Daca blocul de date ce trebuie alocat incape in eventualul 
                    spatiu liber dintre blocul de date curent si blocul de date 
                    urmator,se fac legaturile intre cele 3 blocuri si se returneaza
                    indicele de start al noului bloc de date */
                        if (l+3*sizeof(int)<=*(int *)&c[i]-i-*(int *)&c[i+2*sizeof(int)]) {
                                *(int *)&c[*(int*)&c[i]+sizeof(int)]=i +*(int *)&c[i+2*sizeof(int)]; 
                                *(int *)&c[i +*(int *)&c[i+2*sizeof(int)]]= *(int *)&c[i];
                                *(int *)&c[i]=i +*(int *)&c[i+2*sizeof(int)];
                                *(int *)&c[*(int *)&c[i]+sizeof(int)]=i;
                                *(int *)&c[*(int *)&c[i]+2*sizeof(int)]=l+3*sizeof(int);
                                return *(int *)&c[i]+3*sizeof(int);
                       }
                       i=*(int *)&c[i];
              }
              /* Se verifica daca exista loc pentru blocul de date ce trebuie 
              alocat dupa ultimul bloc de date din arena,iar in caz afirmativ
              se fac legaturile intre cele 2 blocuri si se returneaza indicele
              de start al noului bloc de date */
              if (i+ *(int*)&c[i+2*sizeof(int)]+ l + 3* sizeof(int)<=n){
                    *(int *)&c[i]=i + *(int*)&c[i+2*sizeof(int)];
                    *(int *)&c[*(int *)&c[i]]=0;
                    *(int *)&c[*(int *)&c[i]+sizeof(int)]=i;
                    *(int *)&c[*(int *)&c[i]+2*sizeof(int)]=l+3*sizeof(int);
                    return *(int *)&c[i] + 3*sizeof(int);

              }
              else return 0; /* Se semnaleaza ca nu s-a gasit loc pentru alocarea
                                unui numar de l octeti */
                              
      }
    }
}       
void FILL(unsigned char *c,int i,int nr,int init, int n) {
    int k;
    for (k=i;k<i+nr;k++) {
        c[k]=init;       
    }
}
void DUMP(unsigned char *c,int n) {
    int i=0,k;
    while (i<n) {
        printf("%08X\t",i);
        for (k=1;k<=8&&i<n;k++,i++) {
            printf("%02X ",c[i]);
        }
        printf(" ");
        for (k=9;k<=16&&i<n;k++,i++) {
            printf("%02X ",c[i]);
        }
        printf("\n");
    }
    printf("%08X\n",n);
}
void FREE (unsigned char *c,int i,int n) {
        i=i-3*sizeof(int);
        /* Se verifica daca blocul ce trebuie dezalocat este singurul/primul/ultimul 
        sau un bloc oarecare din arena si se fac legaturi directe intre blocurile
        vecine acestuia. */
        if (*(int *)&c[i+sizeof(int)]==0) {
            if (*(int *)&c[i]!=0) {
                *(int *)&c[0]=*(int *)&c[i];
                *(int*)&c[*(int*)&c[i]+sizeof(int)]=0;
            }
            else *(int *)&c[0]=0;
        }
        else {
            if (*(int *)&c[i]==0) {
                *(int*)&c[*(int*)&c[i+sizeof(int)]]=0;
             }
            else {
                *(int *)&c[*(int*)&c[i+sizeof(int)]]=*(int *)&c[i];
                *(int *)&c[*(int*)&c[i]+sizeof(int)]=*(int *)&c[i+sizeof(int)];
                }
             }
}          
    
void SHOW (unsigned char *c,int n,char command []) {
    int i;
    int free_octets=0;
    int free_blocks=0;
    i=*(int *)&c[0];
    // SHOW FREE
    if (i == 0) { //Se verifica daca nu exista niciun bloc de date in arena
        free_blocks=1;
        free_octets=n-sizeof(int);
    }
    else {
        if (i-sizeof(int)>0) { /* Se verifica daca exista spatiu liber intre
                                indicele de start si primul bloc de date din arena */
            free_octets=i-sizeof(int);
            free_blocks++;
        }
        /* Se merge din bloc de date in bloc de date,pana la ultimul din arena
        si se verifica daca exista spatiu liber dupa blocul de date curent,iar in
        caz afirmativ se actualizeaza variabilele free_blocks si free_octets */   
        while (*(int *)&c[i]!=0) {
            if (*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)])>0) {
                free_blocks++;
                free_octets+=(*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)]));
            }
            i=*(int*)&c[i];
        }
        //Se verifica daca mai este spatiu liber dupa ultimul bloc de date din arena
        if (n-(*(int*)&c[i+2*sizeof(int)]+i)>0) {
            free_blocks++;
            free_octets+=n-(*(int*)&c[i+2*sizeof(int)]+i);
        }
    }
    if (strcmp(command,"FREE") == 0) 
     {
        printf("%d blocks (%d bytes) free\n",free_blocks,free_octets);
     }

    // SHOW USAGE
    int used_blocks=0,used_octets=0;
    i=*(int*)&c[0];
    if (i != 0) { //Se verifica daca arena nu este goala
        /* Se merge din bloc de date in bloc de date pana la ultimul din arena si
        se actualizeaza  variabilele used_blocks si used_octets */
        while (*(int*)&c[i] != 0) {
            used_blocks++;
            used_octets+=*(int*)&c[i+2*sizeof(int)]-3*sizeof(int);
            i=*(int*)&c[i];
        }
        //Se ia in considerare si ultimul bloc de date din arena
        used_blocks++;
        used_octets+=*(int*)&c[i+2*sizeof(int)]-3*sizeof(int);
    }
    if (strcmp(command,"USAGE") == 0) {
        printf("%d blocks (%d bytes) used\n",used_blocks,used_octets); 
        printf("%d%% efficiency\n",used_octets*100/(n-free_octets));
        if (*(int*)&c[0] == 0) 
        { printf("0%% fragmentation\n");}
        else {printf("%d%% fragmentation\n",(free_blocks-1)*100/used_blocks);}
    }
    
    //ALLOCATIONS
    i=*(int*)&c[0];
    if (strcmp(command,"ALLOCATIONS") == 0) {
    printf("OCCUPIED %d bytes\n",sizeof(int));
    if (i != 0) {
        if (i-sizeof(int)>0) {
            printf("FREE %d bytes\n",i-sizeof(int));
        }
        while (*(int*)&c[i] != 0) {
            printf("OCCUPIED %d bytes\n",*(int*)&c[i+2*sizeof(int)]);
            if (*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)])>0) {
                printf("FREE %d bytes\n",*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)]));
            }
            i=*(int*)&c[i];
        }
        printf("OCCUPIED %d bytes\n",*(int*)&c[i+2*sizeof(int)]);
        if (n-(i+*(int *)&c[i+2*sizeof(int)])>0) {  
            printf("FREE %d bytes\n",n-(i+*(int *)&c[i+2*sizeof(int)]));
        }
    }
    else {printf("FREE %d bytes\n",n-sizeof(int));}
    
    }
    
}

int ALLOCALIGNED (unsigned char *c,int n,int l,int align) {
    int i,index;
    if (*(int *)&c[0]==0 ) { //Se verifica daca arena este goala
        if (l+4*sizeof(int)<=n){ /* Se verifica daca nr de octeti ce trebuie alocat
                                    nu depaseste dimensiunea arenei */
          /* Se cauta primul indice din spatiul liber gasit care sa fie divizibil
          cu align */
          index=4*sizeof(int);
          while (index%align != 0 && index-3*sizeof(int)>=sizeof(int) &&
                    index+l<=n) {
                    index++;
          }
          /* Daca exista acest indice, se fac legaturile intre blocul de start si
          noul bloc de date si se returneaza index */
          if (index+l<=n) {
          *(int *)&c[0]=index-3*sizeof(int);
          *(int*)&c[*(int *)&c[0]]=0;
          *(int*)&c[*(int *)&c[0]+sizeof(int)]=0;
          *(int*)&c[*(int *)&c[0]+2*sizeof(int)]=3*sizeof(int)+l;
          return index;
          }
          //Daca nu s-a gasit,se semnalizeaza
          else return 0;
        }
        //Daca nu exista spatiu suficient,se semnalizeaza
        else return 0;
    }
    else {
        /* Se verifica daca exista spatiu pentru alocarea nr de octeti intre indicele
        de start si rimul bloc de date */
        if (*(int *)&c[0]-sizeof(int)>=l+3*sizeof(int) ) {
            /* Se cauta primul indice din spatiul liber gasit care sa fie divizibil
            cu align */
            index=4*sizeof(int);
            while (index%align != 0 && index-3*sizeof(int)>=sizeof(int) &&
                    index+l<=*(int*)&c[0]) {
                    index++;
            }
            /* Daca acesta s-a gasit se fac legaturile intre cele 3 blocuri si se
            returneaza index */
            if (index+l<=*(int*)&c[0]) {
            *(int*)&c[index-3*sizeof(int)]=*(int *)&c[0];
            *(int*)&c[index-2*sizeof(int)]=0;
            *(int*)&c[index-sizeof(int)]=l+3*sizeof(int);
            *(int*)&c[*(int *)&c[0]+sizeof(int)]=index-3*sizeof(int);
            *(int *)&c[0]=index-3*sizeof(int);
            return index;
            }
            // Daca nu s-a gasit se semnalizeaza
            else return 0;
        }
       else {
                /* Se merge din bloc de date in bloc de date,pana la ultimul din 
                arena si se verifica daca exista spatiu pentru alocarea nr de octeti
                dupa locul de date curent */
                i=*(int *)&c[0];
                while (*(int *)&c[i]!=0) {
                        if (l+3*sizeof(int)<=*(int *)&c[i]-i-*(int *)&c[i+2*sizeof(int)]) {
                                /* Se cauta primul indice din spatiul liber gasit
                                care sa fie divizibil cu align */
                                index=i+*(int*)&c[i+2*sizeof(int)]+3*sizeof(int);
                                while (index%align != 0 && index-3*sizeof(int)>=i+*(int*)&c[i+2*sizeof(int)] &&
                                        index+l<=*(int*)&c[i]) {
                                        index++;
                                }
                                /* Daca indicele s-a gasit se fac legaturile intre
                                cele 3 blocuri de date si se returneaza index */
                                if (index+l<=*(int*)&c[i]) {
                                *(int *)&c[*(int*)&c[i]+sizeof(int)]=index-3*sizeof(int); 
                                *(int *)&c[index-3*sizeof(int)]= *(int *)&c[i];
                                *(int *)&c[i]=index-3*sizeof(int);
                                *(int *)&c[*(int *)&c[i]+sizeof(int)]=i;
                                *(int *)&c[*(int *)&c[i]+2*sizeof(int)]=l+3*sizeof(int);
                                return index;
                                }
                                // Daca nu s-a gasit indicele,se semnaleaza
                                else return 0;
                       }
                       i=*(int *)&c[i];
              }
              /* Se verifica daca exista spatiu liber ptr alocarea nr de octeti dupa
              ultimul bloc de date din arena */
              if (i+ *(int*)&c[i+2*sizeof(int)]+ l + 3* sizeof(int)<=n){
                    /* Se cauta primul indice de dupa ultimul bloc de date din arena
                    care este divizibil cu align */
                    index=i+*(int*)&c[i+2*sizeof(int)]+3*sizeof(int);
                    while (index%align != 0 && index-3*sizeof(int)>=i+*(int*)&c[i+2*sizeof(int)] &&
                            index+l<=n) {
                            index++;
                    }
                    /* Daca indicele s-a gasit,se fac legaturile intre cele 2
                    blocuri de date si se returneaza index */
                    if (index+l<=n) {
                    *(int *)&c[i]=index-3*sizeof(int);
                    *(int *)&c[*(int *)&c[i]]=0;
                    *(int *)&c[*(int *)&c[i]+sizeof(int)]=i;
                    *(int *)&c[*(int *)&c[i]+2*sizeof(int)]=l+3*sizeof(int);
                    return index;
                    }
                    //Daca nu s-a gasit,se semnaleaza
                    else return 0;

              }
              //Daca nu s-a gasit niciun loc liber ptr octeti,se semnaleaza
              else return 0;
                              
      }
    }
}   

  
void SHOW_MAP (unsigned char *c,int n,int l) {
    int i,k=1,symbol,aux;float x=n*1.0/l,j,rest=0,limit;
    // In variabila k se vor retine nr de octeti afisati
    i=*(int*)&c[0];
    // Se afiseaza nr de '*' corespunzator blocului de start
    for (j=1;j<=1.0*sizeof(int)/x;j++,k++) {
        printf("*");
        if (k%80 == 0) printf("\n");
    }
   /* Se verifica daca 1.0*sizeof(int)/x da cu rest,si in caz afirmativ se mai 
   imprumuta sizeof(int)*1.0-aux*x din octetii care urmeaza pentru a se mai \
   afisa o '*',lucru retinut in rest */
    aux=sizeof(int)/x;symbol=1;
    if (1.0*sizeof(int) != 1.0*aux*x ) 
    {   printf("*");k++;
        if (k%80 == 0) printf("\n");
        rest=x-(sizeof(int)*1.0-aux*x);                            
    }       
    else rest=0;   
    if (i != 0) {
        /* Se verifica daca exista spatiu liber intre blocul de start si primul
        bloc din arena,iar in caz afirmativ se procedeaza cum a fost explicat
        in README */
        if (i-sizeof(int)>0) {
            for (j=1;j<=1.0*(i-sizeof(int)-rest)/x;j++,k++) {
                printf(".");symbol=0;
                if (k%80 == 0) printf("\n");
            }        
            if (i-sizeof(int)-rest>0) 
            {   /* Se calculeaza numarul de octeti liberi neluati in considerare 
                    la afisare si se retin in rest */
                aux=(i-sizeof(int)-rest)/x;
                if (1.0*(i-sizeof(int)-rest) != aux*x) 
                {   rest=(1.0*(i-sizeof(int)-rest)-aux*x);  }
                else rest=0;
            }
            /* In cazul in care spatiul liber a fost luat deja in considerare la
            afisarea '*' anterioare,se actualizeaza valoarea lui rest */
            else rest=rest-i+sizeof(int);                    
        }
        /* Pentru fiecare bloc de date din arena (in afara de ultimul ) si spatiu
        liber ce il succede se afiseaza nr de '*' si nr d '.' corespunzator
        explicatiei din README */
        while (*(int*)&c[i] != 0) {                    
            if (symbol==0) limit=1.0*(*(int*)&c[i+2*sizeof(int)]+rest);
            else limit=1.0*(*(int*)&c[i+2*sizeof(int)]-rest);
            for (j=1;j<=limit/x;j++,k++) {
                   printf("*");symbol=1;
                   if (k%80 == 0) printf("\n");
            }
         
            if (limit>0)
            {   /* Se verifica daca s-au luat in considerare pentru afisare octeti
                din continuarea blocului de date curent si se retine nr lor in rest */
                aux=(int)(limit/x);
                if ((int)(limit) != aux*x) 
                {   printf("*");symbol=1;k++;
                    if (k%80 == 0) printf("\n");
                    rest=x-(limit-aux*x);
                }   
                else rest=0;
            }
            /* Daca octetii din blocul de date curent au fost luati deja in considerare
            la afisarea '*' sau '.' trecute, se marcheaza lucrul acesta in variabila rest */
            else rest=rest-*(int*)&c[i+2*sizeof(int)];
           
            if (*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)])>0) {
                 for (j=1;j<=1.0*(*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)])-rest)/x;j++,k++) {
                    printf(".");symbol=0;
                    if (k%80 == 0) printf("\n");
                 }
              
                 if (*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)])-rest>0)
                 {  /* Se verifica daca au ramas octeti din spatiul liber neluati 
                    in considerare la afisare si se retine nr lor in variabila rest */
                    aux=(*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)])-rest)/x;
                    if (1.0*(*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)])-rest) != aux*x ) 
                    {   rest=(1.0*(*(int*)&c[i]-(i+*(int*)&c[i+2*sizeof(int)])-rest)-aux*x);    }
                    else rest=0;
                 }
                 /* Daca octetii din spatiul liber curent au fost luati deja in 
                 considerare la afisarea '*' sau '.' trecute,se marcheaza lucrul
                 acesta in variabila rest */
                 else rest=rest-*(int*)&c[i]+(i+*(int*)&c[i+2*sizeof(int)]);
                
            }
            i=*(int*)&c[i];
        }
        /* Se afiseaza nr de '*' corespunzator parcurgerii ultimului bloc de date 
        din arena conform explicatiei din README */        
        if (symbol==0) limit=1.0*(*(int*)&c[i+2*sizeof(int)]+rest);
        else limit=1.0*(*(int*)&c[i+2*sizeof(int)]-rest);
        
        for (j=1;j<=limit/x;j++,k++) {
                   printf("*");symbol=1;
                   if (k%80 == 0) printf("\n");
        }
        if (limit>0)
        {   /* Se verifica daca s-au luat in considerare la afisare octeti din
            continuarea ultimului bloc de date si se retine nr lor in variabila
            rest */
            aux=(int)(limit/x);
            if ((int)(limit) != x*aux )
            {       printf("*");symbol=1;k++;
                    if (k%80 == 0) printf("\n");
                    rest=x-(limit-x*aux);
            }
            else rest=0;
        }
        /* Daca octetii din ultimul bloc de date au fost deja luati in considerare
        la afisarea '*' sau '.' trecute, se marcheaza acest lucru in variabila rest */
        else rest=rest-*(int*)&c[i+2*sizeof(int)];
        /* Se afiseaza nr de '.' corespunzatoare spatiului liber de dupa ultimul
        bloc din arena (daca acesta exista) conform explicatiei din README */
        if (n-(i+*(int *)&c[i+2*sizeof(int)])>0) {  
             for (j=1;j<=1.0*(n-(i+*(int*)&c[i+2*sizeof(int)])-rest)/x;j++,k++) {
                    printf(".");symbol=0;
                    if (k%80 == 0) printf("\n");
             }
                
             if (n-(i+*(int*)&c[i+2*sizeof(int)])-rest>0) 
             {  /* Se verifica daca au ramas octeti din spatiul liber neluati in 
                considerare la afisare si in caz afirmatiz se afiseaza un '.' in plus */
                aux=(n-(i+*(int*)&c[i+2*sizeof(int)])-rest)/x;
                if (1.0*(n-(i+*(int*)&c[i+2*sizeof(int)])-rest != aux*x ))
                {   printf("."); }
             }             
        }
    }
    else 
    {   /* Se afiseaza nr de '.' corespunzator spatiului liber din arena conform
        explicatiei din README */
        for (j=1;j<=1.0*(n-sizeof(int)-rest)/x;j++,k++) {
                    printf(".");
                    if (k%80 == 0) printf("\n");
            }      
            if (n-sizeof(int)-rest>0) {
                /* Daca sunt octeti din spatiul liber al arenei neluati in considerare
                la afisare se afiseaza un '.' in plus */
                aux=(n-sizeof(int)-rest)/x;
                if ((int)(n-sizeof(int)-rest) != aux*x) {
                    printf(".");
                }
            }
    }
    printf("\n");    
}   
int REALLOC (unsigned char *c, int index , int size, int n){
    int k,i,j,octets[1000],nr;
    //Se pozitioneaza nr pe indicele corespunzator blocului de date respectiv
    nr = *(int *)&c[index-sizeof(int)]-3*sizeof(int);
    for (i=index,j=1 ; i<=index+nr; i++,j++){
        octets[j]=c[i];
    }    
    FREE(c,index,n);
    k=ALLOC(c,size,n);
    if(k!=0){
        if (nr <= size){
            for (i=1,j=k;i<=nr;i++,j++){
                c[j]=octets[i];
            }
        }
        else {
            for (i=1,j=k;j<k+size;i++,j++){
               c[j]=octets[i];
            }
        
        }
    }
return k;
}
int main () {
   unsigned char *c;
   char *p,command[50];
   int n,i,l,nr,init,size,align;
   fgets(command,50,stdin); //Se citeste comanda INITIALIZE si nr de octeti ai 
                            //  arenei
   p=strtok(command," ");
   p=strtok(NULL," ");   
   n=atoi(p);
   c=malloc(n);   
   // Se initializeaza cu 0 toti octetii arenei
   for (i=0;i<n;i++) {
        c[i]=0;
   }
   /* Se citeste fiecare comanda pana la comanda FINALIZE si dupa ce se aceasta 
   este identificata se citesc si parametrii ei si se apeleaza functia corespunzatoare */
   fgets(command,50,stdin); 
   p=strtok(command," \n");
   while ( strcmp ( command, "FINALIZE" ) != 0) {
       if (strcmp ( command, "ALLOC" ) == 0) 
       {
                p=strtok(NULL," \n");
                size=atoi(p);
                printf("%d\n",ALLOC(c,size,n));
       }
       if (strcmp (command, "FILL") == 0)
       {                
                p=strtok(NULL," \n");
                i=atoi(p);
                p=strtok(NULL," \n");
                nr=atoi(p);
                p=strtok(NULL," \n");
                init=atoi(p);
                FILL(c,i,nr,init,n);
       }
       if (strcmp(command, "DUMP") == 0 )
       {              
                DUMP(c,n);
       }
       if (strcmp(command, "FREE") == 0 )
       {                
                p=strtok(NULL," \n");
                i=atoi(p);
                FREE(c,i,n);
       }
       if (strcmp(command, "SHOW") == 0)
       {                
                p=strtok(NULL," \n");
                if (strcmp(p, "MAP") == 0) 
                {
                  
                    p=strtok(NULL," \n");
                    l=atoi(p);
                    SHOW_MAP(c,n,l);
                }
                else SHOW(c,n,p);
                
       }
       if (strcmp(command, "ALLOCALIGNED") == 0)
       {              
                p=strtok(NULL," \n");
                size=atoi(p);                
                p=strtok(NULL," \n");
                align=atoi(p);               
                printf("%d\n",ALLOCALIGNED(c,n,size,align));
       }
       if (strcmp(command, "REALLOC") == 0 )
       {              
                p=strtok(NULL," \n");
                i=atoi(p);
                p=strtok(NULL," \n");
                l=atoi(p);               
                printf ("%d\n", REALLOC(c,i,l,n));
        } 
       fgets(command,50,stdin);
       p=strtok(command," \n");
    }
    free(c);
return 0;
}
