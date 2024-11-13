/*
 * Cette classe permet le débogage et l'affichage de données .Elle comprend les fonctionnalités suivantes :l'initialisation
 * du débogueur,méthodes pour afficher différentes formes de données(char,string, différents nombres entiers) en utilisant 
 * singleton pour garantir une seule instance de la classe USART (static USART instance_)
 *
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours INF1900
 *
 * Vincent Charbonneau, Rebecca Abi Chahine, Charles-Emile Leblond, Camil Bisson
 * 2024
 *
 * Code qui n'est sous aucune license.
 */

#ifdef DEBUG
# define DEBUG_PRINT(x) USART::getInstance()->print(x)
#else
# define DEBUG_PRINT(x) do {} while (0) // code mort
#endif

#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <stdlib.h>

class USART {
private:
    static USART instance_;
    USART();
    USART(const USART&) = delete;
    USART operator=(const USART&) = delete;
    void init(void) const;
    void unicodeToUtf8(uint16_t unicodeChar, uint8_t utf8Bytes[4]);

public: 
    static USART* getInstance(void);
    ~USART() = default;

    unsigned char receive(void) const;

    void print(const unsigned char data) const;
    void print(const char* data) const;
    void print(const char* label, const char* data) const;
    void print(const char* label, const uint8_t data) const;
    void print(const char* label, const uint16_t data) const;
    void print(const char* label, const uint32_t data) const;
    void print(const char* label, bool data) const;
    void printUnicode(uint16_t unicode);
};

#endif
