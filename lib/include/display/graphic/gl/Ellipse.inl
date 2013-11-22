/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Fill an ellipse with the foreground colour. This is the algorithm documented in the paper
     * "Drawing Ellipses Using Filled Rectangles" by L. Patrick.
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::fillEllipse(const Point& center,const Size& size) {

      int16_t x=0,y=size.Height;
      int16_t rx=x,ry=y;
      uint32_t width=1,height=1;
      int32_t a2=(int32_t)size.Width*(int32_t)size.Width,b2=(int32_t)size.Height*(int32_t)size.Height;
      int32_t crit1=-(a2 / 4 + size.Width % 2 + b2);
      int32_t crit2=-(b2 / 4 + size.Height % 2 + a2);
      int32_t crit3=-(b2 / 4 + size.Height % 2);
      int32_t t=-a2 * y; // e(x+1/2,y-1/2) - (width_^2+height_^2)/4
      int32_t dxt=2 * b2 * x,dyt=-2 * a2 * y;
      int32_t d2xt=2 * b2,d2yt=2 * a2;

      if(size.Height==0) {
        fillRectangle(NormalizedRectangle(center.X-size.Width,center.Y,2*size.Width+ 1,1));
        return;
      }

      while(y >= 0 && x <= size.Width) {
        if(t + b2 * x <= crit1 || /* e(x+1,y-1/2) <= 0 */
        t + a2 * y <= crit3) { /* e(x+1/2,y) <= 0 */
          if(height == 1) {}      // draw nothing
          else if(ry * 2 + 1 > ((int32_t)height - 1) * 2) {
            fillRectangle(NormalizedRectangle(center.X - rx,center.Y - ry,width,height - 1));
            fillRectangle(NormalizedRectangle(center.X - rx,center.Y + ry + 1,width,1 - height));
            ry-=height - 1;
            height=1;
          } else {
            fillRectangle(NormalizedRectangle(center.X - rx,center.Y - ry,width,ry * 2 + 1));
            ry-=ry;
            height=1;
          }
          x++,dxt+=d2xt,t+=dxt;
          rx++;
          width+=2;
        } else if(t - a2 * y > crit2) { /* e(x+1/2,y-1) > 0 */
          y--,dyt+=d2yt,t+=dyt;
          height++;
        } else {
          if(ry * 2 + 1 > (int32_t)height * 2) {
            fillRectangle(NormalizedRectangle(center.X - rx,center.Y - ry,width,height));
            fillRectangle(NormalizedRectangle(center.X - rx,center.Y + ry + 1,width,-height));
          } else {
            fillRectangle(NormalizedRectangle(center.X - rx,center.Y - ry,width,ry * 2 + 1));
          }
          x++,dxt+=d2xt,t+=dxt;
          y--,dyt+=d2yt,t+=dyt;
          rx++;
          width+=2;
          ry-=height;
          height=1;
        }
      }

      if(ry > (int32_t)height) {
        fillRectangle(NormalizedRectangle(center.X - rx,center.Y - ry,width,height));
        fillRectangle(NormalizedRectangle(center.X - rx,center.Y + ry + 1,width,-height));
      } else {
        fillRectangle(NormalizedRectangle(center.X - rx,center.Y - ry,width,ry * 2 + 2));
      }
    }

    /**
     * Draw an ellipse with the foreground colour using the fast bresenham algorithm. This method is an implementation of
     * the pseudo-code in the paper "A Fast Bresenham Type Algorithm For Drawing Ellipses" by John Kennedy.
     *
     * @param[in] center The center point.
     * @param[in] size The radius width and height.
     */

    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::drawEllipse(const Point& center,const Size& size) {
      int32_t x,y,xchange,ychange,ellipseError,twoASquare,twoBSquare,stoppingX,stoppingY,width,height;

      width=size.Width;
      height=size.Height;

      twoASquare=2 * width * width;
      twoBSquare=2 * height * height;

      x=width;
      y=0;

      xchange=height * height * (1 - 2 * width);
      ychange=width * width;

      ellipseError=0;
      stoppingX=twoBSquare * width;
      stoppingY=0;

      while(stoppingX >= stoppingY) {

        plot4EllipsePoints(center.X,center.Y,x,y);
        y++;
        stoppingY+=twoASquare;
        ellipseError+=ychange;

        ychange+=twoASquare;

        if(2 * ellipseError + xchange > 0) {
          x--;
          stoppingX-=twoBSquare;
          ellipseError+=xchange;
          xchange+=twoBSquare;
        }
      }

      x=0;
      y=height;

      xchange=height * height;
      ychange=width * width * (1 - 2 * height);

      ellipseError=0;
      stoppingX=0;
      stoppingY=twoASquare * height;

      while(stoppingX <= stoppingY) {
        plot4EllipsePoints(center.X,center.Y,x,y);
        x++;
        stoppingX+=twoBSquare;
        ellipseError+=xchange;
        xchange+=twoBSquare;

        if((2 * ellipseError + ychange) > 0) {
          y--;
          stoppingY-=twoASquare;
          ellipseError+=ychange;
          ychange+=twoASquare;
        }
      }
    }


    template<class TDevice,typename TDeviceAccessMode>
    inline void GraphicsLibrary<TDevice,TDeviceAccessMode>::plot4EllipsePoints(int16_t cx,int16_t cy,int16_t x,int16_t y) {

      plotPoint(Point(cx + x,cy + y)); // point in quadrant 1
      plotPoint(Point(cx - x,cy + y)); // point in quadrant 2
      plotPoint(Point(cx - x,cy - y)); // point in quadrant 3
      plotPoint(Point(cx + x,cy - y)); // point in quadrant 4
    }
  }
}
