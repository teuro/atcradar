#ifndef PELISUORITE_HPP
#define PELISUORITE_HPP

#include <string>
#include <ostream>

#include "apuvalineet.hpp"

class pelisuorite {
    int pelaaja_id;
    int taso;
    int konemaara;
    int porrastusvirheet;
    int muut_virheet;
    int pisteet;
    int tarkiste;
public:
    pelisuorite(int pelaaja_id, int taso, int konemaara, int porrastusvirheet, int muut_virheet, int pisteet, int tarkiste);
    pelisuorite(int pelaaja_id, int taso, int konemaara, int porrastusvirheet, int muut_virheet, int pisteet);
    std::string anna_suorite();
    int laske_tiiviste();
    bool tarkista(int tarkiste);
    int anna_id();
    int anna_pisteet();
    int anna_konemaara();
    int anna_porrastusvirheet();
    int anna_virheet();
    int anna_taso();
    int anna_tarkiste();
};

#endif // PELISUORITE_HPP
