#ifndef _LUKIJA_HPP
#define _LUKIJA_HPP

#include <string>

class syotteenluku {
    private:
		//The storage string
		std::string str;

    public:
		//Initializes variables
		syotteenluku();
		std::string anna_viesti() { return this->str; }
		void aseta_viesti(std::string viesti) { this->str = viesti; }
		void lue_syote();
		void tyhjenna() { this->str = ""; }
	};

#endif
