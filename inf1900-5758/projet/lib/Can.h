/*
 * Classe permettant l'acces au convertisseur analogique/numerique
 * du microcontroleur ATMega16 de Atmel.
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1995
 *
 * Matthew Khouzam et Jerome Collin
 * 2005-2006
 * Modifie par l'equipe 5758 en 2024 pour
 * y inclure le Singleton
 *
 * Code qui n'est sous aucune license.
 *
 */

#ifndef CAN_H
#define CAN_H

#include <avr/io.h> 
#include "Connection.h"
/*
 * Classe can:
 *   Le constructeur initialise le convertisseur.
 *   Une lecture enclanche une conversion et le resultat
 *   est retourne sur 16 bits.
 *
 */

class Can
{
public:
   ~Can();
   static Can* getInstance(void);
   // retourne la valeur numerique correspondant a la valeur
   // analogique sur le port A.  pos doit etre entre 0 et 7
   // inclusivement.  Seulement les 10 bits de poids faible
   // sont significatifs.
   uint16_t lecture(uint8_t pos);

private:
   // Donnees membres - aucun
   Can();
   Can(const Can&) = delete;
   Can operator=(const Can&) = delete;   

   static Can instance_;
};

#endif /* CAN_H */
