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
