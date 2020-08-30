/*
 * Décodeur infrarouge
 * by Nanomaitre 2015 pour OpenClassRooms
 * Ce programme vous permet de décoder et stocker
 * des commandes infrarouges liées à votre télécommande
 * Si la commande est reconnue, il vous indique la corrrespondance
 * Sinon, il vous propose de l'ajouter en la nommant.
 * Le programme affiche aussi le tableau de résultats
 * qu'il suffit ensuite de copier/coller dans un autre code si besoin
 * /!\ Le pin 8 est utilisé pour ce test, il ne faut pas le changer !!
 * Ce code est dans le domaine public
 */


const int pin = 8; // pin de lecture du TSOP
const int nbMax = 32; // nombre maximum de lectures
const int nbMaxCom = 5; // nombre maximum de commandes stockées
String noms[nbMaxCom] = {""}; // tableau pour stocker les noms des commandes (sur 5 char)
const int temporisation = 20; // temporisation de lecture des signaux pulsés
unsigned int tableau[nbMax * 2] = {0}; // tableau pour stocker les temps lus
unsigned int commandes[nbMaxCom][nbMax * 2] = {0}; // tableau pour stocker les temps des commandes reconnues
int nbCommande = 0; // numéro de commande en cours

void setup() {
  pinMode(pin, INPUT); // mode INPUT pour le pin de lecture
  Serial.begin(9600); //initialisation communication série.
}

void loop() {
  Serial.println("Go..."); //signal l'attente d'un appui
  while (PINB & B00000001);//attend un signal haut en boucle
  lecture(); // appel de la fonction de lecture du code
  affichage(); // appel de la fonction d'affichage
  delay(1000); // attente d'une seconde
}

//fonction de lecture du code
void lecture() {
  for (int t = 0; t < nbMax * 2; t += 2) { //boucle pour le nombre de lectures, progresse de 2 en 2
    while (PINB & B00000001) { // tant que état pulsé
      tableau[t] += temporisation; // on incrémente la valeur du tableau
      delayMicroseconds(temporisation); //attente
    }
    while (!(PINB & B00000001)) { // puis tant que état non pulsé
      tableau[t + 1] += temporisation; // on incrémente l'autre valeur du tableau
      delayMicroseconds(temporisation);
    }
  }
}

// fonction d'affichage du résultat
void affichage() {
  // on crée un affichage copiable sur la console
  Serial.print("const unsigned int code[");
  Serial.print(nbMax * 2 + 1, DEC);
  Serial.print("]={");
  for (int t = 0; t < nbMax * 2; t += 2) {
    Serial.print(tableau[t], DEC);
    Serial.print(", ");
    Serial.print(tableau[t + 1], DEC);
    Serial.print(", ");
  }
  Serial.println("0}");
  compare();//on appelle la fonction de comparaison
  //on efface le tableau pour la prochaine lecture
  for (int t = 0; t < nbMax * 2; t++) {
    tableau[t] = 0;
  }
}

//fonction de comparaison
int compare () {
  boolean trouve = 0; // drapeau de commande trouvée
  for (int c = 0; c < nbMaxCom; c++) { //boucle sur les commandes
    int nbCor = 0; // variable de validation de comparaison
    for (int t = 0; t < nbMax * 2; t++) { // on parcourt les résultats
      int tmoins = tableau[t] - tableau[t] * 20 / 100; // valeur-20%
      int tplus = tableau[t] + tableau[t] * 20 / 100; // valeur+20%
      if (commandes[c][t] > tmoins && commandes[c][t] < tplus && t > 0) { //si dans la fourchette (sauf première valeur)
        nbCor++; // on valide la comparaison
      }
    }
    if (nbCor == nbMax * 2 - 1) { //si tout est validé (sauf première valeur)
      // On affiche la touche correspondante
      Serial.print("Correspondance avec commande ");
      Serial.print(c);
      Serial.print(" = ");
      Serial.println(noms[c]);
      trouve = 1; // on indique que la commande est déjà existante
    }
  }
  if (!trouve && (nbCommande < nbMaxCom)) { //si commande non existante et encore possible d'en ajouter
    //on crée la commande en copiant le tableau des résultats
    for (int t = 0; t < nbMax * 2; t++) {
      commandes[nbCommande][t] = tableau[t];
    }
    //on affiche qu'une nouvelle commande est trouvée
    Serial.println("ajout commande");
    Serial.println("Donner un nom (5 char max) :");
    //et on saisit son nom
    while (!Serial.available())
      noms[nbCommande] = "";
    for (int t = 0; t < 5; t++) {
      char ch = Serial.read();
      if (ch > 32)
        noms[nbCommande] += String(ch);
      delay(10);
    }
    //on vide le cache de réception
    while (Serial.available()) {
      Serial.read();
      delay(10);
    }

    nbCommande++; //on incrémente la commande en cours
  }
  Serial.println();
}
