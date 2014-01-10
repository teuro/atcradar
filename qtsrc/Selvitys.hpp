#ifndef SELVITYS_HPP
#define SELVITYS_HPP

#include "navipiste.hpp"

class selvitys {
    double suunta;
    double korkeus;
    double nopeus;
    navipiste kohde;
    double aika;
    int kaarto;
public:
    selvitys(double suunta, double korkeus, double nopeus, double aika, int kaarto, navipiste kohde);
    selvitys(){ }

    double anna_suunta();
    double anna_korkeus();
    double anna_nopeus();
    navipiste anna_kohde();
    double anna_aika();
    int anna_kaarto();

    void aseta_suunta(double suunta);
    void aseta_korkeus(double korkeus);
    void aseta_nopeus(double nopeus);
    void aseta_kohde(navipiste kohde);
    void aseta_aika(double aika);
    void aseta_kaarto(int kaarto);
};

#endif // SELVITYS_HPP
