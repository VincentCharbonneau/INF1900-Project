/*
 * Cette classe permet de contrôler le Timer1 de 16 bits de l'ATmega324PA. Elle permet
 * de configurer le mode de fonctionnement du timer, de configurer les durées de deux
 * interruptions de comparaison, de configurer les valeurs de comparaison, de réinitialiser
 * le timer, en plus de permettre la scrutation de la valeur du timer.
 * 
 * Utilise le pattern singleton pour garantir une seule instance de la classe Timer (static Timer instance_)
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

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include "USART.h"

enum class TimerModes {
	CTC, NORMAL
};

enum class ConnectionMode {
    TOGGLE_MATCH, OFF
};

class Timer {
    private:
    static Timer instance_ ;
    Timer ();
    Timer (const Timer&) = delete;
    Timer operator=(const Timer&) = delete;

    uint16_t prescalar_;

    uint16_t countCompareA_;
    uint16_t countCompareB_;
    
    TimerModes mode_;

    public:
    static Timer* getInstance(void);
    ~Timer() = default;
	
	void setMode(const TimerModes mode);
    void setConnectionMode(const ConnectionMode mode);

    void setPrescalar1024();
    void setPrescalar1();

    void setDurationCompareA(const uint16_t duration_ms);
    void setDurationCompareB(const uint16_t duration_ms);
    void setCountCompareA(const uint16_t count);
    void setCountCompareB(const uint16_t count);
    void clearTimer();
    const uint16_t durationMsToCount(const uint16_t duration_ms);
    const TimerModes getMode() const;
    const uint16_t getPrescalar() const;
    const uint16_t getCount() const;
    const uint16_t getCountCompareA() const;
    const uint16_t getCountCompareB() const;
};

#endif
