//
//  main.c
//  Compito_07_03_2017_Conti_Correnti_Banca
//
//  Created by Davide Segullo on 23/05/18.
//  Copyright © 2018 davidesegullo. All rights reserved.
//

/*
  STRUTTURA FILE
  Cognome Nome  CodiceFiscale E-mail  Numero_Conto  username  Password
    - ammontare N_CONTO data
    - ....
    - ...N
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 21

typedef enum{false=0, true=1} bool;

typedef struct {
    int dd;
    int mm;
    int yyyy;
}data;

typedef struct {
    int ammontare;
    char n_conto[11];
    data data_mov;
}dati_movimento;

typedef struct movimento{
    dati_movimento dati;
    struct movimento *next;
} movimento;

typedef struct {
    int saldo;
    movimento *lista_movimenti;
}conto;

typedef struct {
    char cognome[N];
    char nome[N];
    char codice_fiscale[17];
    char email[N];
    char n_conto[11];
    char username[11];
    char password[11];
    bool impiegato;
    conto conto_corrente;
}dati_cliente;

typedef struct cliente{
    dati_cliente dati;
    struct cliente *next;
} cliente;

bool confronta_date(data d1, data d2){ //Ritorna vero se la data uno è piu' grande della due    10/11/2015 e 8/12/2015

    if (d1.yyyy > d2.yyyy)
        return true;
    else if (d1.yyyy == d2.yyyy){
        if (d1.mm > d2.mm)
            return true;
        else if (d1.mm == d2.mm){
            if (d1.dd >= d2.dd)
                return true;
            else
                return false;
        } else
            return false;
    } else
        return false;

    return false;
}

bool crea_lista_movimenti(movimento **lista_movimenti, dati_movimento d){

    movimento *p = (movimento *)malloc(sizeof(movimento));

    if (p == NULL)
        return false;

    p->dati = d;
    p->next = NULL;
    *lista_movimenti = p;

    return true;
}

bool inserimento_lista_movimenti(movimento **lista_movimenti, dati_movimento d){

    if (*lista_movimenti == NULL)
        return crea_lista_movimenti(lista_movimenti, d);

    movimento *current = NULL;
    movimento *successive = *lista_movimenti;

    while (successive!=NULL && confronta_date(successive->dati.data_mov, d.data_mov)) {
        current = successive;
        successive = successive->next;
    }

    movimento *p = (movimento *)malloc(sizeof(movimento));
    p->dati = d;
    p->next = NULL;

    if (current == NULL){
        p->next = *lista_movimenti;
        *lista_movimenti = p;
        return true;
    }

    if (successive == NULL){
        current->next = p;
        return true;
    }

    current->next = p;
    p->next = successive;
    return true;
}

bool crea_lista_clienti(cliente **clienti, dati_cliente d){

    cliente *p = (cliente *)malloc(sizeof(cliente));

    if (p == NULL)
        return false;

    p->dati = d;
    p->next = NULL;
    *clienti = p;

    return true;
}

bool inserimento_lista_clienti(cliente **clienti, dati_cliente d){

    if (*clienti == NULL)
        return crea_lista_clienti(clienti, d);

    cliente *current = NULL;
    cliente *successive = *clienti;

    while (successive!=NULL && strcmp(successive->dati.n_conto, d.n_conto)<0) {
        current = successive;
        successive = successive->next;
    }

    cliente *p = (cliente *)malloc(sizeof(cliente));

    if (p==NULL)
        return false;

    p->dati = d;
    p->next = NULL;

    if (current == NULL){
        p->next = *clienti;
        *clienti = p;
        return true;
    }

    if (successive == NULL){
        current->next = p;
        return true;
    }

    current->next = p;
    p->next = successive;
    return true;
}

void registrazione_utente(cliente **clienti){

    dati_cliente d;
    d.conto_corrente.lista_movimenti = NULL;

    printf("Registrazione Utente\nCognome: ");
    scanf("%s",d.cognome);
    printf("Nome: ");
    scanf("%s",d.nome);
    printf("Codice Fiscale: ");
    scanf("%s",d.codice_fiscale);
    printf("E-mail: ");
    scanf("%s",d.email);
    printf("N. Conto Corrente: ");
    scanf("%s",d.n_conto);
    printf("Saldo: ");
    scanf("%d",&d.conto_corrente.saldo);
    printf("Username: ");
    scanf("%s",d.username);
    printf("Password: ");
    scanf("%s",d.password);
    printf("Tipo: 0.Utente 1.Impiegato");
    scanf("%d",&d.impiegato);

    int n=0;

    printf("Quanti movimenti vuoi inserire? ");
    scanf("%d",&n);

    for (int i=0; i<n; i++){
        dati_movimento dat;

        printf("Numero Del Conto (Emittente): ");
        scanf("%s",dat.n_conto);
        printf("Inserire Ammontare: ");
        scanf("%d",&dat.ammontare);
        printf("Inserire la data nel formato gg-mm-aaaa : ");
        scanf("%d-%d-%d",&dat.data_mov.dd,&dat.data_mov.mm,&dat.data_mov.yyyy);

        inserimento_lista_movimenti(&d.conto_corrente.lista_movimenti, dat);
    }

    inserimento_lista_clienti(clienti, d);
}

void azioni_cliente(cliente **cliente){

    int scelta=0;
    dati_movimento dat;
    int mese=0,anno=0;

    do {

        printf("0.Saldo\n1.Bonifico\n2.Lista Movimenti\n3.Esci\n: ");
        scanf("%d", &scelta);

        switch (scelta) {
            case 0:
                printf("Saldo: %d\n",(*cliente)->dati.conto_corrente.saldo);
                break;
            case 1:
                printf("Numero Del Conto (Emittente): ");
                scanf("%s",dat.n_conto);
                printf("Inserire Ammontare: ");
                scanf("%d",&dat.ammontare);
                printf("Inserire la data nel formato gg-mm-aaaa : ");
                scanf("%d-%d-%d",&dat.data_mov.dd,&dat.data_mov.mm,&dat.data_mov.yyyy);

                inserimento_lista_movimenti(&((*cliente)->dati.conto_corrente.lista_movimenti), dat);
                break;
            case 2:
                printf("Mese: \n");
                scanf("%d",&mese);
                printf("Anno: \n");
                scanf("%d",&anno);

                for(movimento *p=(*cliente)->dati.conto_corrente.lista_movimenti; p!=NULL; p=p->next){
                    if (mese == p->dati.data_mov.mm && anno == p->dati.data_mov.yyyy)
                      printf("Ammontare: %d\nNumero Conto Del Mittente:%s\nData:%d-%d-%d\n", p->dati.ammontare, p->dati.n_conto, p->dati.data_mov.dd, p->dati.data_mov.mm, p->dati.data_mov.yyyy);
                }
                break;
            case 3:
                printf("Logout..\n");
                break;
            default:
                printf("Comando inesistente\n");
                break;
        }
    } while (scelta!=3);
}

void saldo_cliente(cliente *clienti, char codice_fiscale[]){
    cliente *p;

    for(p=clienti; p!=NULL && strcmp(p->dati.codice_fiscale, codice_fiscale) != 0; p=p->next);

    if (p==NULL)
      printf("Nessun cliente corrispondente al CF inserito\n");
    else
      printf("Saldo del cliente %s : %d\n",codice_fiscale,p->dati.conto_corrente.saldo);
}

void lista_movimenti_cliente(cliente *clienti, char codice_fiscale[], data dat){
    cliente *p;

    for(p=clienti; p!=NULL && strcmp(p->dati.codice_fiscale, codice_fiscale) != 0; p=p->next);

    if (p==NULL)
      printf("Nessun cliente corrispondente al CF inserito\n");
    else
      for(movimento *q=p->dati.conto_corrente.lista_movimenti; q!=NULL; q=q->next)
        if(q->dati.data_mov.mm == dat.mm && q->dati.data_mov.yyyy == dat.yyyy)
          printf("Numero Conto:%s\nAmmontare:%d\nData: %d-%d-%d\n",q->dati.n_conto,q->dati.ammontare,q->dati.data_mov.dd,q->dati.data_mov.mm,q->dati.data_mov.yyyy);
}

void lista_movimenti_clienti(cliente *clienti, data dat){
    for(cliente *p=clienti; p!=NULL; p=p->next){
      if (p->dati.conto_corrente.lista_movimenti != NULL)
        printf("Cognome:%s\n Nome:%s\n", p->dati.cognome, p->dati.nome);

      for(movimento *q=p->dati.conto_corrente.lista_movimenti; q!=NULL; q=q->next)
        if(q->dati.data_mov.mm == dat.mm && q->dati.data_mov.yyyy == dat.yyyy)
          printf("Numero Conto:%s\nAmmontare:%d\nData: %d-%d-%d\n",q->dati.n_conto,q->dati.ammontare,q->dati.data_mov.dd,q->dati.data_mov.mm,q->dati.data_mov.yyyy);
    }
}

bool salva_file(cliente *clienti, char conto_corrente[], data dat){

    cliente *p;

    for(p=clienti; p!=NULL && strcmp(p->dati.n_conto,conto_corrente)!=0; p=p->next);

    if (p==NULL){
        printf("Il conto corrente cercato non esiste\n");
        return false;
    }

    FILE *file;

    file = fopen("clienti.txt","a");

    if (file==NULL)
        return false;

    if (p->dati.conto_corrente.lista_movimenti == NULL){
      printf("Il cliente inserito ha una lista movimenti vuota\n");
      return false;
    }

    fprintf(file,"%s\t%s\t%s\t%s\t%s\t%s\t%s\n", p->dati.cognome, p->dati.nome, p->dati.codice_fiscale, p->dati.email, p->dati.n_conto, p->dati.username, p->dati.password);

    for(movimento *q=p->dati.conto_corrente.lista_movimenti; q!=NULL; q=q->next){
        if(q->dati.data_mov.mm == dat.mm && q->dati.data_mov.yyyy == dat.yyyy)
            fprintf(file,"\t- %d\t%s\t%d-%d-%d\n",q->dati.ammontare,q->dati.n_conto,q->dati.data_mov.dd,q->dati.data_mov.mm,q->dati.data_mov.yyyy);
    }

    fclose(file);
    return true;
}

void azioni_impiegato(cliente **clienti){

    int scelta=0;
    char codice_fiscale[17];
    data dat;
    char n_conto[11];

    do{

      printf("0.Saldo Cliente\n1.Lista Movimenti Cliente\n2.Lista Movimenti Clienti\n3.Salva\n4.Esci\n: ");
      scanf("%d", &scelta);

      switch (scelta) {
        case 0:
          printf("CF Del cliente:");
          scanf("%s",codice_fiscale);
          saldo_cliente(*clienti,codice_fiscale);
          break;
        case 1:
          printf("CF Del cliente:");
          scanf("%s", codice_fiscale);
          printf("Mese: ");
          scanf("%d", &dat.mm);
          printf("Anno: ");
          scanf("%d", &dat.yyyy);
          lista_movimenti_cliente(*clienti, codice_fiscale, dat);
          break;
        case 2:
          printf("Mese: ");
          scanf("%d", &dat.mm);
          printf("Anno: ");
          scanf("%d", &dat.yyyy);
          lista_movimenti_clienti(*clienti,dat);
          break;
        case 3:
          printf("Numero Conto: ");
          scanf("%s", n_conto);
          printf("Mese: ");
          scanf("%d", &dat.mm);
          printf("Anno: ");
          scanf("%d", &dat.yyyy);
          salva_file(*clienti,n_conto,dat);
          break;
        case 4:
          printf("Logout...\n");
          break;
        default:
          printf("Comando non riconosciuto\n");
          break;
      }
    } while (scelta!=4);

}

void login_banca(cliente **clienti){

    if (*clienti == NULL){
        printf("Nessun utente registrato!\n");
        return;
    }

    char username[11];
    char password[11];

    printf("Login...\nUsername: ");
    scanf("%s",username);
    printf("Password: ");
    scanf("%s",password);

    cliente **p;
    for(p=clienti; p!=NULL && strcmp((*p)->dati.username,username)!=0; (*p)=(*p)->next);

    if (p==NULL){
        printf("Username errato o inesistente\n");
        return;
    }

    if (strcmp(password, (*p)->dati.password)==0){
        if((*p)->dati.impiegato)
          azioni_impiegato(clienti);
        else
          azioni_cliente(p);
    }else
        printf("Password errata\n");
}

void stampa(cliente *clienti){

    for(cliente *p=clienti; p!=NULL; p=p->next){
        printf("%s - %s ",p->dati.cognome,p->dati.nome);
        for(movimento *q=p->dati.conto_corrente.lista_movimenti; q!=NULL; q=q->next)
            printf("N_CONTO: %s \n",q->dati.n_conto);
    }

}

int main() {

    cliente *clienti = NULL;
    int scelta=0;

    do{
        printf("0.Login\n1.Registrazione\n2.Esci\n: ");
        scanf("%d",&scelta);
        switch (scelta) {
            case 0:
                login_banca(&clienti);
                break;
            case 1:
                registrazione_utente(&clienti);
                break;
            case 2:
                printf("Arrivederci...\n");
                break;
            case 3:
                stampa(clienti);
                break;
            default:
                printf("Comando non valido!\n");
                break;
        }
    } while (scelta != 2);

    return 0;
}
