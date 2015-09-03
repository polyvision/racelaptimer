#include "sfx.h"
#include <QSoundEffect>
#include "settings.h"

SFX::SFX()
{
    m_pSFX_Beep = new QSoundEffect();
    m_pSFX_Beep->setSource(QUrl::fromLocalFile(Settings::instance()->getLapBeepPath()));

    m_pSFX_FastestLap = new QSoundEffect();
    m_pSFX_FastestLap->setSource(QUrl::fromLocalFile(Settings::instance()->getFastestLapSoundPath()));
}

void SFX::playBeep(){
    m_pSFX_Beep->play();
}

void SFX::playFastestLap(){
    m_pSFX_FastestLap->play();
}
