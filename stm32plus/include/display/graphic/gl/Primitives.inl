/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#include <stdlib.h>
#include "util/MinMax.h"


namespace stm32plus {
	namespace display {

		/**
		 * plot a point in the foreground colour
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::plotPoint(const Point& p) {

			this->moveTo(
					Rectangle(
							p.X,
							p.Y,
							1,
							1
					)
			);

			this->beginWriting();
			this->writePixel(_foreground);
		}


		/**
		 * Draw a line between two points. Modified slightly by me (Andy Brown) to include the final point
		 * in the line. Therefore drawing from (x,y) to (x,y) will result in a single point being plotted.
		 * Freely useable in non-commercial applications as long as credits
		 * to Po-Han Lin and link to http://www.edepot.com is provided in
		 * source code and can been seen in compiled executable.
		 * Commercial applications please inquire about licensing the algorithms.
		 */

		template<class TDevice,typename TDeviceAccessMode>
		inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::drawLine(const Point& p1,const Point& p2) {

			// optimisation for straight lines. filling rectangles is much more efficient than plotting points

			if(p1.X==p2.X)
				fillRectangle(Rectangle(p1.X,Min<int16_t>(p1.Y,p2.Y),1,Abs<int16_t>(p2.Y-p1.Y)+1));
			else if(p1.Y==p2.Y)
				fillRectangle(Rectangle(Min<int16_t>(p1.X,p2.X),p1.Y,Abs<int16_t>(p2.X-p1.X)+1,1));
			else {
				// can't optimise, use the algorithm

				bool yLonger=false;
				int32_t incrementVal,endVal;
				int32_t shortLen=p2.Y - p1.Y;
				int32_t longLen=p2.X - p1.X;

				if(abs(shortLen)>abs(longLen)) {
					int swap=shortLen;
					shortLen=longLen;
					longLen=swap;
					yLonger=true;
				}

				endVal=longLen;

				if(longLen < 0) {
					incrementVal=-1;
					longLen=-longLen;
					endVal--;
				} else {
					incrementVal=1;
					endVal++;
				}

				int decInc;

				if(longLen == 0)
					decInc=0;
				else
					decInc=(shortLen << 16) / longLen;

				int32_t j=0;

				if(yLonger) {
					for(int i=0;i != endVal;i+=incrementVal) {
						plotPoint(Point(p1.X + (j >> 16),p1.Y + i));
						j+=decInc;
					}
				} else {
					for(int i=0;i != endVal;i+=incrementVal) {
						plotPoint(Point(p1.X + i,p1.Y + (j >> 16)));
						j+=decInc;
					}
				}
			}
		}
	}
}
