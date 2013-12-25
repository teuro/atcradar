#ifndef _KIELI_HPP
#define _KIELI_HPP

#include "apuvalineet.hpp"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

class Kieli {
private:
	std::string kieli;
	std::map <int, std::string> tekstit;
public:
	Kieli(std::string kieli);
	Kieli(){ }
	void lisaa_teksti(int id, std::string teksti);
	std::string anna_teksti(int id);
};

#endif