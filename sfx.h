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
    void    playFastestLap();

private:
    QSoundEffect *m_pSFX_Beep;
    QSoundEffect *m_pSFX_FastestLap;
};

#endif // SFX_H
