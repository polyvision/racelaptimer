#ifndef SFX_H
#define SFX_H

#include "singleton.h"

class QSoundEffect;

class SFX:  public Singleton<SFX>
{
     friend class Singleton<SFX>;
public:
    SFX();
    void    playBeep();


private:
    QSoundEffect *m_pSFX_Beep;
};

#endif // SFX_H
