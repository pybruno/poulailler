// Déclaration des variables et des constantes
int PhotoR = A0;// entrée analogique de la photorésistance
int FinCHaut = 9;// entrée du fin de course HAUT. Une broche sur la masse l'autre sur l'arduino
int FinCBas = 8;// entrée du fin de course BAS. Une broche sur la masse l'autre sur l'arduino
int enablePin = 5;
int D2 = 2;//monté
int D3 = 3;//descente
int buttonManu = 10;
int buttonHautBas = 11;

//variable pour stocker la valeur lue après conversion
int PhotoRLue;

void setup()
{
// démarrage la liaison série entre entrée analogique et ordi
Serial.begin(9600);
//Déclaration des contacts fin de course en entrée avec utilisation de la fonction PULLUP interne
pinMode(FinCHaut, INPUT_PULLUP); //pinMode(FinCHaut, INPUT_PULLUP)Les Pull Up sont des résistances internes à l'arduino.
//Donc de base lorsque le boutton n'est pas appuyé on lit un état haut (5V = niveau logique 1)
pinMode(FinCBas, INPUT_PULLUP);   //pinMode(FinCHaut, INPUT_PULLUP)
pinMode(D2, OUTPUT);
pinMode(D3, OUTPUT);
pinMode(enablePin, OUTPUT);
pinMode(buttonManu, INPUT_PULLUP);
pinMode(buttonHautBas, INPUT_PULLUP);

}

void loop()
{ 
    if (digitalRead(buttonManu) == 0) {  // manuel mode
      Serial.println("manuel mode");
      Serial.println("bouton bas:");
      Serial.println(digitalRead(buttonHautBas));
      delay(1000);
          if (digitalRead(buttonHautBas) == 1) {
             while(digitalRead(FinCBas) == 1){  //Tant que la porte n'est pas fermé, le moteur tourne   
                  //Fermeture
                analogWrite(enablePin, 200);
                digitalWrite(D2, LOW);
                digitalWrite(D3, HIGH);
                if (digitalRead(buttonManu) == 1 || (digitalRead(buttonHautBas) == 0)) {  //exit si mode manu est arrete
                  digitalWrite(D3, HIGH);
                  digitalWrite(D2, HIGH);
                  break;
                }
            }
              //digitalWrite(D3, LOW);    //On arrete le moteur car le contact fin de course est activé
              digitalWrite(D3, HIGH);
              digitalWrite(D2, HIGH);
  
          }

          else if (digitalRead(buttonHautBas) == 0) {
              while(digitalRead(FinCHaut) == 1){  //Tant que la porte n'est pas ouverte, le moteur tourne   
                  //Fermeture
                digitalWrite(D2, HIGH);
                digitalWrite(D3, LOW);
                if ((digitalRead(buttonManu) == 1) || (digitalRead(buttonHautBas) == 1)){
                  digitalWrite(D2, LOW);   //stop motor
                  break;
                }
              }
              digitalWrite(D2, LOW);   //stop motor
          }            
     }  //end if manu

    else {   // automatique mode
    Serial.println("automatique mode");
    PhotoRLue = analogRead(PhotoR);
    //Valeur comprise entre 0 et 1024
    //on affiche la valeur lue sur la liaison série
    Serial.print("PhotoRLue = ");
    Serial.print(PhotoRLue);
    //on saute une ligne entre deux affichages
   
    Serial.println();
    Serial.println("bas:");
    Serial.println(digitalRead(FinCBas));
    Serial.println("haut");
    Serial.println(digitalRead(FinCHaut));
    delay(5000);
    if (PhotoRLue <80 & digitalRead(FinCBas) == 1)         //Condition : Detection de la nuit et la porte est ouverte
    //Alors on ferme la porte
    {
      while(digitalRead(FinCBas) == 1){  //Tant que la porte n'est pas fermé, le moteur tourne   
          //Fermeture
         analogWrite(enablePin, 200);
        digitalWrite(D2, LOW);
        digitalWrite(D3, HIGH);
      }
        //digitalWrite(D3, LOW);    //On arrete le moteur car le contact fin de course est activé
        digitalWrite(D3, HIGH);
        digitalWrite(D2, HIGH);
    }   
    if (PhotoRLue > 520 && digitalRead(FinCHaut) == 1)         //Condition : Detection du jour et la porte est fermée FinCHaut = 1 signifie le boutton fin de course haut n'est pas appuyé
    //if (PhotoRLue > 520 )  
    {
      while(digitalRead(FinCHaut) == 1){  //Tant que la porte n'est pas ouverte, le moteur tourne   
          //Fermeture
        digitalWrite(D2, HIGH);
        digitalWrite(D3, LOW);
      }
    digitalWrite(D2, LOW);
    }
   }  // end else button manu
}
