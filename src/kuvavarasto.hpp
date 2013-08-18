// Otsikot pitää sisällyttää vain kerran.
// Tämä varmistetaan esikääntäjällä.
#ifndef IMAGE_CACHE_HPP
#define IMAGE_CACHE_HPP
#define USE_SDL_IMAGE 1

// Käytetään joitakin standardikirjaston luokkia.
#include <string>
#include <map>
#include <stdexcept>

// Tehdään ensin yleinen cache-luokan malli, jossa ovat yleispätevät funktiot.
// T: säilöttävästä tyypistä luotu rakenne, jossa ovat staattiset jäsenet
//    - typedef  ???  type;       // säilöttävä tyyppi,
//    - void free(type& t);       // funktio arvon vapauttamiseen,
//    - type null();              // funktio, joka palauttaa tyhjän arvon,
//    - void null(type const& t); // funktio, joka tarkistaa, onko arvo tyhjä.
// K: avaimen tyyppi; oletuksena std::string
// C: tietorakenne tallennukseen; oletuksena std::map<K, T::type>.
template <typename T, typename K = std::string, typename C = std::map<K, typename T::type> > class cache {
    // Säilöjä ei kuulu kopioida, joten kopiomuodostin on yksityinen.
    cache(const cache& t) {
        throw std::logic_error("cache(const cache& t)");
    }
protected:
    // Tyypit: avain, säilöttävä objekti, tietorakenne,
    // tietorakenteen iteraattori sekä tämä luokka itse.
    typedef K key;
    typedef typename T::type object;
    typedef C container;
    typedef typename container::iterator iterator;
    typedef cache<T, K, C> cache_type;

    // Data aiemmin määrätyssä tietorakenteessa (map)
    container data;

    // Muodostin on tyhjä.
    cache() {}
public:
    // Jäsenen asetus: poistetaan vanha, lisätään uusi, jos se ei ole tyhjä.
    void set(const key& k, object t) {
        erase(k);
        if (T::is_null(t)) {
            data[k] = t;
        }
    }
    // Haku: etsitään ja palautetaan, jos löytyi; muuten palautetaan tyhjä.
    object find(const key& name) {
        iterator i = data.find(name);
        if (i == data.end()) {
            return T::null();
        }
        return i->second;
    }
    // Tyhjennys: vapautetaan ja poistetaan kaikki.
    void clear() {
        for (iterator i = data.begin(); i != data.end(); ++i) {
            T::free(i->second);
        }
        data.clear();
    }
    // Poisto nimen perusteella: etsitään, vapautetaan ja poistetaan.
    // Toisella parametrilla voi kieltää vapauttamisen, jos jostain syystä
    // haluaa vain ottaa arvon pois säilöstä ja huolehtia itse lopusta.
    bool erase(const std::string& key, bool free_data = true) {
        iterator i = data.find(key);
        if (i == data.end()) {
            return false;
        }
        if (free_data) {
            T::free(i->second);
        }
        data.erase(i);
        return true;
    }
    // Poisto datan perusteella: käydään listaa läpi, poistetaan osuma.
    // Toinen parametri sama kuin yllä.
    bool erase(object t, bool free_data = true) {
        for (iterator i = data.begin(); i != data.end(); ++i) {
            // Jos löytyi, vapautetaan ja poistetaan.
            if (i->second == t) {
                if (free_data) {
                    T::free(i->second);
                }
                data.erase(i);
                return true;
            }
        }
        // Ei löytynyt.
        return false;
    }
    // Tuhoaja: tyhjennetään sisältö.
    ~cache() {
        clear();
    }
};

// Varsinainen SDL-osuus alkaa tästä:

#include <SDL/SDL.h>
#ifdef USE_SDL_IMAGE
    // Tuki muillekin kuin BMP-kuville saadaan SDL_image-kirjastosta:
    #include <SDL/SDL_image.h>
#endif

// Toteutetaan SDL_Surface*-tyypille cache-mallin tarvitsema rakenne.
struct image_cache_data_type {
    // Tyyppi: SDL_Surface *.
    typedef SDL_Surface* type;

    // Vapautus: SDL_FreeSurface.
    static void free(type& t) {
        SDL_FreeSurface(t);
    }

    // Tyhjä arvo: 0-osoitin (NULL-osoitin).
    static type null() {
        return 0;
    }

    // Tyhjän tarkistus: vertailu nollaan.
    static bool is_null(type const& t) {
        return t == 0;
    }
};

// Tehdään mallista tämän tyypin avulla luokka kuville.
class image_cache: public cache<image_cache_data_type> {
    // Tätäkään ei kuulu kopioida, joten kopiomuodostin on yksityinen.
    image_cache(const image_cache& t): cache_type() {
        throw std::logic_error("image_cache(const image_cache& t)");
    }
public:
    // Muodostin on tyhjä.
    image_cache() {}

    // Latausfunktio: haetaan vanha kuva tai ladataan tiedostosta.
    SDL_Surface *get(const std::string& file) {
        // Etsitään vanhaa kuvaa, ja jos löytyi, palautetaan se.
        SDL_Surface *tmp = find(file);
        if (tmp) {
            return tmp;
        }
        // Ladataan kuva, ja jos epäonnistui, heitetään virhe.
        #ifdef USE_SDL_IMAGE
            // SDL_image-kirjastossa on tuki monelle formaatille.
            tmp = IMG_Load(file.c_str());
        #else
            tmp = SDL_LoadBMP(file.c_str());
        #endif
        if (!tmp) {
            throw std::runtime_error(SDL_GetError());
        }
        // Laitetaan kuva säilöön ja palautetaan se.
        data[file] = tmp;
        return tmp;
    }
    // Staattinen funktio, jossa on staattinen image_cache-olio.
    // Staattinen olio luodaan ohjelman alussa ja tuhotaan lopussa aivan
    // automaattisesti, ja olion tuhoajafunktiossa tuhotaan myös sen
    // sisältämät kuvat (~cache => clear => free => SDL_FreeSurface).
    // Funktio palauttaa viittauksen olioon, joten kaikki pääsevät
    // käyttämään samaa image_cache-oliota.
    static image_cache& common() {
        static image_cache c;
        return c;
    }
};

#endif
