#include "olcPixelGameEngine.h"
#include <math.h>

class Mandelbrot : public olc::PixelGameEngine
{
	
public:

    Mandelbrot()
    {
        sAppName = "mandelbrot";
    }
	
private:

    int MAXITERATIONS = 2500;
    double rangeX = 4.0f;
    double rangeY;
    double xmin; // x goes from xmin to xmax
    double ymin;
    double xmax;
    double ymax;
    bool zoom = false;
    double offsetX = 0.0f;
	double offsetY = 0.0f;
    double scaleX = 1.0f;
    double scaleY = 1.0f;

    bool OnUserCreate() override
    {
        xmin = -rangeX/2.0f; // Start at negative half the width and height
        rangeY = (rangeX * ScreenHeight()) / ScreenWidth();
        ymin = -rangeY/2.0f;
        xmax = -xmin; + rangeX;
        ymax = -ymin; + rangeY;
        offsetX = 0.0f;
		offsetY = 0.0f;
        return true;
    }

    void WorldToScreen(double fWorldX, double fWorldY, int &nScreenX, int &nScreenY)
	{
		nScreenX = (int)((fWorldX - offsetX) * scaleX);
		nScreenY = (int)((fWorldY - offsetY) * scaleY);
	}

	void ScreenToWorld(int nScreenX, int nScreenY, double &fWorldX, double &fWorldY)
	{
		fWorldX = ((double)nScreenX / scaleX) + offsetX;
		fWorldY = ((double)nScreenY / scaleY) + offsetY;
	}

    bool OnUserUpdate(float fElapsedTime) override
    {
        double width = ScreenWidth();
        double height = ScreenHeight();

        double locX = GetMouseX();
		double locY = GetMouseY();

        double fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom;
		ScreenToWorld(locX, locY, fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom);

        if(GetKey(olc::Key::Q).bHeld)//Zoom in
        {
            scaleX *= 1.1f;
            scaleY *= 1.1f;
            zoom = true;
        }
        if(GetKey(olc::Key::A).bHeld)//Zoom out
        {
            scaleX *= 0.9f;
            scaleY *= 0.9f;
            zoom = true;
        }

        double fMouseWorldX_AfterZoom, fMouseWorldY_AfterZoom;
		ScreenToWorld(locX, locY, fMouseWorldX_AfterZoom, fMouseWorldY_AfterZoom);		
		offsetX += (fMouseWorldX_BeforeZoom - fMouseWorldX_AfterZoom);
		offsetY += (fMouseWorldY_BeforeZoom - fMouseWorldY_AfterZoom);

        FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

        double fWorldLeft, fWorldTop, fWorldRight, fWorldBottom;
		ScreenToWorld(0, 0, fWorldLeft, fWorldTop);
		ScreenToWorld(ScreenWidth(), ScreenHeight(), fWorldRight, fWorldBottom);

        double den = (width/2.0f)/(rangeX/2.0f);
        if(zoom)
        {
            xmin = (fWorldLeft-width/2.0f)/den;
            ymin = (fWorldTop-height/2.0f)/den;
            xmax = (fWorldRight-width/2.0f)/den;
            ymax = (fWorldBottom-height/2.0f)/den;
            zoom = false;
        }

        double dx = (xmax - xmin) / (width);
        double dy = (ymax - ymin) / (height);

        // Start y
        double y = ymin;
        for (int j = 0; j < height; j++)
        {
            // Start x
            double x = xmin;
            for (int i = 0; i < width; i++)
            {
                // Now we test, as we iterate z = z^2 + cm does z tend towards infinity?
                double a = x;
                double b = y;
                int n = 0;
                while (n < MAXITERATIONS)
                {
                    double aa = a * a;
                    double bb = b * b;
                    if (aa + bb > 4.0f)// All numbers outside circle radius 2 tend to infinity
                        break;  // Bail
                    double twoab = 2.0f * a * b;
                    a = aa - bb + x;
                    b = twoab + y;
                    n++;
                }
                // We color each pixel based on how long it takes to get to infinity
                // If we never got there, let's pick the color black
				
                setColor(i,j,n);
                x += dx;
            }
            y += dy;
        }
        return true;
    }
	
	void setColor(int x,int y,int n)
	{
		if (n < MAXITERATIONS && n > 0)
		{
			int i = n % 16;
			switch(i)
			{
				case 0:
					Draw(x,y,olc::Pixel(66, 30, 15));
					break;
				case 1:
					Draw(x,y,olc::Pixel(25, 7, 26));
					break;
				case 2:
					Draw(x,y,olc::Pixel(9, 1, 47));
					break;
				case 3:
					Draw(x,y,olc::Pixel(4, 4, 73));
					break;
				case 4:
					Draw(x,y,olc::Pixel(0, 7, 100));
					break;
				case 5:
					Draw(x,y,olc::Pixel(12, 44, 138));
					break;
				case 6:
					Draw(x,y,olc::Pixel(24, 82, 177));
					break;
				case 7:
					Draw(x,y,olc::Pixel(57, 125, 209));
					break;
				case 8:
					Draw(x,y,olc::Pixel(134, 181, 229));
					break;
				case 9:
					Draw(x,y,olc::Pixel(211, 236, 248));
					break;
				case 10:
					Draw(x,y,olc::Pixel(241, 233, 191));
					break;
				case 11:
					Draw(x,y,olc::Pixel(248, 201, 95));
					break;
				case 12:
					Draw(x,y,olc::Pixel(255, 170, 0));
					break;
				case 13:
					Draw(x,y,olc::Pixel(204, 128, 0));
					break;
				case 14:
					Draw(x,y,olc::Pixel(153, 87, 0));
					break;
				case 15:
					Draw(x,y,olc::Pixel(106, 52, 3));
					break;
			}
		}
		else
			Draw(x,y,olc::BLACK);
	}
};


int main()
{
    Mandelbrot p;
    int pix = 3;
    if(p.Construct(540/pix,540/pix,pix,pix))
        p.Start();
    return 0;
}