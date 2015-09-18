/**
 * OpenRaceLapTimer - Copyright 2015 by airbirds.de, a project of polyvision UG (haftungsbeschränkt)
 *
 * Author: Alexander B. Bierbrauer
 *
 * This file is part of OpenRaceLapTimer.
 *
 * OpenRaceLapTimer is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * OpenRaceLapTimer is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Foobar. If not, see http://www.gnu.org/licenses/.
 **/

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
