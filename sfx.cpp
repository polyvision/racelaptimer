#include "sfx.h"
#include <QSoundEffect>

SFX::SFX()
{
    m_pSFX_Beep = new QSoundEffect();
    m_pSFX_Beep->setSource(QUrl::fromLocalFile("E:/Dropbox/code/RaceLapTimer/sfx_tracked_lap.wav"));
}

void SFX::playBeep(){
    m_pSFX_Beep->play();
}
