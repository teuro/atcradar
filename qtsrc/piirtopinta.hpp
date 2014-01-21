#ifndef _PIIRTOPINTA_HPP
#define _PIIRTOPINTA_HPP

// Abstrakti piirtopinta, jota n‰kym‰t (View) voivat k‰ytt‰‰ piirt‰miseen

class IPiirtoPinta
{
public:
    virtual void rectangleColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2) = 0;
    virtual void lineColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2) = 0;
    virtual void circleColor(unsigned short x, unsigned short y, unsigned short rad) = 0;
    virtual void trigonColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3) = 0;

    virtual void kirjoita_tekstia(std::string teksti, int x, int y) = 0;

	virtual int get_fontinkoko() = 0;
	virtual void flip() = 0;
};

#endif
